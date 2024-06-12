/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emilin <emilin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 01:28:41 by abelayad          #+#    #+#             */
/*   Updated: 2024/06/12 14:59:14 by emilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_minishell(char **env, t_minishell *myshell)
{
	ft_memset(myshell, 0, sizeof(t_minishell));
	myshell->environ = env;
	ft_init_envlst(myshell);
	myshell->stdin = dup(0);
	myshell->stdout = dup(1);
	tcgetattr(STDIN_FILENO, &myshell->original_term);
}

static void	ft_start_execution(t_minishell *myshell)
{
	signal(SIGQUIT, ft_sigquit_handler);
	ft_init_tree(myshell->ast);
	if (myshell->heredoc_sigint)
	{
		ft_clear_ast(&myshell->ast);
		myshell->heredoc_sigint = false;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &myshell->original_term);
	myshell->exit_s = ft_exec_node(myshell->ast, false);
	ft_clear_ast(&myshell->ast);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	myshell;
	
	((void)argc, (void)argv);
	ft_init_minishell(env, &myshell);
	while (1)
	{
		ft_init_signals(&myshell);
		myshell.line = readline(PROMPT_MSG);
		if (!myshell.line)
			(ft_clean_ms(),
				ft_putstr_fd("exit\n", 1), exit(myshell.exit_s));
		if (myshell.line[0])
			add_history(myshell.line);
		myshell.tokens = ft_tokenize(&myshell.line);
		if (!myshell.tokens)
			continue ;
		myshell.ast = ft_parse(&myshell->ast);
		if (myshell.parse_err.type)
		{
			ft_handle_parse_err();
			continue ;
		}
		ft_start_execution(&myshell);
	}
	ft_garbage_collector(NULL, true);
	return (ft_clean_ms(), myshell.exit_s);
}
