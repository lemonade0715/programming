#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "func.h"
#include "color.h"
#include "system.h"

void print_systemMsg(System *system_setting){
    printf("\033[1;33m System Setting:\n");
    printf("\tPlayer Number: %d\n", system_setting->player_num);
    printf("\tBank Develop Card: ");
    for (int i = 0; i < 19; i++)
    {
        printf("%d ", system_setting->bank_develop_card[i]);
    }
    printf("\n");
    printf("\tBank Resource: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%ld ", system_setting->bank_resource[i]);
    }
    printf("\n");
    printf("\tLongest Road: %d\n", system_setting->longest_road);
    printf("\tLongest Road Player: %d\n", system_setting->lr_player);
    printf("\tMax Knight: %d\n", system_setting->max_knight);
    printf("\tMax Knight Player: %d\n", system_setting->mk_player);
    printf("\tPlayer Score: ");
    for (int i = 0; i < 4; i++)
    {
        printf("%d ", system_setting->player_score[i]);
    }
    printf("\n\033[m");
}

int special_show(System *system, int32_t i){
    if(system->lr_player == -1 && system->mk_player == -1){
        return 0;
    }
    else{
        return system->lr_player == i || system->mk_player == i;
    }
}

void print_allResource(Player player_list[], System *system){
    const char resource_name[][7] = {"小麥", "木頭", "羊毛", "石頭", "磚頭"};
    printf(BLUE"\n你的資源: \n");
    printf("  玩家0: %d\t", player_list[0].total_resource);
    for(int32_t i = 0; i < 5; ++i){
        printf("  %s: %d\t", resource_name[i], player_list[0].resource[i]);
    }
    printf(NONE);
    if( special_show(system, 0) ){
        printf("  你的成就: ");
        if(system->lr_player == 0){
            printf(BROWN"最長的道路 "NONE);
        }
        if(system->mk_player == 0){
            printf(BROWN"最大的騎士 "NONE);
        }
        printf("\n");
    }
    printf("\n其他玩家擁有的資源: \n");
    for(int32_t i = 1; i < system->player_num; ++i){
        if(i == 1)          printf(RED);
        else if(i == 2)     printf(GREEN);
        else if(i == 3)     printf(PURPLE);
        printf("  玩家%d: %d\t", i, player_list[i].total_resource);
#if DEBUG
        for(int32_t j = 0; j < 5; ++j){
            printf("  %s: %d\t", resource_name[j], player_list[i].resource[j]);
        }
#endif
        printf(NONE"\n");
        if( special_show(system, i) ){
            printf("  成就: ");
            if(system->lr_player == i){
                printf(BROWN"最長的道路 "NONE);
            }
            if(system->mk_player == i){
                printf(BROWN"最大的騎士 "NONE);
            }
            printf("\n");
        }
    }
}

// return 1 代表遊戲結束
int refresh(Player *player_list, System system_setting)
{
    // player total resource
    for (int i = 0; i < system_setting.player_num; i++)
    {
        player_list[i].total_resource = 0;
        for (int j = 0; j < 5; j++)
        {
            player_list[i].total_resource += player_list[i].resource[j];
        }
    }
    
    // TODO: Check longest road
    int longest_road = 0;
    for (int i = 0; i < system_setting.player_num; i++) {
        calculate_player_longest_road(player_list, i);
        if(player_list[i].longest_roads > longest_road){
            longest_road = player_list[i].longest_roads;
        }
    }
    if(longest_road >= 5){
        system_setting.longest_road = longest_road;
        system_setting.lr_player = -1;
        for (int i = 0; i < system_setting.player_num; i++) {
            if(player_list[i].longest_roads == longest_road){
                system_setting.lr_player = i;
                break;
            }
        }
    }

    // Check max_knight
    for (int i = 0; i < system_setting.player_num; i++)
    {
        int temp = 0;
        for (int j = 0; j < 14; j++)
        {
            temp += (player_list[i].develop_cards[j]) ? 1 : 0;
        }
        // 根據遊戲規則，必須要超過才會易主
        if (temp > system_setting.max_knight)
        {
            system_setting.max_knight = temp;
            system_setting.mk_player = i;
        }
    }
    
    // TODO: others
    
    // Player score
    for (int i = 0; i < system_setting.player_num; i++)
    {
        system_setting.player_score[i] = 0;
        
        // Count Villages and Cities
        for (int j = 0; j < MAX_VILLAGES; j++)
        {
            system_setting.player_score[i] += (player_list[i].village[j] != 0);
        }
        for(int j = 0; j < MAX_CITIES; j++)
        {
            system_setting.player_score[i] += (player_list[i].city[j] != 0);
        }
        // Victory Point Cards
        for (int j = 20; j < 25; j++)
        {
            system_setting.player_score[i] += player_list[i].develop_cards[j];
        }
        // Longest Road & Max Knight
        system_setting.player_score[i] += 2 * (system_setting.lr_player == i);
        system_setting.player_score[i] += 2 * (system_setting.mk_player == i);
        
        if (system_setting.player_score[i] >= 10)
        {
            printf("\033[1m【遊戲結束】\033[0m\n");
            for (int k = 0; k < system_setting.player_num; k++)
            {
                printf("玩家%d：%2d分\n", k, system_setting.player_score[k]);
            }
            printf("\033[1m恭喜玩家%d獲得勝利！\033[0m\n", i);
            return 1;
        }
    }
    return 0;
}
/*
這個function的使用如下：
if (refresh(player_list, system_setting))
{
    return 0;
}
*/

int calculate_longest_road(Player *player_list, int player_id, int current_node, int visited_nodes[MAX_ROADS][2], int visited_count) {
    int max_length = 0;
    
    // 標記目前節點為已訪問
    visited_nodes[visited_count][0] = player_id;
    visited_nodes[visited_count][1] = current_node;
    visited_count++;
    
    // 遍歷所有鄰接的道路
    for (int i = 0; i < MAX_ROADS; i++) {
        int connected_node = -1;
        
        // 找到與目前節點相連的道路的另一個節點
        if (player_list[player_id].road[i][0] == current_node)
            connected_node = player_list[player_id].road[i][1];
        else if (player_list[player_id].road[i][1] == current_node)
            connected_node = player_list[player_id].road[i][0];
        
        // 檢查節點是否已被訪問過
        int is_visited = 0;
        for (int j = 0; j < visited_count; j++) {
            if (visited_nodes[j][0] == player_id && visited_nodes[j][1] == connected_node) {
                is_visited = 1;
                break;
            }
        }
        
        // 如果節點未被訪問過且與目前節點相連，則繼續遞迴搜尋
        if (!is_visited && connected_node != -1) {
            int length = calculate_longest_road(player_list, player_id, connected_node, visited_nodes, visited_count);
            max_length = (length > max_length) ? length : max_length;
        }
    }
    
    // 取得最長道路的長度
    return max_length + 1;
}

void calculate_player_longest_road(Player *player_list, int player_id) {
    int longest_road = 0;
    
    // 檢查每個玩家的道路連接情況
    for (int i = 0; i < MAX_ROADS; i++) {
        // 取得道路的兩個節點
        int node1 = player_list[player_id].road[i][0];
        int node2 = player_list[player_id].road[i][1];
        
        // 只計算未被訪問過的節點
        if (node1 != 0 && node2 != 0) {
            // 建立已訪問節點的陣列，並將目前道路加入其中
            int visited_nodes[MAX_ROADS][2];
            visited_nodes[0][0] = player_id;
            visited_nodes[0][1] = node1;
            visited_nodes[1][0] = player_id;
            visited_nodes[1][1] = node2;
            
            // 開始遞迴搜尋最長道路
            int length = calculate_longest_road(player_list, player_id, node1, visited_nodes, 2);
            longest_road = (length > longest_road) ? length : longest_road;
            
            length = calculate_longest_road(player_list, player_id, node2, visited_nodes, 2);
            longest_road = (length > longest_road) ? length : longest_road;
        }
    }
    
    // 將最長道路長度更新到玩家資料中
    player_list[player_id].longest_roads = longest_road;
}
