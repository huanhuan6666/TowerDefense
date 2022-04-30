#include "enemy.h"
#include "tower.h"

//设置
Enemy::Enemy(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all):
    path(_path), game_map(map->game_map), tower_all(tower_all)
{
    all_health = 100;
    cur_health = 100;
    damage = 3;
    state = LIVE;
    speed = 5;
    step = 0;
    path_index = 0;
    picture = "../source/Pig1 1.png";
    weight = 40;
    height = 40;

    x = kCellLen * path[0].col + (kCellLen - weight)/2;
    y = kCellLen * path[0].row + (kCellLen - height)/2;

    type = 1;   //地面进程攻击
    interval = 5; //调用五次
    counter = 0;
    range = 100;
}
//析构函数
Enemy::~Enemy() {
    ;
}

//按照路径移动
bool Enemy::basic_move() {
    switch (path[path_index].direct) {
        case NONE: { //到达终点 终点方向为NONE==-1
            cout << "to end!!!" << endl;
            return true;
            break;
        }
        case UP: {
            y -= speed;
            break;
        }
        case DOWN: {
            y += speed;
            break;
        }
        case LEFT: {
            x -= speed;
            break;
        }
        case RIGHT: {
            x += speed;
            break;
        }
    }
    //cout << this << " move " << x << ", " << y << endl;
    return false;   //未到达终点
}

//攻击tower2attack里的塔
void Enemy::attack_tower(vector<Tower *>& tower2attack) {
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

//每个敌人根据自己的路径移动 死亡返回2 到达终点返回1 仍在行走返回0 被阻挡返回3
int Enemy::update_each() {
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

    /*
    for(auto tower : tower_all) {
        //cout << "tower count: " << tower_all.size() << endl;
        //cout << "enemy row: " << row << " col: " << col << " with " << tower->row << ":" << tower->col << endl;
        if(row == tower->row && col == tower->col) {
            if(tower->cur_block < tower->all_block) {
                state = BLOCKED;
                cout << this << "be blocked!!!" << endl;
                break;
            }
        }
    }
    */
    vector<Tower *> tower2attack;
    for(auto tower : tower_all) {
        if(tower->type == 1) { //近战单位塔

            if(Distance(this->x, this->y, tower->x, tower->y) < this->range) {
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
    cout << "path_index is " << path_index << endl;

    int direct = path[path_index].direct;
    if(direct == LEFT){
        switch (step) {
        case 0: {
            picture = "../source/Pig1 1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/Pig2 1.png";
            step = 0;
        }
        }
    }else if(direct == RIGHT){
        switch (step) {
        case 0: {
            picture = "../source/Pig3 1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/Pig4 1.png";
            step = 0;
        }
        }
    }else {
        switch (step) {
        case 0: {
            picture = "../source/Pig1 1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/Pig2 1.png";
            step = 0;
        }
        }
    }

    if(state == BLOCKED) { //被阻挡停止移动
        //state = LIVE;
        return 3;
    }

    bool to_end = basic_move();
    return to_end;
}
