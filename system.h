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

static int neightbors[55][3] = {
    { 0,  0,  0}, 
    { 2,  4,  0}, { 1,  5,  0}, { 4,  8,  0}, { 1,  3,  9}, { 2,  6, 10},
    { 5, 11,  0}, { 8, 13,  0}, { 3,  7, 14}, { 4, 10, 15}, { 5,  9, 16},

    { 6, 12, 17}, {11, 18,  0}, { 7, 19,  0}, { 8, 15, 20}, { 9, 14, 21},
    {10, 17, 22}, {11, 16, 23}, {12, 24,  0}, {13, 20, 25}, {14, 19, 26},

    {15, 22, 27}, {16, 21, 28}, {17, 24, 29}, {18, 23, 30}, {19, 31,  0},
    {20, 27, 32}, {21, 26, 33}, {22, 29, 34}, {23, 28, 35}, {24, 36,  0},

    {25, 32, 37}, {26, 31, 38}, {27, 34, 39}, {28, 33, 40}, {29, 35, 36},
    {30, 35, 42}, {31, 43,  0}, {32, 39, 44}, {33, 38, 45}, {34, 41, 46},
    
    {35, 40, 47}, {36, 48,  0}, {37, 44,  0}, {38, 43, 49}, {39, 46, 50},
    {40, 45, 51}, {41, 48, 52}, {42, 47,  0}, {44, 50,  0}, {45, 49, 53},
    {46, 52, 54}, {47, 51,  0}, {50, 54,  0}, {51, 53,  0}
};


void print_systemMsg(System *system_setting);

void print_playerMsg(Player player_list[], System *system);

void print_userMsg(Player player_list[], System *system);


#endif