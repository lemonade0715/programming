#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "system.h"
#include "computer.h"

void computer_trade(Player *player_list, int player, System *sys, struct CatanTile tiles[19])
{
    // printf("\n玩家%d放棄交易\n", player);
    // return ;
    int NPC_src[5] = {0};
    int total_src = 0;
    for(int32_t i = 0; i < 5; ++i){
        NPC_src[i] = player_list[player].resource[i];
        total_src += NPC_src[i];
    }
    printf("NPC_src = {%d, %d, %d, %d, %d}\n", NPC_src[0], NPC_src[1], NPC_src[2], NPC_src[3], NPC_src[4]);
    
    if(player_list[player].NPC_difficulty != 1 && total_src < 4){
        printf("\n玩家%d放棄交易\n", player);
        return ;
    } else if(player_list[player].NPC_difficulty != 1){
        int src = rand() % 5;
        while(!NPC_src[src]){
            printf("LINE%d\n", __LINE__);
            src = rand() % 5;
        }
        int src_trade = rand() % NPC_src[src] + 1;
        while(src_trade > NPC_src[src]){
            printf("src_trade = %d\n NPC_src[src] = %d\n", src_trade, NPC_src[src]);
            src_trade = rand() % NPC_src[src] + 1;
        }
        int32_t object = rand() % sys->player_num;
        while(object == player){
            printf("LINE%d\n", __LINE__);
            object = rand() % sys->player_num;
        } 
        int object_src = rand() % 5;
        while(!player_list[object].resource[object_src]){
            printf("LINE%d\n", __LINE__);
            object_src = rand() % 5;
        }
        while(object_src == src && !player_list[object].resource[object_src]){
            printf("LINE%d\n", __LINE__);
            object_src = rand() % 5;
        }
        int object_trade = rand() % player_list[object].resource[object_src] + 1;
        while(object_trade > player_list[object].resource[object_src]){
            printf("LINE%d\n", __LINE__);
            object_trade = rand() % player_list[object].resource[object_src] + 1;
        }
        if(!object){
            printf("\n玩家%d想要用%d個%s與玩家%d交換%d個%s\n", player, src_trade, resource_name[src], object, object_trade, resource_name[object_src]);
            int option = 0;
            while (1){
                printf("你是否同意交易？（0.拒絕 1.同意）：");
                if(scanf("%d", &option) != 1){
                    printf("請重新輸入0或1代表拒絕或同意交易\n");
                    while(getchar() != '\n');
                } else if(option != 0 && option != 1){
                    printf("請重新輸入0或1代表拒絕或同意交易\n");
                } else {
                    break;
                }
            }
            if(option){
                player_list[player].resource[src] += object_trade;
                player_list[player].resource[object_src] -= object_trade;
                player_list[object].resource[src] -= src_trade;
                player_list[object].resource[object_src] += src_trade;
                player_list[player].total_resource += object_trade - src_trade;
                player_list[object].total_resource += src_trade - object_trade;
                printf("你同意了交易\n");
                return ;
            } else {
                printf("你拒絕了交易\n");
                return;
            }

        } else {
            printf("\n玩家%d想要用%d個%s與玩家%d交換%d個%s\n", player, src_trade, resource_name[src], object, object_trade, resource_name[object_src]);
            int option = rand() % 2;
            if(option){
                player_list[player].resource[src] += object_trade;
                player_list[player].resource[object_src] -= object_trade;
                player_list[object].resource[src] -= src_trade;
                player_list[object].resource[object_src] += src_trade;
                player_list[player].total_resource += object_trade - src_trade;
                player_list[object].total_resource += src_trade - object_trade;
                printf("玩家%d同意了交易\n", object);
                return ;
            } else {
                printf("玩家%d拒絕了交易\n", object);
                return;
            }

        }
    }
    if(player_list[player].NPC_difficulty != 1){
        return ;
    }
    // difficulty == 1
    if(total_src < 3){
        printf("\n玩家%d放棄交易\n", player);
        return ;
    } else {
        int src1 = 0, src2 = 0;
        int max_src = NPC_src[0], min_src = NPC_src[0];
        for(int32_t i = 0; i < 5; ++i){
            if(NPC_src[i] > max_src){
                max_src = NPC_src[i];
                src1 = i;
            }
            if(NPC_src[i] < min_src){
                min_src = NPC_src[i];
                src2 = i;
            }
        }
        int src_trade = rand() % NPC_src[src1] + 1;
        while(src_trade > NPC_src[src1]){
            src_trade = rand() % NPC_src[src1] + 1;
        }
        int obj_src = src2;
        int32_t object = rand() % sys->player_num;
        while(object == player){
            object = rand() % sys->player_num;
        }
        int object_trade = rand() % player_list[object].resource[obj_src] + 1;
        while(object_trade > player_list[object].resource[obj_src]){
            object_trade = rand() % player_list[object].resource[obj_src] + 1;
        }
        if(!object){
            printf("\n玩家%d想要用%d個%s與玩家%d交換%d個%s\n", player, src_trade, resource_name[src1], object, object_trade, resource_name[obj_src]);
            int option = 0;
            while (1){
                printf("你是否同意交易？（0.拒絕 1.同意）：");
                if(scanf("%d", &option) != 1){
                    printf("請重新輸入0或1代表拒絕或同意交易\n");
                    while(getchar() != '\n');
                } else if(option != 0 && option != 1){
                    printf("請重新輸入0或1代表拒絕或同意交易\n");
                } else {
                    break;
                }
            }
            if(option){
                player_list[player].resource[src1] += object_trade;
                player_list[player].resource[obj_src] -= object_trade;
                player_list[object].resource[src1] -= src_trade;
                player_list[object].resource[obj_src] += src_trade;
                player_list[player].total_resource += object_trade - src_trade;
                player_list[object].total_resource += src_trade - object_trade;
                printf("你同意了交易\n");
                return ;
            } else {
                printf("你拒絕了交易\n");
                return;
            }

        } else {
            printf("\n玩家%d想要用%d個%s與玩家%d交換%d個%s\n", player, src_trade, resource_name[src1], object, object_trade, resource_name[obj_src]);
            int option = rand() % 2;
            if(option){
                player_list[player].resource[src1] += object_trade;
                player_list[player].resource[obj_src] -= object_trade;
                player_list[object].resource[src1] -= src_trade;
                player_list[object].resource[obj_src] += src_trade;
                player_list[player].total_resource += object_trade - src_trade;
                player_list[object].total_resource += src_trade - object_trade;
                printf("玩家%d同意了交易\n", object);
                return ;
            } else {
                printf("玩家%d拒絕了交易\n", object);
                return;
            }

        }
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
            printf("玩家%d拒絕了你的交易\n", player);
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
            printf("玩家%d拒絕了你的交易\n", player);
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
