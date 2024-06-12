/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emilin <emilin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 21:52:56 by abelayad          #+#    #+#             */
/*   Updated: 2024/06/12 12:19:58 by emilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_tokenize(char *line, int *exit_code)
{
	t_token	*token_list;

	token_list = ft_tokenization_handler(line, exit_code);
	return (token_list);
}
