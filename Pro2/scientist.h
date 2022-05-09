#ifndef SCIENTIST_H
#define SCIENTIST_H
#include "tower.h"

class Scientist : public Tower
{
public:
    int direction;  //科学家有攻击朝向
    QString wave;   //发波的图片
    int wave_step;  //发波步骤
    Scientist(int r, int c, vector<Enemy *>& enemy_all);
    int update_each() override; //更新
};

#endif // SCIENTIST_H
