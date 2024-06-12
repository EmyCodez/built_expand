/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esimpson <esimpson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:07:56 by abelayad          #+#    #+#             */
/*   Updated: 2024/06/12 17:42:29 by esimpson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void
ft_exec_pipe_child(t_node *node, int pfds[2], t_ast_direction direction,
		t_minishell *myshell)
{
	int	status;

	if (direction == TD_LEFT)
	{
		close(pfds[0]);
		dup2(pfds[1], STDOUT_FILENO);
		close(pfds[1]);
	}
	else if (direction == TD_RIGHT)
	{
		close(pfds[1]);
		dup2(pfds[0], STDIN_FILENO);
		close(pfds[0]);
	}
	status = ft_exec_node(node, true, myshell);
	(ft_clean_ms(myshell), exit(status));
}

int	ft_get_exit_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

static int	ft_exec_pipeline(t_node *tree, t_minishell *myshell)
{
	int	status;
	int	pfds[2];
	int	pid_l;
	int	pid_r;

	myshell->signint_child = true;
	pipe(pfds);
	pid_l = fork();
	if (!pid_l)
		ft_exec_pipe_child(tree->left, pfds, TD_LEFT, myshell);
	else
	{
		pid_r = fork();
		if (!pid_r)
			ft_exec_pipe_child(tree->right, pfds, TD_RIGHT, myshell);
		else
		{
			(close(pfds[0]), close(pfds[1]), waitpid(pid_l, &status, 0),
					waitpid(pid_r, &status, 0));
			myshell->signint_child = false;
			return (ft_get_exit_status(status));
		}
	}
	return (ENO_GENERAL);
}

int	ft_exec_node(t_node *tree, bool piped, t_minishell *myshell)
{
	if (!tree)
		return (1);
	if (tree->type == N_PIPE)
		return (ft_exec_pipeline(tree, myshell));
	else
		return (ft_exec_simple_cmd(tree, piped, myshell));
	return (ENO_GENERAL);
}
