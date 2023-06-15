#ifndef __FUNC_H__
#define __FUNC_H__

#include <stdint.h>
#include <stdio.h>

// 麥田x4、森林x4、草原x4、山脈x3、丘陵x3、沙漠x1
#define NUM_TILES 19  // 板塊的總數量
#define WIN_SCORE = 10;

enum {WHEAT = 1, WOOD, WOOL, STONE, BRICK};

struct  _player {
    int32_t resource[5]; // not show to other players
    int32_t total_resource; //show to other players
    int32_t is_used_develop_card; // initalize to 0 each round
    int road; // 玩家擁有的道路數量
    int village; // 玩家擁有的村莊數量
    int city; // 玩家擁有的城市數量
    int develop_cards[25]; // 14+6+5，0:未擁有 1:未使用 2:已使用 (分數卡只有0,1)
    int new_develop_card; // 當回合購買的發展卡，每回合初始化為-1
    int32_t NPC_difficulty; // user is 0, only for NPC
}__attribute__ ((__packed__));
typedef struct _player Player;

struct  _system {
    int32_t player_num;     // 3 or 4
    int bank_develop_card[25]; // 14+6+5，銀行的發展卡，0為已被購買，1為有庫存
    size_t bank_resource[5]; // 各19個，發放時會扣除，記得檢查是否>0
    
    int32_t longest_road;   // num of longest road in total players
    int32_t lr_player;      // the player_id who has longest road card
    int32_t max_knight;     // num of max knight card in total players
    int32_t mk_player;      // the player_id who has max_num knight cards
    
    int32_t player_score[4]; // 如果只有三個玩家，第四項就保留為0
}__attribute__ ((__packed__));
typedef struct _system System;

enum ResourceType { // 宣告六種版塊類型
    Wheat = 0,
    Wood,
    Grassland,
    Mountain,
    Hill,
    Desert
};

struct CatanTile { // 定義每個版塊內容
    enum ResourceType resourceType; //類型
    int number;        // 分配到的數字指示物
    int hasRobber;     // 是否有盜賊
};

//

void generateCatanTiles(struct CatanTile* tiles);
void printCatanTiles(struct CatanTile* tiles);
void shuffleResource(enum ResourceType* array, int length);
void shuffleNumbers(int* array, int length);

//

#endif
