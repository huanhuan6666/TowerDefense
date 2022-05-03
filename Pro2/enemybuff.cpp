#include "enemybuff.h"
#include "tower.h"

//给敌人加buff的敌方单位 type = 4
EnemyBuff::EnemyBuff(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all, vector<Enemy *>& enemy_all):
    Enemy(_path, map, tower_all), enemy_all(enemy_all)
{
    type = 4;   //给敌人加buff的

    all_health = 100;
    cur_health = 100;
    damage = 10;
    state = LIVE;
    speed = 7;
    step = 0;
    path_index = 0;
    picture = "../source/buffl1.png";
    weight = 60;
    height = 60;

    x = kCellLen * path[0].col + (kCellLen - weight)/2;
    y = kCellLen * path[0].row + (kCellLen - height)/2;

    interval = 5; //调用五次
    counter = 0;
    range = 200;
}


//给范围内所有敌人加血
void EnemyBuff::buff_enemy(vector<Enemy *> enemy2buff) {
    if(enemy2buff.empty()){
        return;
    }
    ++counter;
    if(counter >= interval) { //间隔过后才加buff
        counter = 0;
        for(auto enemy : enemy2buff) {
            if(enemy->state == LIVE && enemy->cur_health + damage < enemy->all_health) {
                enemy->cur_health += damage;
            }
        }
    }
}
int EnemyBuff::update_each() {
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

    //寻找范围内所有敌人并加buff
    vector<Enemy *> enemy2buff;
    for(auto enemy : enemy_all) {
        if(enemy->type != 4) { //不能给同类型加buff
            //二者中心距离在攻击范围内
            if(Distance(x + weight/2, y + height/2, enemy->x + enemy->weight/2, enemy->y + enemy->height/2) < this->range) {
                enemy2buff.push_back(enemy);
            }
        }
    }
    buff_enemy(enemy2buff);


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
            picture = "../source/buffl1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/buffl2.png";
            step = 0;
        }
        }
    }else if(direct == RIGHT){
        switch (step) {
        case 0: {
            picture = "../source/buffr1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/buffr1.png";
            step = 0;
        }
        }
    }else {
        switch (step) {
        case 0: {
            picture = "../source/buffl1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/buffl2.png";
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
