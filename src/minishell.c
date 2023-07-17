/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahallali <ahallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 21:56:47 by ahallali          #+#    #+#             */
/*   Updated: 2023/07/17 21:06:50 by ahallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*g_minishell;

int	main(int ac, char **av, char **env)
{
	char			*line;
	t_parse_utils	*p_prompt;

	(void)ac;
	(void)av;
	line = NULL;
	g_minishell = ft_calloc(1, sizeof(t_minishell));
	if (!g_minishell)
		return (0);
	if (*env)
		g_minishell->env = ft_env(env, g_minishell);
	else
		g_minishell->env = ft_empty();
	fill_export_env(&g_minishell->env);
	g_minishell->last_exitstatus = 0;
	g_minishell->home = get_home(g_minishell);
	while (1)
	{
		rl_catch_signals = 0;
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, (void *)handler);
		g_minishell->sigint_flag = 0;
		g_minishell->heredoc_flag = 0;
		line = readline("minishell>");
		if (!line)
		{
			ft_putstr_fd("exit\n", STDIN_FILENO);
			exit(0);
		}
		p_prompt = ft_calloc(1, sizeof(t_parse_utils));
		if (!p_prompt)
			return (1);
		p_prompt->prompt = ft_strdup(line);
		g_minishell->list_exec = parse_prompt(p_prompt->prompt, p_prompt);
		if (!g_minishell->sigint_flag && g_minishell->list_exec)
			execute(g_minishell);
		free(line);
	}
}
