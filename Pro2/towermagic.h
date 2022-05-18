#ifndef TOWERMAGIC_H
#define TOWERMAGIC_H
#include "tower.h"


class TowerMagic : public Tower
{
public:
    int& can_summon;
    TowerMagic(int r, int c, vector<Enemy *>& enemy_all, int&can_summon);
    int update_each() override; //更新
};

#endif // TOWERMAGIC_H
