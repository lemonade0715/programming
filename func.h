#ifndef __FUNC_H__
#define __FUNC_H__

#include <stdint.h>
#include <stdio.h>

// 麥田x4、森林x4、草原x4、山脈x3、丘陵x3、沙漠x1
#define NUM_TILES 19  // 板塊的總數量
#define WIN_SCORE = 10
#define MAX_SETTLEMENTS 5 
#define MAX_CITIES 4

typedef enum Resource
{
    WHEAT = 0,
    WOOD,
    WOOL,
    STONE,
    BRICK
} resource;

struct  _player {
    int32_t resource[5]; // not show to other players
    int32_t total_resource; //show to other players
    int32_t is_used_develop_card; // initalize to 0 each round
    int road[15][2]; // 玩家擁有的道路數量 //TODO
    int village[MAX_SETTLEMENTS]; // 玩家擁有的村莊編號
    int city[MAX_CITIES]; // 玩家擁有的城市編號
    int develop_cards[25]; // 14+6+5，0:未擁有 1:未使用 2:已使用 (分數卡只有0,1)
    int new_develop_card; // 當回合購買的發展卡，每回合初始化為-1
    int32_t NPC_difficulty; // user is 0, only for NPC, 1 is the hardest, 2 is the easiest
}__attribute__ ((__packed__));
typedef struct _player Player;

enum ResourceType { // 宣告六種版塊類型
    Field = 0,
    Forest,
    Grassland,
    Mountain,
    Hill,
    Desert
};

struct CatanTile { // 定義每個版塊內容
    enum ResourceType resourceType; //類型
    char typeName[19]; // 類型名稱
    int number;        // 分配到的數字指示物
    int hasRobber;     // 是否有盜賊
    
    int corner_id[6];
    int has_village[6]; // 0: no village, 1: village, 2: city
};

// connectedLine[0][3]這組不算，因為端點的編號從1開始
// 每個端點最多與3個點相連
// connectedLine[1][3]紀錄與01相連的3點，不足3點的部份用0表示
// connectedLine[2][3]紀錄與02相連的3點，不足3點的部份用0表示
// connectedLine[3][3]紀錄與03相連的3點…依此類推
int connectedPoint[55][3] = {
    {0,0,0},  {2,4,0},  {1,5,0}, {4,8,0},  {1,3,9},
    {2,6,10}, {5,11,0}, {8,13,0},{3,7,14}, {4,10,15},
    {5,9,16}, {6,12,17},{11,18,0},{7,19,0},{8,15,20},
    {9,14,21},{10,17,22},{11,16,23},{12,24,0},{13,20,25},
    {14,19,26},{15,22,27},{16,21,28},{17,24,19},{18,23,30},
    {19,31,0},{20,27,32},{21,26,33},{22,29,34},{23,28,35},
    {24,36,0},{25,32,37},{26,31,38},{27,34,39},{28,33,40},
    {29,36,41},{30,35,42},{31,43,0},{32,39,44},{33,38,45},
    {34,41,46},{35,40,47},{36,48,0},{37,44,0},{38,43,49},
    {39,46,50},{40,45,51},{41,48,52},{42,47,0},{44,50,0},
    {45,49,53},{46,52,54},{47,51,0},{50,54,0},{51,53,0}
};

//

void generateCatanTiles(struct CatanTile* tiles);
void printCatanTiles(struct CatanTile* tiles);
void shuffleResource(enum ResourceType* array, int length);
void shuffleNumbers(int* array, int length);
//道路連接
int8_t check_connected(int p1, int p2);
int8_t check_if_has_road(Player *playlist, int p1, int p2);
int8_t check_if_connected_build(Player *playlist, int currentPlayer, int p1, int p2);
void add_road_to_player(Player *playlist, int currentPlayer, int p1, int p2);
//

#endif
