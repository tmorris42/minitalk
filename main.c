#include <stdio.h>
#include <unistd.h>
#include <signal.h>

char	MSG = 0;
int		CLIENT_ID = 0;
int		LOOPING = 2;

void	user1(int sig, siginfo_t *info, void *uap)
{
	LOOPING = 2;
	if (CLIENT_ID != (int)info->si_pid)
	{
		CLIENT_ID = (int)info->si_pid;
		printf("CLIENT #%d: ", CLIENT_ID);
	}
	MSG += 1;
	kill(CLIENT_ID, SIGUSR1);
}

void	user2(int sig, siginfo_t *info, void *uap)
{
	char c;
	c = MSG;
	MSG = 0;
//	printf("%d\n", c);
	write(1, &c, 1);
	--LOOPING; // uncomment this to terminate after 1 message
	if (!LOOPING)
	{
		LOOPING = 2;
		write(1, "\n", 1);
	}
	kill(CLIENT_ID, SIGUSR2);
}



int	main(void)
{
	struct sigaction act1;
	struct sigaction act2;

	int	pid;
	int	looping;

	act1.sa_sigaction = &user1;
	act1.sa_flags = SA_SIGINFO;
	act2.sa_sigaction = &user2;
	act2.sa_flags = SA_SIGINFO;

	
	if (sigaction(SIGUSR1, &act1, NULL) < 0)
	{
		printf("ERROR ENCOUNTERED with act1!\n");
		return (0);
	}
	if (sigaction(SIGUSR2, &act2, NULL) < 0)
	{
		printf("ERROR ENCOUNTERED with act2!\n");
		return (0);
	}

	pid = getpid();
	printf("My PID is %d\n", pid);
	while (LOOPING)
		pause();
	printf("I'm after my pause...\n");
	return (0);
}
