#include "common.h"

Cell::Cell(): row(0), col(0), state(0), next_move(0), looks(nullptr), planted(0) {
    ;
}

Cell::Cell(const Cell& c) :row(c.row), col(c.col), state(c.state), next_move(c.next_move), looks(c.looks), planted(c.planted) {
    ;
}

Cell::Cell(int x, int y, int s, int d): row(x), col(y), state(s), next_move(d), planted(0) {
    //根据状态 设置单元格的图片
    switch(state){
        case WALL: //0墙
            looks = new QPixmap("../source/empty.png");
            break;
        case PATH:  //1敌人路径
            looks = new QPixmap("../source/treez.png");
            break;
        case BEGIN: //2起点
            looks = new QPixmap("../source/monster-door.png");
            break;
        case END:   //3终点
            looks = new QPixmap("../source/tower.png");
            break;
        case REMOTE: //4我方远程
            looks = new QPixmap("../source/remote_path.png");
            break;
        case FLY_PATH: //5飞行路径
            looks = new QPixmap("../source/fly-path.png");
            break;
    }

}

//画一个格子的图片
void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawPixmap(row*kCellLen, col*kCellLen, kCellLen, kCellLen, *looks);
}

//返回格子的边界
QRectF Cell::boundingRect() const {
    int left_top_r = row * kCellLen;
    int left_top_c = col * kCellLen;
    return QRect(left_top_r, left_top_c, kCellLen, kCellLen);
}

void Cell::setMovie(QString path) {
    if (movie)
            delete movie;
    movie = new QMovie(path);
    movie->start();
}
