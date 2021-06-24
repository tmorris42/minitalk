#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

typedef struct	s_msg
{
	int		pid;
	char	c;
	struct s_msg	*next;
}				t_msg;

typedef struct	s_conn
{
	int				pid;
	t_msg			*msg;
	struct s_conn	*next;
}				t_conn;

t_msg	*msg_new(int pid, char c);

t_conn	*CONNS = NULL; // perhaps use a static variable in handler function instead
// then handle SIGINT to go free everything
/* */

t_conn	*conn_new(int pid)
{
	t_conn	*conn;
	t_msg	*msg;

	conn = (t_conn*)malloc(sizeof(*conn));
	if (!conn)
		return (NULL);
	msg = msg_new(pid, 0);
	if (!msg)
	{
		free(conn);
		return (NULL);
	}
	conn->pid = pid;
	conn->msg = msg;
	conn->next = NULL;
	return (conn);

}

t_msg	*msg_new(int pid, char c)
{
	t_msg	*msg;

	msg = (t_msg *)malloc(sizeof(*msg));
	if (!msg)
		return (NULL);
	msg->pid = pid;
	msg->c = c;
	msg->next = NULL;
	return (msg);
}

t_msg	*msg_last(t_msg *list)
{
	while (list)
	{
		if (list && list->next)
			list = list->next;
		else
			return (list);
	}
	return (NULL);
}

t_msg	*msg_add_back(t_msg **list, t_msg *new)
{
	t_msg	*last;

	if (!list || !new)
		return (NULL);
	if (!(*list))
	{
		(*list) = new;
		return (new);
	}
	last = msg_last(*list);
	last->next = new;
	return (new);
}

t_msg	*msg_clear(t_msg **list)
{
	t_msg	*curr;
	t_msg	*to_delete;

	if (!list)
		return (NULL);
	curr = (*list);
	while (curr)
	{
		to_delete = curr;
		curr = curr->next;
		free(to_delete);
	}
	(*list) = NULL;
	return (NULL);
}

void	msg_print(t_msg *msg)
{
	while (msg && msg->c)
	{
		write(1, &msg->c, 1);
		msg = msg->next;
	}
	write(1, "\n", 1);
}

void	user1(int sig, siginfo_t *info, void *uap)
{
	t_msg	*last;
	int		client_id;

	client_id = (int)info->si_pid;
	if (!CONNS)
	{
		CONNS = conn_new(client_id);
		if (!CONNS)
		{
			printf("ERROR! connection could not be establish due to malloc failure");
			return ;
		}
		else if (CONNS && CONNS->msg)
		{
			printf("I got a new signal 1 and created a new connection!\nNow I'm going to sleep\n");
//			sleep(10);
			printf("Done sleeping!\n");
			CONNS->msg->c = 0;
		}
	}
	else if (CONNS->pid != client_id)
	{
//		printf("ERROR! Wrong client ID (got:%d but expected:%d)\n", client_id, CONNS->pid);
//		return ;
	}
//	else
//	{
//		printf("I got a new signal 1!\n");
		last = msg_last(CONNS->msg);
		last->c += 1;
//	}
	kill(client_id, SIGUSR1);
}

void	user2(int sig, siginfo_t *info, void *uap)
{
	char c;
	int		client_id;
	t_msg	*last;

	client_id = (int)info->si_pid;
//	if (CONNS && CONNS->pid == client_id)
	if (CONNS)
	{
		last = msg_last(CONNS->msg);
		if (last->c == 0)
		{
			printf("Got signal 2 on empty char, printing message\n");
			msg_print(CONNS->msg);
			msg_clear(&(CONNS->msg));
		}
		else
		{
			printf("Got signal 2 on char, adding next char slot\n");
			last->next = msg_new(client_id, 0);
			if (!(last->next))
				printf("ERROR, MALLOC FAILED TO CREATE NEW LINK\n"); //return error
		}
	}


	kill(client_id, SIGUSR2);
}

void	universal(int sig, siginfo_t *info, void *uap)
{
	static char c = 0;
	int	client_id;

	client_id = info->si_pid;
	if (info->si_signo == SIGINT)
	{
	}
	else if (info->si_signo == SIGUSR1)
	{
		c += 1;
	}
	else if (info->si_signo == SIGUSR2)
	{
		if (c)
		{
			write(1, &c, 1);
		}
		c = 0;
	}
	kill(client_id, SIGUSR1);
}



int	main(void)
{
	struct sigaction act1;
//	struct sigaction act2;

	int	pid;
//	int	looping;

	act1.sa_sigaction = &universal;
	act1.sa_flags = SA_SIGINFO;
	sigemptyset(&act1.sa_mask);
//	sigaddset(&act1.sa_mask, SIGINT);
	sigaddset(&act1.sa_mask, SIGUSR1);
	sigaddset(&act1.sa_mask, SIGUSR2);

//	act2.sa_sigaction = &user2;
//	act2.sa_flags = SA_SIGINFO;
//	sigemptyset(&act2.sa_mask);
//	sigaddset(&act2.sa_mask, SIGINT);
//	sigaddset(&act2.sa_mask, SIGUSR1);
//	sigaddset(&act2.sa_mask, SIGUSR2);

	
/*	if (sigaction(SIGUSR1, &act1, NULL) < 0)
	{
		printf("ERROR ENCOUNTERED with act1!\n");
		return (0);
	}
	if (sigaction(SIGUSR2, &act2, NULL) < 0)
	{
		printf("ERROR ENCOUNTERED with act2!\n");
		return (0);
	}
*/
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
