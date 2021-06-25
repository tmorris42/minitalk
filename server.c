#include "minitalk.h"

static void	ft_putnbr(int n)
{
	char	c;

	if (n < 0)
		return ;
	if (n < 10)
	{
		c = n + '0';
		write(1, &c, 1);
		return ;
	}
	ft_putnbr(n / 10);
	ft_putnbr(n % 10);
}

void	universal(int sig, siginfo_t *info, void *uap)
{
	static char	c = 0;
	static int	i = 1;

	if (!uap)
		return ;
	if (sig == SIGUSR1)
		c = (c | i);
	i = i << 1;
	if (i > 255)
	{
		if (c)
			write(1, &c, 1);
		c = 0;
		i = 1;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	act1;
	int					pid;

	act1.sa_sigaction = &universal;
	act1.sa_flags = SA_SIGINFO;
	sigemptyset(&act1.sa_mask);
	sigaddset(&act1.sa_mask, SIGUSR1);
	sigaddset(&act1.sa_mask, SIGUSR2);
	if (sigaction(SIGUSR1, &act1, NULL) < 0)
		return (1);
	if (sigaction(SIGUSR2, &act1, NULL) < 0)
		return (1);
	pid = getpid();
	if (pid < 1)
		return (-1);
	ft_putnbr(pid);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
