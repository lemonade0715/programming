#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "func.h"
#include "system.h"

#define WINDOWS 0

const char resource_name[][7] = {"小麥", "木頭", "羊毛", "石頭", "磚頭"};
const char develop_card_name[][13] = {"騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "資源壟斷", "資源壟斷", "道路建設", "道路建設", "創新發明", "創新發明", "分數卡", "分數卡", "分數卡", "分數卡", "分數卡"};
const int harbor[18] = {3, 4, 5, 6, 30, 36, 25, 31, 44, 49, 7, 13, 12, 18, 47, 52, 53, 54};
const char harbor_name[][10] = {"小麥2:1", "小麥2:1", "木頭2:1", "木頭2:1", "羊毛2:1", "羊毛2:1", "石頭2:1", "石頭2:1", "磚頭2:1", "磚頭2:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1"};
const int harbor_type[18] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6};

typedef enum State
{
    State_Prepare, // 準備
    State_Production, // 收成
    State_Trade, // 交易
    State_Build, // 建築
    State_Robber, // 強盜（收成階段擲到七點時）
} state;

System system_setting  = {
    .player_num = 3,
    .bank_develop_card = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1
    },
    .bank_resource = {19, 19, 19, 19, 19}, 
    .longest_road = 2,
    .lr_player = -1,
    .max_knight = 0,
    .mk_player = -1,
    .player_score = {0, 0, 0, 0}
};
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
    Player player_list[system_setting.player_num];
    
    generateCatanTiles(tiles);
    printCatanTiles(tiles);

    int current_player = 0;
    // TODO: Some Initialization
    
    srand(time(NULL));
    game_state = State_Production;
    
    while (1)
    {
        // TODO: Some Initialization
        for (int i = 0; i < system_setting.player_num; i++)
        {
            player_list[i].is_used_develop_card = 0;
            player_list[i].new_develop_card = -1;
        }
        //
        
        sleep(1);
        printf("\n\033[1m【玩家%d - 收成階段】\033[0m\n", current_player);
        production(player_list, current_player);
        game_state = State_Trade;
        
        sleep(1);
        printf("\n\033[1m【玩家%d - 交易階段】\033[0m", current_player);
        trade(player_list, current_player);
        game_state = State_Build;
        
        sleep(1);
        printf("\n\n\033[1m【玩家%d - 建築階段】\033[0m\n", current_player);
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
    #if WINDOWS
    int temp = (rand() % 6) + 2;
    temp += rand() % 6;
    #else
    int temp = arc4random_uniform(5) + 2;
    temp += arc4random_uniform(5);
    #endif
    
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
            while (1)
            {
                printf("\n你要和誰交易？\n（0為放棄交易，1-%d為其它玩家，%d為與銀行交易，%d選擇擁有的港口交易）：", system_setting.player_num - 1, system_setting.player_num, system_setting.player_num + 1);
                if (scanf("%d", &option) != 1)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                break;
            }
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
                    while (1)
                    {
                        printf("\n你想要用哪個資源兌換？(4:1交易)\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                        if (scanf("%d", &option_2) != 1)
                        {
                            while (getchar() != '\n');
                            continue;
                        }
                        while (getchar() != '\n');
                        break;
                    }
                    
                    if (option_2 == 0)
                    {
                        break;
                    }
                    else if (option_2 > 0 && option_2 < 6)
                    {
                        if (player_list[player].resource[option_2 - 1] < 4)
                        {
                            printf("\n您的%s只有%d個，需要4個才能進行與銀行的交易！\n", resource_name[option_2 - 1], player_list[player].resource[option_2 - 1]);
                            continue;
                        }
                        while (1)
                        {
                            int option_3 = 0;
                            while (1)
                            {
                                printf("\n你有%d個%s，請問您想兌換成哪種資源？(4:1交易)\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：", player_list[player].resource[option_2 - 1], resource_name[option_2 - 1]);
                                if (scanf("%d", &option_3) != 1)
                                {
                                    while (getchar() != '\n');
                                    continue;
                                }
                                while (getchar() != '\n');
                                break;
                            }
                            
                            if (option_3 == 0)
                            {
                                break;
                            }
                            else if (option_3 == option_2)
                            {
                                printf("\n不能兌換為同類型的資源，請選擇其它資源！\n");
                                continue;
                            }
                            else if (option_3 > 0 && option_3 < 6)
                            {
                                if (system_setting.bank_resource[option_3 - 1] < 1)
                                {
                                    printf("\n銀行的%s不足，請選擇其它資源！\n", resource_name[option_3 - 1]);
                                    continue;
                                }
                                player_list[player].resource[option_2 - 1] -= 4;
                                system_setting.bank_resource[option_2 - 1] += 4;
                                system_setting.bank_resource[option_3 - 1] -= 1;
                                player_list[player].resource[option_3 - 1] += 1;
                                printf("\n交易成功！\n");
                                option_2 = -1;
                                break;
                            }
                            else
                            {
                                printf("\n請輸入0-5之間的值！\n");
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
                        printf("\n請輸入0-5之間的值！\n");
                        continue;
                    }
                }
            }
            else if (option == system_setting.player_num + 1) // 港口交易
            {
                // Get player harbor infos
                printf("您擁有的港口類型：\n");
                int has_harbor[6] = {0};
                for (int i = 0; i < 18; i++)
                {
                    for (int j = 0; j < MAX_VILLAGES; j++)
                    {
                        has_harbor[harbor_type[i] - 1] += (harbor[i] == player_list[player].village[j]) ? 1 : 0;
                    }
                    for (int j = 0; j < MAX_CITIES; j++)
                    {
                        has_harbor[harbor_type[i] - 1] += (harbor[i] == player_list[player].city[j]) ? 1 : 0;
                    }
                }
                for (int i = 0; i < 6; i++)
                {
                    if (has_harbor[i])
                    {
                        printf("%d. %s x%d\n", i+1, harbor_name[2*i], has_harbor[i]);
                    }
                }
                // Ask player which harbor
                int option_2 = 0;
                while (1)
                {
                    printf("請選擇您要使用的港口（輸入0回到交易選單）：\n");
                    if (scanf("%d", &option_2) != 1)
                    {
                        while (getchar() != '\n');
                        continue;
                    }
                    while (getchar() != '\n');
                    if (option_2 < 0 || option_2 > 6)
                    {
                        printf("請輸入0-6之間的數值！\n");
                        continue;
                    }
                    else if (option_2 == 0)
                    {
                        break;
                    }
                    else if (has_harbor[option_2 - 1] == 0)
                    {
                        printf("您沒有這個港口！\n");
                        continue;
                    }
                    else if (option_2 == 6)
                    {
                        while (1)
                        {
                            int option_3 = 0;
                            printf("您想要使用哪個資源兌換？\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                            if (scanf("%d", &option_3) != 1)
                            {
                                while (getchar() != '\n');
                                continue;
                            }
                            while (getchar() != '\n');
                            if (option_3 < 0 || option_3 > 5)
                            {
                                printf("請輸入0-5之間的數值！\n");
                                continue;
                            }
                            else if (option_3 == 0)
                            {
                                break;
                            }
                            else if (player_list[player].resource[option_3 - 1] < 3)
                            {
                                printf("您的%s不足！\n", resource_name[option_3 - 1]);
                                continue;
                            }
                            else
                            {
                                int option_4 = 0;
                                while (1)
                                {
                                    printf("請選擇您想兌換的資源\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                                    if (scanf("%d", &option_4) != 1)
                                    {
                                        while (getchar() != '\n');
                                        continue;
                                    }
                                    while (getchar() != '\n');
                                    if (option_4 < 0 || option_4 > 5)
                                    {
                                        printf("請輸入0-5之間的數值！\n");
                                        continue;
                                    }
                                    else if (option_4 == 0)
                                    {
                                        break;
                                    }
                                    else if (system_setting.bank_resource[option_4 - 1] < 1)
                                    {
                                        printf("銀行的%s不足！\n", resource_name[option_4 - 1]);
                                        continue;
                                    }
                                    printf("您已使用3個%s兌換1個%s！\n", resource_name[option_3 - 1], resource_name[option_4 - 1]);
                                    player_list[player].resource[option_3 - 1] -= 3;
                                    system_setting.bank_resource[option_3 - 1] += 3;
                                    system_setting.bank_resource[option_4 - 1] -= 1;
                                    player_list[player].resource[option_4 - 1] += 1;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    else // 1 <= option_2 <= 5
                    {
                        if (player_list[player].resource[option_2 - 1] < 2)
                        {
                            printf("您的%s不足！\n", resource_name[option_2 - 1]);
                            break;
                        }
                        while (1)
                        {
                            int option_3 = 0;
                            printf("請選擇您想兌換的資源\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                            if (scanf("%d", &option_3) != 1)
                            {
                                while (getchar() != '\n');
                                continue;
                            }
                            while (getchar() != '\n');
                            if (option_3 < 0 || option_3 > 5)
                            {
                                printf("請輸入0-5之間的數值！\n");
                                continue;
                            }
                            else if (option_3 == 0)
                            {
                                break;
                            }
                            else if (system_setting.bank_resource[option_3 - 1] < 1)
                            {
                                printf("銀行的%s不足！\n", resource_name[option_3 - 1]);
                                continue;
                            }
                            printf("您已使用2個%s兌換1個%s！\n", resource_name[option_2 - 1], resource_name[option_3 - 1]);
                            player_list[player].resource[option_2 - 1] -= 2;
                            system_setting.bank_resource[option_2 - 1] += 2;
                            system_setting.bank_resource[option_3 - 1] -= 1;
                            player_list[player].resource[option_3 - 1] += 1;
                            break;
                        }
                        break;
                    }
                }
            }
            else
            {
                printf("\n請輸入0-%d之間的值！\n", system_setting.player_num + 1);
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
    
    if (player == 0) // 真人玩家
    {
        while (1)
        {
            while (1)
            {
                printf("請選擇您要將強盜移動至：");
                if (scanf("%d", &robber_position) != 1)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                break;
            }
            while (1)
            {
                printf("請選擇您要將強盜移動至：");
                if (scanf("%d", &robber_position) != 1)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                break;
            }
            
            if (robber_position < 0 || robber_position > 18)
            {
                printf("板塊需介於 0 ~ %d 之間！\n！\n", NUM_TILES - 1);
                continue;
                continue;
            }
            else if (robber_position == robber_position_prev) if (robber_position == robber_position_prev)
            {
                printf("強盜必須移動！\n！\n");
                continue;
                continue;
            }
            break;
            break;
        }
    }
    else // 電腦玩家
    {
        #if WINDOWS
        robber_position = rand() % NUM_TILES;
        while (robber_position == robber_position_prev)
        {
            robber_position = rand() % NUM_TILES;
        }
        #else
        robber_position = arc4random_uniform(NUM_TILES - 1);
        while (robber_position == robber_position_prev)
        {
            robber_position = arc4random_uniform(NUM_TILES - 1);
        }
        #endif
    }
    // 移動強盜，更新強盜位置
    tiles[robber_position_prev].hasRobber = 0;
    tiles[robber_position].hasRobber = 1;
    printf("強盜已經被玩家%d移動至板塊%d！\n", player, robber_position);
    
    // 搶奪資源
    int robbable_players[4] = {0};
    
    for (int i = 0; i < system_setting.player_num; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < MAX_VILLAGES; k++)
            {
                robbable_players[i] += (player_list[player].village[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
            }
            for (int k = 0; k < MAX_CITIES; k++)
            {
                robbable_players[i] += (player_list[player].city[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
            }
        }
    }
    robbable_players[player] = 0; // 不能搶自己
    // 不能搶沒有資源的人
    for (int i = 0; i < system_setting.player_num; i++)
    {
        if (player_list[i].total_resource < 1)
        {
            robbable_players[i] = 0;
        }
    }
    if (robbable_players[0] + robbable_players[1] + robbable_players[2] + robbable_players[3] == 0)
    {
        printf("板塊%d附近沒有村莊或城市！\n", robber_position);
        return 0;
    }
    
    if (player != 0)
    {
        while (1)
        {
            int option = 0;
            #if WINDOWS
            option = rand() % system_setting.player_num;
            #else
            option = arc4random_uniform(system_setting.player_num - 1);
            #endif
        
            if (option == player || robbable_players[option] == 0)
            {
                continue;
            }
            // 搶奪該位玩家的資源卡
            int temp_rob;
            #if WINDOWS
            temp_rob = rand() % player_list[option].total_resource;
            #else
            temp_rob = arc4random_uniform(player_list[option].total_resource - 1);
            #endif
            
            for (int i = 0; i < 5; i++)
            {
                if (temp_rob < player_list[option].resource[i])
                {
                    printf("玩家%d已從玩家%d搶奪1個%s！\n", player, option, resource_name[i]);
                    player_list[option].resource[i] -= 1;
                    player_list[player].resource[i] += 1;
                    break;
                }
                temp_rob -= player_list[option].resource[i];
            }
            break;
        }
        return 0;
    }
    // 人類玩家
    printf("您可以搶奪資源卡的玩家：\n");
    for (int i = 0; i < 4; i++)
    {
        if (robbable_players[i])
        {
            printf("%d. 玩家%d,\t", i, i);
        }
    }
    while (1)
    {
        int option = 0;
        printf("請問您要搶奪哪一位玩家？");
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        if (option < 0 || option >= system_setting.player_num)
        {
            printf("請輸入0~%d之間的值！\n", system_setting.player_num - 1);
            continue;
        }
        else if (option == player)
        {
            printf("不能搶奪自己的資源卡！\n");
            continue;
        }
        else if (robbable_players[option] == 0)
        {
            printf("該位玩家在強盜所在板塊的附近沒有村莊或城市！\n");
            continue;
        }
        // 搶奪該位玩家的資源卡
        int temp_rob;
        #if WINDOWS
        temp_rob = rand() % player_list[option].total_resource;
        #else
        temp_rob = arc4random_uniform(player_list[option].total_resource - 1);
        #endif
        
        for (int i = 0; i < 5; i++)
        {
            if (temp_rob < player_list[option].resource[i])
            {
                printf("您已從玩家%d搶奪1個%s！\n", option, resource_name[i]);
                player_list[option].resource[i] -= 1;
                player_list[player].resource[i] += 1;
                break;
            }
            temp_rob -= player_list[option].resource[i];
        }
        break;
    }
    return 0;
}

int use_develop_card(Player *player_list, int player)
{
    int option = 0;
    if (player == 0)
    {
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
            
            while (1)
            {
                printf("請選擇您要使用的發展卡代號（-1為不使用）：");
                if (scanf("%d", &option) != 1)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                break;
            }
            
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
                        while (1)
                        {
                            printf("請選擇您要將強盜移動至：");
                            if (scanf("%d", &robber_position) != 1)
                            {
                                while (getchar() != '\n');
                                continue;
                            }
                            while (getchar() != '\n');
                            break;
                        }
                        
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
                        
                        // 搶奪資源
                        int robbable_players[4] = {0};
                        
                        for (int i = 0; i < system_setting.player_num; i++)
                        {
                            for (int j = 0; j < 6; j++)
                            {
                                for (int k = 0; k < MAX_VILLAGES; k++)
                                {
                                    robbable_players[i] += (player_list[player].village[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
                                }
                                for (int k = 0; k < MAX_CITIES; k++)
                                {
                                    robbable_players[i] += (player_list[player].city[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
                                }
                            }
                        }
                        robbable_players[player] = 0; // 不能搶自己
                        // 不能搶沒有資源的人
                        for (int i = 0; i < system_setting.player_num; i++)
                        {
                            if (player_list[i].total_resource < 1)
                            {
                                robbable_players[i] = 0;
                            }
                        }
                        if (robbable_players[0] + robbable_players[1] + robbable_players[2] + robbable_players[3] == 0)
                        {
                            printf("板塊%d附近沒有村莊或城市！\n", robber_position);
                            return 0;
                        }
                        
                        if (player != 0)
                        {
                            while (1)
                            {
                                int option = 0;
                                #if WINDOWS
                                option = rand() % system_setting.player_num;
                                #else
                                option = arc4random_uniform(system_setting.player_num - 1);
                                #endif
                            
                                if (option == player || robbable_players[option] == 0)
                                {
                                    continue;
                                }
                                // 搶奪該位玩家的資源卡
                                int temp_rob;
                                #if WINDOWS
                                temp_rob = rand() % player_list[option].total_resource;
                                #else
                                temp_rob = arc4random_uniform(player_list[option].total_resource - 1);
                                #endif
                                
                                for (int i = 0; i < 5; i++)
                                {
                                    if (temp_rob < player_list[option].resource[i])
                                    {
                                        printf("玩家%d已從玩家%d搶奪1個%s！\n", player, option, resource_name[i]);
                                        player_list[option].resource[i] -= 1;
                                        player_list[player].resource[i] += 1;
                                        break;
                                    }
                                    temp_rob -= player_list[option].resource[i];
                                }
                                break;
                            }
                            return 0;
                        }
                        // 人類玩家
                        printf("您可以搶奪資源卡的玩家：\n");
                        for (int i = 0; i < 4; i++)
                        {
                            if (robbable_players[i])
                            {
                                printf("%d. 玩家%d,\t", i, i);
                            }
                        }
                        while (1)
                        {
                            int option = 0;
                            printf("請問您要搶奪哪一位玩家？");
                            if (scanf("%d", &option) != 1)
                            {
                                while (getchar() != '\n');
                                continue;
                            }
                            while (getchar() != '\n');
                            if (option < 0 || option >= system_setting.player_num)
                            {
                                printf("請輸入0~%d之間的值！\n", system_setting.player_num - 1);
                                continue;
                            }
                            else if (option == player)
                            {
                                printf("不能搶奪自己的資源卡！\n");
                                continue;
                            }
                            else if (robbable_players[option] == 0)
                            {
                                printf("該位玩家在強盜所在板塊的附近沒有村莊或城市！\n");
                                continue;
                            }
                            // 搶奪該位玩家的資源卡
                            int temp_rob;
                            #if WINDOWS
                            temp_rob = rand() % player_list[option].total_resource;
                            #else
                            temp_rob = arc4random_uniform(player_list[option].total_resource - 1);
                            #endif
                            
                            for (int i = 0; i < 5; i++)
                            {
                                if (temp_rob < player_list[option].resource[i])
                                {
                                    printf("您已從玩家%d搶奪1個%s！\n", option, resource_name[i]);
                                    player_list[option].resource[i] -= 1;
                                    player_list[player].resource[i] += 1;
                                    break;
                                }
                                temp_rob -= player_list[option].resource[i];
                            }
                            break;
                        }
                        break;
                    }
                }
                else if (option == 14 || option == 15) // 資源壟斷
                {
                    while (1)
                    {
                        int option_2 = 0;
                        while (1)
                        {
                            printf("選擇一種資源，其它玩家必須將他手上所有該種資源交給你\n（1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                            if (scanf("%d", &option_2) != 1)
                            {
                                while (getchar() != '\n');
                                continue;
                            }
                            while (getchar() != '\n');
                            break;
                        }
                        
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
                        while (1)
                        {
                            printf("請選擇您要拿取的第一張資源卡\n（1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                            if (scanf("%d", &option_2) != 1)
                            {
                                while (getchar() != '\n');
                                continue;
                            }
                            while (getchar() != '\n');
                            break;
                        }
                        
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
                        while (1)
                        {
                            printf("請選擇您要拿取的第二張資源卡\n（1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：");
                            if (scanf("%d", &option_2) != 1)
                            {
                                while (getchar() != '\n');
                                continue;
                            }
                            while (getchar() != '\n');
                            break;
                        }
                        
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
    
    // Computer
    switch (player_list[player].NPC_difficulty)
    {
        case 1:
            for (int i = 0; i < 1000; i++)
            {
                #if WINDOWS
                option = rand() % 20;
                #else
                option = arc4random_uniform(19);
                #endif
                if (player_list[player].develop_cards[option] == 1)
                {
                    break;
                }
            }
            break;
        case 2:
            #if WINDOWS
            option = rand() % 20;
            #else
            option = arc4random_uniform(19);
            #endif
            break;
    }
    
    int option_2 = 0;
    if (player_list[player].develop_cards[option] == 1)
    {
        printf("玩家%d使用發展卡 - %d. %s！\n", player, option, develop_card_name[option]);
        if (option < 14) // 騎士卡
        {
            int robber_position = -1;
            int robber_position_prev = 0;
            for (int i = 0; i < NUM_TILES; i++)
            {
                robber_position_prev = (tiles[i].hasRobber) ? i : robber_position_prev;
            }
            
            #if WINDOWS
            robber_position = rand() % NUM_TILES;
            while (robber_position == robber_position_prev)
            {
                robber_position = rand() % NUM_TILES;
            }
            #else
            robber_position = arc4random_uniform(NUM_TILES - 1);
            while (robber_position == robber_position_prev)
            {
                robber_position = arc4random_uniform(NUM_TILES - 1);
            }
            #endif
            
            // 移動強盜，更新強盜位置
            tiles[robber_position_prev].hasRobber = 0;
            tiles[robber_position].hasRobber = 1;
            printf("強盜已經被移動至板塊%d！\n", robber_position);
            
            // 搶奪資源
            int robbable_players[4] = {0};
            
            for (int i = 0; i < system_setting.player_num; i++)
            {
                for (int j = 0; j < 6; j++)
                {
                    for (int k = 0; k < MAX_VILLAGES; k++)
                    {
                        robbable_players[i] += (player_list[player].village[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
                    }
                    for (int k = 0; k < MAX_CITIES; k++)
                    {
                        robbable_players[i] += (player_list[player].city[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
                    }
                }
            }
            robbable_players[player] = 0; // 不能搶自己
            // 不能搶沒有資源的人
            for (int i = 0; i < system_setting.player_num; i++)
            {
                if (player_list[i].total_resource < 1)
                {
                    robbable_players[i] = 0;
                }
            }
            if (robbable_players[0] + robbable_players[1] + robbable_players[2] + robbable_players[3] == 0)
            {
                printf("板塊%d附近沒有村莊或城市！\n", robber_position);
                return 0;
            }
            
            if (player != 0)
            {
                while (1)
                {
                    int option = 0;
                    #if WINDOWS
                    option = rand() % system_setting.player_num;
                    #else
                    option = arc4random_uniform(system_setting.player_num - 1);
                    #endif
                
                    if (option == player || robbable_players[option] == 0)
                    {
                        continue;
                    }
                    // 搶奪該位玩家的資源卡
                    int temp_rob;
                    #if WINDOWS
                    temp_rob = rand() % player_list[option].total_resource;
                    #else
                    temp_rob = arc4random_uniform(player_list[option].total_resource - 1);
                    #endif
                    
                    for (int i = 0; i < 5; i++)
                    {
                        if (temp_rob < player_list[option].resource[i])
                        {
                            printf("玩家%d已從玩家%d搶奪1個%s！\n", player, option, resource_name[i]);
                            player_list[option].resource[i] -= 1;
                            player_list[player].resource[i] += 1;
                            break;
                        }
                        temp_rob -= player_list[option].resource[i];
                    }
                    break;
                }
                return 0;
            }
            // 人類玩家
            printf("您可以搶奪資源卡的玩家：\n");
            for (int i = 0; i < 4; i++)
            {
                if (robbable_players[i])
                {
                    printf("%d. 玩家%d,\t", i, i);
                }
            }
            while (1)
            {
                int option = 0;
                printf("請問您要搶奪哪一位玩家？");
                if (scanf("%d", &option) != 1)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                if (option < 0 || option >= system_setting.player_num)
                {
                    printf("請輸入0~%d之間的值！\n", system_setting.player_num - 1);
                    continue;
                }
                else if (option == player)
                {
                    printf("不能搶奪自己的資源卡！\n");
                    continue;
                }
                else if (robbable_players[option] == 0)
                {
                    printf("該位玩家在強盜所在板塊的附近沒有村莊或城市！\n");
                    continue;
                }
                // 搶奪該位玩家的資源卡
                int temp_rob;
                #if WINDOWS
                temp_rob = rand() % player_list[option].total_resource;
                #else
                temp_rob = arc4random_uniform(player_list[option].total_resource - 1);
                #endif
                
                for (int i = 0; i < 5; i++)
                {
                    if (temp_rob < player_list[option].resource[i])
                    {
                        printf("您已從玩家%d搶奪1個%s！\n", option, resource_name[i]);
                        player_list[option].resource[i] -= 1;
                        player_list[player].resource[i] += 1;
                        break;
                    }
                    temp_rob -= player_list[option].resource[i];
                }
                break;
            }
        }
        else if (option == 14 || option == 15) // 資源壟斷
        {
            #if WINDOWS
            option_2 = rand() % 5;
            #else
            option_2 = arc4random_uniform(4);
            #endif
            
            for (int i = 0; i < system_setting.player_num; i++)
            {
                if (i == player)
                {
                    continue;
                }
                printf("玩家%d向玩家%d搶奪%d個%s！\n", player, i, player_list[i].resource[option_2], resource_name[option_2]);
                player_list[player].resource[option_2] += player_list[i].resource[option_2];
                player_list[i].resource[option_2] = 0;
            }
        }
        else if (option == 16 || option == 17) // 道路建設
        {
            // TODO: 道路建設
        }
        else if (option == 18 || option == 19) // 創新發明
        {
            while (1)
            {
                #if WINDOWS
                option_2 = rand() % 5;
                #else
                option_2 = arc4random_uniform(4);
                #endif
                
                if (system_setting.bank_resource[option_2] < 1)
                {
                    continue;
                }
                printf("玩家%d向銀行拿取1個%s！\n", player, resource_name[option_2]);
                system_setting.bank_resource[option_2] -= 1;
                player_list[player].resource[option_2] += 1;
                break;
            }
            while (1)
            {
                #if WINDOWS
                option_2 = rand() % 5;
                #else
                option_2 = arc4random_uniform(4);
                #endif
                
                if (system_setting.bank_resource[option_2] < 1)
                {
                    continue;
                }
                printf("玩家%d向銀行拿取1個%s！\n", player, resource_name[option_2]);
                system_setting.bank_resource[option_2] -= 1;
                player_list[player].resource[option_2] += 1;
                break;
            }
        }
    }
    return 0;
}
