#include "towernut.h"

//坚果塔 type = 1
TowerNut::TowerNut(int r, int c, vector<Enemy *>& enemy_all): Tower(r, c, enemy_all)
{
    type = 1; //攻击力为0 坚果

    step = 0;
    all_health = 100;
    cur_health = 100;
    level = 1;
    damage = 1;
    state = LIVE;
    speed = 0;
    range = kCellLen/2;
    all_block = 3;
    cur_block = 0;
    row = r;
    col = c;
    weight = 60;
    height = 60;

    x = c * kCellLen + (kCellLen - weight)/2; //根据传入的格子坐标得到像素坐标
    y = r * kCellLen + (kCellLen - height)/2;
    picture = "../source/WallNut.gif";


    interval = 5; //调用五次
    counter = 0;
}

//更新
int TowerNut::update_each() {
    //cur_health -= 1;
    if(state == DEAD){ //死了不再移动
        return 2;
    }

    if(cur_health <= 0){ //死亡
        picture = "../source/Burn.gif";
        state = DEAD;
        return 2;
    }

    double rate = (double)cur_health / all_health;

    if(rate > 0.4 && rate < 0.7) {
        picture = "../source/WallNut1.gif";
    }
    else if(rate < 0.4) {
        picture = "../source/WallNut2.gif";
    }

    cur_block = 0;
    int t_cx = x + weight/2, t_cy = y + height/2; //图元中心坐标
    //阻拦敌人

    for(auto& enemy : enemy_all) {
        //敌人图元的中心坐标
        int e_cx =  enemy->x + enemy->weight/2, e_cy =  enemy->y + enemy->height/2;
        //只能阻拦all_block个地面敌人
        if(enemy->type <= 4 && cur_block < all_block &&
          (abs(e_cx - t_cx) < range && abs(e_cy - t_cy) < range) )
        {
            enemy->state = BLOCKED;
            cur_block += 1;
        }

        if(enemy->type <= 4) { //地面敌人
            cout << enemy << "distance is " << Distance(this->x, this->y, enemy->x, enemy->y)
                 << " range is " << this->range << endl;

        }
    }

    cout << this << " tower blocks " << cur_block << " enemies!" << endl;
    return false;
}
