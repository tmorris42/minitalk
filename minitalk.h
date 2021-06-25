#ifndef MINITALK_H
# define MINITALK_H
# include <unistd.h>
# include <signal.h>

# ifndef DELAY
#  define DELAY 1000
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

#endif
