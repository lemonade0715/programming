#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "func.h"

// 產生板塊
void generateCatanTiles(struct CatanTile* tiles) {
// 內陸板塊有6種場地：麥田x4、森林x4、草原x4、山脈x3、丘陵x3、沙漠x1。
    enum ResourceType resources[NUM_TILES] = {
      Wheat, Wheat, Wheat, Wheat,
      Wood, Wood, Wood, Wood,
      Grassland, Grassland, Grassland, Grassland,
      Mountain, Mountain, Mountain,
      Hill, Hill, Hill,
      Desert };

    int numbers[NUM_TILES] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    int remaining = NUM_TILES;

    shuffleResource(resources, NUM_TILES);  // 將resource洗牌
    shuffleNumbers(numbers, NUM_TILES - 1); // 將數字指示物洗牌

    int numberIdx = 0;
    for (int resourceIdx = 0; resourceIdx < NUM_TILES; resourceIdx++) {
        tiles[resourceIdx].resourceType = resources[resourceIdx];
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
    }
}

// 測試函式：列印Catan板塊的資源類型和點數
void printCatanTiles(struct CatanTile* tiles) {
    printf("\033[1m【板塊分布】\033[0m\n");
    
    for (int i = 0; i < NUM_TILES; i++) {
        printf("板塊 %02d - ", i);
        switch (tiles[i].resourceType) {
            case Wheat:
                printf("麥田");
                break;
            case Wood:
                printf("森林");
                break;
            case Grassland:
                printf("草原");
                break;
            case Mountain:
                printf("山脈");
                break;
            case Hill:
                printf("丘陵");
                break;
            case Desert:
                printf("沙漠");
                break;
            default:
              printf("%d", tiles[i].resourceType);
              break;
        }
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
