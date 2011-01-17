#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <unistd.h>
#include "main.h"


v_info_t fb_v;

void create_scr_fb( void )
{
	int fd;
	struct fb_var_screeninfo fb_var;
	u32_t *p = NULL;

	fd = open("/dev/fb0", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	if(ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
	{
		perror("ioctl");
		exit(1);
	}

	fb_v.w = fb_var.xres;
	fb_v.h = fb_var.yres;
	fb_v.bpp = fb_var.bits_per_pixel;

	printf("w = %d\th = %d\tbpp = %d\n", fb_v.w, fb_v.h, fb_v.bpp);

	p = mmap(NULL, fb_v.w*fb_v.h*fb_v.bpp/8, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
	if(p == MAP_FAILED)
	{
		perror("map");
		exit(1);
	}
	*p = 0x00ff0000;
	memset(p, 0, fb_v.w*fb_v.h*fb_v.bpp/8);
	fb_v.fb_mem = p;
	close(fd);
}

int fb_one_pixel(int x, int y, u32_t color)
{
	u32_t *p = fb_v.fb_mem;

	p[x+y*fb_v.w] = color;
	return 0;
}


int main(void)
{
	create_scr_fb();
//	fb_line( 300, 767,100, 0,0x0000ff00);
	print_board();
//	fb_circle(512, 350, 300, 0x00ff0000);
//	print_one_chess(304, 207, 0x00ff0000);
//	draw_cursor(512, 300);
//	sleep(4);
//	restore_shape(512, 300);
//	draw_cursor(300, 200);
	mouse_doing();
	return 0;
}
