#ifndef ENEMY_H
#define ENEMY_H

#include "map.h"
#include "common.h"

class Tower;
class Bullet;

class Enemy
{
public:
    int x, y;          //图片左上角像素坐标
    int all_health;    //初始生命值
    int cur_health;    //实时生命值
    int damage;     //攻击力
    int state;      //状态
    int speed;      //移动速度
    int type;       //敌人类型 1~4为地面 5~6为飞行
    int range;      //敌人攻击范围

    QString picture;    //图片路径
    int weight;         //图片宽和高
    int height;
    int step;           //用于切换图片显示动态效果

    vector<Pos_t> path;     //自己的移动路线
    int path_index;         //在路径上的第几格
    int interval;   //攻击间隔
    int counter;    //计数器

    Map* map; //地图组件
    const vector<vector<Cell>>& game_map;   //原始的cell二维数组地图
    vector<Tower *> &tower_all;       //场景中所有塔
    Tower *target_tower;              //要攻击的目标敌人 可以有不同的选择策略

    vector<Bullet *> bullet_all;      //自己发射出的所有子弹

public:
    Enemy(const vector<Pos_t>& _path, Map *map, vector<Tower *>& tower_all);
    virtual ~Enemy();
    bool basic_move();
    virtual int update_each(); //敌人根据自己的path移动
    virtual void attack_tower(vector<Tower *>& tower2attack); //攻击范围内的塔
};

#endif // ENEMY_H
