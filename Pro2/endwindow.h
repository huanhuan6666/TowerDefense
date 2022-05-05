#ifndef ENDWINDOW_H
#define ENDWINDOW_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QInputDialog>
#include "common.h"

//失败界面
class EndWindow : public QWidget
{
public:
    QPushButton *Quit;
    QPushButton *Again;
    QPushButton *Rank;
    QLabel *Score;
    QLabel *Text;
    int m_score;
    int m_level;
public:
    EndWindow(QWidget *parent = 0);
    EndWindow(int score, int level);
    void quitgame();
    void again();
    void rank();
};

//胜利界面
class WinWindow : public EndWindow
{
public:
    QPushButton *Input;
    WinWindow(int score, int level);

    void input_name();
};

//展示排行榜界面
class RankWindow : public QWidget{
public:
    RankWindow();
};

#endif // ENDWINDOW_H
