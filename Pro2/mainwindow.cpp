#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "map.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setObjectName("mainWindow");
    this->setStyleSheet("#mainWindow{border-image:url(../source/Bg.png);}");
    setFixedSize(600, 400);

    //setFixedSize(1140, 800);
    setWindowTitle("ProJect_2");

    QPushButton *level1 = new QPushButton(this);
    level1->move(100, 200);
    level1->setText("关卡一");
    QPushButton *level2 = new QPushButton(this);
    level2->move(220, 200);
    level2->setText("关卡二");
    QPushButton *level3 = new QPushButton(this);
    level3->move(330, 200);
    level3->setText("关卡三");

    connect(level1, &QPushButton::clicked, [&]()
    {
        generateMap("../map_config_default1.txt");   //扫描配置文件生成地图到map
        auto all_paths = map->all_paths;
        //int path_counts = all_paths.size();

        game = new GameWindow(map);
        game->show();
        close();
    });
    connect(level2, &QPushButton::clicked, [&]()
    {
        generateMap("../map_config_default2.txt");   //扫描配置文件生成地图到map
        auto all_paths = map->all_paths;
        //int path_counts = all_paths.size();

        game = new GameWindow(map);
        game->show();
        close();
    });
    connect(level3, &QPushButton::clicked, [&]()
    {
        generateMap("../map_config_default3.txt");   //扫描配置文件生成地图到map
        auto all_paths = map->all_paths;
        //int path_counts = all_paths.size();

        game = new GameWindow(map);
        game->show();
        close();
    });

    show();


}

MainWindow::~MainWindow()
{
    delete ui;
}

//绘图事件
void MainWindow::paintEvent(QPaintEvent *){
    //cout << "paint mainwindow!!!" << endl;
    /*QPainter painter(this);
    QPixmap pixmap("../source/Bg.png");
    //添加主界面背景
    painter.drawPixmap(0,0,this->width(),this->height(),pixmap);*/
}

//根据配置文件生成地图
void MainWindow::generateMap(const string& map_file) {
    ifstream fin(map_file);
    if(!fin) {
        cout << "open file fail!" << endl;
        exit(1);
    }

    string line;
    vector<vector<string>> map_config; //读文件到二维数组中

    while(getline(fin, line)) {
        vector<string> res; //每一行的格子情况
        istringstream is(line);
        string tmp;
        while (getline(is, tmp, ',')) {
            res.push_back(tmp);
        }
        map_config.push_back(res);
    }

    map = new Map(map_config); //根据配置构造地图

}

