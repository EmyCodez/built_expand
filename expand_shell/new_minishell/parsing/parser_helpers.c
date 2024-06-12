/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emilin <emilin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:50:50 by emilin            #+#    #+#             */
/*   Updated: 2024/06/12 11:08:28 by emilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_get_io_list(t_io_node **io_list, t_parse_err *parse_error, t_token **curr_token)
{
	t_token_type		redir_type;
	t_io_node			*tmp_io_node;

	if (parse_err->type)
		return (false);
	while ((*curr_token) && ft_is_redir((*curr_token)->type))
	{
		redir_type = (*curr_token)->type;
		(*curr_token) = (*curr_token)->next;
		if (!*curr_token)
			return (ft_set_parse_err(E_SYNTAX), false);
		if ((*curr_token)->type != T_IDENTIFIER)
			return (ft_set_parse_err(E_SYNTAX), false);
		tmp_io_node = ft_new_io_node(redir_type, (*curr_token)->value);
		if (!tmp_io_node)
			return (ft_set_parse_err(E_MEM), false);
		ft_append_io_node(io_list, tmp_io_node);
		(*curr_token) = (*curr_token)->next;
	}
	return (true);
}

bool	ft_join_args(char **args, t_parse_err *parse_error, t_token **curr_token)
{
	char	*to_free;

	if (parse_error->type)
		return (false);
	if (!*args)
		*args = ft_strdup("");
	if (!*args)
		return (false);
	while ((*curr_token)
		&& (*curr_token) -> type == T_IDENTIFIER)
	{
		to_free = *args;
		*args = ft_strjoin_with(*args, (*curr_token) -> value, ' ');
		if (!*args)
			return (free(to_free), false);
		free(to_free);
		(*curr_token) = (*curr_token)->next;
	}
	return (true);
}

t_node	*ft_get_simple_cmd(t_token **curr_token, t_parse_err *parse_error)
{
	t_node	*node;

	if (parse_err->type)
		return (NULL);
	node = ft_new_node(N_CMD);
	if (!node)
		return (ft_set_parse_err(E_MEM), NULL);
	while ((*curr_token)
		&& ((*curr_token)->type == T_IDENTIFIER
			|| ft_is_redir((*curr_token)->type)))
	{
		if ((*curr_token)->type == T_IDENTIFIER)
		{
			if (!ft_join_args(&(node -> args), parse_error, curr_token))
				return (ft_clear_cmd_node(node), ft_set_parse_err(E_MEM), NULL);
		}
		else if (ft_is_redir((*curr_token)->type))
		{
			if (!ft_get_io_list(&(node->io_list), parse_error, curr_token))
				return (free(node->args), free(node), NULL);
		}
	}
	return (node);
}
