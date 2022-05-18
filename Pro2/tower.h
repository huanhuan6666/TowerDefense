#ifndef TOWER_H
#define TOWER_H
#include<QLabel>
#include<QMovie>
#include<QPainter>
#include<QGraphicsItem>
#include "common.h"
#include "enemy.h"
//塔单位不会移动

class Bullet;

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

    int price;      //价格
    int level;      //等级 初始为1
    bool selected;  //是否被鼠标选中
    int attacked;   //被攻击

    QString picture;    //图片路径
    double angle;       //旋转角度
    int weight;
    int height;
    int step;       //用于切换图片显示动态效果
    vector<Enemy *>& enemy_all;       //场景中所有敌人
    Enemy *target_enemy;              //要攻击的目标敌人 可以有不同的选择策略

    vector<Bullet *> bullet_all;      //自己发射出的所有子弹

public:
    Tower(int r, int c, vector<Enemy *>& enemy_all);
    virtual ~Tower();
    virtual int update_each(); //更新
    virtual void attack_enemys(vector<Enemy *>& enemy2attack); //攻击enemys数组里的敌人
};

//子弹类
class Bullet {
public:
    int x, y;       //子弹的坐标
    int w, h;       //子弹图片的宽和高
    int flag;       //是否斜率为正无穷
    int direct;     //子弹方向 =1沿x正方向 =-1沿x负方向
    int speed;      //子弹的移动速度
    double k, b;    //子弹用运行轨迹 利用方程y = k*x+b计算

    QString picture;    //子弹图片路径

public:
    Bullet(int _x, int _y, int _s) : x(_x), y(_y), speed(_s){
        k = b = 0;
        flag = 0;
        w = 20;
        h = 20;
    }

};

#endif // TOWER_H
