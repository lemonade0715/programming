#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "func.h"
#include "system.h"

void initCornerId(struct CatanTile* tiles);
void print_corner_id(struct CatanTile* tiles);

// connectedPoint[0][3]這組不算，因為端點的編號從1開始
// 每個端點最多與3個點相連
// connectedPoint[1][3]紀錄與01相連的3點，不足3點的部份用0表示
// connectedPoint[2][3]紀錄與02相連的3點，不足3點的部份用0表示
// connectedPoint[3][3]紀錄與03相連的3點…依此類推
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

// 產生板塊
void generateCatanTiles(struct CatanTile* tiles) {
// 內陸板塊有6種場地：麥田x4、森林x4、草原x4、山脈x3、丘陵x3、沙漠x1。
    enum ResourceType resources[NUM_TILES] = {
      Field, Field, Field, Field,
      Forest, Forest, Forest, Forest,
      Grassland, Grassland, Grassland, Grassland,
      Mountain, Mountain, Mountain,
      Hill, Hill, Hill,
      Desert };

    char *typeNames[] = {
        "麥田", "森林", "草原", "山脈", "丘陵", "沙漠"
    };

    int numbers[NUM_TILES] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    int remaining = NUM_TILES;

    shuffleResource(resources, NUM_TILES);  // 將resource洗牌
    shuffleNumbers(numbers, NUM_TILES - 1); // 將數字指示物洗牌

    int numberIdx = 0;
    for (int resourceIdx = 0; resourceIdx < NUM_TILES; resourceIdx++) {
        initCornerId(tiles);
        tiles[resourceIdx].resourceType = resources[resourceIdx];
        strcpy(tiles[resourceIdx].typeName, typeNames[resources[resourceIdx]]);
        if (tiles[resourceIdx].resourceType == Desert) {
            // 是沙漠的話，不放數字，放盜賊
            tiles[resourceIdx].hasRobber = 1;
            tiles[resourceIdx].number = 0;
        } else {
            // 不是沙漠的話，放數字，不放盜賊
            tiles[resourceIdx].hasRobber = 0;
            tiles[resourceIdx].number = numbers[numberIdx];
            numberIdx++;
        }
        for(int i = 0; i < 6; ++i){
            tiles[resourceIdx].has_village[i] = 0;
        }
    }

}

// 測試函式：列印Catan板塊的資源類型和點數
void printCatanTiles(struct CatanTile* tiles) {
    printf("\033[1m【板塊分布】\033[0m\n");
    
    for (int i = 0; i < NUM_TILES; i++) {
        printf("板塊 %02d - %s", i, tiles[i].typeName);
        // switch (tiles[i].resourceType) {
        //     case Wheat:
        //         printf("麥田");
        //         break;
        //     case Wood:
        //         printf("森林");
        //         break;
        //     case Grassland:
        //         printf("草原");
        //         break;
        //     case Mountain:
        //         printf("山脈");
        //         break;
        //     case Hill:
        //         printf("丘陵");
        //         break;
        //     case Desert:
        //         printf("沙漠");
        //         break;
        //     default:
        //       printf("%d", tiles[i].resourceType);
        //       break;
        // }
        printf("(%d)", tiles[i].number);
        if (tiles[i].hasRobber == 1) {
            printf(" * 有盜賊");
        }
        printf("\n");
    }
}

void shuffleResource(enum ResourceType* array, int length) {
    srand(time(NULL));
    
    for (int i = length - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        enum ResourceType temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void shuffleNumbers(int* array, int length) {
    srand(time(NULL));
    
    for (int i = length - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


void initCornerId(struct CatanTile* tiles){
    tiles[0].corner_id[0] = 1;
    tiles[0].corner_id[1] = 2;
    tiles[0].corner_id[2] = 4;
    tiles[0].corner_id[3] = 5;
    tiles[0].corner_id[4] = 9;
    tiles[0].corner_id[5] = 10;

    for(int i = 0; i < 2; ++i){
        tiles[i+1].corner_id[0] = i*2 + 3;
        tiles[i+1].corner_id[1] = i*2 + 4;
        tiles[i+1].corner_id[2] = i*2 + 8;
        tiles[i+1].corner_id[3] = i*2 + 9;
        tiles[i+1].corner_id[4] = i*2 + 14;
        tiles[i+1].corner_id[5] = i*2 + 15;
    }
    for(int i = 0; i < 3; ++i){
        tiles[i+3].corner_id[0] = i*2 + 7;
        tiles[i+3].corner_id[1] = i*2 + 8;
        tiles[i+3].corner_id[2] = i*2 + 13;
        tiles[i+3].corner_id[3] = i*2 + 14;
        tiles[i+3].corner_id[4] = i*2 + 19;
        tiles[i+3].corner_id[5] = i*2 + 20;
    }
    for(int i = 0; i < 2; ++i){
        tiles[i+6].corner_id[0] = i*2 + 14;
        tiles[i+6].corner_id[1] = i*2 + 15;
        tiles[i+6].corner_id[2] = i*2 + 20;
        tiles[i+6].corner_id[3] = i*2 + 21;
        tiles[i+6].corner_id[4] = i*2 + 26;
        tiles[i+6].corner_id[5] = i*2 + 27;
    }
    for(int i = 0; i < 3; ++i){
        tiles[i+8].corner_id[0] = i*2 + 19;
        tiles[i+8].corner_id[1] = i*2 + 20;
        tiles[i+8].corner_id[2] = i*2 + 25;
        tiles[i+8].corner_id[3] = i*2 + 26;
        tiles[i+8].corner_id[4] = i*2 + 31;
        tiles[i+8].corner_id[5] = i*2 + 32;
    }
    for(int i = 0; i < 2; ++i){
        tiles[i+11].corner_id[0] = i*2 + 26;
        tiles[i+11].corner_id[1] = i*2 + 27;
        tiles[i+11].corner_id[2] = i*2 + 32;
        tiles[i+11].corner_id[3] = i*2 + 33;
        tiles[i+11].corner_id[4] = i*2 + 38;
        tiles[i+11].corner_id[5] = i*2 + 39;
    }
    for(int i = 0; i < 3; ++i){
        tiles[i+13].corner_id[0] = i*2 + 31;
        tiles[i+13].corner_id[1] = i*2 + 32;
        tiles[i+13].corner_id[2] = i*2 + 37;
        tiles[i+13].corner_id[3] = i*2 + 38;
        tiles[i+13].corner_id[4] = i*2 + 43;
        tiles[i+13].corner_id[5] = i*2 + 44;
    }
    for(int i = 0; i < 2; ++i){
        tiles[i+16].corner_id[0] = i*2 + 38;
        tiles[i+16].corner_id[1] = i*2 + 39;
        tiles[i+16].corner_id[2] = i*2 + 44;
        tiles[i+16].corner_id[3] = i*2 + 45;
        tiles[i+16].corner_id[4] = i*2 + 49;
        tiles[i+16].corner_id[5] = i*2 + 50;
    }
    
    tiles[18].corner_id[0] = 45;
    tiles[18].corner_id[1] = 46;
    tiles[18].corner_id[2] = 50;
    tiles[18].corner_id[3] = 51;
    tiles[18].corner_id[4] = 53;
    tiles[18].corner_id[5] = 54;

}

void print_corner_id(struct CatanTile* tiles){
    for(int i = 0; i < 19; ++i){
        printf("tile %d: ", i);
        for(int j = 0; j < 6; ++j){
            printf("%d ", tiles[i].corner_id[j]);
        }
        printf("\n");
    }
}

int8_t check_connected(int p1, int p2) {
    int8_t connected = 0;
    int i = 0;
    for (i = 0; i < 3; ++i) {
        if (connectedPoint[p1][i] == p2) {
            connected = 1;
            break;
        }
    }
    return connected;
}

int8_t check_if_has_road(Player *playlist, int p1, int p2) {
    int playIdx = 0;
    int roadIdx = 0;
    for (playIdx = 0; playIdx < 4; ++playIdx) {
        for (roadIdx = 0; roadIdx < 15; ++roadIdx) {
            if (p1 == playlist[playIdx].road[roadIdx][0] && p2 == playlist[playIdx].road[roadIdx][1]) {
                return 1;
            }
            if (p1 == playlist[playIdx].road[roadIdx][1] && p2 == playlist[playIdx].road[roadIdx][0]) {
                return 1;
            }
        }
    }
    return 0;
}

int8_t check_if_connected_build(Player *playlist, int currentPlayer, int p1, int p2) {
    Player player = playlist[currentPlayer];
    int roadIdx = 0;
    // 判斷是否有連結在自己的道路上
    for (roadIdx = 0; roadIdx < 15; ++roadIdx) {
        if (p1 == player.road[roadIdx][0] ||
            p1 == player.road[roadIdx][1] ||
            p2 == player.road[roadIdx][0] ||
            p2 == player.road[roadIdx][1]) {
                return 1;
            }
    }

    // 判斷是否有連接在自己的建築上
    for (int i = 0; i < MAX_VILLAGES; ++i) {
        if (p1 == player.village[i] || p2 == player.village[i]) {
            return 1;
        }
    }
    for (int i = 0; i < MAX_CITIES; ++i) {
        if (p1 == player.city[i] || p2 == player.city[i]) {
            return 1;
        }
    }

    return 0;
}

void add_road_to_player(Player *playlist, int currentPlayer, int p1, int p2) {
    Player player = playlist[currentPlayer];
    int roadIdx = 0;
    for (roadIdx = 0; roadIdx < 15; ++roadIdx)
    {
        if (player.road[roadIdx][0] == 0 && player.road[roadIdx][1] == 0)
        {
            player.road[roadIdx][0] = p1;
            player.road[roadIdx][1] = p2;
            return;
        }
    }
}

int robber_location(struct CatanTile* tiles){
    for(int i = 0; i < 19; ++i){
        if(tiles[i].hasRobber == 1){
            return i;
        }
    }
    return -1;
}
