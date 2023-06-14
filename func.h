#ifndef __FUNC_H__
#define __FUNC_H__

#include <stdint.h>
#include <stdio.h>

enum {WHEAT = 1, WOOD, WOOL, STONE, BRICK};

struct  _player {
    int32_t id;        // 可能不會用，應該會用player[3]的array處理
    int32_t resource[6]; // not show to other players
    int32_t total_resource; //show to other players
    int32_t Is_use_develop_card; // one round use once
    
    int32_t NPC_difficulty; // user are 0, only for NPC
}__attribute__ ((__packed__));
typedef struct _player Player;

const int32_t WIN_SCORE = 10;
struct  _system {
    int32_t player_num;     // 3 or 4
    int32_t knight_card;    // at most 14
    int32_t score_card;     // at most 5
    size_t bank_resource[6]; // all: 19, 19, 19, 19, 19. Can't be zero.
    
    int32_t longest_road;   // num of longest road in total players
    int32_t lr_player;      // the player_id who has longest road card
    int32_t max_knight;     // num of max knight card in total players
    int32_t mk_player;      // the player_id who has max_num knight cards
    
    int32_t player_score[4];
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
