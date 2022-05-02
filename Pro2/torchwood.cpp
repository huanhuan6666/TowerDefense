#include "torchwood.h"

//火焰树 我方近战单位 可以攻击地方地面和飞行单位 type = 4
TorchWood::TorchWood(int r, int c, vector<Enemy *>& enemy_all) : Tower(r, c, enemy_all)
{
    type = 4; //火炬树
    step = 0;
    all_health = 100;
    cur_health = 100;
    level = 1;
    damage = 5;
    state = LIVE;
    speed = 0;
    range = kCellLen/2;
    all_block = 1;
    cur_block = 0;
    row = r;
    col = c;
    weight = 65;
    height = 60;

    x = c * kCellLen + (kCellLen - weight)/2; //根据传入的格子坐标得到像素坐标
    y = r * kCellLen + (kCellLen - height)/2;
    picture = "../source/TorchWood (3).png";

    interval = 5; //调用五次
    counter = 0;
}

//无间隔 攻击enemy2attack里的所有敌人
void TorchWood::attack_enemys(vector<Enemy *>& enemy2attack) {
    if(enemy2attack.empty()){
        return;
    }

    for(auto enemy : enemy2attack) {
        if(enemy->state == LIVE || enemy->state == BLOCKED) {
            cout << enemy << " enemy be attacked by " << this << " with -" << damage << endl;
            enemy->cur_health -= damage;
        }
    }
}

int TorchWood::update_each() {
    //cur_health -= 1;
    if(state == DEAD){ //死了不再移动
        return 2;
    }

    if(cur_health <= 0){ //死亡
        picture = "../source/Burn.gif";
        state = DEAD;
        return 2;
    }
    cur_block = 0;

    switch (step) {
    case 0: {
        picture = "../source/TorchWood (1).png";
        step = 1;
        break;
    }
    case 1: {
        picture = "../source/TorchWood (2).png";
        step = 2;
    }
    case 2: {
        picture = "../source/TorchWood (3).png";
        step = 3;
    }
    case 3: {
        picture = "../source/TorchWood (4).png";
        step = 0;
    }
    }

    int t_cx = x + weight/2, t_cy = y + height/2; //图元中心坐标
    //阻拦敌人 并且攻击范围内的敌人
    vector<Enemy *> enemy2attack;
    for(auto& enemy : enemy_all) {
        //敌人图元的中心坐标
        int e_cx =  enemy->x + enemy->weight/2, e_cy =  enemy->y + enemy->height/2;

        if(cur_block < all_block &&
           abs(e_cx - t_cx) < range && abs(e_cy - t_cy) < range)
        {
            enemy->state = BLOCKED;
            cur_block += 1;
        }

        //地面和飞行都可以攻击
        cout << enemy << "distance is " << Distance(this->x, this->y, enemy->x, enemy->y)
             << " range is " << this->range << endl;
        //两个单位的中心距离小于攻击范围
        if(Distance(t_cx, t_cy, e_cx, e_cy) < this->range) {
            enemy2attack.push_back(enemy);
        }

    }

    attack_enemys(enemy2attack);
    cout << this << " tower blocks " << cur_block << " enemies!" << endl;
    return false;
}
