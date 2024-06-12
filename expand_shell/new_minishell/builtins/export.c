/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lps <lps@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 13:21:40 by emilin            #+#    #+#             */
/*   Updated: 2024/06/12 16:10:29 by lps              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_env(t_env *env_list)
{
	t_env	*temp;

	temp = env_list;
	while (temp)
	{
		if (temp->value)
			printf("declare -x %s=%s\n", temp->key, temp->value);
		else
			printf("declare -x %s\n", temp->key);
		temp = temp->next;
	}
}

void	insert_export_var(char *key, t_env **env_list, char *equal_loc)
{
	t_env	*new_node;

	if (!equal_loc)
		new_node = new_env_node(key, NULL);
	else
		new_node = new_env_node(key, ft_strdup(equal_loc + 1));
	env_sort_insert(env_list, new_node);
}

char	*extract_key(char *str, char *equal_loc)
{
	char	*key;

	if (!equal_loc)
		key = ft_strdup(str);
	else
		key = ft_substr(str, 0, equal_loc - str);
	return (key);
}

int	is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (0);
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
	}
	return (1);
}

int	ft_export(char **argv, t_env **env_list, int *exit_status)
{
	int		i;
	char	*key;
	char	*equal_loc;

	if (!argv[1])
		return(print_env(*env_list),0);
	i = 0;
	while (argv[++i])
	{
		equal_loc = ft_strchr(argv[i], '=');
		key = extract_key(argv[i], equal_loc);
		if (!is_valid_key(key))
		{
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("' : not a valid identifier\n", 2);
			free(key);
			*exit_status = 1;
			return (*exit_status);
		}
		else
			insert_export_var(key, env_list, equal_loc);
	}
	return (0);
}
