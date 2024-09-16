/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleaner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskhairi <mskhairi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:08:33 by rmarzouk          #+#    #+#             */
/*   Updated: 2024/08/04 16:56:56 by mskhairi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	ft_free_redir(t_redir *redir, int num)
{
	while (num--)
		free(redir[num].path_or_limiter);
	free(redir);
}

void	del_cmd_node(t_simple_cmd *lst)
{
	if (lst)
	{
		free(lst->cmd_name);
		ft_free(lst->cmd);
		ft_free_redir(lst->redirs, lst->redir_num);
		free(lst->heredoc_arr);
		free(lst);
	}
}

void	ft_clear_cmd_lst(t_simple_cmd **lst)
{
	t_simple_cmd	*current;
	t_simple_cmd	*tmp;

	current = *lst;
	if (lst)
	{
		while (current)
		{
			tmp = current;
			current = current->next;
			del_cmd_node(tmp);
		}
		*lst = NULL;
	}
}

void	ft_clear_limits(t_cmd_limits **lst)
{
	t_cmd_limits	*current;
	t_cmd_limits	*tmp;

	current = *lst;
	if (lst)
	{
		while (current)
		{
			tmp = current;
			current = current->next;
			free(tmp);
		}
		*lst = NULL;
	}
}
