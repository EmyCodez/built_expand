/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esimpson <esimpson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:13:45 by abelayad          #+#    #+#             */
/*   Updated: 2024/06/12 17:50:42 by esimpson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_path	ft_get_env_path(char *path, char *cmd)
{
	size_t	i;
	t_err	err;
	char	*cmd_path;
	char	**split_path;

	split_path = ft_split(path, ':');
	i = 0;
	while (split_path[i])
	{
		cmd_path = ft_garbage_collector(ft_strjoin_with_f(
											ft_strdup(split_path[i]),
												ft_strdup(cmd), '/'),
										false);
		err = ft_check_exec(cmd_path, true);
		if (err.no == ENO_SUCCESS)
			return ((t_path){(t_err){ENO_SUCCESS, 42, cmd_path}, cmd_path});
		i++;
	}
	ft_free_char2(split_path);
	return ((t_path){(t_err){ENO_NOT_FOUND, ERRMSG_CMD_NOT_FOUND, cmd}, NULL});
}

t_path	ft_get_path(char *cmd, t_env *env_list)
{
	char	*value;

	if (*cmd == '\0')
		return ((t_path){
			(t_err){ENO_NOT_FOUND, ERRMSG_CMD_NOT_FOUND, cmd}, NULL});
	if (ft_strnstr(cmd, "/", ft_strlen(cmd)))
		return ((t_path){ft_check_exec(cmd, false), cmd});
	value = get_env_value("PATH", env_list);
	if (value)
		return (ft_get_env_path(value, cmd));
	return ((t_path){(t_err){ENO_NOT_FOUND, ERRMSG_NO_SUCH_FILE, cmd}, NULL});
}
