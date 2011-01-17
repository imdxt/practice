#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include "main.h"
#include <unistd.h>

v_info_t fb_v;

void create_scr_fb(void)
{
    int fd;
    struct fb_var_screeninfo fb_var;
    u32_t *p = NULL;

    fd = open("/dev/fb0",O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }
    if(ioctl(fd,FBIOGET_VSCREENINFO, &fb_var) < 0)
    {
        perror("ioctl");
        exit(1);
    }
    fb_v.w = fb_var.xres;
    fb_v.h = fb_var.yres;
    fb_v.bpp = fb_var.bits_per_pixel;
   
   printf("w = %d\th = %d\tbpp = %d\n",fb_v.w,fb_v.h,fb_v.bpp);
    p = mmap(NULL,fb_v.w*fb_v.h*fb_v.bpp/8,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p == MAP_FAILED)
    {
        perror("map");
        exit(1);
    }
    *p = 0x00ff0000;
    close(fd);
}

int main(void)
{
    create_scr_fb();
    return 0;
}
