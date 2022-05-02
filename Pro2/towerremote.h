#ifndef TOWERREMOTE_H
#define TOWERREMOTE_H
#include "tower.h"

//普通远程塔 只攻击敌方地面单位 type = 5
class TowerRemote : public Tower
{
public:
    TowerRemote(int r, int c, vector<Enemy *>& enemy_all);
    int update_each() override; //更新
    void attack_enemys(vector<Enemy *>& enemy2attack) override;
};

#endif // TOWERREMOTE_H
