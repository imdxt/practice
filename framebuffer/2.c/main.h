#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
typedef unsigned int u32_t;
typedef unsigned char u8_t;

typedef struct
{
    int w;
    int h;
    int bpp;
    void *fb_mem;
}v_info_t;
typedef struct
{
    int dx;
    int dy;
    int dz;
    int button;
}m_event;

extern void create_scr_fb(void);
extern int fb_one_pixel(int x, int y , u32_t color);
extern int fb_line(int x1,int y1,int x2,int y2,u32_t color);
extern int fb_circle(int x0,int y0,int r,u32_t color);
extern void print_board(int x1,int y1,u32_t color);
extern int create_spot(int x1,int y1,u32_t color,int w);
extern v_info_t fb_v;
extern int save_shape(int x,int y);
extern int restore_shape(int x,int y);
extern int draw_cursor(int x,int y);
extern int mouse_doing(void);
#endif
