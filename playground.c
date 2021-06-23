#include <stdio.h> //
#include <stdlib.h> //
#include <signal.h>
#include <unistd.h>

void	get_receipt()
{
}

void	send_char(int pid, char c)
{
	char	sent;

	sent = 0;
	while (sent != c)
	{
		kill(pid, SIGUSR1);
		pause();
		++sent;
	}
	kill(pid, SIGUSR2);
	pause();
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
	kill(pid, SIGUSR2);
	pause();
	printf("Quitting\n");
	return (0);
}
