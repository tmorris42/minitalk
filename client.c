#include "minitalk.h"

static int	ft_atoi(char *msg)
{
	unsigned int	nbr;
	int				i;

	nbr = 0;
	i = 0;
	while (msg && msg[i])
	{
		if (msg[i] < '0' || msg[i] > '9')
			return (-1);
		nbr = nbr * 10;
		nbr += (msg[i] - '0');
		++i;
	}
	i = (int)nbr;
	if ((unsigned int)i != nbr)
		return (-1);
	return (i);
}

void	send_char(pid_t pid, char c)
{
	int		i;

	i = 1;
	while (i < 256)
	{
		if ((c & i) > 0)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(DELAY);
		i *= 2;
		usleep(DELAY);
	}
}

void	send_msg(pid_t pid, char *msg)
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
	pid_t	pid;
	char	*msg;

	signal(SIGUSR1, SIG_IGN);
	signal(SIGUSR2, SIG_IGN);
	if (argc == 3)
	{
		pid = ft_atoi(argv[1]);
		if (pid < 1)
			return (1);
		msg = argv[2];
	}
	else
		return (1);
	send_msg(pid, msg);
	return (0);
}
