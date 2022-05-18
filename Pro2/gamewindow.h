#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include "common.h"
#include "map.h"
#include "endwindow.h"

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
#include "towermagic.h"

#include<QLabel>
#include<QWidget>
#include<QMainWindow>
#include<QTimer>
#include<QMouseEvent>
#include<QMediaPlayer>
#include<QMediaPlaylist>

class SelectBox;
class GameWindow : public QWidget
{
public:
    Map* map; //地图组件
    SelectBox *selectbox;        //选择框

    vector<vector<Pos_t>>& all_paths; //所有路径
    vector<vector<Cell>>& game_map;   //原始的cell二维数组地图
    vector<int> fly_index;      //飞行路径的下标
    int path_count;             //路径的条数

    QTimer* get_enemy_timer;    //敌人工厂计时器
    QTimer* update_timer;       //场景更新计时器

    QLabel* money_lable;            //显示当前钱数
    QLabel* health_lable;           //显示当生命
    QLabel* money;
    QLabel* health;

    int my_health;                  //我的生命
    int my_money;                   //我的钱
    int waves;                      //敌人波数
    int waves_fly;                  //飞行敌人
    int counter;                    //计数器
    int level;                      //关卡
    int can_summon;                 //是否可以召唤
    int summon_counter;             //召唤计数器
    bool magic_tower;               //是否有魔塔

    vector<Enemy *> enemy_all;       //场景中所有敌人
    vector<Tower *> tower_all;       //场景中所有塔

    QMediaPlayer *music;

public:
    GameWindow(Map* map, int l);           //构造函数 需传入生成好的map指针
    virtual ~GameWindow();                  //析构函数
    void generateEnemy();           //生产敌人
    int update_all();              //驱动游戏

    void drawTower(QPainter&);          //画出塔
    void drawEnemy(QPainter&);          //画出敌人
    void drawHitEffect(QPainter&);      //画出命中效果
    void drawSelectBox(QPainter&);  //画出选择框

    void mousePressEvent(QMouseEvent*) override; //鼠标点击事件
    void paintEvent(QPaintEvent*) override;      //绘图事件

};

class SelectBox {
public:
    int x, y;
    bool display;       //是否显示
    int type;           //近战塔的选择框type=0  远程塔的选择框type=1
    QString near_picture[4] = { //四种近战塔的图片
        "../source/Spikerock-show.png",
        "../source/TorchWood-show.png",
        "../source/two-show.png",
        "../source/WallNut-show.png",
    };    //图片路径

    QString remote_picture[3] = { //两种远程塔的图片
        "../source/bottle-show.png",
        "../source/FireBottle-show.png",
        "../source/magic-show.png",
    };
    int near_pos[4][2];
    int remote_pos[3][2];

    SelectBox() : display(false), type(-1) { ; }
    void set_postion(int _x, int _y) {
        x = _x;
        y = _y;
        for(int i = 0; i < 4; i++) {
            near_pos[i][0] = x + directions[i][0]*kCellLen;
            near_pos[i][1] = y + directions[i][1]*kCellLen;
        }
        remote_pos[0][0] = x;
        remote_pos[0][1] = y - kCellLen;
        remote_pos[1][0] = x;
        remote_pos[1][1] = y + kCellLen;
        remote_pos[2][0] = x - kCellLen;
        remote_pos[2][1] = y;
    }
};

#endif // GAMEWINDOW_H
