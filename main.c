#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "func.h"
#include "system.h"
#include "map.h"
#include "computer.h"

#define WINDOWS 0

const char develop_card_name[][13] = {"騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "騎士卡", "資源壟斷", "資源壟斷", "道路建設", "道路建設", "創新發明", "創新發明", "分數卡", "分數卡", "分數卡", "分數卡", "分數卡"};
const int harbor[18] = {3, 4, 5, 6, 30, 36, 25, 31, 44, 49, 7, 13, 12, 18, 47, 52, 53, 54};
const char harbor_name[][10] = {"小麥2:1", "小麥2:1", "木頭2:1", "木頭2:1", "羊毛2:1", "羊毛2:1", "石頭2:1", "石頭2:1", "磚頭2:1", "磚頭2:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1", "任意3:1"};
const int harbor_type[18] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6};
int robber_loc = 0;

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
void init_player(Player *players, System *system_setting);
void set_village(Player *players, System *system_setting, struct CatanTile *tiles);
int is_has_built(Player *players, int point_id);
int connect_village_limit(Player *players, int point_id);
void NPC_build(Player *players, int player, System *sys);

int main()
{
    // 將遊戲設定至準備階段
    state game_state = State_Prepare;
    Player player_list[system_setting.player_num];
    
    generateCatanTiles(tiles);
    printCatanTiles(tiles);

    int current_player = 0;
    // TODO: Some Initialization
    init_player(player_list, &system_setting);
    robber_loc = robber_location(tiles);

    print_map_state2(player_list, tiles, &system_setting, robber_loc);
    set_village(player_list, &system_setting, tiles);
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
        robber_loc = robber_location(tiles);
        if(!current_player) {
            print_map_state2(player_list, tiles, &system_setting, robber_loc);
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
        printf("--------------------------------------------\n");
    }
    return 0;
}

int production(Player *player_list, int player)
{
    // 擲骰子
    #if WINDOWS
    int temp = (rand() % 6) + 2;
    temp += rand() % 6;
    #else
    int temp = arc4random_uniform(6) + 2;
    temp += arc4random_uniform(6);
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
        // 資源發放
        for (int i = 0; i < NUM_TILES; i++)
        {
            if (tiles[i].number == temp && tiles[i].resourceType == 5)
            {
                printf("沙漠板塊不會產生資源！\n");
            }
            else if (tiles[i].number == temp)
            {
                for (int j = 0; j < system_setting.player_num; j++)
                {
                    for (int k = 0; k < 6; k++)
                    {
                        for (int l = 0; l < MAX_VILLAGES; l++)
                        {
                            if (player_list[j].village[l] == 0 || tiles[i].corner_id[k] == 0)
                            {
                                continue;
                            }
                            else if (player_list[j].village[l] == tiles[i].corner_id[k])
                            {
                                printf("玩家%d的村莊(%d)收穫1個%s！\n", j, player_list[j].village[l], resource_name[tiles[i].resourceType]);
                                player_list[j].resource[tiles[i].resourceType] += 1;
                                player_list[j].total_resource += 1;
                            }
                        }
                        for (int l = 0; l < MAX_CITIES; l++)
                        {
                            if (player_list[j].city[l] == 0 || tiles[i].corner_id[k] == 0)
                            {
                                continue;
                            }
                            if (player_list[j].city[l] == tiles[i].corner_id[k])
                            {
                                printf("玩家%d的城市(%d)收穫2個%s！\n", j, player_list[j].city[l], resource_name[tiles[i].resourceType]);
                                player_list[j].resource[tiles[i].resourceType] += 2;
                                player_list[j].total_resource += 2;
                            }
                        }
                    }
                }
            }
        }
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
                print_allResource(player_list, &system_setting);
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
                printf("\n交易階段結束");
                return 0;
            }
            else if (option > 0 && option < system_setting.player_num) // 玩家交易
            {
                int object_id = option;
                printf("\n你想要用哪個資源與玩家%d進行兌換？可用任意數量\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：", option);
                if (scanf("%d", &option) != 1)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');

                if(!option) continue;
                else if(option < 0 || option > 5) printf("交易失敗，請輸入0-5之間的數值\n");
                else trade_with_player(player_list, object_id, option);
            }
            else if (option == system_setting.player_num) // 銀行交易
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
                                player_list[player].total_resource -= 3;
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
                                    player_list[player].total_resource -= 2;
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
                            player_list[player].total_resource -= 1;
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
    
    // 電腦玩家
    computer_trade(player_list, player, &system_setting, tiles);
    return 0;
}

int build(Player *player_list, int player)
{
    // 電腦
    if (player != 0)
    {
        // TODO: ...
        NPC_build(player_list, player, &system_setting);
        
        return 0;
    }
    // 玩家
    while (1)
    {
        int option = 0;
        printf("請選擇您要執行的動作：\n（0.結束建築 1.建造道路 2.建造村莊 3.建造城市 4.購買發展卡）：");
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        if (option == 0)
        {
            break;
        }
        else if (option == 1)
        {
            // 檢查資源是否足夠
            if (player_list[player].resource[1] < 1 || player_list[player].resource[4] < 1)
            {
                printf("您的資源不足！建造道路需要1個木頭、1個磚頭！\n");
                continue;
            }
            
            while (1)
            {
                int road_1 = 0, road_2 = 0;
                printf("請選擇您要蓋的道路（例如17,23，回上頁請輸入0,0）：");
                if (scanf("%d,%d", &road_1, &road_2) != 2)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                if (road_1 == 0 && road_2 == 0)
                {
                    break;
                }
                else if (road_1 == 0 || road_2 == 0) // xor
                {
                    printf("道路的兩個頂點皆需輸入 1-54 的數值！\n");
                    continue;
                }
                else if (road_1 == road_2)
                {
                    printf("道路的兩端點不能為同一點！\n");
                    continue;
                }
                else if (check_connected(road_1, road_2) == 0)
                {
                    printf("您選取的點 %d 與 %d 無法構成一個有效的道路！\n", road_1, road_2);
                    continue;
                }
                else if (check_if_has_road(player_list, road_1, road_2) == 1)
                {
                    printf("您選取的道路 %d,%d 已經被佔有！\n", road_1, road_2);
                    continue;
                }
                else if (check_if_connected_build(player_list, player, road_1, road_2) == 0)
                {
                    printf("您選取的道路 %d,%d 沒有跟您的其它道路或建築連接！\n", road_1, road_2);
                    continue;
                }
                // 已確認道路合法、資源足夠
                while (1)
                {
                    int option_2 = 0;
                    printf("請確認您是否要用1個木頭、1個磚頭建造道路 %d,%d？（0:否 1:是）：", road_1, road_2);
                    if (scanf("%d", &option_2) != 1)
                    {
                        while (getchar() != '\n');
                        continue;
                    }
                    while (getchar() != '\n');
                    if (option_2 != 0 && option_2 != 1)
                    {
                        continue;
                    }
                    else if (option_2 == 0)
                    {
                        break;
                    }
                    // option_2 == 1
                    printf("您已成功用1個木頭、1個磚頭建造道路 %d,%d！\n", road_1, road_2);
                    player_list[player].resource[1] -= 1;
                    player_list[player].resource[4] -= 1;
                    add_road_to_player(player_list, player, road_1, road_2);
                    break;
                }
                break;
            }
        }
        else if (option == 2)
        {
            // 檢查鄉村數量
            int village_count = 0;
            for (int i = 0; i < MAX_VILLAGES; i++)
            {
                village_count += (player_list[player].village[i]) ? 1 : 0;
            }
            if (village_count == MAX_VILLAGES)
            {
                printf("您已經有%d座村莊！\n", MAX_VILLAGES);
                continue;
            }
            
            // 檢查資源是否足夠
            if (player_list[player].resource[0] < 1 || player_list[player].resource[1] < 1 || player_list[player].resource[2] < 1 || player_list[player].resource[4] < 1)
            {
                printf("您的資源不足，建造村莊需要1個小麥、1個木頭、1個羊毛、1個磚頭！\n");
                continue;
            }
            
            while (1)
            {
                int option_2 = 0;
                printf("請選擇您要將村莊蓋在何處？\n");
                if (scanf("%d", &option_2) != 1)
                {
                    while (getchar() != '\n');
                    printf("請輸入1-54的數字！\n");
                    continue;
                }
                while (getchar() != '\n');
                if (option_2 < 1 || option_2 > 54)
                {
                    printf("請輸入1-54的數字！\n");
                    continue;
                }
                
                int valid_flag = 1;
                // 如果該處已經有村莊或城市
                for (int i = 0; i < MAX_VILLAGES * system_setting.player_num; i++)
                {
                    if (player_list[i / MAX_VILLAGES].village[i % MAX_VILLAGES] == option_2)
                    {
                        valid_flag = 0;
                        break;
                    }
                }
                for (int i = 0; i < MAX_CITIES * system_setting.player_num; i++)
                {
                    if (player_list[i / MAX_CITIES].city[i % MAX_CITIES] == option_2)
                    {
                        valid_flag = 0;
                        break;
                    }
                }
                // 如果附近已有村莊或城市
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < MAX_VILLAGES * system_setting.player_num; j++)
                    {
                        if (player_list[j / MAX_VILLAGES].village[j % MAX_VILLAGES] == connectedPoint[option_2][i])
                        {
                            valid_flag = 0;
                            break;
                        }
                    }
                    for (int j = 0; j < MAX_CITIES * system_setting.player_num; j++)
                    {
                        if (player_list[j / MAX_CITIES].city[j % MAX_CITIES] == connectedPoint[option_2][i])
                        {
                            valid_flag = 0;
                            break;
                        }
                    }
                }
                
                if (valid_flag)
                {
                    int option_3 = 0;
                    while (1)
                    {
                        printf("您是否確定花費1個小麥、1個木頭、1個羊毛、1個磚頭在%d建造村莊？（0:否 1:是）\n", option_2);
                        if (scanf("%d", &option_3) != 1)
                        {
                            while (getchar() != '\n');
                            option_3 = 0;
                            continue;
                        }
                        while (getchar() != '\n');
                        if (option_3 != 0 && option_3 != 1)
                        {
                            printf("請輸入0或1！\n");
                            continue;
                        }
                        break;
                    }
                    if (option_3 == 0)
                    {
                        continue;
                    }
                    // option_3 == 1
                    printf("您已花費1個小麥、1個木頭、1個羊毛、1個磚頭在%d建造村莊！\n", option_2);
                    player_list[player].resource[0] -= 1;
                    player_list[player].resource[1] -= 1;
                    player_list[player].resource[2] -= 1;
                    player_list[player].resource[4] -= 1;
                    player_list[player].village[option_2] += 1;
                    player_list[player].total_resource -= 4;
                    break;
                }
            }
        }
        else if (option == 3)
        {
            // 檢查城市數量
            int city_count = 0;
            for (int i = 0; i < MAX_CITIES; i++)
            {
                city_count += (player_list[player].city[i]) ? 1 : 0;
            }
            if (city_count == MAX_CITIES)
            {
                printf("您已經有%d座城市！\n", MAX_CITIES);
                continue;
            }
            
            // 檢查鄉村數量
            int village_count = 0;
            for (int i = 0; i < MAX_VILLAGES; i++)
            {
                village_count += (player_list[player].village[i]) ? 1 : 0;
            }
            if (village_count == 0)
            {
                printf("您沒有可以升級成城市的村莊！\n");
                continue;
            }
            
            // 檢查資源是否足夠
            if (player_list[player].resource[0] < 2 || player_list[player].resource[3] < 3)
            {
                printf("您的資源不足，建造城市需要2個小麥、3個石頭！\n");
                continue;
            }
            
            // 選擇升級的村莊
            while (1)
            {
                printf("請選擇您要升級的村莊（輸入0為放棄升級）\n（您擁有的村莊為 ");
                for (int i = 0; i < MAX_VILLAGES; i++)
                {
                    if (player_list[player].village[i])
                    {
                        printf("%d ", player_list[player].village[i]);
                    }
                }
                printf("）：");
                int option_2 = 0;
                if (scanf("%d", &option_2) != 1)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                if (option_2 == 0)
                {
                    break;
                }
                else if (option_2 < 1 || option_2 > 54)
                {
                    printf("請輸入1~54之間的數字！\n");
                    continue;
                }
                
                int has_village = 0;
                for (int i = 0 ; i < MAX_VILLAGES; i++)
                {
                    has_village += (player_list[player].village[i] == option_2) ? 1 : 0;
                }
                if (has_village == 0)
                {
                    printf("您沒有這個村莊！\n");
                    continue;
                }
                
                while (1)
                {
                    int option_3 = 0;
                    printf("您是否確定要用2個小麥、3個石頭將村莊(%d)升級為城市？（0:回上頁 1:確認）\n", option_2);
                    if (scanf("%d", &option_3) != 1)
                    {
                        while (getchar() != '\n');
                        continue;
                    }
                    while (getchar() != '\n');
                    if (option_3 != 0 && option_3 != 1)
                    {
                        continue;
                    }
                    else if (option_3 == 1)
                    {
                        player_list[player].resource[0] -= 2;
                        player_list[player].resource[3] -= 3;
                        system_setting.bank_resource[0] += 2;
                        system_setting.bank_resource[3] += 3;
                        player_list[player].total_resource -= 5;

                        printf("您已成功用2個小麥、3個石頭將村莊%d升級為城市！\n", option_2);
                        for (int i = 0; i < MAX_VILLAGES; i++)
                        {
                            if (player_list[player].village[i] == option_2)
                            {
                                player_list[player].village[i] = 0;
                                break;
                            }
                        }
                        for (int i = 0; i < MAX_CITIES; i++)
                        {
                            if (player_list[player].city[i] == 0)
                            {
                                player_list[player].city[i] = option_2;
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }
        else if (option == 4)
        {
            // 檢查分數卡庫存
            int devcards = 0;
            for (int i = 0; i < 25; i++)
            {
                devcards += system_setting.bank_develop_card[i];
            }
            if (devcards == 0)
            {
                printf("銀行已經沒有發展卡！\n");
                continue;
            }
            
            // 檢查玩家是否有足夠資源購買 (小麥x1 + 羊毛x1 + 石頭x1)
            if (player_list[player].resource[0] < 1 || player_list[player].resource[2] < 1 || player_list[player].resource[3] < 1)
            {
                printf("您的資源不足，購買發展卡需要1個小麥、1個羊毛、1個石頭！\n");
                continue;
            }
            
            // 購買發展卡
            while (1)
            {
                int option_2 = 0;
                printf("請確認是否要以1個小麥、1個羊毛、1個石頭兌換1張分數卡？\n（0:否 1:是）\n");
                if (scanf("%d", &option) != 1)
                {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                if (option == 0)
                {
                    break;
                }
                while (1)
                {
                    int option_3 = 0;
                    #if WINDOWS
                    option_3 = rand() % 25;
                    #else
                    option_3 = arc4random_uniform(25);
                    #endif
                    if (system_setting.bank_develop_card[option_3])
                    {
                        printf("您已成功以1個小麥、1個羊毛、1個石頭兌換1張分數卡！\n");
                        system_setting.bank_develop_card[option_3] = 0;
                        player_list[player].develop_cards[option_3] = 1;
                        break;
                    }
                }
            }
            continue;
        }
        else
        {
            printf("請輸入0~4之間的數字！\n");
            continue;
        }
    }
    printf("建築階段結束\n");
    return 0;
}

int robber(Player *player_list, int player)
{
    // 檢查是否有人資源卡超過七張
    printf("強盜來ㄌ！資源卡超過八張的人將被搶走一半的資源卡！\n");
    for (int i = 0; i < system_setting.player_num; i++)
    {
        if (player_list[i].total_resource > 7)
        {
            int discard = player_list[i].total_resource / 2;
            while(discard > 0){
                // 捨棄最多的資源卡，若最多的不止一個，則捨棄其中的第一個
                int max_pos = 4;
                for (int j = 4; j >= 0; j--)
                {
                    max_pos = (player_list[i].resource[j] > player_list[i].resource[max_pos]) ? j : max_pos;
                }
                player_list[i].resource[max_pos]--;
                player_list[i].total_resource--;
                discard--;
            }
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
        #if WINDOWS
        robber_position = rand() % NUM_TILES;
        while (robber_position == robber_position_prev)
        {
            robber_position = rand() % NUM_TILES;
        }
        #else
        robber_position = arc4random_uniform(NUM_TILES);
        while (robber_position == robber_position_prev)
        {
            robber_position = arc4random_uniform(NUM_TILES);
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
                robbable_players[i] += (player_list[i].village[k] == tiles[robber_position].corner_id[j]);
            }
            for (int k = 0; k < MAX_CITIES; k++)
            {
                robbable_players[i] += (player_list[i].city[k] == tiles[robber_position].corner_id[j]);
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
        printf("(1) 板塊%d附近沒有村莊或城市！\n(2) 沒有可以搶奪的資源\n", robber_position);
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
            option = arc4random_uniform(system_setting.player_num);
            #endif
            
            if (robbable_players[0] + robbable_players[1] + robbable_players[2] + robbable_players[3] == 0)
            {
                printf("沒有可搶奪的玩家！\n");
                return 0;
            }
            else if (option == player || robbable_players[option] == 0)
            {
                continue;
            }
            // 搶奪該位玩家的資源卡
            int temp_rob;
            #if WINDOWS
            temp_rob = rand() % player_list[option].total_resource;
            #else
            temp_rob = arc4random_uniform(player_list[option].total_resource);
            #endif
            
            for (int i = 0; i < 5; i++)
            {
                if (temp_rob < player_list[option].resource[i])
                {
                    printf("玩家%d已從玩家%d搶奪1個%s！\n", player, option, resource_name[i]);
                    player_list[option].resource[i] -= 1;
                    player_list[player].resource[i] += 1;
                    player_list[option].total_resource -= 1;
                    player_list[player].total_resource += 1;
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
        temp_rob = arc4random_uniform(player_list[option].total_resource);
        #endif
        
        for (int i = 0; i < 5; i++)
        {
            if (temp_rob < player_list[option].resource[i])
            {
                printf("您已從玩家%d搶奪1個%s！\n", option, resource_name[i]);
                player_list[option].resource[i] -= 1;
                player_list[player].resource[i] += 1;
                player_list[option].total_resource -= 1;
                player_list[player].total_resource += 1;
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
                                    robbable_players[i] += (player_list[i].village[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
                                }
                                for (int k = 0; k < MAX_CITIES; k++)
                                {
                                    robbable_players[i] += (player_list[i].city[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
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
                                option = arc4random_uniform(system_setting.player_num);
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
                                temp_rob = arc4random_uniform(player_list[option].total_resource);
                                #endif
                                
                                for (int i = 0; i < 5; i++)
                                {
                                    if (temp_rob < player_list[option].resource[i])
                                    {
                                        printf("玩家%d已從玩家%d搶奪1個%s！\n", player, option, resource_name[i]);
                                        player_list[option].resource[i] -= 1;
                                        player_list[player].resource[i] += 1;
                                        player_list[option].total_resource -= 1;
                                        player_list[player].total_resource += 1;
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
                            temp_rob = arc4random_uniform(player_list[option].total_resource);
                            #endif
                            
                            for (int i = 0; i < 5; i++)
                            {
                                if (temp_rob < player_list[option].resource[i])
                                {
                                    printf("您已從玩家%d搶奪1個%s！\n", option, resource_name[i]);
                                    player_list[option].resource[i] -= 1;
                                    player_list[player].resource[i] += 1;
                                    player_list[option].total_resource -= 1;
                                    player_list[player].total_resource += 1;
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
                            player_list[player].total_resource += player_list[j].resource[option_2 - 1];
                            player_list[j].total_resource = 0;
                        }
                    }
                }
                else if (option == 16 || option == 17) // 道路建設
                {
                    for (int k = 0; k < 2; k++)
                    {
                        while (1)
                        {
                            int road_1 = 0, road_2 = 0;
                            printf("請選擇您要蓋的道路（例如17,23，回上頁請輸入0,0）：");
                            if (scanf("%d,%d", &road_1, &road_2) != 2)
                            {
                                while (getchar() != '\n');
                                continue;
                            }
                            while (getchar() != '\n');
                            if (road_1 == 0 && road_2 == 0)
                            {
                                break;
                            }
                            else if (road_1 == 0 || road_2 == 0) // xor
                            {
                                printf("道路的兩個頂點皆需輸入 1-54 的數值！\n");
                                continue;
                            }
                            else if (road_1 == road_2)
                            {
                                printf("道路的兩端點不能為同一點！\n");
                                continue;
                            }
                            else if (check_connected(road_1, road_2) == 0)
                            {
                                printf("您選取的點 %d 與 %d 無法構成一個有效的道路！\n", road_1, road_2);
                                continue;
                            }
                            else if (check_if_has_road(player_list, road_1, road_2) == 1)
                            {
                                printf("您選取的道路 %d,%d 已經被佔有！\n", road_1, road_2);
                                continue;
                            }
                            else if (check_if_connected_build(player_list, player, road_1, road_2) == 0)
                            {
                                printf("您選取的道路 %d,%d 沒有跟您的其它道路或建築連接！\n", road_1, road_2);
                                continue;
                            }
                            // 已確認道路合法
                            while (1)
                            {
                                int option_2 = 0;
                                printf("請確認您是否要用1個木頭、1個磚頭建造道路 %d,%d？（0:否 1:是）：", road_1, road_2);
                                if (scanf("%d", &option_2) != 1)
                                {
                                    while (getchar() != '\n');
                                    continue;
                                }
                                while (getchar() != '\n');
                                if (option_2 != 0 && option_2 != 1)
                                {
                                    continue;
                                }
                                else if (option_2 == 0)
                                {
                                    break;
                                }
                                // option_2 == 1
                                printf("您已成功建造道路 %d,%d！\n", road_1, road_2);
                                add_road_to_player(player_list, player, road_1, road_2);
                                break;
                            }
                            break;
                        }
                    }
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
                        player_list[player].total_resource += 1;
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
                        player_list[player].total_resource += 1;
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
                option = arc4random_uniform(20);
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
            option = arc4random_uniform(20);
            #endif
            break;
    }
    
    int option_2 = 0;
    if (player_list[player].develop_cards[option] == 1)
    {
        printf("玩家%d使用發展卡 - %d. %s！\n", player, option, develop_card_name[option]);
        if (option < 14) // 騎士卡(電腦)
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
            robber_position = arc4random_uniform(NUM_TILES);
            while (robber_position == robber_position_prev)
            {
                robber_position = arc4random_uniform(NUM_TILES);
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
                        robbable_players[i] += (player_list[i].village[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
                    }
                    for (int k = 0; k < MAX_CITIES; k++)
                    {
                        robbable_players[i] += (player_list[i].city[k] == tiles[robber_position].corner_id[j]) ? 1 : 0;
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
            
            while (1)
            {
                int option = 0;
                #if WINDOWS
                option = rand() % system_setting.player_num;
                #else
                option = arc4random_uniform(system_setting.player_num);
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
                temp_rob = arc4random_uniform(player_list[option].total_resource);
                #endif
                
                for (int i = 0; i < 5; i++)
                {
                    if (temp_rob < player_list[option].resource[i])
                    {
                        printf("玩家%d已從玩家%d搶奪1個%s！\n", player, option, resource_name[i]);
                        player_list[option].resource[i] -= 1;
                        player_list[player].resource[i] += 1;
                        player_list[option].total_resource -= 1;
                        player_list[player].total_resource += 1;
                        break;
                    }
                    temp_rob -= player_list[option].resource[i];
                }
                break;
            }
            return 0;
        }
        else if (option == 14 || option == 15) // 資源壟斷(電腦)
        {
            #if WINDOWS
            option_2 = rand() % 5;
            #else
            option_2 = arc4random_uniform(5);
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
                player_list[player].total_resource += player_list[i].resource[option_2];
                player_list[i].total_resource = 0;
            }
        }
        else if (option == 16 || option == 17) // 道路建設(電腦)
        {
            // TODO: 道路建設
        }
        else if (option == 18 || option == 19) // 創新發明(電腦)
        {
            while (1)
            {
                #if WINDOWS
                option_2 = rand() % 5;
                #else
                option_2 = arc4random_uniform(5);
                #endif
                
                if (system_setting.bank_resource[option_2] < 1)
                {
                    continue;
                }
                printf("玩家%d向銀行拿取1個%s！\n", player, resource_name[option_2]);
                system_setting.bank_resource[option_2] -= 1;
                player_list[player].resource[option_2] += 1;
                player_list[player].total_resource += 1;
                break;
            }
            while (1)
            {
                #if WINDOWS
                option_2 = rand() % 5;
                #else
                option_2 = arc4random_uniform(5);
                #endif
                
                if (system_setting.bank_resource[option_2] < 1)
                {
                    continue;
                }
                printf("玩家%d向銀行拿取1個%s！\n", player, resource_name[option_2]);
                system_setting.bank_resource[option_2] -= 1;
                player_list[player].resource[option_2] += 1;
                player_list[player].total_resource += 1;
                break;
            }
        }
    }
    return 0;
}

void build_village(Player *players, int player, int point_id){
    for(int32_t j = 0; j < MAX_VILLAGES; ++j){
        if(players[player].village[j] == 0){
            players[player].village[j] = point_id;
            if(!player)     printf("你在%d號板塊建造了村莊！\n", point_id);
            else            printf("玩家%d在%d號板塊建造了村莊！\n", player, point_id);
            break;
        }
    }
}

// 1: 有村莊或城市，0: 沒有村莊
int is_has_built(Player *players, int point_id){
    for(int32_t i = 0; i < system_setting.player_num; ++i){
        for(int32_t j = 0; j < MAX_VILLAGES; ++j)
        {
            if(players[i].village[j] == point_id){
                return 1;
            }
        }
        for(int32_t j = 0; j < MAX_CITIES; ++j)
        {
            if(players[i].city[j] == point_id){
                return 1;
            }
        }
    }
    return 0;
}

// 1: 相連村莊或城市，0: 沒有相連
int connect_village_limit(Player *players, int point_id){
    for(int32_t i = 0; i < system_setting.player_num; ++i){
        for(int32_t j = 0; j < MAX_VILLAGES; ++j)
        {
            if(connectedPoint[ players[i].village[j] ][0] == point_id || connectedPoint[ players[i].village[j] ][1] == point_id 
                                    || connectedPoint[ players[i].village[j] ][2] == point_id){
                return 1;
            }
        }
        for(int32_t j = 0; j < MAX_CITIES; ++j)
        {
            if(connectedPoint[ players[i].city[j] ][0] == point_id || connectedPoint[ players[i].city[j] ][1] == point_id 
                                    || connectedPoint[ players[i].city[j] ][2] == point_id){

                return 1;
            }
        }
    }
    return 0;
}

void set_village(Player *players, System *sys, struct CatanTile *tiles){
    int option = 0;
    for(int32_t i = 0; i < sys->player_num; ++i){
        if(i == 0){
            while(1){
                printf("\n請選擇你想要建造村莊的位置(1-54)：");
                if( scanf("%d", &option) != 1){
                    while(getchar() != '\n');
                    continue;
                }
                while(getchar() != '\n');
                if(option < 1 || option > 54){
                    printf("請輸入1-54之間的數字！\n");
                    continue;
                }
                break;
            }
            build_village(players, i, option);
            continue;
        }
    usleep(500000);
        if(players[i].NPC_difficulty == 1){
            for(int32_t j = 0; j < NUM_TILES; ++j){
                if(tiles[j].resourceType != Forest && tiles[j].resourceType != Hill){
                    continue;
                }
                if( is_has_built(players, tiles[j].corner_id[5])  && !connect_village_limit(players, tiles[j].corner_id[5])){
                    build_village(players, i, tiles[j].corner_id[5]);
                }
            }
        } else {
            int32_t random_point = 0;
            while (1){
                #if WINDOWS
                random_point = rand() % 54 + 1;
                #else
                random_point = arc4random_uniform(54) + 1;
                #endif

                if(is_has_built(players, random_point)){
                    continue;
                }
                if(connect_village_limit(players, random_point)){
                    continue;;
                }
                break;
            }

            build_village(players, i, random_point);
        }
    }
    for(int32_t i = sys->player_num-1; i >= 0; --i){
        if(i == 0){
            print_map_state2(players, tiles, sys, robber_loc);

            while(1){
                printf("請選擇你想要建造村莊的位置：(1-54)\n");
                if( scanf("%d", &option) != 1){
                    while(getchar() != '\n');
                    continue;
                }
                while(getchar() != '\n');
                if(option < 1 || option > 54){
                    printf("請輸入1-54之間的數字！\n");
                    continue;
                }
                if(is_has_built(players, option)){
                    printf("此處已經有村莊或城市了！\n");
                    continue;
                }
                if(connect_village_limit(players, option)){
                    printf("新建的村莊或城市不能與其他村莊或城市相鄰！\n");
                    continue;
                }
                break;
            }
            build_village(players, i, option);
            continue;
        }
    usleep(500000);
        if(players[i].NPC_difficulty == 1){
            for(int32_t j = 0; j < NUM_TILES; ++j){
                if(tiles[j].resourceType != Forest && tiles[j].resourceType != Hill){
                    continue;
                }
                if(is_has_built(players, tiles[j].corner_id[5])){
                    continue;
                }
                if(connect_village_limit(players, tiles[j].corner_id[5])){
                    continue;
                }
                build_village(players, i, tiles[j].corner_id[5]);
            }
        } else {
            int32_t random_point = 0;
            while (1){
                #if WINDOWS
                random_point = rand() % 54 + 1;
                #else
                random_point = arc4random_uniform(54) + 1;
                #endif

                if(is_has_built(players, random_point)){
                    continue;
                }
                if(connect_village_limit(players, random_point)){
                    continue;;
                }
                break;
            }
            build_village(players, i, random_point);
        }
    }
}

void NPC_build(Player *players, int player, System *sys){
    int option[4] = {0};
    if(players[player].resource[BRICK] >= 1 && players[player].resource[WOOD] >= 1){
        option[0] = 1;
    } // 道路
    if(players[player].resource[BRICK] >= 1 && players[player].resource[WHEAT] >= 1 && players[player].resource[WOOL] >= 1 && players[player].resource[WOOD] >= 1){
        option[1] = 1;
    } // 村莊
    if(players[player].resource[STONE] >= 3 && players[player].resource[WHEAT] >= 2){
        option[2] = 1;
    } // 城市
    if(players[player].resource[STONE] >= 1 && players[player].resource[WOOL] >= 1 && players[player].resource[WHEAT] >= 1 ){
        option[3] = 1;
    } // 發展卡

    int32_t random_option = -1;
    for(int32_t i = 0; i < 4; ++i){
        if(i == 3)          random_option = 0;
        if(option[i] != 1)     continue;
    }
    if(!random_option){
        printf("玩家%d結束建築\n", player);
        return ;
    }

    while(1){
        random_option = rand() % 4;
        if(option[random_option] == 0){
            continue;
        }
        break;
    }
    
    if(random_option == 1){
        while (1){
            #if WINDOWS
            random_option = rand() % 54 + 1;
            #else
            random_option = arc4random_uniform(54) + 1;
            #endif

            if(is_has_built(players, random_option)){
                continue;
            }
            if(connect_village_limit(players, random_option)){
                continue;;
            }
            break;
        }
        build_village(players, player, random_option);
    }
    else if(random_option == 3){
        // 檢查分數卡庫存
        int devcards = 0;
        for (int i = 0; i < 25; i++)
        {
            devcards += system_setting.bank_develop_card[i];
        }
        if (devcards == 0)
        {
            printf("玩家%d結束建築\n", player);
            return ;
        }
                
        // 購買發展卡
        while (1)
        {
            int option_2 = 0;
            int option_3 = 0;
            #if WINDOWS
            option_3 = rand() % 25;
            #else
            option_3 = arc4random_uniform(25);
            #endif
            if (system_setting.bank_develop_card[option_3])
            {
                printf("玩家%d以1個小麥、1個羊毛、1個石頭兌換1張分數卡！\n", player);
                system_setting.bank_develop_card[option_3] = 0;
                players[player].develop_cards[option_3] = 1;
                players[player].resource[WHEAT]--;
                players[player].resource[WOOL]--;
                players[player].resource[STONE]--;
                players[player].total_resource -= 3;
                return ;
            }
        }
    }
}

void init_player(Player *players, System *system_setting){
    for(int32_t i = 0; i < system_setting->player_num; ++i){
        for(int32_t j = 0; j < MAX_VILLAGES; ++j){
            players[i].village[j] = 0;
        }
        for(int32_t j = 0; j < MAX_CITIES; ++j){
            players[i].city[j] = 0;
        }
        for(int32_t j = 0; j < MAX_ROADS; ++j){
            players[i].road[j][0] = 0;
            players[i].road[j][1] = 0;
        }
        for(int32_t j = 0; j < 5; ++j){
            players[i].resource[j] = 0;
        }
        for(int32_t j = 0; j < 25; ++j){
            players[i].develop_cards[j] = 0;
        }
        players[i].total_resource = 0;
        players[i].num_roads = 0;
        players[i].is_longest = 0;
        
        players[i].NPC_difficulty = 0;
        if(!i)  players[i].NPC_difficulty = 2;
    }
}
