#ifndef MAP_H
#define MAP_H
#include "common.h"

class Map
{
public:
    vector<vector<Cell>> game_map; //地图二维数组
    vector<vector<Pos_t>> all_paths; //所有路径
public:
    Map(vector<vector<string>> the_map); //构造函数
    void drawMap(QPainter& painter); //画地图
    void getPaths(vector<vector<string>>& map_config);
    void dfs(vector<vector<string>>& map_config, int i, int j, vector<Pos_t>& a_path);
};

#endif // MAP_H
