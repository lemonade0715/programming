#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "func.h"

const char resource_name[][6] = {"小麥", "木頭", "羊毛", "石頭", "磚頭"};
const char develop_card_name[][12] = {"騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "資源壟斷", "資源壟斷", "道路建設", "道路建設", "創新發明", "創新發明", "分數卡", "分數卡", "分數卡", "分數卡", "分數卡"};

typedef enum State
{
    State_Prepare, // 準備
    State_Production, // 收成
    State_Trade, // 交易
    State_Build, // 建築
    State_Robber, // 強盜（收成階段擲到七點時）
} state;

System system_setting;
struct CatanTile tiles[NUM_TILES];

int production(Player *player_list, int player);
int trade(Player *player_list, int player);
int build(Player *player_list, int player);
int robber(Player *player_list, int player);
int use_develop_card(Player *player_list, int player);

int main()
{
    // 將遊戲設定至準備階段
    state game_state = State_Prepare;
    system_setting.player_num = 4;
    Player player_list[system_setting.player_num];
    
    generateCatanTiles(tiles);
    printCatanTiles(tiles);
    
    int current_player = 0;
    // TODO: Some Initialization
    
    game_state = State_Production;
    current_player = 0;
    
    while (1)
    {
        // TODO: Some Initialization
        for (int i = 0; i < system_setting.player_num; i++)
        {
            player_list[i].is_used_develop_card = 0;
            player_list[i].new_develop_card = -1;
        }
        //
        
        printf("\033[1m【玩家%d - 收成階段】\033[0m\n", current_player);
        production(player_list, current_player);
        game_state = State_Trade;
        
        printf("\033[1m【玩家%d - 交易階段】\033[0m\n", current_player);
        trade(player_list, current_player);
        game_state = State_Build;
        
        printf("\033[1m【玩家%d - 建築階段】\033[0m\n", current_player);
        build(player_list, current_player);
        game_state = State_Production;
        
        current_player = (current_player + 1) % system_setting.player_num;
    }
    
    return 0;
}

int production(Player *player_list, int player)
{
    printf("當前玩家：玩家%d\n", player);
    // 擲骰子
    int temp = arc4random_uniform(5) + 2;
    temp += arc4random_uniform(5);
    // 印出擲骰資訊
    printf("骰子點數和：%d，", temp);
    
    // 盜賊
    if (temp == 7)
    {
        printf("觸發強盜事件！\n");
        robber(player_list, player);
    }
    else
    {
        printf("即將發放資源！\n");
        // TODO: 資源發放
    }
    
    return 0;
}

int trade(Player *player_list, int player)
{
    if (player == 0)
    {
        while (1)
        {
            int option = 0;
            printf("你要和誰交易？\n（0為放棄交易，1-%d為其它玩家，%d為與銀行交易，%d選擇擁有的港口交易）：", system_setting.player_num - 1, system_setting.player_num, system_setting.player_num + 1);
            scanf("%d", &option);
            if (option == 0)
            {
                printf("交易階段結束\n");
                return 0;
            }
            else if (option > 0 && option < system_setting.player_num)
            {
                // TODO: Trade with other players
            }
            else if (option == system_setting.player_num)
            {
                while (1)
                {
                    int option_2 = 0;
                    printf("你想要用哪個資源兌換？(4:1交易)\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                    scanf("%d", &option_2);
                    if (option_2 == 0)
                    {
                        break;
                    }
                    else if (option_2 > 0 && option_2 < 6)
                    {
                        if (player_list[player].resource[option_2 - 1] < 4)
                        {
                            printf("您的%s只有%d個，需要4個才能進行與銀行的交易！\n", resource_name[option_2 - 1], player_list[player].resource[option_2 - 1]);
                            continue;
                        }
                        while (1)
                        {
                            int option_3 = 0;
                            printf("你有%d個%s，請問您想兌換成哪種資源？(4:1交易)\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：", player_list[player].resource[option_2 - 1], resource_name[option_2 - 1]);
                            scanf("%d", &option_3);
                            if (option_3 == 0)
                            {
                                break;
                            }
                            else if (option_3 > 0 && option_3 < 6)
                            {
                                if (system_setting.bank_resource[option_3 - 1] < 1)
                                {
                                    printf("銀行的%s不足，請選擇其它資源！\n", resource_name[option_3 - 1]);
                                    continue;
                                }
                                player_list[player].resource[option_2 - 1] -= 4;
                                system_setting.bank_resource[option_2 - 1] += 4;
                                system_setting.bank_resource[option_3 - 1] -= 1;
                                player_list[player].resource[option_3 - 1] += 1;
                                printf("交易成功！\n");
                                option_2 = -1;
                                break;
                            }
                            else
                            {
                                printf("請輸入0-5之間的值！\n");
                                continue;
                            }
                        }
                        
                        if (option_2 == -1)
                        {
                            break;
                        }
                    }
                    else
                    {
                        printf("請輸入0-5之間的值！\n");
                        continue;
                    }
                }
            }
            else if (option == system_setting.player_num + 1)
            {
                // TODO: 港口交易
            }
            else
            {
                printf("請輸入0-%d之間的值！\n", system_setting.player_num + 1);
                continue;
            }
        }
        return 0;
    }
    
    // TODO: computer player
    
    return 0;
}

int build(Player *player_list, int player)
{
    
    
    
    
    return 0;
}

int robber(Player *player_list, int player)
{
    // 檢查是否有人資源卡超過七張
    printf("強盜來ㄌ！資源卡超過七張的人將被搶走一半的資源卡！\n");
    for (int i = 0; i < system_setting.player_num; i++)
    {
        while (player_list[i].total_resource > 7)
        {
            // 捨棄最多的資源卡，若最多的不止一個，則捨棄其中的第一個
            int max_pos = 5;
            for (int j = 4; j >= 0; j--)
            {
                max_pos = (player_list[i].resource[j] > player_list[i].resource[max_pos]) ? j : max_pos;
            }
            player_list[i].resource[max_pos]--;
            player_list[i].total_resource--;
        }
    }
    
    // 移動強盜
    int robber_position = -1;
    int robber_position_prev = 0;
    for (int i = 0; i < NUM_TILES; i++)
    {
        robber_position_prev = (tiles[i].hasRobber) ? i : robber_position_prev;
    }
    
    printf("請玩家%d移動強盜的位置。\n", player);
    if (player == 0) // 真人玩家
    {
        while (1)
        {
            printf("請選擇您要將強盜移動至：");
            scanf("%d", &robber_position);
            if (robber_position < 0 || robber_position > 18)
            {
                printf("板塊需介於 0 ~ %d 之間！\n", NUM_TILES - 1);
                continue;
            }
            else if (robber_position == robber_position_prev)
            {
                printf("強盜必須移動！\n");
                continue;
            }
            break;
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
        printf("%d\n", robber_position);
    }
    // 移動強盜，更新強盜位置
    tiles[robber_position_prev].hasRobber = 0;
    tiles[robber_position].hasRobber = 1;
    printf("強盜已經被移動至板塊%d！\n", robber_position);
    
    // TODO: 搶奪資源
    
    return 0;
}

int use_develop_card(Player *player_list, int player)
{
    if (player == 0)
    {
        int option = 0;
        while (1)
        {
            printf("以下為您可以使用的發展卡（灰色代表已使用）\n");
            for (int i = 0; i < 20; i++)
            {
                if (player_list[player].develop_cards[i] == 1)
                {
                    printf("\033[90m%2d. %s\033[0m\n", i, develop_card_name[i]);
                }
                else if (player_list[player].develop_cards[i] == 2)
                {
                    printf("%2d. %s\n", i, develop_card_name[i]);
                }
            }
            printf("請選擇您要使用的發展卡代號（-1為不使用）：");
            scanf("%d", &option);
            if (option == -1)
            {
                return 0;
            }
            else if (option >= 0 && option < 20)
            {
                if (player_list[player].develop_cards[option] == 0)
                {
                    printf("你未擁有此張發展卡，請重新輸入\n");
                    continue;
                }
                else if (player_list[player].develop_cards[option] == 2)
                {
                    printf("您已經使用過此張發展卡，請重新輸入\n");
                    continue;
                }
                
                // 將該卡片更改為已使用
                player_list[player].develop_cards[option] = 2;
                
                if (option < 14) // 騎士卡
                {
                    int robber_position = -1;
                    int robber_position_prev = 0;
                    for (int i = 0; i < NUM_TILES; i++)
                    {
                        robber_position_prev = (tiles[i].hasRobber) ? i : robber_position_prev;
                    }
                    while (1)
                    {
                        printf("請選擇您要將強盜移動至：");
                        scanf("%d", &robber_position);
                        if (robber_position < 0 || robber_position > 18)
                        {
                            printf("板塊需介於 0 ~ %d 之間！\n", NUM_TILES - 1);
                            continue;
                        }
                        else if (robber_position == robber_position_prev)
                        {
                            printf("強盜必須移動！\n");
                            continue;
                        }
                        // 移動強盜，更新強盜位置
                        tiles[robber_position_prev].hasRobber = 0;
                        tiles[robber_position].hasRobber = 1;
                        printf("強盜已經被移動至板塊%d！\n", robber_position);
                        
                        // TODO: 搶奪資源
                        
                        break;
                    }
                }
                else if (option == 14 || option == 15) // 資源壟斷
                {
                    while (1)
                    {
                        int option_2 = 0;
                        printf("選擇一種資源，其它玩家必須將他手上所有該種資源交給你\n（1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                        scanf("%d", &option_2);
                        if (option_2 < 1 || option_2 > 5)
                        {
                            printf("輸入錯誤，請輸入資源代碼(1-5)！\n");
                            continue;
                        }
                        for (int j = 1; j < system_setting.player_num; j++)
                        {
                            printf("您從玩家%d獲得%d個%s！\n", j, player_list[j].resource[option_2 - 1], resource_name[option_2 - 1]);
                            player_list[player].resource[option_2 - 1] += player_list[j].resource[option_2 - 1];
                            player_list[j].resource[option_2 - 1] = 0;
                        }
                    }
                }
                else if (option == 16 || option == 17) // 道路建設
                {
                    // TODO: 道路建設
                }
                else if (option == 18 || option == 19) // 創新發明
                {
                    printf("您可以從銀行選擇兩張任意的資源卡：\n");
                    while (1)
                    {
                        int option_2 = 0;
                        printf("請選擇您要拿取的第一張資源卡\n（1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                        scanf("%d", &option_2);
                        if (option_2 < 1 || option_2 > 5)
                        {
                            printf("輸入錯誤，請輸入資源代碼(1-5)！\n");
                            continue;
                        }
                        else if (system_setting.bank_resource[option_2 - 1] < 1)
                        {
                            printf("銀行的%s不足，請選擇其它資源！\n", resource_name[option_2 - 1]);
                            continue;
                        }
                        system_setting.bank_resource[option_2 - 1] -= 1;
                        player_list[player].resource[option_2 - 1] += 1;
                        printf("您已獲得1個%s！\n", resource_name[option_2 - 1]);
                        break;
                    }
                    while (1)
                    {
                        int option_2 = 0;
                        printf("請選擇您要拿取的第二張資源卡\n（1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                        scanf("%d", &option_2);
                        if (system_setting.bank_resource[option_2 - 1] < 1)
                        {
                            printf("銀行的%s不足，請選擇其它資源！\n", resource_name[option_2 - 1]);
                            continue;
                        }
                        system_setting.bank_resource[option_2 - 1] -= 1;
                        player_list[player].resource[option_2 - 1] += 1;
                        printf("您已獲得1個%s！\n", resource_name[option_2 - 1]);
                        break;
                    }
                }
                player_list[player].is_used_develop_card = 1;
                return 0;
            }
            else if (option >= 20 && option < 25)
            {
                printf("無法使用分數卡，請輸入代號(0-19)，離開請輸入-1\n");
                continue;
            }
            else
            {
                printf("輸入錯誤，請輸入代號(0-19)，離開請輸入-1\n");
                continue;
            }
        }
    }
    
    // TODO: Computer
    
    
    return 0;
}
