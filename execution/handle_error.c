/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarzouk <rmarzouk@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 10:36:05 by mskhairi          #+#    #+#             */
/*   Updated: 2024/08/05 13:14:40 by rmarzouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

extern int	g_exit_status;

int	handle_pipes(t_simple_cmd *cmd)
{
	if (cmd->pipe_flag == AFTER_PIPE)
	{
		cmd->fd.in = cmd->prev->pipe[0];
	}
	else if (cmd->pipe_flag == BEFORE_PIPE)
	{
		pipe(cmd->pipe);
		cmd->fd.out = cmd->pipe[1];
	}
	else if (cmd->pipe_flag == BETWEEN_PIPES)
	{
		pipe(cmd->pipe);
		cmd->fd.in = cmd->prev->pipe[0];
		cmd->fd.out = cmd->pipe[1];
	}
	return (0);
}

void	print_error(char *cmd, char *error)
{
	ft_putstr_fd("minishell : ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(error, 2);
}

void	commun_errors(char *cmd, struct stat cmd_info)
{
	if (S_ISDIR(cmd_info.st_mode))
	{
		print_error(cmd, ": is a directory\n");
		exit(126);
	}
	else if (!(cmd_info.st_mode & S_IXUSR))
	{
		print_error(cmd, ": Permission denied\n");
		exit(126);
	}
	else
	{
		print_error(cmd, " command not found\n");
		exit(127);
	}
}

void	handle_errors(char *cmd, bool is_path)
{
	struct stat	cmd_info;
	int			flag;

	flag = stat(cmd, &cmd_info);
	if (!is_path)
	{
		print_error(cmd, ": command not found\n");
		exit(127);
	}
	if (flag != -1)
		commun_errors(cmd, cmd_info);
	else
	{
		if (errno == ENOENT)
		{
			print_error(cmd, ": No such file or directory\n");
			exit(127);
		}
		else
		{
			perror(cmd);
			exit(126);
		}
	}
}
