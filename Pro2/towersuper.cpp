#include "towersuper.h"

//超级远程塔 可以攻击敌方近战单位和远程单位 type=6
TowerSuper::TowerSuper(int r, int c, vector<Enemy *>& enemy_all) : Tower(r, c, enemy_all)
{
    type = 6; //超级远程塔
    step = 0;
    all_health = 100;
    cur_health = 100;
    level = 1;
    damage = 15;
    state = LIVE;
    speed = 0;
    range = 300;
    all_block = 1;
    cur_block = 0;
    row = r;
    col = c;
    weight = 60;
    height = 40;

    x = c * kCellLen + (kCellLen - weight)/2; //根据传入的格子坐标得到像素坐标
    y = r * kCellLen + (kCellLen - height)/2;
    angle = 0;

    picture = "../source/TFireBottle.png";

    price = 150;
    interval = 4; //调用五次
    counter = 0;
}

//攻击enemy2attack里的敌人
void TowerSuper::attack_enemys(vector<Enemy *>& enemy2attack) {
    for(auto it = bullet_all.begin(); it != bullet_all.end(); ) {
        //移动子弹
        if((*it)->flag == 1) { //斜率为正无穷
            (*it)->y += (*it)->speed * (*it)->direct;
        }
        else if((*it)->flag == 2) { //|斜率| <= 1 在x方向变
            (*it)->x += (*it)->direct * (*it)->speed;
            (*it)->y = (*it)->k * (*it)->x + (*it)->b;
        }
        else { //|斜率| > 1 在y方向变
            (*it)->y += (*it)->direct * (*it)->speed;
            (*it)->x = 1/(*it)->k * (*it)->y - (*it)->b/(*it)->k;
        }

        if(Distance((*it)->x+(*it)->w/2, (*it)->y+(*it)->h/2, x, y) > range) { //清除超出射程的子弹
            delete (*it);
            it = bullet_all.erase(it);
        }
        else {
            int flag = 0;   //是否打到了敌人
            for(auto& enemy : enemy2attack) {
                if(InArea((*it)->x+(*it)->w/2, (*it)->y+(*it)->h/2,
                          enemy->x, enemy->y, enemy->weight, enemy->height)) //子弹碰到了敌人
                {
                    if(cur_health < 40) //低血量伤害提升
                        enemy->cur_health -= 25;
                    else
                        enemy->cur_health -= damage;

                    //enemy->state = BEEN_ATTACKED;
                    enemy->attacked = 1;
                    //(*it)->picture = "../source/bullet-boom.png";
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
    picture = "../source/TFireBottle.png";

    if(enemy2attack.empty()){ //范围内没有敌人
        return;
    }

    /*
    for(auto &enemy : enemy2attack) {
        if(enemy->state != DEAD) {
            target_enemy = enemy;
            break;
        }
    }*/

    double closest = range;
    int t_cx = x + weight/2, t_cy = y + height/2; //图元中心坐标
    for(auto &enemy : enemy2attack) { //寻找最近的敌人
        int e_cx =  enemy->x + enemy->weight/2, e_cy =  enemy->y + enemy->height/2;
        //攻击范围内的地面敌人
        double dis = Distance(t_cx, t_cy, e_cx, e_cy);
        if(enemy->state != DEAD  && dis < closest) {
            target_enemy = enemy;
            closest = dis;
        }
    }

    ++counter;
    if(counter >= interval) { //间隔过后才发射子弹
        counter = 0;
        //创建子弹 并计算其方向系数k b
        Bullet* b = new Bullet(x + weight/2, y + height/2, 15);
        b->picture = "../source/bullet_1";
        angle = atan2(target_enemy->y + target_enemy->height/2 - (y + height/2),
                      target_enemy->x + target_enemy->weight/2 - (x + weight/2)) * 180/PI;

        double delta_x = target_enemy->x + target_enemy->weight/2 - b->x;
        double delta_y = target_enemy->y + target_enemy->height/2 - b->y;

        cout << "delta_x is : " << delta_x << endl;
        if(abs(delta_x) < 15) { //斜率为正无穷
            b->direct = (target_enemy->y > b->y)? 1 : -1;
            b->flag = 1;
        }
        else {
            b->k = (delta_y)/(delta_x);
            b->b = target_enemy->y + target_enemy->height/2 - (target_enemy->x + target_enemy->weight/2)* b->k;
            if(abs(b->k) <= 1) { //|斜率| <= 1
                b->flag = 2;
                b->direct = (target_enemy->x > b->x)? 1: -1;
            }
            else {
                b->flag = 3;
                b->direct = (target_enemy->y > b->y)? 1: -1;
            }
        }
        switch (step) {
        case 0: {
            picture = "../source/TFireBottle-shoot1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/TFireBottle-shoot2.png";
            step = 0;
        }
        }

        bullet_all.push_back(b);
    }
    cout << "bullet count: " << bullet_all.size() << endl;
}


//更新
int TowerSuper::update_each() {
    //cur_health -= 1;
    if(state == DEAD){ //死了不再移动
        return 2;
    }

    if(cur_health <= 0){ //死亡
        picture = "../source/Burn.gif";
        state = DEAD;
        return 2;
    }


    int t_cx = x + weight/2, t_cy = y + height/2; //图元中心坐标

    cur_block = 0;

    //远程塔无法阻拦 只能攻击敌人
    vector<Enemy *> enemy2attack;
    for(auto& enemy : enemy_all) {
        //敌人图元的中心坐标
        int e_cx =  enemy->x + enemy->weight/2, e_cy =  enemy->y + enemy->height/2;
        //地面敌人和飞行敌人都能攻击
        if(Distance(t_cx, t_cy, e_cx, e_cy) < this->range) {
            enemy2attack.push_back(enemy);
        }

    }

    if(state == BEEN_ATTACKED) {
        state = LIVE;
    }

    attack_enemys(enemy2attack);
    return false;

}
