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

//

void generateCatanTiles(struct CatanTile* tiles);
void printCatanTiles(struct CatanTile* tiles);
void shuffleResource(enum ResourceType* array, int length);
void shuffleNumbers(int* array, int length);
//

#endif
