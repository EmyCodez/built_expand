/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lps <lps@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:33:33 by emilin            #+#    #+#             */
/*   Updated: 2024/06/12 15:57:11 by lps              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	change_to_home(int *exit_code, t_env *env_list)
{
	char	*dir_home;

	dir_home = get_env_value(env_list, "HOME");
	if (dir_home)
	{
		if (chdir(dir_home))
		{
			perror("cd");
			*exit_code = 1;
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		*exit_code = 1;
	}
}

static void	update_pwd(t_env *env_list)
{
	char	*pwd;
	t_env	*temp;
	t_env	*old_pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("minishell: cd: error retrieving current directory",
						STDERR_FILENO);
		return ;
	}
	temp = env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, "PWD") == 0)
		{
			old_pwd = new_env_node(ft_strdup("OLDPWD"), temp->value);
			env_sort_insert(&env_list, old_pwd);
			temp->value = ft_strdup(pwd);
			free(pwd);
			return ;
		}
		temp = temp->next;
	}
}

static void	check_old_pwd(t_env *env_list, int *exit_code)
{
	char	*old_pwd;

	old_pwd = get_env_value(env_list, "OLDPWD");
	if (chdir(old_pwd) != 0)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		*exit_code = 1;
	}
	else
	{
		ft_putendl_fd(old_pwd, 1);
		update_pwd(env_list);
	}
}

static void	check_path(t_env *env_list, char **argv, int *exit_code)
{
	char	*path;

	path = ft_strjoin(get_env_value(env_list, "HOME"), argv[1] + 1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		*exit_code = 1;
	}
	else
		update_pwd(env_list);
	free(path);
}

int	ft_cd(char **argv, int *exit_code, t_env *env_list)
{
	if (!argv[1])
	{
		change_to_home(exit_code, env_list);
		update_pwd(env_list);
		return (0);
	}
	if (ft_strcmp(argv[1], "-") == 0)
		check_old_pwd(env_list, exit_code);
	else if (ft_strcmp(argv[1], "~") == 0)
		check_path(env_list, argv, exit_code);
	else if (chdir(argv[1]) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		*exit_code = 1;
	}
	else
		update_pwd(env_list);
	return (0);
}
