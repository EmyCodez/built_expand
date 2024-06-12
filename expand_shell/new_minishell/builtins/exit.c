/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esimpson <esimpson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:29:45 by esimpson          #+#    #+#             */
/*   Updated: 2024/06/12 13:29:49 by esimpson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_str_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	determine_exit_code(char **argv, int *exit_code)
{
	if (argv[1] && is_str_digit(argv[1]))
		*exit_code = ft_atoi(argv[1]);
	if (argv[1] && !is_str_digit(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		*exit_code = 255;
	}
	if (argv[1] && argv[2] && is_str_digit(argv[1]))
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		*exit_code = 1;
	}
	else if (!argv[1])
		*exit_code = 0;
	exit(*exit_code);
}

void	ft_exit(char **argv, int *exit_code)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	determine_exit_code(argv, exit_code);
}
