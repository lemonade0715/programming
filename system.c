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

void print_userMsg(Player player_list[], System *system){
        printf(BLUE"自己: \n");
        printf("\t你的資源: %d\n", player_list[0].total_resource);
        if( special_show(system, 0) ){
            printf("\t你的成就: ");
            if(system->lr_player == 0){
                printf(BROWN"最長的道路 "NONE);
            }
            if(system->mk_player == 0){
                printf(BROWN"最大的騎士 "NONE);
            }
            printf("\n");
        }
}

void print_playerMsg(Player player_list[], System *system){
    for(int32_t i = 1; i<system->player_num; ++i){
        printf(BLUE"玩家%d:\n", i);
        printf("\t玩家資源: %d\n", player_list[i].total_resource);
        if( special_show(system, i) ){
            printf("\t成就: ");
            if(system->lr_player == i){
                printf(BROWN"最長的道路 "NONE);
            }
            if(system->mk_player == i){
                printf(BROWN"最大的騎士 "NONE);
            }
            printf("\n");
        }
    }
    print_playerMsg(player_list, system);
    return ;
}

