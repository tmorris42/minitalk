#include "minitalk.h"

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
