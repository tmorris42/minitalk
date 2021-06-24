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
			printf("ERROR! connection could not be establish due to malloc failure");
		else if (CONNS && CONNS->msg)
		{
			printf("I got a new signal 1 and created a new connection!\n");
			CONNS->msg->c = 1;
		}
	}
	else if (CONNS->pid != client_id)
	{
		printf("ERROR! Wrong client ID (got:%d but expected:%d)\n", client_id, CONNS->pid);
		return ;
	}
	else
	{
		printf("I got a new signal 1!\n");
		last = msg_last(CONNS->msg);
		last->c += 1;
	}
	kill(client_id, SIGUSR1);
}

void	user2(int sig, siginfo_t *info, void *uap)
{
	char c;
	int		client_id;
	t_msg	*last;

	client_id = (int)info->si_pid;
	if (CONNS && CONNS->pid == client_id)
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
	while (1)
		pause();
	printf("I'm after my pause...\n");
	return (0);
}
