#ifndef FLYAFRAID_H
#define FLYAFRAID_H
#include "enemy.h"

class FlyAfraid : public Enemy
{
public:
    FlyAfraid(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all);
    int update_each() override;
    void attack_tower(vector<Tower *>& tower2attack) override;
};

#endif // FLYAFRAID_H
