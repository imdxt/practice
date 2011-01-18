#include "main.h"

v_info_t fb_v;
char buffer[BUFFER_SIZE];
int client_sock;
int local_id,remote_id,n;
socklen_t server_len;
struct sockaddr_in server;
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
    printf("w = %d\th = %d\tbpp = %d\n",fb_v.w,fb_v.h,fb_v.bpp);
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
	if(argc < 2)
	{
		printf("\n");
		printf("Sorry, your type is wrong.\n");
		printf("Usage: %s x.x.x.x(server IP)\n", argv[0]);
		printf("\n");
		exit(EXIT_FAILURE);
	}
    printf("input local user id: ");
    n = scanf("%d",&local_id);
//    getchar();
    printf("input remote user id: ");
    n = scanf("%d",&remote_id);
//    getchar();

	// 1. create socket - create an endpoint for communication
	// int socket(int domain, int type, int protocol);
	if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("create socket ok!\n");
	}

	// make server address information
	bzero(&server, sizeof(server));
//	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(argv[1]);
    buffer[0] = REGISTER_MSG;
    buffer[1] = local_id;
    server_len = sizeof(server);
    n = sendto(client_sock,buffer,2,0,(struct sockaddr *)&server,server_len);
    create_scr_fb();
    print_board(350,70,0x008b6914);    
    mouse_doing();    
	close(client_sock);
   
   return 0;
}
