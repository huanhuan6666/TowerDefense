#ifndef COMMON_H
#define COMMON_H

#include<QGraphicsItem>
#include<QPainter>
#include<QMovie>
#include<vector>
#include<string>
#include<iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;


//地图配置文件
const string map_file = "../map_config_default.txt";

enum DirectIndex{ //每个格子上的行走方向 -1 不动 0上 1下 2左 3右
    NONE=-1,
    UP=0,
    DOWN,
    LEFT,
    RIGHT,

};

const vector<vector<int>> directions = { //四个方向
    {0, -1},    //上
    {0, 1},     //下
    {-1, 0},    //左
    {1, 0},     //右
};

enum STATE{ //格子的状态
    WALL    =0,     //不能放单位

    PATH    =1,     //路径 可以放近战单位
    BEGIN   =2,     //路径起点
    END     =3,     //路径终点

    REMOTE  =4,     //我方远程单位
};

enum ENEMY_STATE { //敌人的状态
    DEAD = 0,
    LIVE = 1,
    BLOCKED = 2, //被阻拦
};

const int kCellLen = 80; //单元格边长

struct Pos_t {
    int row, col;
    int direct;     //指示在这个格子的运动方向
    Pos_t() :row(0), col(0), direct(NONE) { ; }
    Pos_t(int r, int c): row(r), col(c), direct(NONE) { ; }
};

inline ostream& operator<<(ostream& out, const Pos_t &p) {
    out << "[" << p.row << ", " << p.col << "]" <<  p.direct << " ";
    return out;
}

class Cell : public QGraphicsItem {
public:
    int row, col;       //格子坐标
    int state;      //格子状态
    int planted;    //是否种植塔 只有PATH和REMOTE才有效
    int next_move;   //敌人在此格子上的运动方向
    QPixmap* looks;          //格子的图片
    QMovie* movie = nullptr;
    Cell();
    Cell(const Cell& c);
    Cell(int x, int y, int s, int d);


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void setMovie(QString path);
};

inline ostream& operator<<(ostream& out, const Cell &c) {
    out << "[" << c.row << ", " << c.col << "]" << "s:" << c.state << " ";
    return out;
}

// 计算点(x1, y1)和 (x2, y2)的距离
inline double Distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
#endif // COMMON_H
