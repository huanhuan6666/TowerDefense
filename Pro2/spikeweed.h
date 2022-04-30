#ifndef SPIKEWEED_H
#define SPIKEWEED_H
#include "tower.h"

//地刺 阻拦数为0 不会收到攻击
class SpikeWeed : public Tower
{
public:
    SpikeWeed(int r, int c, vector<Enemy *>& enemy_all);
    int update_each() override; //更新
};

#endif // SPIKEWEED_H
