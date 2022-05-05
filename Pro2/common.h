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
const double PI = 3.14;
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
    FLY_PATH=5,     //飞行路径
};

enum ENEMY_STATE { //敌人的状态
    DEAD = 0,
    LIVE = 1,
    BLOCKED = 2, //被阻拦
    ATTACK = 3,  //好战的敌人攻击时会停下来
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

// 计算点(x1, y1)是否在左上角为 (x2, y2) 宽为w 高为h的区域内
inline bool InArea(double x1, double y1, double x2, double y2, double w, double h) {
    return (x1 >= x2 && x1 <= x2 + w) &&
            (y1 >= y2 && y1 <= y2 + h);
}

inline void my_split(const string &cmd, const char &pattern, vector<string> &res) {
    res.clear();
    istringstream is(cmd);
    string tmp;

    while (getline(is, tmp, pattern)) {
        res.push_back(tmp);
    }
}

struct record_t {
    string name;
    string level;
    int score;
    string time;
    record_t(vector<string> each) {
        name = each[0];
        level = each[1];
        score = stoi(each[2]);
        time = each[3];
    }
};


inline bool score_greater(const record_t& l, const record_t& r){ //分数降序
    return l.score > r.score;
}


inline string get_curtime()
{
    time_t t = time(nullptr);
    char tmp[32] = { 0 };
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));
    string cur_time(tmp);
    return cur_time;
}

#endif // COMMON_H
