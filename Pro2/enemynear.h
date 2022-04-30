#ifndef ENEMYNEAR_H
#define ENEMYNEAR_H
#include "enemy.h"

//近战敌人单位 只能攻击近战塔
class EnemyNear : public Enemy
{
public:
    EnemyNear(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all);
    int update_each() override;
    void attack_tower(vector<Tower *>& tower2attack) override;
};

#endif // ENEMYNEAR_H
