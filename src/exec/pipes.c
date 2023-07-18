/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichaiq <ichaiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:59:44 by ahallali          #+#    #+#             */
/*   Updated: 2023/07/18 20:40:16 by ichaiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(t_minishell *minishell)
{
	if (ft_strequals(expand_dquotes(minishell->list->cmd), "cd") || \
		(ft_strequals(expand_dquotes(minishell->list->cmd), "env") && \
		ft_lstsize(minishell->list->args) == 0) || \
		ft_strequals(expand_dquotes(minishell->list->cmd), "pwd") || \
		ft_strequals(expand_dquotes(minishell->list->cmd), "exit") || \
		(ft_strequals(expand_dquotes(minishell->list->cmd), "unset") && \
			ft_lstsize(minishell->list->args)) || \
		(ft_strequals(expand_dquotes(minishell->list->cmd), "echo")) || \
		(ft_strequals(expand_dquotes(minishell->list->cmd), "export")))
		return (1);
	return (0);
}

void	ft_exit(t_minishell *minishell, char **cmd)
{
	if (!minishell->list->cmd || !*minishell->list->cmd)
		return ;
	if (!cmd || !*cmd)
		do_clean_exit(NULL, 1, 0, 0);
	else if (cmd[0] && !check_cmd_num(cmd[0]) && !cmd[1])
	{
		ft_putstr_fd("minishell>:exit:numeric argument required\n", 2);
		do_clean_exit(NULL, 1, 255, 0);
	}
	else if (!cmd[1] && cmd[0] && check_cmd_num(cmd[0]))
		do_clean_exit(NULL, 1, ft_atoi(cmd[0]), 0);
	else
		ft_putstr_fd("minishell>: exit: too many arguments\n", 2);
}

int	check_cmd_num(char *cmd)
{
	int	i;

	i = 0;
	if (!cmd || !*cmd)
		return (0);
	while (cmd[i])
	{
		if (!ft_isdigit(cmd[i]))
			return (0);
		i++;
	}
	return (1);
}
