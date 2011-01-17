#include <stdio.h>
#include "main.h"

#define		STARTING_X	100
#define		STARTING_Y	40
#define		SPACE		30

void print_board(void)
{
	int i = 0;

	for(i=0; i<P_NUM; i++)
	{
		fb_line(STARTING_X, STARTING_Y+i*SPACE, STARTING_X+(V_NUM-1)*SPACE, STARTING_Y+i*SPACE, 0x0000ff00);
	}
	for(i=0; i<V_NUM; i++)
	{
		fb_line(STARTING_X+i*SPACE, STARTING_Y, STARTING_X+i*SPACE, STARTING_Y+(P_NUM-1)*SPACE, 0x0000ff00);
	}
}
