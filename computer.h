#ifndef NPC_H
#define NPC_H

#include <stdio.h>

void computer_trade(Player *player_list, int player, System *system_setting, struct CatanTile tiles[19]);

void trade_with_player(Player *player_list, int player, int option);

#endif