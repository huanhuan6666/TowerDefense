#include "tower.h"
#include "enemy.h"
Tower::Tower(int r, int c, vector<Enemy *>& enemy_all): enemy_all(enemy_all) {
    all_health = 300;
    cur_health = 300;
    step = 0;
    level = 1;
    damage = 10;
    state = LIVE;
    speed = 0;
    range = 300;
    all_block = 3;
    cur_block = 0;
    row = r;
    col = c;
    weight = 40;
    height = 40;

    x = c * kCellLen + (kCellLen - weight)/2; //根据传入的格子坐标得到像素坐标
    y = r * kCellLen + (kCellLen - height)/2;
    picture = "../source/WallNut.gif";
    target_enemy = nullptr;

    selected = false;
    type = 1; //近战单位塔
    interval = 3; //调用五次
    counter = 0;
}
//析构函数
Tower::~Tower() {
    for(auto& enemy : enemy_all) {
        if(enemy->state == BLOCKED)
            enemy->state = LIVE;
    }
}

//攻击enemy2attack里的敌人
void Tower::attack_enemys(vector<Enemy *>& enemy2attack) {
    for(auto it = bullet_all.begin(); it != bullet_all.end(); ) {
        if((*it)->flag == 1) { //斜率为正无穷
            (*it)->y += (*it)->speed * (*it)->direct;
        }
        else {
            (*it)->x += (*it)->direct * (*it)->speed; //移动子弹
            (*it)->y = (*it)->k * (*it)->x + (*it)->b;
        }
        if(Distance((*it)->x, (*it)->y, x, y) > range) { //子弹超出射程
            delete (*it);
            it = bullet_all.erase(it);
        }
        else {
            int flag = 0;   //是否打到了敌人
            for(auto& enemy : enemy2attack) {
                if(InArea((*it)->x+(*it)->w/2, (*it)->y+(*it)->h/2,
                          enemy->x, enemy->y, enemy->weight, enemy->height)) //子弹碰到了敌人
                {
                    enemy->cur_health -= damage;
                    delete (*it);
                    flag = 1;
                    it = bullet_all.erase(it);
                    break;
                }
            }
            if(flag == 0) { //没有打到敌人看下一个子弹
                ++it;
            }
        }
    }

    if(enemy2attack.empty()){ //范围内没有敌人
        return;
    }

    //if(!target_enemy)
    //    target_enemy = enemy2attack[enemy2attack.size() - 1];
    target_enemy = enemy2attack[0];

    ++counter;
    if(counter >= interval) { //间隔过后才发射子弹
        counter = 0;
        /*for(auto enemy : enemy2attack) {
            if(enemy->state == LIVE || enemy->state == BLOCKED) {
                cout << enemy << " enemy be attacked by " << this << " with -" << damage << endl;
                enemy->cur_health -= damage;
            }
        }*/
        //创建子弹 并计算其方向系数k b
        Bullet* b = new Bullet(x + weight/2, y + height/2, 15);
        double delta_x = target_enemy->x - b->x;
        cout << "delta_x is : " << delta_x << endl;
        if(abs(delta_x) < 30) { //斜率为正无穷
            b->direct = (target_enemy->y > b->y)? 1 : -1;
            b->flag = 1;
        }
        else {
            b->k = (target_enemy->y - b->y)/(delta_x);
            b->b = target_enemy->y - target_enemy->x * b->k;
            b->direct = (target_enemy->x > b->x)? 1: -1;
        }

        bullet_all.push_back(b);
    }
    cout << "bullet count: " << bullet_all.size() << endl;
}

//更新
int Tower::update_each() {
    //cur_health -= 1;
    if(state == DEAD){ //死了不再移动
        return 2;
    }

    if(cur_health <= 0){ //敌人死亡
        picture = "../source/Burn.gif";
        state = DEAD;
        return 2;
    }
    cur_block = 0;

    int t_cx = x + weight/2, t_cy = y + height/2; //图元中心坐标
    //阻拦敌人 并且攻击范围内的敌人
    vector<Enemy *> enemy2attack;
    for(auto enemy : enemy_all) {
        //敌人图元的中心坐标
        int e_cx =  enemy->x + enemy->weight/2, e_cy =  enemy->y + enemy->height/2;

        //只拦地面敌人
        if((abs(e_cx - t_cx) < kCellLen/2 && abs(e_cy - t_cy) < kCellLen/2)
                && cur_block < all_block && enemy->type <= 4) {
            enemy->state = BLOCKED;
            cur_block += 1;
        }

        if(enemy->type <= 4) { //只攻击地面敌人
            cout << enemy << "distance is " << Distance(this->x, this->y, enemy->x, enemy->y)
                 << " range is " << this->range << endl;
            //两个单位的中心距离小于攻击范围
            if(Distance(t_cx, t_cy, e_cx, e_cy) < this->range) {
                enemy2attack.push_back(enemy);
            }
        }
    }
    attack_enemys(enemy2attack);
    cout << this << " tower blocks " << cur_block << " enemies!" << endl;
    return false;
}
