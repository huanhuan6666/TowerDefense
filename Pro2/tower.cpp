#include "tower.h"
#include "enemy.h"
Tower::Tower(int r, int c, vector<Enemy *>& enemy_all): enemy_all(enemy_all) {
    all_health = 200;
    cur_health = 200;
    level = 1;
    damage = 1;
    state = LIVE;
    speed = 0;
    range = kCellLen/2;
    all_block = 3;
    cur_block = 0;
    row = r;
    col = c;
    weight = 40;
    height = 40;

    x = c * kCellLen + (kCellLen - weight)/2; //根据传入的格子坐标得到像素坐标
    y = r * kCellLen + (kCellLen - height)/2;
    picture = "../source/WallNut.gif";


    type = 1; //近战单位塔
    interval = 5; //调用五次
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
    if(enemy2attack.empty()){
        return;
    }
    ++counter;
    if(counter >= interval) { //间隔过后才攻击
        counter = 0;
        for(auto enemy : enemy2attack) {
            if(enemy->state == LIVE || enemy->state == BLOCKED) {
                cout << enemy << " enemy be attacked by " << this << " with -" << damage << endl;
                enemy->cur_health -= damage;
            }
        }
    }
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

        if(((e_cx == t_cx && abs(e_cy - t_cy) < range) ||(e_cy == t_cy && abs(e_cx - t_cx) < range))
                && cur_block < all_block) {
            enemy->state = BLOCKED;
            cur_block += 1;
        }

        if(enemy->type <= 4) { //地面敌人
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
