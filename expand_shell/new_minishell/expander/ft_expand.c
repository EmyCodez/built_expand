/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emilin <emilin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:49:30 by emilin            #+#    #+#             */
/*   Updated: 2024/06/12 15:49:35 by emilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_handle_dollar(char *str, size_t *i, int *exit_code, t_env **env_list)
{
	size_t	start;
	char	*var;
	char	*env_val;

	(*i)++;
	if (ft_isdigit(str[*i]) || str[*i] == '@')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	else if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(*exit_code));
	}
	else if (!ft_is_valid_var_char(str[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (ft_is_valid_var_char(str[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	env_val = ft_get_envlst_val(var);
	if (!env_val)
		return (free(var), ft_strdup(""));
	return (free(var), ft_strdup(env_val));
}

static char	*ft_cmd_pre_expander(char *str, int *exit_code, t_env **env_list)
{
	char	*ret;
	size_t	i;

	ret = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			ret = ft_strjoin_f(ret, ft_handle_squotes(str, &i));
		else if (str[i] == '"')
			ret = ft_strjoin_f(ret, ft_handle_dquotes(str, &i, exit_code, env_list));
		else if (str[i] == '$')
			ret = ft_strjoin_f(ret, ft_handle_dollar(str, &i, exit_code, env_list));
		else
			ret = ft_strjoin_f(ret, ft_handle_normal_str(str, &i));
	}
	return (ret);
}

char	**ft_expand(char *str, int *exit_code, t_env **env_list)
{
	char	**expanded;
	size_t	i;

	str = ft_cmd_pre_expander(str, exit_code, env_list);
	if (!str)
		return (NULL);
	str = ft_clean_empty_strs(str);
	if (!str)
		return (NULL);
	expanded = ft_expander_split(str);
	free(str);
	if (!expanded)
		return (NULL);
	i = 0;
	while (expanded[i])
	{
		expanded[i] = ft_strip_quotes(expanded[i]);
		i++;
	}
	return (expanded);
}
