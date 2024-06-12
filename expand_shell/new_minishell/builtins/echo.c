/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lps <lps@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:22:04 by esimpson          #+#    #+#             */
/*   Updated: 2024/06/12 15:45:53 by lps              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	flag_nl(char *flag)
{
	int	i;

	i = 0;
	if (ft_strlen(flag) <= 1)
		return (0);
	if (flag[0] != '-')
		return (0);
	while (flag[++i])
		if (flag[i] != 'n')
			return (0);
	return (1);
}

int	ft_echo(char **av)
{
	int	flag;

	if (av[1])
		flag = flag_nl(av[1]);
	else
	{
		flag = 0;
		printf("\n");
	}
	while (av[++flag])
	{
		printf("%s", av[flag]);
		if (av[flag + 1])
			printf(" ");
	}
	if (av[1] && !flag_nl(av[1]))
		printf("\n");
	return (0);
}
