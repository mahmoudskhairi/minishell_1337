/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarzouk <rmarzouk@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:05:09 by rmarzouk          #+#    #+#             */
/*   Updated: 2024/08/05 15:35:17 by rmarzouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

extern int	g_exit_status;

char	*cmd_exist(char *cmd, char *cmd_name, char **path)
{
	bool	is_path;
	char	*full_path;

	is_path = false;
	if (cmd && !ft_strchr(cmd, '/'))
	{
		full_path = check_full_path(cmd, path);
		if (full_path)
			return (full_path);
	}
	else
	{
		is_path = true;
		if (is_valid(cmd) == 1)
			return (cmd);
	}
	handle_errors(cmd_name, is_path);
	return (NULL);
}

int	_execute(t_simple_cmd *cmd, t_data *data)
{
	char	*error_str;
	char	**path;
	char	**env;

	error_str = NULL;
	env = list_to_arr(data->env_l);
	path = get_path_env(data->env_l);
	if (!cmd->cmd[0])
		exit(0);
	cmd->cmd[0] = cmd_exist(cmd->cmd[0], cmd->cmd_name, path);
	dup_and_close(cmd);
	execve(cmd->cmd[0], cmd->cmd, env);
	exit(g_exit_status);
}

int	handle_cmd(t_simple_cmd *cmd, t_data *data, int *fork_pid)
{
	int	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (!pid)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		handle_redirections(cmd);
		if (check_builtin(cmd->cmd_name))
			builtin_cmd(cmd, data, check_builtin(cmd->cmd_name), false);
		else
			_execute(cmd, data);
	}
	else
	{
		*fork_pid = pid;
		if (cmd->pipe_flag == AFTER_PIPE || cmd->pipe_flag == BETWEEN_PIPES)
		{
			close(cmd->prev->pipe[0]);
			close(cmd->prev->pipe[1]);
		}
	}
	return (0);
}

void	cmd_signals(t_data *data, int *state)
{
	int	i;

	i = 0;
	while (waitpid(data->fork_pid[i++], state, 0) > 0 && i < data->cmd_nbr)
		;
	if (WIFSIGNALED(*state))
		g_exit_status = WTERMSIG(*state) + 128;
	else if (WIFEXITED(*state))
		g_exit_status = WEXITSTATUS(*state);
	signal(SIGINT, handle_sigint);
}

int	execute_cmd(t_simple_cmd *cmd, t_data *data)
{
	int		i;
	int		state;
	bool	flag;

	i = 0;
	flag = false;
	data->fork_pid = malloc(data->cmd_nbr * sizeof(int));
	if (handle_all_heredocs(cmd, &state))
		return (g_exit_status = EXIT_FAILURE, 1);
	if (!cmd->next && check_builtin(cmd->cmd_name))
	{
		flag = true;
		handle_redirections(cmd);
		builtin_cmd(cmd, data, check_builtin(cmd->cmd_name), flag);
		return (0);
	}
	while (cmd)
	{
		handle_pipes(cmd);
		handle_cmd(cmd, data, &data->fork_pid[i++]);
		cmd = cmd->next;
	}
	cmd_signals(data, &state);
	return (0);
}
