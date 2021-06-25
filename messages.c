#include "minitalk.h"

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

