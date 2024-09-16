/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarzouk <rmarzouk@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 17:59:01 by rmarzouk          #+#    #+#             */
/*   Updated: 2024/08/05 15:34:50 by rmarzouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

t_env	*env_last(t_env *node)
{
	if (!node)
		return (NULL);
	while (node->next != NULL)
		node = node->next;
	return (node);
}

t_env	*env_new_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	env_add_back(t_env **head, t_env *new)
{
	t_env	*last;

	last = *head;
	if (head && new)
	{
		if (!*head)
			*head = new;
		else
		{
			last = env_last(*head);
			last->next = new;
			new->prev = last;
		}
	}
}

void	env_delone(t_env **head, t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		if (!node->prev && !node->next)
		{
			free(node);
			*head = NULL;
			return ;
		}
		if (!node->prev)
		{
			*head = node->next;
			node->next->prev = NULL;
		}
		else if (!node->next)
			node->prev->next = NULL;
		else
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	free(node);
}
