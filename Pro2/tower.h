#ifndef TOWER_H
#define TOWER_H
#include<QLabel>
#include<QMovie>
#include<QPainter>
#include<QGraphicsItem>
#include "common.h"
#include "enemy.h"
//塔单位不会移动

class Tower
{
public:
    int x, y;          //像素坐标
    int row, col;
    int type;          //类型
    int all_health;    //初始生命值
    int cur_health;    //实时生命值 初始为all_health

    int all_block;  //可以阻拦的最大敌人数
    int cur_block;  //实时阻拦数 初始为0

    int damage;     //攻击力
    int state;      //状态
    int speed;      //移动速度
    int range;      //攻击范围
    int interval;   //攻击间隔
    int counter;    //计数器
    int level;      //等级 初始为1

    QString picture;    //图片路径
    int weight;
    int height;

    vector<Enemy *>& enemy_all;       //场景中所有敌人

public:
    Tower(int r, int c, vector<Enemy *>& enemy_all);
    virtual ~Tower();
    virtual int update_each(); //更新
    virtual void attack_enemys(vector<Enemy *>& enemy2attack); //攻击enemys数组里的敌人
};

#endif // TOWER_H
