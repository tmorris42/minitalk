#ifndef MINITALK_H
# define MINITALK_H
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>

# ifndef DELAY
#  define DELAY 185
# endif

typedef struct s_msg
{
	int				pid;
	char			c;
	struct s_msg	*next;
}				t_msg;

typedef struct s_conn
{
	int				pid;
	t_msg			*msg;
	struct s_conn	*next;
}				t_conn;

t_msg	*msg_new(int pid, char c);
t_msg	*msg_last(t_msg *list);
t_msg	*msg_add_back(t_msg **list, t_msg *new);
t_msg	*msg_new_add_back(t_msg **list, int pid, char c);
t_msg	*msg_clear(t_msg **list);
void	msg_print(t_msg *msg);

#endif
