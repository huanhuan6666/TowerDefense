#include "scientist.h"

//我方近战塔科学家 只能攻击地面单位 type=4
Scientist::Scientist(int r, int c, vector<Enemy *>& enemy_all) : Tower(r, c, enemy_all)
{
    type = 4; //科学家
    step = 0;
    all_health = 100;
    cur_health = 100;
    level = 1;
    damage = 2;
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
    picture = "../source/two_r0.png";

    price = 70;
    interval = 5; //调用五次
    counter = 0;
}

//更新
int Scientist::update_each() {
    //cur_health -= 1;
    if(state == DEAD){ //死了不再移动
        return 2;
    }

    if(cur_health <= 0){ //死亡
        picture = "../source/Burn.gif";
        state = DEAD;
        return 2;
    }


    switch (step) {
    case 0: {
        picture = "../source/two_r0.png";
        step = 1;
        break;
    }
    case 1: {
        picture = "../source/two_r1.png";
        step = 2;
    }
    case 2: {
        picture = "../source/two_r2.png";
        step = 3;
    }
    case 3: {
        picture = "../source/two_r3.png";
        step = 0;
    }
    }

    int t_cx = x + weight/2, t_cy = y + height/2; //图元中心坐标

    cur_block = 0;

    //阻拦攻击范围内的敌人
    vector<Enemy *> enemy2attack;
    for(auto& enemy : enemy_all) {
        //敌人图元的中心坐标
        int e_cx =  enemy->x + enemy->weight/2, e_cy =  enemy->y + enemy->height/2;
        //攻击范围内的地面敌人
        if(enemy->type <= 4  &&
           abs(e_cx - t_cx) < range && abs(e_cy - t_cy) < range)
        {
            enemy->cur_health -= damage;
            if(cur_block < all_block) {
                enemy->state = BLOCKED;
                cur_block += 1;
            }
            cout << "block count is : " << cur_block << endl;
        }

    }

    return false;
}
