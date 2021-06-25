/* 
#include <stdio.h> //
#include <stdlib.h> //
 */
#include <signal.h>
#include <unistd.h>

#define DELAY 50
int	g_confirmed = 0;

void	send_char(int pid, char c)
{
	char	sent;
	int		i;

	sent = 0;
	i = 1;
	while (i < 256)
	{
		if ((c & (0x1 * i)) > 0)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(DELAY);
		++sent;
		i *= 2;
		usleep(DELAY);
	}
}

void	send_msg(int pid, char *msg)
{
	int	i;

	i = 0;
	while (msg && msg[i])
	{
		send_char(pid, msg[i]);
		++i;
	}
	i = 0;
	while (i < 8)
	{
		kill(pid, SIGUSR2);
		usleep(DELAY);
		usleep(DELAY);
		++i;
	}
}

int	main(int argc, char **argv)
{
	int		pid;
	char	*msg;

	signal(SIGUSR1, SIG_IGN);
	signal(SIGUSR2, SIG_IGN);
	if (argc == 3)
	{
		pid = atoi(argv[1]);
		if (pid < 1)
			return (0);
		msg = argv[2];
	}
	else
		return (0);
	send_msg(pid, msg);
	return (0);
}
