#pragma once
#include <stdio.h>
#define RED "\033[0;32;31m"
#define BROWN "\033[0;33m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define NONE "\033[m"
void red(){
    printf("\033[31m");
}
void green(){
    printf("\033[32m");
}
void yellow(){
    printf("\033[33m");
}
void blue(){
    printf("\033[34m");
}
void purple(){
    printf("\033[35m");
}
void gray(){
    printf("\033[2;37m");
}
void white(){
    printf("\033[0;37m");
}
void reset(){
    printf("\033[0m");
}
// https://chunchaichang.blogspot.com/2011/07/printf.html
/*
'1';33m -->1: 加粗
'2'     -->2: 暗沉
'3'     -->3: 斜體
'4'     -->4: 下劃線

顏色分為背景色和字體色，30~39用來設置字體色，40~49設置背景：
背景色      字體色
40: 黑      30: 黑
41: 紅      31: 紅
42: 綠      32: 綠
43: 黃      33: 黃
44: 藍      34: 藍
45: 紫      35: 紫
46: 深綠    36: 深綠
47: 白色    37: 白色
*/
/*
\033[0m 關閉所有屬性
\033[1m 設置高亮度
\033[4m 下劃線
\033[5m 閃爍
\033[7m 反顯
\033[8m 消隱
\033[30m -- \033[37m 設置前景色
\033[40m -- \033[47m 設置背景色
\033[nA 光標上移n行
\033[nB 光標下移n行
\033[nC 光標右移n行
\033[nD 光標左移n行
\033[y;xH設置光標位置
\033[2J 清屏
\033[K 清除從光標到行尾的內容
\033[s 保存光標位置
\033[u 恢復光標位置
\033[?25l 隱藏光標
\033[?25h 顯示光標
*/