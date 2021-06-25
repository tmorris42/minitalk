#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void	universal(int sig, siginfo_t *info, void *uap)
{
	static char	c = 0;
	static int	i = 1;

	if (sig == SIGUSR1)
	{
		c = (c | (0x1 * i));
		i *= 2;
	}
	else if (sig == SIGUSR2)
	{
		c = (c | (0x0 * i));
		i *= 2;
	}
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
	{
		printf("ERROR ENCOUNTERED with act1!\n");
		return (0);
	}
	if (sigaction(SIGUSR2, &act1, NULL) < 0)
	{
		printf("ERROR ENCOUNTERED with act2!\n");
		return (0);
	}
	pid = getpid();
	printf("My PID is %d\n", pid);
	while (1)
		pause();
	printf("I'm after my pause...\n");
	return (0);
}
