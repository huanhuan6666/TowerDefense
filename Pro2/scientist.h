#ifndef SCIENTIST_H
#define SCIENTIST_H
#include "tower.h"

class Scientist : public Tower
{
public:
    Scientist(int r, int c, vector<Enemy *>& enemy_all);
    int update_each() override; //更新
};

#endif // SCIENTIST_H
