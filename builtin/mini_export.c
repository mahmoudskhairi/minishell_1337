/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarzouk <rmarzouk@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:31:02 by rmarzouk          #+#    #+#             */
/*   Updated: 2024/08/05 15:28:21 by rmarzouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

extern int	g_exit_status;

void	print_env(t_env *env_l)
{
	while (env_l)
	{
		if (env_l->value_falg)
			printf("declare -x %s=\"%s\"\n", env_l->key, env_l->value);
		else
			printf("declare -x %s\n", env_l->key);
		env_l = env_l->next;
	}
}

int	is_exist(t_data *data, char *key, char *value, bool value_flag)
{
	t_env	*tmp;

	tmp = data->env_l;
	while (tmp)
	{
		if (ft_strlen(key) == 0)
		{
			tmp = tmp->next;
			continue ;
		}
		else if (!ft_strcmp(tmp->key, key))
		{
			if (value_flag)
			{
				change_env_value(tmp, key, value, value_flag);
				tmp->value_falg = value_flag;
			}
			free(key);
			free(value);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	check_key(char *arg, bool *flag)
{
	int	i;

	i = 0;
	if (!arg || arg[i] == '\0')
	{
		*flag = true;
		return (1);
	}
	if (arg[i] != '_' && !ft_isalpha(arg[i]))
	{
		*flag = true;
		return (1);
	}
	while (arg[++i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[i] != '=')
		{
			*flag = true;
			return (1);
		}
	}
	return (0);
}

void	check_exit_status(bool flag)
{
	if (flag == 0)
		g_exit_status = EXIT_SUCCESS;
	else
		g_exit_status = EXIT_FAILURE;
}

int	mini_export(t_simple_cmd *export, t_data *data, bool flag, bool value_flag)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*new;

	i = 0;
	if (!export->cmd[1])
		print_env(data->env_l);
	while (export->cmd[++i])
	{
		value_flag = get_key_and_value(export->cmd[i], &key, &value);
		if (is_exist(data, key, value, value_flag))
			continue ;
		if (check_key(key, &flag) && flag == 1)
		{
			ft_printf_error("minishell: `%s': not a valid identifier\n", key);
			continue ;
		}
		new = env_new_node(key, value);
		new->value_falg = value_flag;
		env_add_back(&data->env_l, new);
	}
	check_exit_status(flag);
	return (g_exit_status);
}
