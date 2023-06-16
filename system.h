#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include <stdio.h>

struct _system {
    int32_t player_num;     // 3 or 4
    int bank_develop_card[25]; // 14+6+5，銀行的發展卡，0為已被購買，1為有庫存
    size_t bank_resource[5]; // 各19個，發放時會扣除，記得檢查是否>0
    
    int32_t longest_road;   // num of longest road in total players
    int32_t lr_player;      // the player_id who has longest road card
    int32_t max_knight;     // num of max knight card in total players
    int32_t mk_player;      // the player_id who has max_num knight cards
    
    int32_t player_score[4]; // 如果只有三個玩家，第四項就保留為0
}__attribute__ ((__packed__));
typedef struct _system System;

void print_systemMsg(System *system_setting);

void print_playerMsg(Player player_list[], System *system);

void print_userMsg(Player player_list[], System *system);


#endif