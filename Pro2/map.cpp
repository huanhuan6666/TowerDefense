#include "map.h"
#include<vector>
using namespace std;
//初始化地图
Map::Map(vector<vector<string>> map_config)
{
    int row = map_config.size(), col = map_config[0].size();
    game_map.resize(row);
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            switch (stoi(map_config[i][j])) {
                case 0: {//WALL是墙
                    Cell tmp0(i, j, 0, 0);
                    game_map[i].push_back(tmp0);
                    break;
                }
                case 1: {//路径
                    Cell tmp1(i, j, 1, 0);
                    game_map[i].push_back(tmp1);
                    break;
                }
                case 2: {//起点
                    Cell tmp2(i, j, 2, 0);
                    game_map[i].push_back(tmp2);
                    break;
                }
                case 3: {//终点
                    Cell tmp3(i, j, 3, 0);
                    game_map[i].push_back(tmp3);
                    break;
                }
                case 4: { //远程
                    Cell tmp4(i, j, 4, 0);
                    game_map[i].push_back(tmp4);
                    break;
                }
                default:{
                    cout << "the index is [" << i << "," << j << "]" << endl;
                    cout << "can't support the num " << map_config[i][j]  << endl;
                    exit(1);
                }
            }
        }
    }

    getPaths(map_config); //生成所有路径到all_paths
}


//回溯法 a_path为所有尝试
void Map::dfs(vector<vector<string>>& map_config, int i, int j, vector<Pos_t>& a_path) {
    if(i < 0 || i >= map_config.size()
            || j < 0 || j >= map_config[0].size()
            ) //defensive
        return;
    int state = stoi(map_config[i][j]);
    if(state == -1 || state == 0 || state == 4) //终止条件
        return;

    if(state == 3) { //3为终点 此时a_path即为一条路径
        Pos_t tmp(i, j);
        a_path.push_back(tmp);
        all_paths.push_back(a_path);
        return;
    }

    if(state == 1 || state == 2) { //1路径 或者2起点继续递归
        Pos_t tmp(i, j);
        a_path.push_back(tmp); //这一步
        for(int c = 0; c < 4; c++) { //递归下一步
            map_config[i][j] = "-1"; //防止回头无穷递归
            dfs(map_config, i + directions[c][0], j + directions[c][1], a_path);
            map_config[i][j] = to_string(state); //1或2
        }
        a_path.pop_back(); //状态回退
    }
}

//根据map_config找出所有的路径vector<vector<Pos_t>> all_paths
void Map::getPaths(vector<vector<string>>& map_config) {
    int row = map_config.size(), col = map_config[0].size();

    for(int i = 0; i < row; i++){ //row对应y坐标
        for(int j = 0; j < col; j++){ //col对应x坐标
            if(stoi(map_config[i][j]) == 2) {//2为起点
                vector<Pos_t> a_path;
                dfs(map_config, i, j, a_path);
            }
            else
                continue;
        }
    }
    for(auto& path: all_paths){
        int size = path.size();
        for(int i = 0; i < size - 1; i++) {
            int in_row = path[i+1].row - path[i].row;
            int in_col = path[i+1].col - path[i].col;
            if(in_row == 0) {//左右移动
                if(in_col == 1) path[i].direct = RIGHT;
                if(in_col == -1) path[i].direct = LEFT;
            }
            else{ //上下方向
                if(in_row == 1) path[i].direct = DOWN;
                if(in_row == -1) path[i].direct = UP;
            }

        }
    }

    for(const auto& path:all_paths){
        cout << "path: ";
        for(const auto& pos: path) {
            cout << pos;
        }
        cout << endl;
    }
}


//画地图
void Map::drawMap(QPainter& painter) {
    int row = game_map.size(), col = game_map[0].size();
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            painter.drawPixmap(j * kCellLen, i * kCellLen, kCellLen, kCellLen,
                *(game_map[i][j].looks));
            painter.drawRect(j * kCellLen, i * kCellLen, kCellLen, kCellLen);
        }
    }
}
