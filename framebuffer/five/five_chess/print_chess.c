#include <stdio.h>

#include "main.h"

extern u32_t color_choice;
extern char who;

int choose_color(int x, int y)
{
	if((x>=20) && (x<=60))	
	{
		if((y>=40) && (y<=80))
		{
			color_choice = 0x000000ff;
			who = 1;
		}
		else if((y>=120) && (y<=160))
		{
			color_choice = 0x00ff0000;
			who = 2;
		}
	}
	return 0;	
}

int print_one_chess(int x, int y)
{
	int c_x = x;
	int c_y = y;

	if((x < 100) || (x > (30*29+100)))
	{
		choose_color(x, y);
		return 1;	
	}
	if((y < 40) || (y > (30*23+40)))
	{
		choose_color(x, y);
		return 1;
	}

	x = (x-100)%30;
	y = (y-40)%30;

	c_x += ((x>15) ? (30-x) : -x);
	c_y += ((y>15) ? (30-y) : -y);

	fb_circle(c_x, c_y, 13, color_choice);

	return 0;
}
