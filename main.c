#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "func.h"

typedef enum State
{
    State_Prepare, // 準備
    State_Production, // 收成
    State_Trade, // 交易
    State_Build, // 建築
    State_Robber, // 強盜（收成階段擲到七點時）
} state;

System system_setting;

int production();
int trade();
int build();
int robber();

int main()
{
    // 將遊戲設定至準備階段
    state game_state = State_Prepare;
    system_setting.player_num = 4;
    Player players[system_setting.player_num];
    
    struct CatanTile tiles[NUM_TILES];
    generateCatanTiles(tiles);
    printCatanTiles(tiles);
    
    int current_player = 0;
        // 做一些初始化設定
    
    game_state = State_Production;
    current_player = 0;
    
    while (1)
    {
        printf("玩家%d收成階段：\n", current_player);
        production(current_player);
        game_state = State_Trade;
        
        printf("玩家%d交易階段：\n", current_player);
        trade(current_player);
        game_state = State_Build;
        
        printf("玩家%d建築階段：\n", current_player);
        build(current_player);
        game_state = State_Production;
        
        current_player = (current_player + 1) % system_setting.player_num;
    }
    
    return 0;
}

int production(int player)
{
    printf("當前玩家：代號%d\n", player);
    // 擲骰子
    srand(time(NULL));
    int temp = (rand() % 6) + 2;
    srand(time(NULL) + 1);
    temp += rand() % 6;
    // 印出擲骰資訊
    printf("骰子點數和：%d，", temp);
    
    // 盜賊
    if (temp == 7)
    {
        printf("觸發強盜事件！\n");
        robber(player);
    }
    else
    {
        printf("即將發放資源！\n");
        // TODO: 資源發放
    }
    
    return 0;
}

int trade(int player)
{
    
    
    
    return 0;
}

int build(int player)
{
    
    
    
    
    return 0;
}

int robber(int player)
{
    // 檢查是否有人資源卡超過七張
    for (int i = 0; i < system_setting.player_num; i++)
    {
        while (players[i].total_resource > 7)
        {
            // 捨棄最多的資源卡，若最多的不止一個，則捨棄其中的第一個
            int max_pos = 6;
            for (int j = 5; j >= 0; j--)
            {
                max_pos = (players[i].resource[j] > players[i].resource[max_pos]) ? j : max_pos;
            }
            players[i].resource[max_pos]--;
            players[i].total_resource--;
        }
    }
    
    // 移動強盜
    int robber_position = 0;
    int robber_position_prev = 0;
    // TODO: 取得目前的強盜位置
    printf("請玩家%d移動強盜的位置。\n", player);
    if (player == 0) // 真人玩家
    {
        printf("請選擇您要將強盜移動至：");
        scanf("%d", &robber_position);
        while (robber_position == robber_position_prev)
        {
            printf("強盜必須移動，請選擇您要將強盜移動至：");
            scanf("%d", &robber_position);
        }
    }
    else // 電腦玩家
    {
        printf("請選擇您要將強盜移動至：");
        srand(time(NULL));
        robber_position = rand() % NUM_TILES;
        while (robber_position == robber_position_prev)
        {
            robber_position = rand() % NUM_TILES;
        }
    }
    // TODO: 移動強盜，更新強盜位置
    printf("強盜已經被移動至板塊%d！\n", robber_position);
    
    // TODO: 搶奪資源
    
    return 0;
}
