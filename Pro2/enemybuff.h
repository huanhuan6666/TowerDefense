#ifndef ENEMYBUFF_H
#define ENEMYBUFF_H
#include "enemy.h"

class EnemyBuff : public Enemy
{
public:
    EnemyBuff(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all);
    int update_each() override;
    void attack_tower(vector<Tower *>& tower2attack) override;
};

#endif // ENEMYBUFF_H
