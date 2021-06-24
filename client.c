#include <stdio.h> //
#include <stdlib.h> //
#include <signal.h>
#include <unistd.h>

const int	DELAY = 20;
int	g_confirmed = 0;

void	hold_on(void)
{
	int	i;

	i = 0;
	while (!g_confirmed && i < 100)
	{
		usleep(DELAY);
		++i;
	}
}

void	get_receipt()
{
	++g_confirmed;
}

void	send_char(int pid, char c)
{
	char	sent;

	sent = 0;
	g_confirmed = 0;
	while (sent != c)
	{
		kill(pid, SIGUSR1);
		usleep(DELAY);
		if (g_confirmed > 0)
		{
			--g_confirmed;
		}
		++sent;
		usleep(DELAY);
	}
	g_confirmed = 0;
	while (!g_confirmed)
	{
		kill(pid, SIGUSR2);
		usleep(DELAY);
		usleep(DELAY);
	}
	g_confirmed = 0;
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
}

int	main(int argc, char **argv)
{
	int	pid;
	char	sent;
	char	*msg;

	signal(SIGUSR1, get_receipt);
	signal(SIGUSR2, get_receipt);

	if (argc == 3)
	{
		pid = atoi(argv[1]);
		msg = argv[2];
	}
	else
		return (0);

	printf("My PID = %d\n", getpid());
	send_msg(pid, msg);
//	kill(pid, SIGUSR2);
//	pause();
	printf("Quitting\n");
	return (0);
}
