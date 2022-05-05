#include "endwindow.h"
#include "mainwindow.h"
#include <fstream>
#include <iostream>
using namespace std;

EndWindow::EndWindow(QWidget *parent) : QWidget(parent)
{
    ;
}

EndWindow::EndWindow(int score, int level) : m_score(score), m_level(level)
{
    setFixedSize(600, 400);
    Quit = new QPushButton("退出游戏", this);
    Quit->move(350, 280);
    Again = new QPushButton("主界面", this);
    Again->move(150, 280);

    Score = new QLabel(this);
    Score->move(200, 200);
    Score->setText(QString("最终得分: %1").arg(score));
    Text = new QLabel(this);
    Text->move(200, 100);
    Text->setText("你输了/(QAQ)/~~");


    Again->setFont(QFont("Algerian",18));
    Quit->setFont(QFont("Algerian",18));
    Score->setFont(QFont("Algerian",18));
    Text->setFont(QFont("Algerian",18));

    /*Lose->setStyleSheet("QLabel{background: transparent; color:white; }"
                                );*/

    //设置背景
    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap("../source/Bg.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap.scaled(
               this->size(),
               Qt::IgnoreAspectRatio,
               Qt::SmoothTransformation)));
    setPalette(pal);


    connect(Quit,&QPushButton::clicked,this,&EndWindow::quitgame);
    connect(Again, &QPushButton::clicked,this,&EndWindow::again);
}

void EndWindow::rank() {
    RankWindow * r = new RankWindow;
    r->show();
}

void EndWindow::quitgame() {
    cout << "game over!" << endl;
    close();
}

void EndWindow::again() {
    MainWindow *m = new MainWindow;
    m->show();
    close();
}

WinWindow::WinWindow(int score, int level): EndWindow(score, level) {
    Rank = new QPushButton("排行榜", this);
    Rank->move(350, 340);
    Rank->setFont(QFont("Algerian",18));
    connect(Rank, &QPushButton::clicked,this,&EndWindow::rank);

    Input = new QPushButton("记录成绩", this);
    Input->move(150, 340);
    Input->setFont(QFont("Algerian",18));
    connect(Input, &QPushButton::clicked,this,&WinWindow::input_name);

    Text->setText("你赢了O(∩_∩)O~~");
}

void WinWindow::input_name() {
    QString dlgTitle=QStringLiteral("输入文本对话框");//对话框标题
    QString txtLabel=QStringLiteral("请输入姓名：");//对话框Label显示内容
    QString defaultInput=QStringLiteral("None");//对话框内输入框的默认内容
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;//输入框的文本内容为正常显示

    bool ok=false;
    QString text=QInputDialog::getText(this,dlgTitle,txtLabel,echoMode,defaultInput,&ok,Qt::MSWindowsFixedSizeDialogHint);

    if(ok && !text.isEmpty()) {
        ofstream fout("../rankings.txt", ios::app);
        fout << text.toStdString() << "," << m_level << "," << m_score << ","
             << get_curtime() << endl;
        fout.close();
    }

}

RankWindow::RankWindow() {
    setFixedSize(900, 500);
    move(650, 300);
    setWindowTitle("排行榜");

    //设置背景
    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap("../source/BG-rank.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap.scaled(
               this->size(),
               Qt::IgnoreAspectRatio,
               Qt::SmoothTransformation)));
    setPalette(pal);

    //读取排行榜文件
    ifstream fin("../rankings.txt");
    if (!fin) {
        cout << "Error: open file failed! rankings.txt" << endl;
        return;
    }
    QLabel *Label1 = new QLabel(this);
    Label1->setText("RANK    NAME    LEVEL   SCORE   TIME");
    Label1->setFont(QFont("Algerian",16));
    //Label1->setStyleSheet("QLabel{background:transparent;color:white;}");

    string line;
    getline(fin, line); //第一行表头

    vector<record_t> all_records; //所有记录
    while(getline(fin, line)) {
        vector<string> each;
        my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
        record_t a(each);
        all_records.push_back(a);
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(Label1);
    sort(all_records.begin(), all_records.end(), score_greater); //按照分数降序排列

    vector<record_t> records; //排名前六的记录
    for(int i = 0; i < 6; i++)
        records.push_back(all_records[i]);

    int i = 1;
    for(auto& record : records) {
        string tmp = to_string(i) + "       " + record.name + "    " + record.level + "      "
                + to_string(record.score) + "     " + record.time;
        QLabel *Labeltmp = new QLabel(this);
        Labeltmp->setFont(QFont("Algerian",16));
        if(i <= 3)
            Labeltmp->setStyleSheet("QLabel{background:transparent;color:red;}");
        Labeltmp->setText(QString::fromStdString(tmp));
        lay->addWidget(Labeltmp);
        i += 1;
    }

    setLayout(lay);

}
