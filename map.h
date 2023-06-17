#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_map_point(Player *player, struct CatanTile *tiles, System *sys);
void print_map_state(Player *player, struct CatanTile *tiles, System *sys, int robber_loc);


#endif