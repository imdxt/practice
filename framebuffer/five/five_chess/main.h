#ifndef _MAIN_H
#define	_MAIN_H

#include <stdio.h>

#define		V_NUM	30
#define		P_NUM	24
typedef	unsigned int	u32_t;
typedef unsigned char	u8_t;


typedef	struct 
{
	int w;
	int h;
	int bpp;
	void *fb_mem;
} v_info_t;

typedef struct
{
	int dx;
	int dy;
	int dz;

	int button;
} m_event;

extern int fb_one_pixel(int x, int y, u32_t color);
extern int fb_line(int x1, int y1, int x2, int y2, u32_t color);
extern void print_board(void);
extern int fb_circle(int x, int y, int r, u32_t color);
extern int print_one_chess(int x, int y);
extern int draw_cursor(int x, int y);
extern int restore_shape(int x, int y);
extern int save_shape(int x, int y);
extern int mouse_doing(void);
#endif
