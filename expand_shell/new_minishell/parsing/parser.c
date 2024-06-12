/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emilin <emilin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 22:55:17 by abelayad          #+#    #+#             */
/*   Updated: 2024/06/12 11:57:46 by emilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*ft_term(t_token **curr_token, t_parse_err *parse_error, t_token **token_list)
{
	t_node	*node;

	if (parse_error->type)
		return (NULL);
	if (ft_curr_token_is_binop() || (*curr_token)->type == T_C_PARENT)
		return (ft_set_parse_err(parse_error, E_SYNTAX), NULL);
	else if ((*curr_token)->type == T_O_PARENT)
	{
		(*curr_token) = (*curr_token)->next;
		node = ft_expression(curr_token, parse_error, token_list);
		if (!node)
			return (ft_set_parse_err(parse_error, E_MEM), NULL);
		if (!(*curr_token)
			|| (*curr_token)->type != T_C_PARENT)
			return (ft_set_parse_err(parse_error, E_SYNTAX), node);
		(*curr_token) =(*curr_token)->next ;
		return (node);
	}
	else
		return (ft_get_simple_cmd(curr_token, parse_error));
}

t_node	*ft_expression(t_token **curr_token, t_parse_err *parse_error, t_token **token_list)
{
	t_node			*left;
	t_node			*right;
	t_token_type	op;

	if (parse_error->type || !*curr_token)
		return (NULL);
	left = ft_term(curr_token, parse_error, token_list);
	if (!left)
		return (NULL);
	while (ft_curr_token_is_binop())
	{
		op = (*curr_token)->type;
		(*curr_token) = (*curr_token)->next;
		if (!*curr_token)
			return (ft_set_parse_err(parse_error, E_SYNTAX), left);
		right = ft_expression(curr_token, parse_error, token_list);
		if (!right)
			return (left);
		left = ft_combine(op, left, right, parse_error);
		if (!left)
			return (ft_clear_ast(&left, token_list), ft_clear_ast(&right, token_list), NULL);
	}
	return (left);
}

t_node	*ft_combine(t_token_type op, t_node *left, t_node *right, t_parse_err *parse_error)
{
	t_node	*node;

	if (parse_error->type)
		return (NULL);
	node = ft_new_node(ft_get_node_type(op));
	if (!node)
		return (ft_set_parse_err(parse_error, E_MEM), NULL);
	node -> left = left;
	node -> right = right;
	return (node);
}

t_node	*ft_parse(t_token **token_list, t_token **curr_token, t_parse_err *parse_error)
{
	t_node	*ast;

	ast = ft_expression(curr_token, parse_error, token_list);
	if (*curr_token)
		return (ft_set_parse_err(parse_error, E_SYNTAX), ast);
	return (ast);
}
