/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lps <lps@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:24:29 by esimpson          #+#    #+#             */
/*   Updated: 2024/06/12 15:59:27 by lps              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_env(t_minishell *myshell)
{
	t_env	*temp;

	temp = myshell->envlst;
	while (temp)
	{
		if (temp->value)
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
  return (ENO_SUCCESS);
}
