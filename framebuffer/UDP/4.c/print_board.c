#include <stdio.h>
#include "main.h"

void print_board(int x1,int y1, u32_t color)
{
    int i=0,j=0;
    u32_t *p = fb_v.fb_mem;
    for(i=50;i<780;i++)// Boottom color 
    {
       for(j=330;j<1240;j++)
         p[i*fb_v.w + j]=0x00cd950c;
    }
    for(i=0;i<24;i++)  //hang
        fb_line(x1,y1+i*30,x1+(30-1)*30,y1+i*30,color);
    for(i=0;i<30;i++)  //lie
        fb_line(x1+i*30,y1,x1+i*30,y1+(24-1)*30,color);
}
int create_spot(int x1,int y1,u32_t color,int w)  
{
    int c_x = x1;
    int c_y = y1;
    y1 = (y1-70)%30;
    x1 = (x1-350)%30;
    c_x += ((x1>15)? (30-x1):-x1);
    c_y += ((y1>15)? (30-y1):-y1);
    p->x =(c_x-350)/30;
    p->y =(c_y-70)/30;
    printf("mx = %d my =%d\n",p->x,p->y);
    if(w%2 == 0)
        fb_circle(c_x,c_y,13,color);
    if(w%2 == 1)
        fb_circle(c_x,c_y,13,color);
    return (w%2 + 1);
}
