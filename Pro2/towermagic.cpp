#include "towermagic.h"
#include "torchwood.h"

//可自动放置我方单位的远程单位 type = 7 无攻击能力
TowerMagic::TowerMagic(int r, int c, vector<Enemy *>& enemy_all, int& can_summon) :
    Tower(r, c, enemy_all), can_summon(can_summon)
{
    type = 7;
    step = 0;
    all_health = 100;
    cur_health = 100;
    level = 1;
    damage = 15;
    state = LIVE;
    speed = 0;
    range = 40;
    all_block = 1;
    cur_block = 0;
    row = r;
    col = c;
    weight = 60;
    height = 60;

    x = c * kCellLen + (kCellLen - weight)/2; //根据传入的格子坐标得到像素坐标
    y = r * kCellLen + (kCellLen - height)/2;
    angle = 0;

    picture = "../source/magic-tower.png";

    price = 300;
    interval = 30; //召唤CD
    counter = 0;
}


int TowerMagic::update_each() {
    if(state == DEAD){ //死了不再移动
        return 2;
    }

    if(cur_health <= 0){ //死亡
        picture = "../source/Burn.gif";
        state = DEAD;
        return 2;
    }

    /*
    if(interval > 0) {
        interval -= 1;
        cout << "interval" << interval << endl;
    }
    else {
        interval = 30;
        for(auto& cell_row : game_map) {
            for(auto& cell : cell_row) {
                if(cell.state == PATH && cell.planted == 0) { //确定了位置

                    cell.planted = 1;
                    to_type = 1;
                    to_row = cell.row;
                    to_col = cell.col;
                    getto = true;
                    cout << "tower size is : " << tower_all.size()
                         << "row col: " << to_row << " " << to_col << endl;
                    Tower *test_tower = new TorchWood(to_row, to_col, enemy_all);
                    cout << "aaa " << test_tower->row << " " << test_tower->col << endl;
                    tower_all.push_back(test_tower);

                    break;
                }
            }
            if(getto)
                break;
        }
    }
    */
    if(can_summon <= 90) {
        picture = "../source/magic-tower.png";
    }
    else {
        picture = "../source/magic-open.png";
    }

    if(state == BEEN_ATTACKED) {
        state = LIVE;
    }
    return false;
}

