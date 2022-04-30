#include "gamewindow.h"
#include<QDebug>

GameWindow::GameWindow(Map *map): map(map), all_paths(map->all_paths), game_map(map->game_map),
    my_health(100), my_money(100)
{
    setFixedSize(1040, 800);
    //setFixedSize(1080, 900);
    setWindowTitle("游戏界面");

    money_lable = new QLabel(this);
    health_lable = new QLabel(this);
    //金钱标签
    money_lable->move(20, 40);
    money_lable->setFont(QFont("黑体", 20));
    money_lable->setText(QString("金钱：%1").arg(my_money));

    //生命标签
    health_lable->move(20, 90);
    health_lable->setFont(QFont("黑体", 20));
    health_lable->setText(QString("生命：%1").arg(my_health));


    //驱动敌游戏计时器 扫描enemy_all上的每个enemy调用其update
    update_timer = new QTimer(this);
    update_timer->start(100);

    update_timer = new QTimer(this);
    update_timer->start(100);
    connect(update_timer, &QTimer::timeout, this, &GameWindow::update_all);

    //生产敌人计时器
    get_enemy_timer = new QTimer(this);
    get_enemy_timer->start(2000);
    connect(get_enemy_timer, &QTimer::timeout, this, &GameWindow::generateEnemy);

}

//生产敌人
void GameWindow::generateEnemy() {
    /*for(int i = 0; i < 4; i++) {
        Enemy *test_enemy = new Enemy(all_paths[i], map, tower_all);
        enemy_all.push_back(test_enemy);
        cout << test_enemy << " has " << enemy_all.size() << endl;
    }*/
    Enemy *test_enemy = new FlyBrave(all_paths[0], map, tower_all);
    enemy_all.push_back(test_enemy);
}

//驱动游戏
void GameWindow::update_all() {

    //扫描每个塔攻击所在范围内的敌人
    for(auto it = tower_all.begin(); it != tower_all.end();) {
        int state = (*it)->update_each();
        if(state == 2) { //塔死亡了
            game_map[(*it)->row][(*it)->col].planted = 0;
            cout << (*it) << " tower dead!!!" << endl;
            delete (*it);
            it = tower_all.erase(it); // 从画面上消失
        }
        else
            ++it;
    }

    //敌人移动
    for(auto it = enemy_all.begin(); it != enemy_all.end();) {
        int state = (*it)->update_each();
        if(state == 1) {   // 敌人到达终点
            it = enemy_all.erase(it); // 从画面上消失
            my_health -= 1;
            health_lable->setText(QString("生命：%1").arg(my_health));
        }
        else if(state == 2){ //敌人死亡
            it = enemy_all.erase(it);
            cout << "enemy count is:" << enemy_all.size() << endl;
            qDebug() << (*it) << "dead!" << endl;
            my_money += 10; //杀敌奖励
            money_lable->setText(QString("金钱：%1").arg(my_money));
        }
        else
            ++it;
    }


}

//绘图事件 绘制画面上的所有内容
void GameWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    QPixmap pixmap("../source/BG-hd.png");
    //添加主界面背景
    painter.drawPixmap(0,0,this->width(),this->height(),pixmap);
    map->drawMap(painter); //绘制地图
    drawEnemy(painter);
    drawTower(painter);
    update();
}

//画出敌人和血条
void GameWindow::drawEnemy(QPainter& painter)
{
    for(auto enemy : enemy_all)
    {
        double rate = (double)enemy->cur_health / enemy->all_health;
        if(rate > 0.8)
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        else if(rate > 0.3)
            painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        else
            painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        if(rate > 0)
            painter.drawRect(enemy->x, enemy->y - 10, enemy->weight*rate, 5);    //画出敌人血条
        painter.drawPixmap(enemy->x, enemy->y, enemy->weight, enemy->height, enemy->picture);
    }
}

//鼠标点击事件
void GameWindow::mousePressEvent(QMouseEvent* e){
    qDebug() << "click!" << endl;
    if(e->button() != Qt::LeftButton)
        return ;
    int click_row = e->y() / kCellLen;
    int click_col = e->x() / kCellLen;
    Cell click_cell = game_map[click_row][click_col];

    if(click_cell.state == REMOTE) { //该格子可种远程塔单位
        cout << "plant Remote tower!" << endl;
        return;
    }
    else if(click_cell.state == PATH) { //该格子可种近战单位
        if(click_cell.planted) { //这个格子已经有塔了
            cout << "row:" << click_row << " col: " << click_col << " has been planted!" << endl;
            return;
        }
        else { //种植近战单位
            cout << "row:" << click_row << " col: " << click_col << " plant!!!" << endl;
            game_map[click_row][click_col].planted = 1;
            Tower *test_tower = new TowerNut(click_row, click_col, enemy_all);
            tower_all.push_back(test_tower);
            cout << "towers' count is: " << tower_all.size() << endl;
        }
    }
    else { //其他格子不能种塔
        cout << "row:" << click_row << " col: " << click_col << "can't plant!" << endl;
        return;
    }

}


//画我方设施
void GameWindow::drawTower(QPainter& painter) {
    for(auto tower : tower_all)
    {
        double rate = (double)tower->cur_health / tower->all_health;
        if(rate > 0.8)
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        else if(rate > 0.3)
            painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        else
            painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        if(rate > 0)
            painter.drawRect(tower->x, tower->y - 10, tower->weight*rate, 5);    //画出塔和血条
        painter.drawPixmap(tower->x, tower->y, tower->weight, tower->height, tower->picture);
    }
}



//画子弹
void GameWindow::drawBull(QPainter& painter){

}

//画子弹效果
void GameWindow::drawHitEffect(QPainter& painter){

}

