/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lps <lps@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 13:44:31 by esimpson          #+#    #+#             */
/*   Updated: 2024/06/12 14:45:00 by lps              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*new_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = NULL;
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

int	env_update_value(t_env *env_list, t_env *new_node)
{
	t_env	*temp;

	temp = env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, new_node->key) == 0)
		{
			if (new_node->value == NULL)
			{
				free_env_node(new_node);
				return (1);
			}
			free(temp->value);
			temp->value = ft_strdup(new_node->value);
			free_env_node(new_node);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

void	env_sort_insert(t_env **env_list, t_env *new_node)
{
	t_env	*temp;
	t_env	*current;

	temp = *env_list;
	current = NULL;
	if (env_update_value(*env_list, new_node))
		return ;
	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	if (ft_strcmp(new_node->key, (*env_list)->key) < 0)
	{
		new_node->next = *env_list;
		*env_list = new_node;
		return ;
	}
	while (temp && ft_strcmp(new_node->key, temp->key) > 0)
	{
		current = temp;
		temp = temp->next;
	}
	current->next = new_node;
	new_node->next = temp;
}

static void	add_to_env_list(char **envp, int index, t_env **env_list)
{
	char	*key;
	char	*value;
	char	*equal_loc;
	t_env	*new_node;

	equal_loc = ft_strchr(envp[index], '=');
	if (equal_loc)
	{
		key = ft_substr(envp[index], 0, equal_loc - envp[index]);
		value = ft_strdup(equal_loc + 1);
		if (ft_strcmp(key, "OLDPWD") == 0)
		{
			new_node = new_env_node(key, NULL);
			free(value);
		}
		else
			new_node = new_env_node(key, value);
	}
	else
		new_node = new_env_node(ft_strdup(envp[index]), NULL);
	env_sort_insert(env_list, new_node);
}

void	init_env_list(char **envp, t_env **env_list)
{
	int	index;

	index = 0;
	while (envp[index])
	{
		add_to_env_list(envp, index, env_list);
		index++;
	}
}
