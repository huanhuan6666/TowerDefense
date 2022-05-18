#include "gamewindow.h"
#include "mainwindow.h"
#include<QDebug>

GameWindow::GameWindow(Map *map, int l): map(map), all_paths(map->all_paths), game_map(map->game_map),
    my_health(10), my_money(1000), waves(0), waves_fly(0), counter(0), level(l)
{
    path_count = all_paths.size();
    for(int i = 0; i < path_count; i++) {
        if(game_map[all_paths[i][1].row][all_paths[i][1].col].state == FLY_PATH) {
            fly_index.push_back(i);
            cout << i << endl;
        }
    }

    setFixedSize(1040, 800);
    //setFixedSize(1080, 900);
    setWindowTitle("游戏界面");

    money_lable = new QLabel(this);
    health_lable = new QLabel(this);
    money = new QLabel(this);
    health = new QLabel(this);
    selectbox = new SelectBox();

    //金钱标签
    QPixmap image0("../source/qian.png");
    money->resize(40, 40);
    money->move(10, 40);
    QPixmap dest0=image0.scaled(money->size(),Qt::KeepAspectRatio);
    money->setPixmap(dest0);

    money_lable->move(55, 40);
    QFont font ( "Microsoft YaHei", 20, 25);
    //money_lable->setFont(QFont("黑体", 20));
    money_lable->setFont(font);
    money_lable->setText(QString("金钱：%1").arg(my_money));

    //生命标签
    QPixmap image("../source/xin.png");
    health->resize(40, 40);
    health->move(10, 90);
    QPixmap dest=image.scaled(health->size(),Qt::KeepAspectRatio);
    health->setPixmap(dest);

    health_lable->move(55, 90);
    //health_lable->setFont(QFont("黑体", 20, 25));
    health_lable->setFont(font);
    health_lable->setText(QString("生命：%1").arg(my_health));


    //驱动敌游戏计时器
    update_timer = new QTimer(this);
    update_timer->start(100);

    //connect(update_timer, &QTimer::timeout, this, &GameWindow::update_all);
    connect(update_timer, &QTimer::timeout, [&]()
    {
        int result = update_all();
        if(result == 1) { //游戏失败
            update_timer->stop();
            get_enemy_timer->stop();
            music->stop();
            EndWindow *e = new EndWindow(my_health/10.0 * my_money, level);
            e->show();
            close();
        }
        else if(result == 2) { //游戏胜利
            update_timer->stop();
            get_enemy_timer->stop();
            music->stop();
            //cout << "WIN!!!!" << " score is " << my_health/100.0 * my_money << endl;
            WinWindow *e = new WinWindow(my_health/10.0 * my_money, level);
            e->show();
            close();
        }
    });

    //生产敌人计时器
    get_enemy_timer = new QTimer(this);
    get_enemy_timer->start(3000);
    connect(get_enemy_timer, &QTimer::timeout, this, &GameWindow::generateEnemy);

    //播放背景音乐
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl::fromLocalFile("../source/CrazyDave.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    music = new QMediaPlayer();
    music->setPlaylist(playlist);
    music->setVolume(30);  //音量
    music->play();


    waves = 0; //波数
    can_summon = 100; //倒计时
    summon_counter = 0;
    magic_tower = false;
}

//析构函数
GameWindow::~GameWindow() {
    delete map;
    delete selectbox;
    delete update_timer;
    delete get_enemy_timer;
    delete money_lable;
    delete health_lable;
    delete money;
    delete health;
    for(auto& enemy : enemy_all)
        delete enemy;
    for(auto& tower: tower_all)
        delete tower;
}

//生产敌人
void GameWindow::generateEnemy() {
    /*Enemy *test_enemy = new EnemySuper(all_paths[0], map, tower_all);
    enemy_all.push_back(test_enemy);
    return;*/
    if(counter >=0 && counter <= 10) {
        //先在一条路径上产生敌人
        if(game_map[all_paths[0][1].row][all_paths[0][1].col].state != FLY_PATH) {
            switch (waves) {
            case 0: {
                Enemy *test_enemy = new EnemyNear(all_paths[0], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves = 1;
                break;
            }
            case 1: {
                Enemy *test_enemy = new EnemyRemote(all_paths[0], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves = 2;
                break;
            }
            case 2: {
                Enemy *test_enemy = new EnemySuper(all_paths[0], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves = 3;
                break;
            }
            case 3: {
                Enemy *test_enemy = new EnemyBuff(all_paths[0], map, tower_all, enemy_all);
                enemy_all.push_back(test_enemy);
                waves = 0;
                break;
            }
            }
        }
        else {
            switch (waves_fly) {
            case 0: {
                Enemy *test_enemy = new FlyAfraid(all_paths[0], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves_fly = 1;
                break;
            }
            case 1: {
                Enemy *test_enemy = new FlyBrave(all_paths[0], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves_fly = 0;
                break;
            }
            }
        }
    }
    else if(counter > 10 && counter <= 20) { //只在地面路径产生敌人
        for(int i = 0; i < path_count; i++) {
            if(game_map[all_paths[i][1].row][all_paths[i][1].col].state != FLY_PATH) {
                switch (waves) {
                case 0: {
                    Enemy *test_enemy = new EnemyNear(all_paths[i], map, tower_all);
                    enemy_all.push_back(test_enemy);
                    waves = 1;
                    break;
                }
                case 1: {
                    Enemy *test_enemy = new EnemyRemote(all_paths[i], map, tower_all);
                    enemy_all.push_back(test_enemy);
                    waves = 2;
                    break;
                }
                case 2: {
                    Enemy *test_enemy = new EnemySuper(all_paths[i], map, tower_all);
                    enemy_all.push_back(test_enemy);
                    waves = 3;
                    break;
                }
                case 3: {
                    Enemy *test_enemy = new EnemyBuff(all_paths[i], map, tower_all, enemy_all);
                    enemy_all.push_back(test_enemy);
                    waves = 0;
                    break;
                }
                }
            }
        }
    }
    else if(counter > 20 && counter <= 30) {
        int fly_count = fly_index.size();
        for(int i = 0; i < fly_count; i++) { //只在飞行路径产生敌人
            switch (waves_fly) {
            case 0: {
                Enemy *test_enemy = new FlyAfraid(all_paths[fly_index[i]], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves_fly = 1;
                break;
            }
            case 1: {
                Enemy *test_enemy = new FlyBrave(all_paths[fly_index[i]], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves_fly = 0;
                break;
            }
            }
        }
    }
    else if(counter > 30 && counter <= 45) {
        for(int i = 0; i < path_count; i++) { //所有的地面路径产生地面敌人
            if(game_map[all_paths[i][1].row][all_paths[i][1].col].state != FLY_PATH) {
                switch (waves) {
                case 0: {
                    Enemy *test_enemy = new EnemyNear(all_paths[i], map, tower_all);
                    enemy_all.push_back(test_enemy);
                    waves = 1;
                    break;
                }
                case 1: {
                    Enemy *test_enemy = new EnemyRemote(all_paths[i], map, tower_all);
                    enemy_all.push_back(test_enemy);
                    waves = 2;
                    break;
                }
                case 2: {
                    Enemy *test_enemy = new EnemySuper(all_paths[i], map, tower_all);
                    enemy_all.push_back(test_enemy);
                    waves = 3;
                    break;
                }
                case 3: {
                    Enemy *test_enemy = new EnemyBuff(all_paths[i], map, tower_all, enemy_all);
                    enemy_all.push_back(test_enemy);
                    waves = 0;
                    break;
                }
                }
            }
        }
        int fly_count = fly_index.size();
        for(int i = 0; i < fly_count; i++) { //飞行路径产生敌人
            switch (waves_fly) {
            case 0: {
                Enemy *test_enemy = new FlyAfraid(all_paths[fly_index[i]], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves_fly = 1;
                break;
            }
            case 1: {
                Enemy *test_enemy = new FlyBrave(all_paths[fly_index[i]], map, tower_all);
                enemy_all.push_back(test_enemy);
                waves_fly = 0;
                break;
            }
            }
        }
    }
    else {
        cout << "all enemies have been generated!" << endl;
    }
    counter += 1;
    cout << "counter is : " << counter << endl;

}

//驱动游戏
int GameWindow::update_all() {

    bool getto = false;
    can_summon -= 1;
    int seed = counter % 4;
    int bias = counter % 7;
    if(magic_tower && can_summon <= 0) {
        can_summon = 100;
        for(auto& cell_row : game_map) {
            for(auto& cell : cell_row) {
                if(cell.state == PATH && cell.planted == 0) { //确定召唤塔位置
                    if(summon_counter < seed+bias) {
                        summon_counter += 1;
                        continue;
                    }
                    else {
                        summon_counter = 0;
                        cell.planted = 1;
                        getto = true;
                        switch (seed) {
                        case 0: {
                            Tower *test_tower = new TorchWood(cell.row, cell.col, enemy_all);
                            tower_all.push_back(test_tower);
                            break;
                        }
                        case 1: {
                            Tower *test_tower = new Scientist(cell.row, cell.col, enemy_all);
                            tower_all.push_back(test_tower);
                            break;
                        }
                        case 2: {
                            Tower *test_tower = new TowerNut(cell.row, cell.col, enemy_all);
                            tower_all.push_back(test_tower);
                            break;
                        }
                        case 3: {
                            Tower *test_tower = new SpikeWeed(cell.row, cell.col, enemy_all);
                            tower_all.push_back(test_tower);
                            break;
                        }
                        }

                        break;
                    }
                }
            }
            if(getto)
                break;
        }
    }
    //扫描每个塔攻击所在范围内的敌人
    for(auto it = tower_all.begin(); it != tower_all.end();) {
        int state = (*it)->update_each();
        if(state == 2) { //塔死亡了
            if((*it)->type == 7) { //是魔塔
                magic_tower = false;
            }
            game_map[(*it)->row][(*it)->col].planted = 0;
            cout << (*it) << " tower dead!!!" << endl;
            delete (*it);
            it = tower_all.erase(it); // 从画面上消失
        }
        else
            ++it;
    }

    //敌人移动
    if(counter > 50 && enemy_all.empty()) //游戏胜利
        return 2;

    for(auto it = enemy_all.begin(); it != enemy_all.end();) {
        int state = (*it)->update_each();
        if(state == 1) {   // 敌人到达终点
            it = enemy_all.erase(it); // 从画面上消失
            my_health -= 1;
            health_lable->setText(QString("生命：%1").arg(my_health));
            if(my_health <= 0) { //游戏失败
                return 1;
            }
        }
        else if(state == 2){ //敌人死亡
            my_money += (*it)->all_health * 0.3; //杀敌奖励
            it = enemy_all.erase(it);
            cout << "enemy count is:" << enemy_all.size() << endl;
            qDebug() << (*it) << "dead!" << endl;
            //my_money += 30;
            money_lable->setText(QString("金钱：%1").arg(my_money));
        }
        else
            ++it;
    }

    return 0;
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
    drawSelectBox(painter);

    update();
}

//画出敌人和血条
void GameWindow::drawEnemy(QPainter& painter)
{
    for(auto enemy : enemy_all)
    {
        painter.setPen(QPen());
        double rate = (double)enemy->cur_health / enemy->all_health;
        if(rate > 0.8)
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        else if(rate > 0.3)
            painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        else
            painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        if(rate > 0)
            painter.drawRect(enemy->x, enemy->y - 10, enemy->weight*rate, 5);    //画出敌人血条
        if(enemy->attacked) { //被攻击标识
            painter.drawPixmap(enemy->x, enemy->y - 40, 30, 30, QString("../source/attacked.png"));
        }
        if(enemy->buffed > 0) { //被加血
            painter.drawPixmap(enemy->x+enemy->weight-30, enemy->y - 40, 35, 35, QString("../source/BuffLife.png"));
            enemy->buffed += 1;
            if(enemy->buffed == 50) {
                enemy->buffed = -50;
            }
        }

        painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
        painter.drawRect(enemy->x+enemy->weight*rate, enemy->y - 10, enemy->weight*(1-rate), 5);

        if(enemy->type == 1) { //近战敌人
            painter.drawPixmap(enemy->x, enemy->y, enemy->weight, enemy->height, enemy->picture);
        }
        else if(enemy->type == 4) { //buff敌人
            //painter.setBrush(QBrush(Qt::gray, Qt::DiagCrossPattern));
            painter.setBrush(QBrush(Qt::NoBrush));
            painter.setPen("#3A0088");
            painter.drawEllipse(QPoint(enemy->x + enemy->weight/2, enemy->y + enemy->height/2), enemy->range, enemy->range);
            painter.drawPixmap(enemy->x, enemy->y, enemy->weight, enemy->height, enemy->picture);
        }
        else { //可以发射子弹的敌人
            painter.drawPixmap(enemy->x, enemy->y, enemy->weight, enemy->height, enemy->picture);
            for(const auto &bullet : enemy->bullet_all) { //画出该敌人的所有子弹
                painter.drawPixmap(bullet->x, bullet->y, bullet->w, bullet->h, bullet->picture);
            }
        }
    }
}

//画出选择框
void GameWindow::drawSelectBox(QPainter& painter) {
    if(!selectbox->display)  //不用画
        return;
    if(selectbox->type == 0) { //近战塔选择框
        painter.drawPixmap(selectbox->x, selectbox->y, kCellLen, kCellLen, QPixmap("../source/select-box.png"));
        for(int i = 0; i < 4; i++) {
           painter.drawPixmap(selectbox->near_pos[i][0] + 10, selectbox->near_pos[i][1] + 10,
                              60, 60, selectbox->near_picture[i]);
           painter.drawPixmap(selectbox->near_pos[i][0], selectbox->near_pos[i][1],
                              80, 80, QPixmap("../source/each-tower.png"));
           //cout << "draw near tower " << i << endl;
        }
    }
    else if(selectbox->type == 1) { //远程塔选择框
        painter.drawPixmap(selectbox->x, selectbox->y, kCellLen, kCellLen, QPixmap("../source/select-box.png"));
        for(int i = 0; i < 3; i++) {
            painter.drawPixmap(selectbox->remote_pos[i][0] + 10, selectbox->remote_pos[i][1] + 10,
                               60, 60, selectbox->remote_picture[i]);
            painter.drawPixmap(selectbox->remote_pos[i][0], selectbox->remote_pos[i][1],
                               80, 80, QPixmap("../source/each-tower.png"));
            //cout << "draw near tower " << i << endl;
        }
    }
    else {
        cout << "neither near nor remote selectbox!" << endl;
    }
}

//画我方设施
void GameWindow::drawTower(QPainter& painter) {
    for(const auto& tower : tower_all)
    {
        painter.setPen(QPen());
        double rate = (double)tower->cur_health / tower->all_health;
        if(rate > 0.8)
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        else if(rate > 0.3)
            painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        else
            painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        if(rate > 0)
            painter.drawRect(tower->x, tower->y - 10, tower->weight*rate, 5);    //画出塔和血条

        if(tower->state == BEEN_ATTACKED) { //被攻击标识
            painter.drawPixmap(tower->x, tower->y - 40, 30, 30, QString("../source/BeenAttacked.png"));
        }

        if(tower->type >= 5) { //如果是远程塔
            for(const auto &bullet : tower->bullet_all) { //画出这个塔的所有子弹
                painter.drawPixmap(bullet->x, bullet->y, bullet->w, bullet->h, bullet->picture);
            }
            if(tower->type == 5) {
                if(tower->cur_health <= 40) { //子弹获得穿透效果
                    painter.drawPixmap(tower->x+tower->weight-30, tower->y-40, 35, 35, QString("../source/buffFire.png"));
                }
            }
            if(tower->type == 6) {
                if(tower->cur_health <= 40) { //伤害提升
                    painter.drawPixmap(tower->x+tower->weight-30, tower->y-40, 35, 35, QString("../source/ondeath.png"));
                }
            }
            if(tower->type == 7) {
                painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
                painter.drawRect(tower->x, tower->y - 20, tower->weight*can_summon/100.0, 5);
                if(can_summon > 10) { //沙漏
                    painter.drawPixmap(tower->x+tower->weight-30, tower->y-40, 35, 35, QString("../source/skill-cd.png"));
                }
                else { //生成塔
                    painter.drawPixmap(tower->x+tower->weight-30, tower->y-40, 35, 35, QString("../source/break.png"));
                }
            }
            painter.translate(tower->x + tower->weight/2, tower->y + tower->height/2);
            painter.rotate(tower->angle);
            painter.translate(-(tower->x + tower->weight/2), -(tower->y + tower->height/2)); //原点复位
            painter.drawPixmap(tower->x, tower->y, tower->weight, tower->height, tower->picture);
            painter.resetTransform();

        }
        else { //近战塔直接画
            if(tower->cur_health < 35) { //画出濒死状态
                painter.drawPixmap(tower->x+tower->weight-30, tower->y-40, 30, 30, QString("../source/angry.png"));
            }
            painter.drawPixmap(tower->x, tower->y, tower->weight, tower->height, tower->picture);
            if(tower->type == 3 && tower->state == ATTACK) { //科学家攻击时需要画波
                int tmp_x = tower->col * kCellLen, tmp_y = tower->row * kCellLen;
                switch (((Scientist *)tower)->direction) { //根据朝向绘制波
                case UP : {
                    painter.drawPixmap(tmp_x+30, tmp_y+kCellLen/2-tower->range, 40, tower->range, ((Scientist *)tower)->wave);
                    break;
                }
                case DOWN : {
                    painter.drawPixmap(tmp_x+30, tmp_y+kCellLen/2, 40, tower->range, ((Scientist *)tower)->wave);
                    break;
                }
                case LEFT : {
                    painter.drawPixmap(tmp_x+kCellLen/2-tower->range, tmp_y+30, tower->range, 40, ((Scientist *)tower)->wave);
                    break;
                }
                case RIGHT : {
                    painter.drawPixmap(tmp_x+kCellLen/2, tmp_y+30, tower->range, 30, ((Scientist *)tower)->wave);
                    break;
                }
                }
            }
        }

        if(tower->selected) { //被选中
            painter.setBrush(QBrush(Qt::NoBrush));
            painter.setPen(QPen(Qt::red, 3));
            if(tower->type != 3) { //除了科学家其余塔都无朝向
                painter.drawEllipse(QPoint(tower->x + tower->weight/2, tower->y + tower->height/2), tower->range, tower->range);
                painter.drawPixmap(tower->x+tower->weight/4, tower->y-60, 40, 40, QPixmap("../source/delete.png"));
            }
            else { //科学家
                int tmp_x = tower->col * kCellLen, tmp_y = tower->row * kCellLen;
                switch (((Scientist *)tower)->direction) { //根据朝向绘制攻击范围
                case UP : {
                    painter.drawRect(tmp_x, tmp_y+kCellLen/2-tower->range, kCellLen, tower->range);
                    break;
                }
                case DOWN : {
                    painter.drawRect(tmp_x, tmp_y+kCellLen/2, kCellLen, tower->range);
                    break;
                }
                case LEFT : {
                    painter.drawRect(tmp_x+kCellLen/2-tower->range, tmp_y, tower->range, kCellLen);
                    break;
                }
                case RIGHT : {
                    painter.drawRect(tmp_x+kCellLen/2, tmp_y, tower->range, kCellLen);
                    break;
                }
                }
                //绘制删除按钮和朝向按钮
                painter.drawPixmap(tower->x, tower->y-60, 40, 40, QPixmap("../source/delete.png"));

                painter.drawPixmap(tower->x+35, tower->y-60, 40, 40, QPixmap("../source/d_up.png"));
                painter.drawPixmap(tower->x+tower->weight/4, tower->y+tower->height, 40, 40, QPixmap("../source/d_down.png"));
                painter.drawPixmap(tower->x-40, tower->y, 40, 40, QPixmap("../source/d_left.png"));
                painter.drawPixmap(tower->x+tower->weight, tower->y, 40, 40, QPixmap("../source/d_right.png"));
            }

        }

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

    for(auto & tower : tower_all) {
        if(tower->selected) { //被选中
            if(tower->type != 3) { //非科学家
                if(InArea(e->x(), e->y(), tower->x+tower->weight/4, tower->y-60, 40, 40)) {
                    tower->state = DEAD; //清除塔只能偿还价格的80%
                    my_money += int(tower->price * 0.8);
                    money_lable->setText(QString("金钱：%1").arg(my_money));
                    return;
                }
            }
            else if(tower->type == 3) { //是科学家
                if(InArea(e->x(), e->y(), tower->x, tower->y-60, 40, 40)) { //删除
                    tower->state = DEAD; //清除塔只能偿还价格的80%
                    my_money += int(tower->price * 0.8);
                    money_lable->setText(QString("金钱：%1").arg(my_money));
                    return;
                }
                else if(InArea(e->x(), e->y(), tower->x+35, tower->y-60, 40, 40)) { //朝上
                    ((Scientist *)tower)->direction = UP;
                }
                else if(InArea(e->x(), e->y(), tower->x+tower->weight/4, tower->y+tower->height, 40, 40)) { //朝下
                    ((Scientist *)tower)->direction = DOWN;
                }
                else if(InArea(e->x(), e->y(), tower->x-40, tower->y, 40, 40)) { //朝左
                    ((Scientist *)tower)->direction = LEFT;
                }
                else if(InArea(e->x(), e->y(), tower->x+tower->weight, tower->y, 40, 40)) { //朝右
                    ((Scientist *)tower)->direction = RIGHT;
                }
                else { //点击其他区域则解除选择
                    tower->selected = false;
                }
                return ;
            }
            else {
                ;
            }
        }
    }

    if(!selectbox->display) { //选择框没有显示
        if(click_cell.state == REMOTE) { //该格子可种远程塔单位
            if(click_cell.planted) { //这个格子已经有塔了
                for(auto &tower : tower_all) {
                    if(tower->col == click_col && tower->row == click_row) {
                        tower->selected = true; //被选中
                    }
                }
                return;
            }
            else {
                selectbox->type = 1;
                selectbox->set_postion(click_col * kCellLen, click_row * kCellLen);
                selectbox->display = true;
            }
        }
        else if(click_cell.state == PATH) { //该格子可种近战单位
            if(click_cell.planted) { //这个格子已经有塔了
                cout << "row:" << click_row << " col: " << click_col << " has been planted!" << endl;
                for(auto &tower : tower_all) {
                    if(tower->col == click_col && tower->row == click_row) {
                        tower->selected = true; //被选中
                    }
                }
                return;
            }
            else {
                selectbox->type = 0;
                selectbox->set_postion(click_col * kCellLen, click_row * kCellLen);
                selectbox->display = true;
            }
        }
        else { //其他格子不能种塔
            cout << "row:" << click_row << " col: " << click_col << "can't plant!" << endl;
            for(auto &tower : tower_all) { //所有塔放弃选中
                tower->selected = false;
            }
        }
    }
    else { //选中了选择框
        int select_row = selectbox->y/kCellLen, select_col = selectbox->x/kCellLen;
        if(selectbox->type == 0) { //近战单位选择框
            if(my_money >= 110 && InArea(e->x(), e->y(), selectbox->x, selectbox->y-80, 80, 80)) { //地刺
                my_money -= 110;
                cout << "row:" << click_row << " col: " << click_col << " plant a spikeweed" << endl;
                game_map[select_row][select_col].planted = 1;
                Tower *test_tower = new SpikeWeed(select_row, select_col, enemy_all);
                tower_all.push_back(test_tower);
            }
            else if(my_money >= 100 && InArea(e->x(), e->y(), selectbox->x, selectbox->y+80, 80, 80)) { //火炬
                my_money -= 100;
                cout << "row:" << click_row << " col: " << click_col << " plant a torchwood" << endl;
                game_map[select_row][select_col].planted = 1;
                Tower *test_tower = new TorchWood(select_row, select_col, enemy_all);
                tower_all.push_back(test_tower);
            }
            else if(my_money >= 70 && InArea(e->x(), e->y(), selectbox->x-80, selectbox->y, 80, 80)) { //科学家
                my_money -= 70;
                cout << "row:" << click_row << " col: " << click_col << " plant a scientist" << endl;
                game_map[select_row][select_col].planted = 1;
                Tower *test_tower = new Scientist(select_row, select_col, enemy_all);
                tower_all.push_back(test_tower);
            }
            else if(my_money >= 30 && InArea(e->x(), e->y(), selectbox->x+80, selectbox->y, 80, 80)) { //坚果
                my_money -= 30;
                cout << "row:" << click_row << " col: " << click_col << " plant a nut" << endl;
                game_map[select_row][select_col].planted = 1;
                Tower *test_tower = new TowerNut(select_row, select_col, enemy_all);
                tower_all.push_back(test_tower);
            }
            else {
                cout << "click other in near select" << endl;
            }
        }
        else if(selectbox->type == 1) { //远程单位选择框
            if(my_money >= 120 && InArea(e->x(), e->y(), selectbox->x, selectbox->y-80, 80, 80)) { //普通远程塔
                my_money -= 120;
                cout << "row:" << click_row << " col: " << click_col << " plant a spikeweed" << endl;
                game_map[select_row][select_col].planted = 1;
                Tower *test_tower = new TowerRemote(select_row, select_col, enemy_all);
                tower_all.push_back(test_tower);
            }
            else if(my_money >= 150 && InArea(e->x(), e->y(), selectbox->x, selectbox->y+80, 80, 80)) { //汽油瓶
                my_money -= 150;
                cout << "row:" << click_row << " col: " << click_col << " plant a spikeweed" << endl;
                game_map[select_row][select_col].planted = 1;
                Tower *test_tower = new TowerSuper(select_row, select_col, enemy_all);
                tower_all.push_back(test_tower);
            }
            else if(!magic_tower && my_money >= 300 && InArea(e->x(), e->y(), selectbox->x-80, selectbox->y, 80, 80)) { //可生成塔的远程塔
                my_money -= 300;
                magic_tower = true;
                game_map[select_row][select_col].planted = 1;
                Tower *test_tower = new TowerMagic(select_row, select_col, enemy_all, can_summon);
                tower_all.push_back(test_tower);
            }
            else {
                cout << "click other in remote select" << endl;
            }
        }
        else {
            cout << "other kind of select box been selected" << endl;
        }
        selectbox->display = false; //最后隐藏选择框

        for(auto &tower : tower_all) { //所有塔放弃选中
                tower->selected = false;
        }
        money_lable->setText(QString("金钱：%1").arg(my_money));
    }
    for(auto &tower : tower_all) { //所有塔放弃选中
        tower->selected = false;
    }
}




//画子弹效果
void GameWindow::drawHitEffect(QPainter& painter) {

}

