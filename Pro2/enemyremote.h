#ifndef ENEMYREMOTE_H
#define ENEMYREMOTE_H
#include "enemy.h"

class EnemyRemote : public Enemy
{
public:
    EnemyRemote(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all);
    int update_each() override;
    void attack_tower(vector<Tower *>& tower2attack) override;
};

#endif // ENEMYREMOTE_H
