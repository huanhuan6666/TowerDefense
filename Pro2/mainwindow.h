#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include "gamewindow.h"
#include "enemy.h"
#include "tower.h"
#include<iostream>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void generateMap(); //根据配置文件生成地图
    void paintEvent(QPaintEvent *) override;


public:
    Ui::MainWindow *ui;
    GameWindow *game;   //游戏界面
    Map *map;           //地图

};
#endif // MAINWINDOW_H
