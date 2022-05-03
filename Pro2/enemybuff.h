#ifndef ENEMYBUFF_H
#define ENEMYBUFF_H
#include "enemy.h"

class EnemyBuff : public Enemy
{
public:
    vector<Enemy *>& enemy_all;
    EnemyBuff(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all, vector<Enemy *>& enemy_all);
    int update_each() override;
    //void attack_tower(vector<Tower *>& tower2attack) override;
    void buff_enemy(vector<Enemy *> enemy2buff); //给范围内的敌人加buff
};

#endif // ENEMYBUFF_H
