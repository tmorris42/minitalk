#ifndef MINITALK_H
# define MINITALK_H
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>

# ifndef DELAY
#  define DELAY 0
# endif

typedef struct s_msg
{
	char			c;
	struct s_msg	*next;
}				t_msg;

t_msg	*msg_new(char c);
t_msg	*msg_last(t_msg *list);
t_msg	*msg_add_back(t_msg **list, t_msg *new);
t_msg	*msg_new_add_back(t_msg **list, char c);
t_msg	*msg_clear(t_msg **list);
void	msg_print(t_msg *msg);

#endif
