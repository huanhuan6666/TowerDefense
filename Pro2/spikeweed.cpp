#include "spikeweed.h"

//地刺塔 type = 2
SpikeWeed::SpikeWeed(int r, int c, vector<Enemy *>& enemy_all) : Tower(r, c, enemy_all)
{
    type = 2; //地刺
    step = 0;
    all_health = 100;
    cur_health = 100;
    level = 1;
    damage = 5;
    state = LIVE;
    speed = 0;
    range = kCellLen/2;
    all_block = 0;
    cur_block = 0;
    row = r;
    col = c;
    weight = 65;
    height = 60;

    x = c * kCellLen + (kCellLen - weight)/2; //根据传入的格子坐标得到像素坐标
    y = r * kCellLen + (kCellLen - height)/2;
    picture = "../source/Spikerock1.png";

    price = 110;
    interval = 5; //调用五次
    counter = 0;
}

//更新
int SpikeWeed::update_each() {

    if(state == DEAD){ //死了
        return 2;
    }

    int t_cx = x + weight/2, t_cy = y + height/2; //图元中心坐标

    // 并且攻击范围内的敌人
    vector<Enemy *> enemy2attack;
    for(auto& enemy : enemy_all) {
        //敌人图元的中心坐标
        int e_cx =  enemy->x + enemy->weight/2, e_cy =  enemy->y + enemy->height/2;
        //攻击范围内的地面敌人
        if(enemy->type <= 4  &&
           abs(e_cx - t_cx) < range && abs(e_cy - t_cy) < range)
        {
            enemy->cur_health -= damage;
        }

    }

    return false;
}
