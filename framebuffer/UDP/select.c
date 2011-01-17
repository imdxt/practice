
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

int main(void)
{
	char buf[32];
	int fd,n;
	fd_set input_fd;

	fd = open("/dev/input/mice", O_RDWR);
	if(fd < 0)
	{
		fprintf(stderr, "Error mouse open:%s\n", strerror(errno));
		exit(1);
	}

	while(1)
	{
		FD_ZERO(&input_fd);
		FD_SET(STDIN_FILENO, &input_fd);
		FD_SET(fd, &input_fd);

		// which device is ready ?
		if((select(fd+1, &input_fd, NULL, NULL, NULL)) < 0)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			continue;
		}

		// mouse is ready ?
		if(FD_ISSET(fd, &input_fd))
		{
			n = read(fd, buf, sizeof(buf));
			if(n > 0)
			{
				printf("read data from mouse: ");
				printf("%x, %d, %d\n", (buf[0]&0x07), buf[1], buf[2]);
				printf("\n");
			}
		}

		// keyboard is ready ?
		if(FD_ISSET(STDIN_FILENO, &input_fd))
		{
			n = read(STDOUT_FILENO, buf, sizeof(buf));
			if(n > 0)
			{
				if(buf[0] == '.') break;

				write(STDOUT_FILENO, ">  ", 3);
			}
		}
	}

	close(fd);

	return 0;
}
