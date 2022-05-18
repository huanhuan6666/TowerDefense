#include "flybrave.h"
#include "tower.h"

//敌方飞行恋战单位 type = 6
FlyBrave::FlyBrave(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all):
    Enemy(_path, map, tower_all)
{
    type = 6;

    all_health = 100;
    cur_health = 100;
    damage = 5;
    state = LIVE;
    speed = 5;
    step = 0;
    path_index = 0;
    picture = "../source/fly-bravel1.png";
    weight = 60;
    height = 60;

    x = kCellLen * path[0].col + (kCellLen - weight)/2;
    y = kCellLen * path[0].row + (kCellLen - height)/2;

    interval = 5; //调用五次
    counter = 0;
    range = 220;
}

//tower2attack为攻击范围内的塔们 这里的策略是全都攻击一遍
void FlyBrave::attack_tower(vector<Tower *>& tower2attack) {
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

        if(Distance((*it)->x+(*it)->w/2, (*it)->y+(*it)->h/2, x+weight/2, y+height/2) > range) { //清除超出射程的子弹
            delete (*it);
            it = bullet_all.erase(it);
        }
        else {
            int flag = 0;   //是否打到了敌人
            for(auto& tower : tower2attack) {
                if(InArea((*it)->x+(*it)->w/2, (*it)->y+(*it)->h/2,
                          tower->x, tower->y, tower->weight, tower->height)) //子弹碰到了敌人
                {
                    tower->cur_health -= damage;
                    tower->state = BEEN_ATTACKED;
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

    if(tower2attack.empty()){ //范围内没有敌人
        return;
    }

    for(auto &tower : tower2attack) {
        if(tower->state != DEAD) {
            target_tower = tower;
            break;
        }
    }

    ++counter;
    if(counter >= interval) { //间隔过后才发射子弹
        counter = 0;
        //创建子弹 并计算其方向系数k b
        Bullet* b = new Bullet(x + weight/2, y + height/2, 15);
        b->picture = "../source/bullet_2";

        double delta_x = target_tower->x + target_tower->weight/2 - b->x;
        double delta_y = target_tower->y + target_tower->height/2 - b->y;
        //cout << "delta_x is : " << delta_x << endl;
        if(abs(delta_x) < 30) { //斜率为正无穷
            b->flag = 1;
            b->direct = (target_tower->y > b->y)? 1 : -1;
        }
        else {
            b->k = (delta_y)/(delta_x);
            b->b = target_tower->y + target_tower->height/2 - (target_tower->x + target_tower->weight/2)* b->k;
            if(abs(b->k) <= 1) { //|斜率| <= 1
                b->flag = 2;
                b->direct = (target_tower->x > b->x)? 1: -1;
            }
            else {
                b->flag = 3;
                b->direct = (target_tower->y > b->y)? 1: -1;
            }
        }
        bullet_all.push_back(b);

    }
    //cout << "bullet count: " << bullet_all.size() << endl;
}


int FlyBrave::update_each() {
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

    //寻找范围内所有近战塔和远程塔攻击
    vector<Tower *> tower2attack;
    for(auto tower : tower_all) {
        if(tower->type != 2) { //地刺不能攻击
            if(Distance(x + weight/2, y + height/2, tower->x + tower->weight/2, tower->y + tower->height/2) < this->range) {
                tower2attack.push_back(tower);
            }
        }
    }
    attack_tower(tower2attack);

    //好战的 攻击时停下脚步
    if(!tower2attack.empty()) {
        state = ATTACK;
    }
    else {
        state = LIVE;
    }

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
            if(attacked)
                attacked = 0;
            picture = "../source/fly-bravel1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/fly-bravel2.png";
            step = 2;
        }
        case 2: {
            picture = "../source/fly-bravel3.png";
            step = 0;
        }
        }
    }else if(direct == RIGHT){
        switch (step) {
        case 0: {
            if(attacked)
                attacked = 0;
            picture = "../source/fly-braver1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/fly-braver2.png";
            step = 0;
        }
        case 2: {
            picture = "../source/fly-braver3.png";
            step = 0;
        }
        }
    }else {
        switch (step) {
        case 0: {
            if(attacked)
                attacked = 0;
            picture = "../source/fly-bravel1.png";
            step = 1;
            break;
        }
        case 1: {
            picture = "../source/fly-bravel2.png";
            step = 0;
        }
        case 2: {
            picture = "../source/fly-bravel3.png";
            step = 0;
        }
        }
    }

    if(state == BLOCKED || state == ATTACK) { //被阻挡停止移动
        return 3;
    }

    int to_end = basic_move();
    return to_end;
}
