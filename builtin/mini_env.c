/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarzouk <rmarzouk@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 17:29:16 by rmarzouk          #+#    #+#             */
/*   Updated: 2024/08/05 15:33:55 by rmarzouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

extern int	g_exit_status;

bool	get_key_and_value(char *env, char **key, char **value)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	*key = ft_substr(env, 0, i);
	*value = ft_strdup(env + i + 1);
	if (!ft_strchr(env, '='))
		return (false);
	return (true);
}

t_env	*env_list(char **env)
{
	int		i;
	t_env	*head;
	t_env	*tmp;
	char	*key;
	char	*value;

	if (!env || !*env)
		return (NULL);
	get_key_and_value(env[0], &key, &value);
	head = env_new_node(key, value);
	head->value_falg = true;
	i = 0;
	while (env[++i])
	{
		get_key_and_value(env[i], &key, &value);
		tmp = env_new_node(key, value);
		tmp->value_falg = true;
		env_add_back(&head, tmp);
	}
	return (head);
}

int	mini_env(t_data *data)
{
	t_env	*tmp;

	if (!data->env_l)
	{
		g_exit_status = EXIT_SUCCESS;
		return (g_exit_status);
	}
	change_env_value(data->env_l, "_", "builtin_mini_env", true);
	tmp = data->env_l;
	while (tmp)
	{
		if (tmp->value_falg)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	g_exit_status = EXIT_SUCCESS;
	return (g_exit_status);
}
