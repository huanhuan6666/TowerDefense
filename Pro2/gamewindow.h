#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include "common.h"
#include "map.h"

#include "enemynear.h"
#include "enemyremote.h"
#include "enemysuper.h"
#include "enemybuff.h"
#include "flyafraid.h"
#include "flybrave.h"

#include "spikeweed.h"
#include "towernut.h"
#include "torchwood.h"
#include "scientist.h"
#include "towerremote.h"
#include "towersuper.h"

#include<QLabel>
#include<QWidget>
#include<QMainWindow>
#include<QTimer>
#include<QMouseEvent>
#include<QMediaPlayer>
#include<QMediaPlaylist>

class GameWindow : public QWidget
{
public:
    Map* map; //地图组件
    vector<vector<Pos_t>>& all_paths; //所有路径
    vector<vector<Cell>>& game_map;   //原始的cell二维数组地图

    QTimer* get_enemy_timer;    //敌人工厂计时器
    QTimer* update_timer;       //场景更新计时器

    QLabel* money_lable;            //显示当前钱数
    QLabel* health_lable;           //显示当生命
    QLabel* money;
    QLabel* health;

    int my_health;                  //我的生命
    int my_money;                   //我的钱
    int waves;                      //敌人波数

    vector<Enemy *> enemy_all;       //场景中所有敌人
    vector<Tower *> tower_all;       //场景中所有塔

public:
    GameWindow(Map* map);           //构造函数 需传入生成好的map指针
    void generateEnemy();           //生产敌人
    void update_all();              //驱动游戏

    void drawTower(QPainter&);          //画出塔
    void drawEnemy(QPainter&);          //画出敌人
    void drawBull(QPainter&);           //画出子弹
    void drawHitEffect(QPainter&);      //画出命中效果

    void mousePressEvent(QMouseEvent*) override; //鼠标点击事件
    void paintEvent(QPaintEvent*) override;      //绘图事件

};

#endif // GAMEWINDOW_H
