#include "minitalk.h"

void	msg_print(t_msg *msg)
{
	while (msg && msg->c)
	{
		write(1, &msg->c, 1);
		msg = msg->next;
	}
}
