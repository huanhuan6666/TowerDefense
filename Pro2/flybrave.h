#ifndef FLYBRAVE_H
#define FLYBRAVE_H
#include "enemy.h"

class FlyBrave : public Enemy
{
public:
    FlyBrave(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all);
    int update_each() override;
    void attack_tower(vector<Tower *>& tower2attack) override;
};

#endif // FLYBRAVE_H
