#include "enemyremote.h"
#include "tower.h"

//地面远程攻击 type = 2 攻击范围大于一格
EnemyRemote::EnemyRemote(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all):
    Enemy(_path, map, tower_all)
{
    type = 2;   //地面远程攻击

    all_health = 100;
    cur_health = 100;
    damage = 3;
    state = LIVE;
    speed = 5;
    step = 0;
    path_index = 0;
    picture = "../source/monster1.png";
    weight = 60;
    height = 60;

    x = kCellLen * path[0].col + (kCellLen - weight)/2;
    y = kCellLen * path[0].row + (kCellLen - height)/2;

    interval = 5; //调用五次
    counter = 0;
    range = 2 * kCellLen;
}

//tower2attack为攻击范围内的塔们 这里的策略是全都攻击一遍
void EnemyRemote::attack_tower(vector<Tower *>& tower2attack) {
    if(tower2attack.empty()){
        return;
    }
    ++counter;
    if(counter >= interval) { //间隔过后才攻击
        counter = 0;
        for(auto tower : tower2attack) {
            if(tower->state == LIVE) {
                cout << tower << " tower be attacked by " << this << " with -" << damage << endl;
                tower->cur_health -= damage;
            }
        }
    }
}


int EnemyRemote::update_each() {
    //根据当前像素位置算出格子下标
    int row = y/kCellLen, col = x/kCellLen;

    if(state == DEAD){ //死了不再移动
        return 2;
    }

    if(cur_health <= 0){ //敌人死亡
        picture = "../source/Burn.gif";
        state = DEAD;
        return 2;
    }

    //寻找范围内所有近战单位塔攻击
    vector<Tower *> tower2attack;
    for(auto tower : tower_all) {
        if(tower->type == 1) { //坚果塔
            //二者中心距离在攻击范围内
            if(Distance(x + weight/2, y + height/2, tower->x + tower->weight/2, tower->y + tower->height/2) < this->range) {
                tower2attack.push_back(tower);
            }
        }
    }
    attack_tower(tower2attack);

    //更新处于path的下一个结点上
    if(row != path[path_index].row || col != path[path_index].col) {
        if(abs(x - (col* kCellLen + (kCellLen - weight)/2)) < 5 &&
           abs(y - (row*kCellLen+ (kCellLen - height)/2)) < 5) //左上角足够靠近时再到下一个
            path_index += 1;
    }


    int direct = path[path_index].direct;
    if(direct == LEFT){
        switch (step) {
        case 0: {
            picture = "../source/monster1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/monster2.png";
            step = 0;
        }
        }
    }else if(direct == RIGHT){
        switch (step) {
        case 0: {
            picture = "../source/monster3.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/monster4.png";
            step = 0;
        }
        }
    }else {
        switch (step) {
        case 0: {
            picture = "../source/monster1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/monster2.png";
            step = 0;
        }
        }
    }

    if(state == BLOCKED) { //被阻挡停止移动
        //state = LIVE;
        return 3;
    }

    int to_end = basic_move();
    return to_end;
}
