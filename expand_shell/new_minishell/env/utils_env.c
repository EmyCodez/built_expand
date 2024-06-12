/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lps <lps@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 14:34:10 by esimpson          #+#    #+#             */
/*   Updated: 2024/06/12 15:35:22 by lps              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_ptr(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
}
void	free_env_node(t_env *node)
{
	if (node->key)
		free_ptr(node->key);
	if (node->value)
		free_ptr(node->value);
	free_ptr(node);
}

void	free_env_list(t_env *env_list)
{
	t_env	*temp;
	t_env	*current;

	temp = env_list;
	while (temp)
	{
		current = temp->next;
		free_env_node(temp);
		temp = current;
	}
}

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*temp;

	temp = env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}
