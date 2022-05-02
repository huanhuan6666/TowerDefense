#ifndef TOWERSUPER_H
#define TOWERSUPER_H
#include "tower.h"

class TowerSuper : public Tower
{
public:
    TowerSuper(int r, int c, vector<Enemy *>& enemy_all);
    int update_each() override; //更新
    void attack_enemys(vector<Enemy *>& enemy2attack) override;
};

#endif // TOWERSUPER_H
