#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "system.h"
#include "map.h"
#include "color.h"

int roads_relation[73][3] = {
    { 0,  0,  0}, { 1,  1,  2}, { 2,  1,  4}, { 3,  2,  5}, { 4,  3,  4}, { 5,  5,  6}, { 6,  3,  8}, { 7,  4,  9}, { 8,  5, 10}, { 9,  6, 11},
    {10,  7,  8}, {11,  9, 10}, {12, 11, 12}, {13,  7, 13}, {14,  8, 14}, {15,  9, 15}, {16, 10, 16}, {17, 11, 17}, {18, 12, 18}, {19, 14, 15}, 
    {20, 16, 17}, {21, 13, 19}, {22, 14, 20}, {23, 15, 21}, {24, 16, 22}, {25, 17, 23}, {26, 18, 24}, {27, 19, 20}, {28, 21, 22}, {29, 23, 24}, 
    {30, 19, 25}, {31, 20, 26}, {32, 21, 27}, {33, 22, 28}, {34, 23, 29}, {35, 24, 30}, {36, 26, 27}, {37, 28, 29}, {38, 25, 31}, {39, 26, 32}, 
    {40, 27, 33}, {41, 28, 34}, {42, 29, 35}, {43, 30, 36}, {44, 31, 32}, {45, 33, 34}, {46, 35, 36}, {47, 31, 37}, {48, 32, 38}, {49, 33, 39},
    {50, 34, 40}, {51, 35, 41}, {52, 36, 42}, {53, 38, 39}, {54, 40, 41}, {55, 37, 43}, {56, 38, 44}, {57, 39, 45}, {58, 40, 46}, {59, 41, 47},
    {60, 42, 48}, {61, 43, 44}, {62, 45, 46}, {63, 47, 48}, {64, 44, 49}, {65, 45, 50}, {66, 46, 51}, {67, 47, 52}, {68, 49, 50}, {69, 51, 52},
    {70, 50, 53}, {71, 51, 54}, {72, 53, 54}
};

// player 0: -1, player 1: 0, player 2: 1, player 3: 2
int has_road[73] = {0};
int has_settlement[55] = {0};

void set_has(Player *player, System *sys){
    for(int32_t i = 0; i < 73; ++i){
        has_road[i] = -1;
    }
    for(int32_t i = 0; i < 55; ++i){
        has_settlement[i] = -1;
    }
    for(int32_t k = 0; k < sys->player_num; ++k){
        for(int32_t i = 0; i < MAX_ROADS; ++i){
            if(player->road[i][0] == 0){
                continue;
            }
            for(int32_t j = 1; j < 73; ++j){
                if(roads_relation[j][1] == player[k].road[i][0] && roads_relation[j][2] == player[k].road[i][1]){
                    has_road[j] = k;
                }
                if(roads_relation[j][1] == player[k].road[i][1] && roads_relation[j][2] == player[k].road[i][0]){
                    has_road[j] = k;
                }
            }
        }
    }
    for(int32_t k = 0; k < sys->player_num; ++k){
        for(int32_t i = 0; i < MAX_VILLAGES;++i){
            if(player[k].village[i] == 0){
                continue;
            }
            for(int32_t j = 1; j < 55; ++j){
                if(player[k].village[i] == j){
                    has_settlement[j-1] = k;
                }
            }
        }
        for(int32_t i = 0; i < MAX_CITIES; ++i){
            if(player[k].city[i] == 0){
                continue;
            }
            for(int32_t j = 1; j < 55; ++j){
                if(player[k].city[i] == j){
                    has_settlement[j-1] = k;
                }
            }
        }
    }
}

void c(const char* text, int32_t colorCode) {
    // printf("::%d::", colorCode);
    if(colorCode == -1){
        printf(NONE"%s"NONE, text);
    }
    if(colorCode == 0){
        printf(BLUE"%s"NONE, text);
    }
    else if(colorCode == 1){
        printf(RED"%s"NONE, text);
    }
    else if(colorCode == 2){
        printf(GREEN"%s"NONE, text);
    }
    else if(colorCode == 3){
        printf(PURPLE"%s"NONE, text);
    }
    else if(colorCode == 4){
        printf(BLUE2"%s"NONE, text);
    }
    else if(colorCode == 5){
        printf(RED2"%s"NONE, text);
    }
    else if(colorCode == 6){
        printf(GREEN2"%s"NONE, text);
    }
    else if(colorCode == 7){
        printf(PURPLE2"%s"NONE, text);
    }
    
}


void print_map_point2(Player *player, struct CatanTile *tiles, System *sys){
    char road[73][15] = { 
                "" ,  "----", 
                        "/", "\\",
                    "----",     "----", 
                "/", "\\",  "/", "\\",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
                "\\",  "/",  "\\",  "/", 
                    "----",     "----", 
                    "\\",  "/", 
                        "----"
        };

    char num [55][3] = {0};
    char num2 [73][3] = {0};
    char num3 [73][3] = {0};
    char num4 [73][3] = {0};
    for(int32_t i = 0; i < 55; i++){
        num[i][0] = (char)( (i+1)/10 + '0');
        num[i][1] = (char)( (i+1)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num2[i][0] = (char)( (i+1)/10 + '0');
        num2[i][1] = (char)( (i+1)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num3[i][0] = (char)( (i)/10 + '0');
        num3[i][1] = (char)( (i)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num4[i][0] = (char)('*');
        num4[i][1] = (char)('*');
    }

    for(int32_t i = 0; i < 55; ++i){
        for(int32_t j = 0; j < MAX_VILLAGES; ++j){
            if(i+1 == player->village[j]){
                num[i][0] = '*';
                num[i][1] = '*';
            }
        }
        for(int32_t j = 0; j < MAX_CITIES; ++j){
            if(i+1 == player->city[j]){
                num[i][0] = '*';
                num[i][1] = '*';
            }
        }
    }

    printf("\n                                                                          \n");
    printf("                           小                木                             \n");
    printf("                           麥                頭                             \n");
    printf("                           2:1   %s%s%s   2:1                            \n", num[0], road[1], num[1]);
    printf("                         /    \\ %s        %s /    \\                       \n", road[2], road[3]);
    printf("                        %s%s%s   %s   %s%s%s                           \n", num[2], road[4], num[3], tiles[0].typeName, num[4], road[5], num[5]);
    printf("                       %s        %s        %s        %s                      \n", road[6], road[7], road[8], road[9]);
    printf("         3:1 --%s%s%s   %s   %s%s%s   %s   %s%s%s-- 3:1              \n", num[6], road[10], num[7], tiles[1].typeName, num[8], road[11], num[9], tiles[2].typeName, num[10], road[12], num[11]);
    printf("            \\ %s        %s        %s        %s        %s        %s /         \n", road[13], road[14], road[15], road[16], road[17], road[18]);
    printf("             %s  %s   %s%s%s   %s   %s%s%s   %s   %s                    \n", num[12], tiles[3].typeName, num[13], road[19], num[14], tiles[4].typeName, num[15], road[20], num[16], tiles[5].typeName, num[17]);
    printf("              %s        %s        %s        %s        %s        %s            \n", road[21], road[22], road[23], road[24], road[25], road[26]);
    printf("               %s%s%s   %s   %s%s%s   %s   %s%s%s                    \n", num[18], road[27], num[19], tiles[6].typeName, num[20], road[28], num[21], tiles[7].typeName, num[22],road[29], num[23]);
    printf("              %s        %s        %s        %s        %s        %s            \n", road[30], road[31], road[32], road[33], road[34], road[35]);
    printf("            %s   %s   %s%s%s   %s   %s%s%s   %s   %s                    \n", num[24], tiles[8].typeName, num[25],road[36], num[26], tiles[9].typeName, num[27], road[37], num[28], tiles[10].typeName, num[29]);
    printf("           /  %s        %s        %s        %s        %s        %s \\         \n", road[38], road[39], road[40], road[41], road[42], road[43]);
    printf("   石頭  2:1 --%s%s%s   %s   %s%s%s   %s   %s%s%s-- 2:1  羊毛            \n", num[30], road[44], num[31], tiles[11].typeName, num[32], road[45], num[33], tiles[12].typeName, num[34], road[46], num[35]);
    printf("              %s        %s        %s        %s        %s        %s            \n", road[47], road[48], road[49], road[50], road[51], road[52]);
    printf("             %s  %s   %s%s%s   %s   %s%s%s   %s   %s                    \n", num[36], tiles[13].typeName, num[37], road[53], num[38], tiles[14].typeName, num[39], road[54], num[40], tiles[15].typeName, num[41]);
    printf("              %s        %s        %s        %s        %s        %s            \n", road[55], road[56], road[57], road[58], road[59], road[60]);
    printf("               %s%s%s   %s   %s%s%s   %s   %s%s%s                    \n", num[42], road[61], num[43], tiles[16].typeName, num[44], road[62], num[45], tiles[17].typeName, num[46], road[63], num[47]);
    printf("                     / %s        %s        %s        %s \\                   \n", road[64], road[65], road[66], road[67]);
    printf("                  2:1 --%s%s%s   %s   %s%s%s-- 3:1                     \n", num[48], road[68], num[49], tiles[18].typeName, num[50], road[69], num[51]);
    printf("              磚頭              %s        %s                                \n", road[70], road[71]);
    printf("                                 %s%s%s                                  \n", num[52], road[72], num[53]);
    printf("                                  \\    /                                  \n");
    printf("                                   3:1               ");
    printf(YELLOW"盜賊現在位置:      \n"NONE);
    printf("                                                                           \n");

}

void print_map_state2(Player *player, struct CatanTile *tiles, System *sys, int robber_loc){
    set_has(player, sys);

    char road[73][15] = { 
                "" ,  "----", 
                        "/", "\\",
                    "----",     "----", 
                "/", "\\",  "/", "\\",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
                "\\",  "/",  "\\",  "/", 
                    "----",     "----", 
                    "\\",  "/", 
                        "----"
        };

    char num [55][3] = {0};
    char num_catan [55][3] = {0};
    char num2 [73][3] = {0};
    char num3 [73][3] = {0};
    char num4 [73][3] = {0};
    for(int32_t i = 0; i < 55; i++){
        num[i][0] = (char)( (i+1)/10 + '0');
        num[i][1] = (char)( (i+1)%10 + '0');
        num_catan[i][0] = (char)( (i)/10 + '0');
        num_catan[i][1] = (char)( (i)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num2[i][0] = (char)( (i+1)/10 + '0');
        num2[i][1] = (char)( (i+1)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num3[i][0] = (char)( (i)/10 + '0');
        num3[i][1] = (char)( (i)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num4[i][0] = (char)('*');
        num4[i][1] = (char)('*');
    }
    for(int32_t k = 0; k < sys->player_num; ++k){
        for(int32_t i = 0; i < 55; ++i){
            for(int32_t j = 0; j < MAX_CITIES; ++j){
                if(i+1 == player[k].city[j]){
                    num[i][0] = '+';
                    num[i][1] = '+';
                    has_settlement[i] = k;
                }
            }
            for(int32_t j = 0; j < MAX_VILLAGES; ++j){
                if(i+1 == player[k].village[j]){
                    num[i][0] = '*';
                    num[i][1] = '*';
                    has_settlement[i] = k;
                }
            }
        }
    }  
    

    printf("\n                                                                      \n");
    printf("                           小                木                         ");
    printf("                           小                木                             \n");
    printf("                           麥                頭                         ");
    printf("                           麥                頭                             \n");
    printf("                          2:1    ");
    c(num[0], has_settlement[0]);
    c(road[1], has_road[1]);
    c(num[1], has_settlement[1]);
    printf("   2:1                           ");
    printf("                        2:1   ");
    c(num[0], has_settlement[0]);
    c(road[1], has_road[1]);
    c(num[1], has_settlement[1]);
    printf("   2:1                            \n");
    printf("                         /    \\ ");
    c(road[2], has_road[2]);
    printf("        ");
    c(road[3], has_road[3]);
    printf(" /    \\                   ");
    printf("                             /   \\ ");
    c(road[2], has_road[2]);
    printf("        ");
    c(road[3], has_road[3]);
    printf("/    \\     \n");
    printf("                        ");
    c(num[2], has_settlement[2]);
    c(road[4], has_road[4]);
    c(num[3], has_settlement[3]);
    printf("   %s   ", tiles[0].typeName);
    c(num[4], has_settlement[4]);
    c(road[5], has_road[5]);
    c(num[5], has_settlement[5]);
    printf("                            ");
    printf("                  ");
    c(num[2], has_settlement[2]);
    c(road[4], has_road[4]);
    c(num[3], has_settlement[3]);
    printf("   %s   ", num_catan[0]);
    c(num[4], has_settlement[4]);
    c(road[5], has_road[5]);
    c(num[5], has_settlement[5]);
    printf("                   \n");
    printf("                       ");
    c(road[6], has_road[6]);
    printf("        ");
    c(road[7], has_road[7]);
    printf("        ");
    c(road[8], has_road[8]);
    printf("        ");
    c(road[9], has_road[9]);
    printf("                 ");
    printf("                           ");
    c(road[6], has_road[6]);
    printf("       ");
    c(road[7], has_road[7]);
    printf("       ");
    c(road[8], has_road[8]);
    printf("        ");
    c(road[9], has_road[9]);
    printf("          \n");
    printf("         3:1 --");
    c(num[6], has_settlement[6]);
    c(road[10], has_road[10]);
    c(num[7], has_settlement[7]);
    printf("   %s   ", tiles[1].typeName);
    c(num[8], has_settlement[8]);
    c(road[11], has_road[11]);
    c(num[9], has_settlement[9]);
    printf("   %s   ", tiles[2].typeName);
    c(num[10], has_settlement[10]);
    c(road[12], has_road[12]);
    c(num[11], has_settlement[11]);
    printf("-- 3:1                 3:1 --");
    c(num[6], has_settlement[6]);
    c(road[10], has_road[10]);
    c(num[7], has_settlement[7]);
    printf("   %s   ", num_catan[1]);
    c(num[8], has_settlement[8]);
    c(road[11], has_road[11]);
    c(num[9], has_settlement[9]);
    printf("   %s   ", num_catan[2]);
    c(num[10], has_settlement[10]);
    c(road[12], has_road[12]);
    c(num[11], has_settlement[11]);
    printf("-- 3:1          \n            \\ ");
    c(road[13], has_road[13]);
    printf("        ");
    c(road[14], has_road[14]);
    printf("        ");
    c(road[15], has_road[15]);
    printf("        ");
    c(road[16], has_road[16]);
    printf("        ");
    c(road[17], has_road[17]);
    printf("        ");
    c(road[18], has_road[18]);
    printf(" /                        \\ ");
    c(road[13], has_road[13]);
    printf("      ");
    c(road[14], has_road[14]);
    printf("        ");
    c(road[15], has_road[15]);
    printf("       ");
    c(road[16], has_road[16]);
    printf("       ");
    c(road[17], has_road[17]);
    printf("       ");
    c(road[18], has_road[18]);
    printf(" /  \n");
    printf("             ");
    c(num[12], has_settlement[12]);
    printf("  %s   ", tiles[3].typeName);
    c(num[13], has_settlement[13]);
    c(road[19], has_road[19]);
    c(num[14], has_settlement[14]);
    printf("   %s   ", tiles[4].typeName);
    c(num[15], has_settlement[15]);
    c(road[20], has_road[20]);
    c(num[16], has_settlement[16]);
    printf("   %s   ", tiles[5].typeName);
    c(num[17], has_settlement[17]);
    printf("                    ");
    printf("     ");
    c(num[12], has_settlement[12]);
    printf("  %s   ", num_catan[3]);
    c(num[13], has_settlement[13]);
    c(road[19], has_road[19]);
    c(num[14], has_settlement[14]);
    printf("   %s   ", num_catan[4]);
    c(num[15], has_settlement[15]);
    c(road[20], has_road[20]);
    c(num[16], has_settlement[16]);
    printf("   %s   ", num_catan[5]);
    c(num[17], has_settlement[17]);
    printf("                \n");
    printf("              ");
    c(road[21], has_road[21]);
    printf("        ");
    c(road[22], has_road[22]);
    printf("        ");
    c(road[23], has_road[23]);
    printf("        ");
    c(road[24], has_road[24]);
    printf("        ");
    c(road[25], has_road[25]);
    printf("        ");
    c(road[26], has_road[26]);
    printf("      ");
    printf("                       ");
    c(road[21], has_road[21]);
    printf("      ");
    c(road[22], has_road[22]);
    printf("       ");
    c(road[23], has_road[23]);
    printf("       ");
    c(road[24], has_road[24]);
    printf("       ");
    c(road[25], has_road[25]);
    printf("       ");
    c(road[26], has_road[26]);
    printf("      \n");
    printf("               ");
    c(num[18], has_settlement[18]);
    c(road[27], has_road[27]);
    c(num[19], has_settlement[19]);
    printf("   %s   ", tiles[6].typeName);
    c(num[20], has_settlement[20]);
    c(road[28], has_road[28]);
    c(num[21], has_settlement[21]);
    printf("   %s   ", tiles[7].typeName);
    c(num[22], has_settlement[22]);
    c(road[29], has_road[29]);
    c(num[23], has_settlement[23]);
    printf("                              ");
    c(num[18], has_settlement[18]);
    c(road[27], has_road[27]);
    c(num[19], has_settlement[19]);
    printf("   %s   ", num_catan[6]);
    c(num[20], has_settlement[20]);
    c(road[28], has_road[28]);
    c(num[21], has_settlement[21]);
    printf("   %s   ", num_catan[7]);
    c(num[22], has_settlement[22]);
    c(road[29], has_road[29]);
    c(num[23], has_settlement[23]);
    printf("                 \n");
    printf("              ");
    c(road[30], has_road[30]);
    printf("        ");
    c(road[31], has_road[31]);
    printf("        ");
    c(road[32], has_road[32]);
    printf("        ");
    c(road[33], has_road[33]);
    printf("        ");
    c(road[34], has_road[34]);
    printf("        ");
    c(road[35], has_road[35]);
    printf("      ");
    printf("                      ");
    c(road[30], has_road[30]);
    printf("       ");
    c(road[31], has_road[31]);
    printf("       ");
    c(road[32], has_road[32]);
    printf("       ");
    c(road[33], has_road[33]);
    printf("       ");
    c(road[34], has_road[34]);
    printf("       ");
    c(road[35], has_road[35]);
    printf("    \n");
    printf("            ");
    c(num[24], has_settlement[24]);
    printf("   %s   ", tiles[8].typeName);
    c(num[25], has_settlement[25]);
    c(road[36], has_road[36]);
    c(num[26], has_settlement[26]);
    printf("   %s   ", tiles[9].typeName);
    c(num[27], has_settlement[27]);
    c(road[37], has_road[37]);
    c(num[28], has_settlement[28]);
    printf("   %s   ", tiles[10].typeName);
    c(num[29], has_settlement[29]);
    printf("                        ");
    c(num[24], has_settlement[24]);
    printf("   %s   ", num_catan[8]);
    c(num[25], has_settlement[25]);
    c(road[36], has_road[36]);
    c(num[26], has_settlement[26]);
    printf("   %s   ", num_catan[9]);
    c(num[27], has_settlement[27]);
    c(road[37], has_road[37]);
    c(num[28], has_settlement[28]);
    printf("   %s   ", num_catan[10]);
    c(num[29], has_settlement[29]);
    printf("                \n");
    printf("           /  ");
    c(road[38], has_road[38]);
    printf("        ");
    c(road[39], has_road[39]);
    printf("        ");
    c(road[40], has_road[40]);
    printf("        ");
    c(road[41], has_road[41]);
    printf("        ");
    c(road[42], has_road[42]);
    printf("        ");
    c(road[43], has_road[43]);
    printf(" \\                       /  ");
    c(road[38], has_road[38]);
    printf("       ");
    c(road[39], has_road[39]);
    printf("       ");
    c(road[40], has_road[40]);
    printf("       ");
    c(road[41], has_road[41]);
    printf("       ");
    c(road[42], has_road[42]);
    printf("       ");
    c(road[43], has_road[43]);
    printf(" \\    \n   石頭  2:1 --");
    c(num[30], has_settlement[30]);
    c(road[44], has_road[44]);
    c(num[31], has_settlement[31]);
    printf("   %s   ", tiles[11].typeName);
    c(num[32], has_settlement[32]);
    c(road[45], has_road[45]);
    c(num[33], has_settlement[33]);
    printf("   %s   ", tiles[12].typeName);
    c(num[34], has_settlement[34]);
    c(road[46], has_road[46]);
    c(num[35], has_settlement[35]);
    printf("-- 2:1  羊毛      石頭  2:1 --");
    c(num[30], has_settlement[30]);
    c(road[44], has_road[44]);
    c(num[31], has_settlement[31]);
    printf("   %s   ", num_catan[11]);
    c(num[32], has_settlement[32]);
    c(road[45], has_road[45]);
    c(num[33], has_settlement[33]);
    printf("   %s   ", num_catan[12]);
    c(num[34], has_settlement[34]);
    c(road[46], has_road[46]);
    c(num[35], has_settlement[35]);
    printf("-- 2:1  羊毛     \n");
    printf("              ");
    c(road[47], has_road[47]);
    printf("        ");
    c(road[48], has_road[48]);
    printf("        ");
    c(road[49], has_road[49]);
    printf("        ");
    c(road[50], has_road[50]);
    printf("        ");
    c(road[51], has_road[51]);
    printf("        ");
    c(road[52], has_road[52]);
    printf("                            ");
    c(road[47], has_road[47]);
    printf("       ");
    c(road[48], has_road[48]);
    printf("       ");
    c(road[49], has_road[49]);
    printf("       ");
    c(road[50], has_road[50]);
    printf("       ");
    c(road[51], has_road[51]);
    printf("       ");
    c(road[52], has_road[52]);
    printf("   \n");
    printf("             ");
    c(num[36], has_settlement[36]);
    printf("  %s   ", tiles[13].typeName);
    c(num[37], has_settlement[37]);
    c(road[53], has_road[53]);
    c(num[38], has_settlement[38]);
    printf("   %s   ", tiles[14].typeName);
    c(num[39], has_settlement[39]);
    c(road[54], has_road[54]);
    c(num[40], has_settlement[40]);
    printf("   %s   ", tiles[15].typeName);
    c(num[41], has_settlement[41]);
    printf("                         ");
    c(num[36], has_settlement[36]);
    printf("   %s   ", num_catan[13]);
    c(num[37], has_settlement[37]);
    c(road[53], has_road[53]);
    c(num[38], has_settlement[38]);
    printf("   %s   ", num_catan[14]);
    c(num[39], has_settlement[39]);
    c(road[54], has_road[54]);
    c(num[40], has_settlement[40]);
    printf("   %s   ", num_catan[15]);
    c(num[41], has_settlement[41]);
    printf(" \n");
    printf("              ");
    c(road[55], has_road[55]);
    printf("        ");
    c(road[56], has_road[56]);
    printf("        ");
    c(road[57], has_road[57]);
    printf("        ");
    c(road[58], has_road[58]);
    printf("        ");
    c(road[59], has_road[59]);
    printf("        ");
    c(road[60], has_road[60]);
    printf("                            ");
    c(road[55], has_road[55]);
    printf("       ");
    c(road[56], has_road[56]);
    printf("       ");
    c(road[57], has_road[57]);
    printf("       ");
    c(road[58], has_road[58]);
    printf("       ");
    c(road[59], has_road[59]);
    printf("       ");
    c(road[60], has_road[60]);
    printf("\n");
    printf("               ");
    c(num[42], has_settlement[42]);
    c(road[61], has_road[61]);
    c(num[43], has_settlement[43]);
    printf("   %s   ", tiles[16].typeName);
    c(num[44], has_settlement[44]);
    c(road[62], has_road[62]);
    c(num[45], has_settlement[45]);
    printf("   %s   ", tiles[17].typeName);
    c(num[46], has_settlement[46]);
    c(road[63], has_road[63]);
    c(num[47], has_settlement[47]);
    printf("                 ");
    printf("            ");
    c(num[42], has_settlement[42]);
    c(road[61], has_road[61]);
    c(num[43], has_settlement[43]);
    printf("   %s   ", num_catan[16]);
    c(num[44], has_settlement[44]);
    c(road[62], has_road[62]);
    c(num[45], has_settlement[45]);
    printf("   %s   ", num_catan[17]);
    c(num[46], has_settlement[46]);
    c(road[63], has_road[63]);
    c(num[47], has_settlement[47]);
    printf(" \n");
    printf("                     / ");
    c(road[64], has_road[64]);
    printf("       ");
    c(road[65], has_road[65]);
    printf("        ");
    c(road[66], has_road[66]);
    printf("        ");
    c(road[67], has_road[67]);
    printf(" \\                                         / ");
    c(road[64], has_road[64]);
    printf("       ");
    c(road[65], has_road[65]);
    printf("       ");
    c(road[66], has_road[66]);
    printf("       ");
    c(road[67], has_road[67]);
    printf(" \\               \n");
    printf("                  2:1 --");
    c(num[48], has_settlement[48]);
    c(road[68], has_road[68]);
    c(num[49], has_settlement[49]);
    printf("   %s   ", tiles[18].typeName);
    c(num[50], has_settlement[50]);
    c(road[69], has_road[69]);
    c(num[51], has_settlement[51]);
    printf("-- 3:1                                 2:1 -- ");
    c(num[48], has_settlement[48]);
    c(road[68], has_road[68]);
    c(num[49], has_settlement[49]);
    printf("   %s   ", num_catan[18]);
    c(num[50], has_settlement[50]);
    c(road[69], has_road[69]);
    c(num[51], has_settlement[51]);
    printf("-- 3:1                \n");
    printf("              磚頭              ");
    c(road[70], has_road[70]);
    printf("        ");
    c(road[71], has_road[71]);
    printf("                                           磚頭              ");
    c(road[70], has_road[70]);
    printf("        ");
    c(road[71], has_road[71]);
    printf(" \n");
    printf("                                 ");
    c(num[52], has_settlement[52]);
    c(road[72], has_road[72]);
    c(num[53], has_settlement[53]);
    printf("                                                               ");
    c(num[52], has_settlement[52]);
    c(road[72], has_road[72]);
    c(num[53], has_settlement[53]);
    printf("                            \n");
    printf("                                  \\    /                               ");
    printf("                                  \\    /                          \n");
    printf("                                   3:1                       ");
    printf(YELLOW"盜賊位置: %d"NONE, robber_loc);
    printf("                                 3:1    \n");

}

void print_map_state(Player *player, struct CatanTile *tiles, System *sys, int robber_loc){
    char road[73][15] = { 
                "" ,  "----", 
                        "/", "\\",
                    "----",     "----", 
                "/", "\\",  "/", "\\",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
                "\\",  "/",  "\\",  "/", 
                    "----",     "----", 
                    "\\",  "/", 
                        "----"
        };

    char num [55][3] = {0};
    char num_catan [55][3] = {0};
    char num2 [73][3] = {0};
    char num3 [73][3] = {0};
    char num4 [73][3] = {0};
    for(int32_t i = 0; i < 55; i++){
        num[i][0] = (char)( (i+1)/10 + '0');
        num[i][1] = (char)( (i+1)%10 + '0');
        num_catan[i][0] = (char)( (i)/10 + '0');
        num_catan[i][1] = (char)( (i)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num2[i][0] = (char)( (i+1)/10 + '0');
        num2[i][1] = (char)( (i+1)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num3[i][0] = (char)( (i)/10 + '0');
        num3[i][1] = (char)( (i)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num4[i][0] = (char)('*');
        num4[i][1] = (char)('*');
    }
    for(int32_t k = 0; k < sys->player_num; ++k){
        for(int32_t i = 0; i < 55; ++i){
            for(int32_t j = 0; j < MAX_VILLAGES; ++j){
                if(i+1 == player[k].village[j]){
                    num[i][0] = '*';
                    num[i][1] = '*';
                }
            }
            for(int32_t j = 0; j < MAX_CITIES; ++j){
                if(i+1 == player[k].city[j]){
                    num[i][0] = '*';
                    num[i][1] = '*';
                }
            }
        }
    }  
    
    printf("\n                                                                      \n");
    printf("                           小                木                         ");
    printf("                           小                木                             \n");
    printf("                           麥                頭                         ");
    printf("                           麥                頭                             \n");
    printf("                          2:1   %s%s%s   2:1                           ", num[0], road[1], num[1]);
    printf("                         2:1   %s%s%s   2:1                            \n", num[0], road[1], num[1]);
    printf("                         /    \\ %s        %s /    \\                   ", road[2], road[3]);
    printf("                             /    \\ %s        %s /    \\     \n", road[2], road[3]);
    printf("                        %s%s%s   %s   %s%s%s                            ", num[2], road[4], num[3], tiles[0].typeName, num[4], road[5], num[5]);
    printf("                  %s%s%s    %s   %s%s%s                   \n", num[2], road[4], num[3], num_catan[0], num[4], road[5], num[5]);
    printf("                       %s        %s        %s        %s                 ", road[6], road[7], road[8], road[9]);
    printf("                           %s        %s       %s       %s          \n", road[6], road[7], road[8], road[9]);
    printf("         3:1 --%s%s%s   %s   %s%s%s   %s   %s%s%s-- 3:1                 ", num[6], road[10], num[7], tiles[1].typeName, num[8], road[11], num[9], tiles[2].typeName, num[10], road[12], num[11]);
    printf("3:1 --%s%s%s   %s   %s%s%s   %s   %s%s%s-- 3:1          \n", num[6], road[10], num[7], num_catan[1], num[8], road[11], num[9], num_catan[2], num[10], road[12], num[11]);
    printf("            \\ %s        %s        %s        %s        %s        %s /   ", road[13], road[14], road[15], road[16], road[17], road[18]);
    printf("                     \\ %s      %s        %s       %s       %s       %s /  \n", road[13], road[14], road[15], road[16], road[17], road[18]);
    printf("             %s  %s   %s%s%s   %s   %s%s%s   %s   %s                    ", num[12], tiles[3].typeName, num[13], road[19], num[14], tiles[4].typeName, num[15], road[20], num[16], tiles[5].typeName, num[17]);
    printf("     %s  %s   %s%s%s   %s   %s%s%s   %s   %s                \n", num[12], num_catan[3], num[13], road[19], num[14], num_catan[4], num[15], road[20], num[16], num_catan[5], num[17]);
    printf("              %s        %s        %s        %s        %s        %s      ", road[21], road[22], road[23], road[24], road[25], road[26]);
    printf("                      %s       %s       %s       %s       %s       %s      \n", road[21], road[22], road[23], road[24], road[25], road[26]);
    printf("               %s%s%s   %s   %s%s%s   %s   %s%s%s                       ", num[18], road[27], num[19], tiles[6].typeName, num[20], road[28], num[21], tiles[7].typeName, num[22],road[29], num[23]);
    printf("       %s%s%s   %s   %s%s%s   %s   %s%s%s                 \n", num[18], road[27], num[19], num_catan[6], num[20], road[28], num[21], num_catan[7], num[22],road[29], num[23]);
    printf("              %s        %s        %s        %s        %s        %s      ", road[30], road[31], road[32], road[33], road[34], road[35]);
    printf("                      %s       %s       %s       %s       %s       %s    \n", road[30], road[31], road[32], road[33], road[34], road[35]);
    printf("            %s   %s   %s%s%s   %s   %s%s%s   %s   %s                    ", num[24], tiles[8].typeName, num[25],road[36], num[26], tiles[9].typeName, num[27], road[37], num[28], tiles[10].typeName, num[29]);
    printf("    %s   %s   %s%s%s   %s   %s%s%s   %s   %s                \n", num[24], num_catan[8], num[25],road[36], num[26], num_catan[9], num[27], road[37], num[28], num_catan[10], num[29]);
    printf("           /  %s        %s        %s        %s        %s        %s \\   ", road[38], road[39], road[40], road[41], road[42], road[43]);
    printf("                    /  %s       %s       %s       %s       %s       %s \\    \n", road[38], road[39], road[40], road[41], road[42], road[43]);
    printf("   石頭  2:1 --%s%s%s   %s   %s%s%s   %s   %s%s%s-- 2:1  羊毛   ", num[30], road[44], num[31], tiles[11].typeName, num[32], road[45], num[33], tiles[12].typeName, num[34], road[46], num[35]);
    printf("   石頭  2:1 --%s%s%s   %s   %s%s%s   %s   %s%s%s-- 2:1  羊毛     \n", num[30], road[44], num[31], num_catan[11], num[32], road[45], num[33], num_catan[12], num[34], road[46], num[35]);
    printf("              %s        %s        %s        %s        %s        %s      ", road[47], road[48], road[49], road[50], road[51], road[52]);
    printf("                      %s       %s       %s       %s       %s       %s        \n", road[47], road[48], road[49], road[50], road[51], road[52]);
    printf("             %s  %s   %s%s%s   %s   %s%s%s   %s   %s               ", num[36], tiles[13].typeName, num[37], road[53], num[38], tiles[14].typeName, num[39], road[54], num[40], tiles[15].typeName, num[41]);
    printf("          %s  %s   %s%s%s   %s   %s%s%s   %s   %s               \n", num[36], num_catan[13], num[37], road[53], num[38], num_catan[14], num[39], road[54], num[40], num_catan[15], num[41]);
    printf("              %s        %s        %s        %s        %s        %s      ", road[55], road[56], road[57], road[58], road[59], road[60]);
    printf("                      %s       %s       %s       %s       %s       %s          \n", road[55], road[56], road[57], road[58], road[59], road[60]);
    printf("               %s%s%s   %s   %s%s%s   %s   %s%s%s                 ", num[42], road[61], num[43], tiles[16].typeName, num[44], road[62], num[45], tiles[17].typeName, num[46], road[63], num[47]);
    printf("            %s%s%s   %s   %s%s%s   %s   %s%s%s                \n", num[42], road[61], num[43], num_catan[16], num[44], road[62], num[45], num_catan[17], num[46], road[63], num[47]);
    printf("                     / %s        %s        %s        %s \\              ", road[64], road[65], road[66], road[67]);
    printf("                           / %s       %s       %s       %s \\               \n", road[64], road[65], road[66], road[67]);
    printf("                  2:1 --%s%s%s   %s   %s%s%s-- 3:1                      ", num[48], road[68], num[49], tiles[18].typeName, num[50], road[69], num[51]);
    printf("           2:1 -- %s%s%s   %s   %s%s%s-- 3:1                \n", num[48], road[68], num[49], num_catan[18], num[50], road[69], num[51]);
    printf("              磚頭              %s        %s                            ", road[70], road[71]);
    printf("               磚頭              %s        %s                          \n", road[70], road[71]);
    printf("                                 %s%s%s                                 ", num[52], road[72], num[53]);
    printf("                              %s%s%s                            \n", num[52], road[72], num[53]);
    printf("                                  \\    /                               ");
    printf("                                  \\    /                          \n");
    printf("                                   3:1                       ");
    printf(YELLOW"盜賊位置: %d"NONE, robber_loc);
    printf("                                 3:1    \n");

}

void print_map_point(Player *player, struct CatanTile *tiles, System *sys){
    char road[73][15] = { 
                "" ,  "----", 
                        "/", "\\",
                    "----",     "----", 
                "/", "\\",  "/", "\\",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
            "/", "\\",  "/",  "\\",  "/",  "\\",
                    "----",     "----", 
        "\\",   "/", "\\",  "/", "\\",   "/",
            "----",     "----",     "----", 
                "\\",  "/",  "\\",  "/", 
                    "----",     "----", 
                    "\\",  "/", 
                        "----"
        };

    char num [55][3] = {0};
    char num2 [73][3] = {0};
    char num3 [73][3] = {0};
    char num4 [73][3] = {0};
    for(int32_t i = 0; i < 55; i++){
        num[i][0] = (char)( (i+1)/10 + '0');
        num[i][1] = (char)( (i+1)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num2[i][0] = (char)( (i+1)/10 + '0');
        num2[i][1] = (char)( (i+1)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num3[i][0] = (char)( (i)/10 + '0');
        num3[i][1] = (char)( (i)%10 + '0');
    }
    for(int32_t i = 0; i < 73; i++){
        num4[i][0] = (char)('*');
        num4[i][1] = (char)('*');
    }

    for(int32_t i = 0; i < 55; ++i){
        for(int32_t j = 0; j < MAX_VILLAGES; ++j){
            if(i+1 == player->village[j]){
                num[i][0] = '*';
                num[i][1] = '*';
            }
        }
        for(int32_t j = 0; j < MAX_CITIES; ++j){
            if(i+1 == player->city[j]){
                num[i][0] = '*';
                num[i][1] = '*';
            }
        }
    }

    printf("\n                                                                          \n");
    printf("                           小                木                             \n");
    printf("                           麥                頭                             \n");
    printf("                           2:1   %s%s%s   2:1                            \n", num[0], road[1], num[1]);
    printf("                         /    \\ %s        %s /    \\                       \n", road[2], road[3]);
    printf("                        %s%s%s   %s   %s%s%s                           \n", num[2], road[4], num[3], tiles[0].typeName, num[4], road[5], num[5]);
    printf("                       %s        %s        %s        %s                      \n", road[6], road[7], road[8], road[9]);
    printf("         3:1 --%s%s%s   %s   %s%s%s   %s   %s%s%s-- 3:1              \n", num[6], road[10], num[7], tiles[1].typeName, num[8], road[11], num[9], tiles[2].typeName, num[10], road[12], num[11]);
    printf("            \\ %s        %s        %s        %s        %s        %s /         \n", road[13], road[14], road[15], road[16], road[17], road[18]);
    printf("             %s  %s   %s%s%s   %s   %s%s%s   %s   %s                    \n", num[12], tiles[3].typeName, num[13], road[19], num[14], tiles[4].typeName, num[15], road[20], num[16], tiles[5].typeName, num[17]);
    printf("              %s        %s        %s        %s        %s        %s            \n", road[21], road[22], road[23], road[24], road[25], road[26]);
    printf("               %s%s%s   %s   %s%s%s   %s   %s%s%s                    \n", num[18], road[27], num[19], tiles[6].typeName, num[20], road[28], num[21], tiles[7].typeName, num[22],road[29], num[23]);
    printf("              %s        %s        %s        %s        %s        %s            \n", road[30], road[31], road[32], road[33], road[34], road[35]);
    printf("            %s   %s   %s%s%s   %s   %s%s%s   %s   %s                    \n", num[24], tiles[8].typeName, num[25],road[36], num[26], tiles[9].typeName, num[27], road[37], num[28], tiles[10].typeName, num[29]);
    printf("           /  %s        %s        %s        %s        %s        %s \\         \n", road[38], road[39], road[40], road[41], road[42], road[43]);
    printf("   石頭  2:1 --%s%s%s   %s   %s%s%s   %s   %s%s%s-- 2:1  羊毛            \n", num[30], road[44], num[31], tiles[11].typeName, num[32], road[45], num[33], tiles[12].typeName, num[34], road[46], num[35]);
    printf("              %s        %s        %s        %s        %s        %s            \n", road[47], road[48], road[49], road[50], road[51], road[52]);
    printf("             %s  %s   %s%s%s   %s   %s%s%s   %s   %s                    \n", num[36], tiles[13].typeName, num[37], road[53], num[38], tiles[14].typeName, num[39], road[54], num[40], tiles[15].typeName, num[41]);
    printf("              %s        %s        %s        %s        %s        %s            \n", road[55], road[56], road[57], road[58], road[59], road[60]);
    printf("               %s%s%s   %s   %s%s%s   %s   %s%s%s                    \n", num[42], road[61], num[43], tiles[16].typeName, num[44], road[62], num[45], tiles[17].typeName, num[46], road[63], num[47]);
    printf("                     / %s        %s        %s        %s \\                   \n", road[64], road[65], road[66], road[67]);
    printf("                  2:1 --%s%s%s   %s   %s%s%s-- 3:1                     \n", num[48], road[68], num[49], tiles[18].typeName, num[50], road[69], num[51]);
    printf("              磚頭              %s        %s                                \n", road[70], road[71]);
    printf("                                 %s%s%s                                  \n", num[52], road[72], num[53]);
    printf("                                  \\    /                                  \n");
    printf("                                   3:1                                     \n");
    printf("                                                                           \n");

}

