#ifndef TORCHWOOD_H
#define TORCHWOOD_H
#include "tower.h"

//火炬树 近战单位 地方地面单位和飞行单位都可以攻击 type = 3
class TorchWood : public Tower
{
public:
    TorchWood(int r, int c, vector<Enemy *>& enemy_all);
    int update_each() override; //更新
    void attack_enemys(vector<Enemy *>& enemy2attack) override;
};

#endif // TORCHWOOD_H
