#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "system.h"
#include "map.h"

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
    printf("                                   3:1                                     \n");
    printf("                                                                           \n");

}

void print_map_state(Player *player, struct CatanTile *tiles, System *sys){
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
        num_catan[i][0] = (char)( (i+1)/10 + '0');
        num_catan[i][1] = (char)( (i+1)%10 + '0');
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
    printf("                                   3:1                                  ");
    printf("                                  3:1                              \n");
    printf("                                                                    \n");

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

