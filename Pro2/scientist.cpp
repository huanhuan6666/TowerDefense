#include "scientist.h"

//我方近战塔科学家 只能攻击地面单位 type=3
Scientist::Scientist(int r, int c, vector<Enemy *>& enemy_all) : Tower(r, c, enemy_all)
{
    type = 3; //科学家
    direction = RIGHT;  //攻击朝向默认为右

    step = 0;
    all_health = 100;
    cur_health = 100;
    level = 1;
    damage = 2;
    state = LIVE;
    speed = 0;
    range = 200;
    all_block = 1;
    cur_block = 0;
    row = r;
    col = c;
    weight = 65;
    height = 60;

    x = c * kCellLen + (kCellLen - weight)/2; //根据传入的格子坐标得到像素坐标
    y = r * kCellLen + (kCellLen - height)/2;
    picture = "../source/two_r0.png";

    wave = "../source/swaveright(1).png";
    wave_step = 0;

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
    //vector<Enemy *> enemy2attack;
    bool haveEnemy = false;
    for(auto& enemy : enemy_all) {
        //敌人图元的中心坐标
        int e_cx =  enemy->x + enemy->weight/2, e_cy =  enemy->y + enemy->height/2;
        int e_row = e_cy / kCellLen, e_col = e_cx / kCellLen;
        //攻击范围内的地面敌人

        if(enemy->type <= 4) { //只能攻击朝向内的地面敌人
            switch (direction) {
            case UP : { //同一列上方
                if(e_col == col && e_row <= row &&
                   t_cy - e_cy > 0 && t_cy - e_cy < range) {
                    enemy->cur_health -= damage;
                    haveEnemy = true;
                    if(cur_block < all_block && t_cy - e_cy < kCellLen/2) {
                        enemy->state = BLOCKED;
                        cur_block += 1;
                    }
                }
                break;
            }
            case DOWN : { //同一列下方
                if(e_col == col && e_row >= row &&
                   e_cy - t_cy > 0 && e_cy - t_cy < range) {
                    enemy->cur_health -= damage;
                    haveEnemy = true;
                    if(cur_block < all_block && e_cy - t_cy < kCellLen/2) {
                        enemy->state = BLOCKED;
                        cur_block += 1;
                    }
                }
                break;
            }
            case LEFT : { //同一行左侧
                if(e_row == row && e_col <= col &&
                   t_cx - e_cx > 0 && t_cx - e_cx < range) {
                    enemy->cur_health -= damage;
                    haveEnemy = true;
                    if(cur_block < all_block && t_cx - e_cx < kCellLen/2) {
                        enemy->state = BLOCKED;
                        cur_block += 1;
                    }
                }
                break;
            }
            case RIGHT : { //同一行右侧
                if(e_row == row && e_col >= col &&
                   e_cx - t_cx > 0 && e_cx - t_cx < range) {
                    enemy->cur_health -= damage;
                    haveEnemy = true;
                    if(cur_block < all_block && e_cx - t_cx < kCellLen/2) {
                        enemy->state = BLOCKED;
                        cur_block += 1;

                    }
                    cout << "cur block is " << cur_block << endl;
                }
                break;
            }
            }
        }

    }

    if(haveEnemy) {
        state = ATTACK;
    }else {
        state = LIVE;
    }

    if(state == ATTACK) { //攻击时发波
        if(direction == RIGHT) {
            switch (wave_step) {
            case 0: {
                wave = "../source/swaveright(1).png";
                wave_step = 1;
                break;
            }
            case 1: {
                wave = "../source/swaveright(2).png";
                wave_step = 2;
            }
            case 2: {
                wave = "../source/swaveright(3).png";
                wave_step = 0;
            }
            }
        }
        else if(direction == LEFT) {
            switch (wave_step) {
            case 0: {
                wave = "../source/swave1left (1).png";
                wave_step = 1;
                break;
            }
            case 1: {
                wave = "../source/swave1left (2).png";
                wave_step = 2;
            }
            case 2: {
                wave = "../source/swave1left (3).png";
                wave_step = 0;
            }
            }
        }
        else if(direction == UP) {
            switch (wave_step) {
            case 0: {
                wave = "../source/swaveup (1).png";
                wave_step = 1;
                break;
            }
            case 1: {
                wave = "../source/swaveup (2).png";
                wave_step = 2;
            }
            case 2: {
                wave = "../source/swaveup (3).png";
                wave_step = 0;
            }
            }
        }
        else if(direction == DOWN) {
            switch (wave_step) {
            case 0: {
                wave = "../source/swavedown(1).png";
                wave_step = 1;
                break;
            }
            case 1: {
                wave = "../source/swavedown(2).png";
                wave_step = 2;
            }
            case 2: {
                wave = "../source/swavedown(3).png";
                wave_step = 0;
            }
            }
        }
    }
    return false;
}
