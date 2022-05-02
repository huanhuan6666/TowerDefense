#ifndef ENEMYSUPER_H
#define ENEMYSUPER_H
#include "enemy.h"

class EnemySuper : public Enemy
{
public:
    EnemySuper(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all);
    int update_each() override;
    void attack_tower(vector<Tower *>& tower2attack) override;
};

#endif // ENEMYSUPER_H
