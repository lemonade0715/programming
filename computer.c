#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "system.h"
#include "computer.h"

void computer_trade(Player *player_list, int player, System *system_setting, struct CatanTile tiles[19])
{
    for(int32_t i = 0; i < 5; ++i){
        // if( player_list[player].resource[i] >= 4 )
    }
}

void trade_with_player(Player *player_list, int player, int option ){
    int src = option -1;

// 指定NPC資源
    printf("\n你想要與玩家%d交換什麼資源\n（0.放棄 1.小麥 2.木頭 3.羊毛 4.石頭 5.磚頭）：", player);
    if (scanf("%d", &option) != 1)
    {
        while (getchar() != '\n');
        printf("交易失敗，請輸入0-5之間的數值\n");
        return ;
    }
    while (getchar() != '\n');
    if(!option) {
        printf("你放棄了交易\n");
        return ;
    } else if(option < 0 || option > 5) {
        printf("交易失敗，請輸入0-5之間的數值\n");
        return ;
    }
    int NPC_src = option -1;

    if(NPC_src == src) {
        printf("交易失敗，請選擇不同的資源\n");
        return ;
    }

// 玩家交換資源的數量
    printf("\n你想要用多少數量的%s與玩家%d進行交易呢\n（0.放棄, ~%d: 欲交易數量）：", resource_name[src], player, player_list[0].resource[src]);
    if (scanf("%d", &option) != 1)
    {
        while (getchar() != '\n');
        printf("交易失敗，請輸入0或你想交易的數值\n");
        return ;
    }
    while (getchar() != '\n');
    if(!option) {
        printf("你放棄了交易\n");
        return ;
    } else if(option < 0 || option > player_list[0].resource[src]) {
        printf("交易失敗，請輸入符合你資源數量的數值\n");
        return ;
    }
    int src_trade = option;

// NPC交換資源的數量
    printf("\n你希望玩家%d用多少數量的%s進行交易呢\n（0.放棄, ~%d: 欲交易數量）：", player, resource_name[NPC_src], player_list[player].total_resource);
    if (scanf("%d", &option) != 1)
    {
        while (getchar() != '\n');
        printf("交易失敗，請輸入0或你想交易的數值\n");
        return ;
    }
    while (getchar() != '\n');
    if(!option) {
        printf("你放棄了交易\n");
        return ;
    } else if(option < 0 || option > player_list[player].total_resource) {
        printf("交易失敗，請輸入符合對方資源數量的數值\n");
        return ;
    }
    int NPC_src_trade = option;

// general difficulty
    if(NPC_src_trade > 4) {
        printf("玩家%d拒絕了你的交易\n", player);
        return ;
    }
    if(player_list[player].NPC_difficulty != 1){
        if(player_list[player].resource[NPC_src] < NPC_src_trade){
            printf("玩家%d沒有足夠的資源\n", player);
            return ;
        } else {
            player_list[player].resource[NPC_src] -= NPC_src_trade;
            player_list[player].resource[src] += src_trade;
            player_list[player].total_resource += src_trade - NPC_src_trade;

            player_list[0].resource[NPC_src] += NPC_src_trade;
            player_list[0].resource[src] -= src_trade;
            player_list[0].total_resource += NPC_src_trade - src_trade;
            printf("交易成功\n");
            return ;
        }
    }
// difficulty = 1
    if(NPC_src_trade > 3) {
        printf("玩家%d拒絕了你的交易\n", player);
        return ;
    }
    if(src_trade < 2 ) {
        printf("玩家%d拒絕了你的交易\n", player);
        return ;
    } 
    if(player_list[player].NPC_difficulty == 1){
        if(player_list[player].resource[NPC_src] < NPC_src_trade){
            printf("玩家%d沒有足夠的資源\n", player);
            return ;
        } else {
            player_list[player].resource[NPC_src] -= NPC_src_trade;
            player_list[player].resource[src] += src_trade;
            player_list[player].total_resource += src_trade - NPC_src_trade;

            player_list[0].resource[NPC_src] += NPC_src_trade;
            player_list[0].resource[src] -= src_trade;
            player_list[0].total_resource += NPC_src_trade - src_trade;
            printf("交易成功\n");
            return ;
        }
    }

}