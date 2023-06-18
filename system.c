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
    printf("玩家0: %d\n", player_list[0].total_resource);
    for(int32_t i = 0; i < 5; ++i){
        printf("  %s: %d\t", resource_name[i], player_list[0].resource[i]);
    }
    printf(NONE);
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
    printf("\n其他玩家擁有的資源: \n");
    for(int32_t i = 1; i < system->player_num; ++i){
        if(i == 1)          printf(RED);
        else if(i == 2)     printf(GREEN);
        else if(i == 3)     printf(YELLOW);
        printf("  玩家%d: %d\t"NONE, i, player_list[i].total_resource);
#if DEBUG
        for(int32_t j = 0; j < 5; ++j){
            printf("  %s: %d\t", resource_name[j], player_list[i].resource[j]);
        }
#endif
        printf("\n");
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
}

