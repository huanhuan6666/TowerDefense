#ifndef TOWERNUT_H
#define TOWERNUT_H
#include "tower.h"

class TowerNut : public Tower
{
public:
    TowerNut(int r, int c, vector<Enemy *>& enemy_all);
    int update_each() override; //更新
};

#endif // TOWERNUT_H
