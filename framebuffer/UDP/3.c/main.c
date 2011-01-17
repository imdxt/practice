#include "main.h"

v_info_t fb_v;
char buffer[BUFFER_SIZE];
int server_sock;
socklen_t client_len;
struct sockaddr_in server,client;
struct in_addr in;
void create_scr_fb(void)
{
    int fd;
    struct fb_var_screeninfo fb_var;
    u32_t *p=NULL;

    fd=open("/dev/fb0",O_RDWR);
    if(fd<0)
    {
        perror("open");
        exit(1);
    }
    if(ioctl(fd,FBIOGET_VSCREENINFO,&fb_var)<0)
    {
        perror("ioctl");
        exit(1);
    }
    fb_v.w=fb_var.xres;  
    fb_v.h=fb_var.yres;  
    fb_v.bpp=fb_var.bits_per_pixel; 
    printf("w = %d\n h = %d\n bpp = %d\n",fb_v.w,fb_v.h,fb_v.bpp);
    p = mmap(NULL,fb_v.w*fb_v.h*fb_v.bpp/8,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    if(p == MAP_FAILED)
    {
        perror("map");
        exit(1);
    }
    fb_v.fb_mem = p;
    close(fd);
}
int fb_one_pixel(int x,int y,u32_t color)
{
    u32_t *p = fb_v.fb_mem;
    p[x+y*fb_v.w] = color;
    return 0;
}
int main(int argc, const char *argv[])
{
	
	if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("create socket ok!\n");
	}
	
	// 2. bind - bind a name to a socket
	// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = htons(INADDR_ANY);

	if(bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("bind addr ok!\n");
		printf("\n");
	}
    create_scr_fb();
    print_board(100,40,0x008b6914);    
    mouse_doing();    
	printf("Client close the socket\n");
	close(server_sock);
   return 0;
}
