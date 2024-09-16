/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarzouk <rmarzouk@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:35:35 by rmarzouk          #+#    #+#             */
/*   Updated: 2024/08/04 16:57:25 by rmarzouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	last_redir(t_redir *redir, int len, int type, int last_fd)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (type == REDIR_IN)
		{
			if (redir[i].type == REDIR_IN_FILE)
				last_fd = redir[i].fd;
			else if (redir[i].type == HERE_DOC_LIMITER)
				last_fd = redir[i].fd;
		}
		else if (type == REDIR_OUT)
		{
			if (redir[i].type == REDIR_OUT_FILE)
				last_fd = redir[i].fd;
			else if (redir[i].type == DREDIR_OUT_FILE)
				last_fd = redir[i].fd;
		}
		i++;
	}
	return (last_fd);
}

int	check_redir_type(t_simple_cmd *cmd, int i, int id)
{
	if (cmd->redirs[i].type == HERE_DOC_LIMITER)
	{
		cmd->redirs[i].fd = open(cmd->heredoc_arr[id], O_RDWR);
		unlink(cmd->heredoc_arr[id++]);
	}
	if (cmd->redirs[i].type == REDIR_IN_FILE)
		cmd->redirs[i].fd = open(cmd->redirs[i].path_or_limiter, O_RDWR);
	else if (cmd->redirs[i].type == REDIR_OUT_FILE)
		cmd->redirs[i].fd = open(cmd->redirs[i].path_or_limiter,
				O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (cmd->redirs[i].type == DREDIR_OUT_FILE)
		cmd->redirs[i].fd = open(cmd->redirs[i].path_or_limiter,
				O_RDWR | O_CREAT | O_APPEND, 0644);
	if (cmd->redirs[i].fd == -1)
	{
		perror(ft_strjoin(ft_strdup("minishell : "),
				ft_strdup(cmd->redirs[i].path_or_limiter)));
		exit(EXIT_FAILURE);
	}
	return (id);
}

int	handle_redirections(t_simple_cmd *cmd)
{
	int	i;
	int	id;

	i = 0;
	id = 0;
	while (i < cmd->redir_num)
	{
		id = check_redir_type(cmd, i, id);
		i++;
	}
	cmd->fd.in = last_redir(cmd->redirs, cmd->redir_num, REDIR_IN, cmd->fd.in);
	cmd->fd.out = last_redir(cmd->redirs, cmd->redir_num, REDIR_OUT,
			cmd->fd.out);
	return (0);
}

int	close_all_fds(t_simple_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_num)
	{
		if (cmd->redirs[i].fd != 0 && cmd->redirs[i].fd != 1)
			close(cmd->redirs[i].fd);
		i++;
	}
	return (0);
}

int	dup_and_close(t_simple_cmd *cmd)
{
	if (cmd->fd.in != 0)
		dup2(cmd->fd.in, STDIN_FILENO);
	if (cmd->fd.out != 1)
		dup2(cmd->fd.out, STDOUT_FILENO);
	if (cmd->pipe_flag == BEFORE_PIPE)
	{
		close(cmd->pipe[0]);
		close(cmd->pipe[1]);
	}
	else if (cmd->pipe_flag == AFTER_PIPE || cmd->pipe_flag == BETWEEN_PIPES)
	{
		close(cmd->prev->pipe[0]);
		close(cmd->prev->pipe[1]);
	}
	close_all_fds(cmd);
	return (0);
}
