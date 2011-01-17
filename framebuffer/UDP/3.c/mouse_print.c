#include "main.h"


#define C_WIDTH 10
#define C_HEIGHT 17
#define T__ 0XFFFFFFFF
#define BORD 0x0
#define X__ 0xFFFF

static u32_t cursor_pixel[C_WIDTH*C_HEIGHT]=
{
    BORD,T__,T__,T__,T__,T__,T__,T__,T__,T__,
    BORD,BORD,T__,T__,T__,T__,T__,T__,T__,T__,
    BORD,X__,BORD,T__,T__,T__,T__,T__,T__,T__,
    BORD,X__,X__,BORD,T__,T__,T__,T__,T__,T__,
    BORD,X__,X__,X__,BORD,T__,T__,T__,T__,T__,
    BORD,X__,X__,X__,X__,BORD,T__,T__,T__,T__,
    BORD,X__,X__,X__,X__,X__,BORD,T__,T__,T__,
    BORD,X__,X__,X__,X__,X__,X__,BORD,T__,T__,
    BORD,X__,X__,X__,X__,X__,X__,X__,BORD,T__,
    BORD,X__,X__,X__,X__,X__,X__,X__,X__,BORD,
    BORD,X__,X__,X__,X__,X__,BORD,BORD,BORD,BORD,
    BORD,X__,X__,BORD,X__,X__,BORD,T__,T__,T__,
    BORD,X__,BORD,T__,BORD,X__,X__,BORD,T__,T__,
    BORD,BORD,T__,T__,BORD,X__,X__,BORD,T__,T__,
    T__,T__,T__,T__,T__,BORD,X__,X__,BORD,T__,
    T__,T__,T__,T__,T__,BORD,X__,X__,BORD,T__,
    T__,T__,T__,T__,T__,T__,BORD,BORD,T__,T__
};

static u32_t shape_save[C_WIDTH*C_HEIGHT] = {0};//init 
char board[24*30];
    msg_t *p = (msg_t *)buffer;
int save_shape(int x,int y)
{
    int i = 0;
    int j = 0;
    u32_t *p = fb_v.fb_mem;
    for(i=0;i<C_HEIGHT;i++)
    {
        for(j=0;j<C_WIDTH;j++)
            shape_save[j+i*C_WIDTH] = p[x+j+(y+i)*fb_v.w];
    }
    return 0;
}
int restore_shape(int x,int y)
{
    int i = 0;
    int j = 0;
    for(i=0;i<C_HEIGHT;i++)
    {
        for(j=0; j<C_WIDTH;j++)
            fb_one_pixel(x+j,y+i,shape_save[j+i*C_WIDTH]);
    }
    return 0;
}
int draw_cursor(int x,int y)
{
    int i = 0;
    int j = 0;
    save_shape(x,y);
    for(i=0;i<C_HEIGHT;i++)
    {   
        for(j=0; j<C_WIDTH;j++)
        {
            if(cursor_pixel[j+i*C_WIDTH] != 0XFFFFFFFF )
            fb_one_pixel(x+j,y+i,cursor_pixel[j+i*C_WIDTH]);
        }
   }
    return 0;
}
int get_m_info(int fd,m_event *event)
{
    char buf[8] = {0};
    int n;
    n = read(fd,buf,8);
    if(n <= 0)
    {
        return 0;
    }
    event->button = (buf[0]& 0x07);
    event->dx = buf[1];
    event->dy = -buf[2];
    event->dz = buf[3];
    return n;
}
int chess_count(int x, int y,int who)
{
    int i=x;
    int j=y;
    i = (i - 100)/30;
    j = (j - 40)/30;
    if(((x-100)%30) > 15)
        i++;
    if(((y-40)%30) > 15)
        j++;
     board[i +j*30] =who;
     return 0;
}

int check(int x,int y)
{
    int i = (x-75)/30;
    int j = (y-25)/30;
    return board[i+j*30];
}

int check_five(int x,int y)
{
    int i = 0;
    int j = 0;

    char counter = 0;
    char storage = 0;
    char nx = 0;
    char ny = 0;
    
    char n_x[4] = {0,1,1,1};
    char n_y[4] = {1,0,1,-1};

    storage = board[x+y*30];
    if(storage == 0)
        return 0;
    for(j=0; j<4; j++)
    {
        counter = 1;
        nx = x;
        ny = y;
        for(i=1; i<5; i++)
        {
            nx += n_x[j];
            ny += n_y[j];
            if(board[nx+ny*30] == storage)
                counter++;
            else
                break;
        }
        if(counter == 5)
            return storage;
   }
   return 0;

}
int check_all(void)
{
    int i = 0;
    int j = 0;
    for(i=0;i<24;i++)
    {
        for(j=0;j<30;j++)
        {
            if(check_five(i,j) != 0)
            {
                return 1;
            }
        }
    }
    return 0;
}
int print_choice(void)
{
    fb_circle(40,60,20,0);
    fb_circle(10,140,20,0x00ffffff);
    return 0;
}
int mouse_doing(void)
{
    m_event mevent;
    fd_set input_fd;
    int fd;
    char c,victory = -1;
    int w=0;
    int who,len,flag = 0;
    u32_t color = 0x00ffffff;
    int mx = 521;
    int my = 384;
    bzero(&board,sizeof(board));
    fd = open("/dev/input/mice",O_RDWR|O_NONBLOCK);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }
    draw_cursor(mx ,my);
    while(1)
    {
        FD_ZERO(&input_fd);
        FD_SET(server_sock,&input_fd);
        FD_SET(fd,&input_fd);
        if((select(fd+1,&input_fd,NULL,NULL,NULL)) < 0)
        {
            perror("Select error");
            continue;
        }
        
        if(FD_ISSET(server_sock,&input_fd))
        {
            restore_shape(mx,my);
            client_len = sizeof(client);
            len = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &client_len);
            if (len < 0)
            {
                close(server_sock);
                fprintf(stderr, "%s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            else
            {
                //printf("recvfrom client ok!\n");
                //in.s_addr = client.sin_addr.s_addr;
                //printf("client ip  : %s\n", inet_ntoa(in));
                //printf("client port: %d\n", ntohs(client.sin_port));
                //printf("\n");
                mx = (p->x)*30 + 100;
                my = (p->y)*30 +40;
               if( !check(mx,my) )
               {
                    color = ~color;
                    who = create_spot(mx,my,color,w);
                    save_shape(mx,my);
                    chess_count(mx,my,who);
                    victory = check_all();
                    w++;
                    draw_cursor(mx,my);
                    flag = 1;
               }
            }
        }
        if((FD_ISSET(fd,&input_fd))&&(get_m_info(fd,&mevent)>0))
        {
            restore_shape(mx,my);
            mx += mevent.dx;
            my += mevent.dy;

            mx = ((mx >= 0)? mx:0);
            my = ((my >= 0)? my:0);
            if(mx >= (fb_v.w-C_WIDTH))
                mx = (fb_v.w-C_WIDTH);
            if(my >= (fb_v.h-C_HEIGHT))
                my = (fb_v.h-C_HEIGHT);

            switch(mevent.button)
            {
                case 1:if(flag == 1)
                       {
                            if((mx > 75)&&(mx < 985)&&(my >25)&&(my < 745))
                            {
                                if( !check(mx,my) )
                                {
                                    color = ~color;
                                    who = create_spot(mx,my,color,w);
                                    save_shape(mx,my);
                                    chess_count(mx,my,who);
                                    victory = check_all();
                                    w++;
                                    sendto(server_sock, buffer, 12, 0, (struct sockaddr *)&client, client_len);
                                    flag = 0;
                                }
                            }
                        }
                        break;
                case 2: break;
                case 4: break;
                default :break;
           }
            draw_cursor(mx,my);
        }
        usleep(1000);
        if( victory == 1)
        {
            printf("play%d won!\n",who);
            printf("continue ? y/n\n");
            scanf("%c",&c);
            getchar();
            if( c=='n')
                break;
           else
            {
                create_scr_fb();
                print_board(100,40,0x008b6914);
                victory = -1;
                w=0;
                color = 0x00ffffff;
                mx = 521;
                my = 384;
                flag = 1;
                bzero(&board,sizeof(board)); 
                draw_cursor(mx,my);
            }
         }

    }
    close(fd);
    return 0;
}
