/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarzouk <rmarzouk@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:25:36 by rmarzouk          #+#    #+#             */
/*   Updated: 2024/08/04 17:26:02 by rmarzouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../lexer/lexer.h"
# include "../minishell.h"

t_cmd_limits	*new_limit(t_item *start);
t_cmd_limits	*last_limit(t_cmd_limits *lst);
t_cmd_limits	*set_cmd_limits(t_item *head);
t_item			*skip_other_types(t_item *tmp);
void			expander(t_env *env_l, t_item *list, char *tmp);

t_simple_cmd	*new_cmd_node(t_cmd_limits *cmd);
int				cmd_number(t_cmd_limits *cmd);
char			**set_cmd_arr(t_cmd_limits *cmd);
int				check_redir(t_cmd_limits *cmd);
t_redir			*set_redirs(t_cmd_limits *cmd, int num);
void			set_pipe_flag(t_simple_cmd *head, int cmd_nbr);
void			add_back_simple_cmd(t_simple_cmd **lst, t_simple_cmd *new);
t_simple_cmd	*last_cmd(t_simple_cmd *lst);

#endif
