/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:33:09 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/04 18:21:50 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token_type	get_token_type(char *value)
{
	if (ft_strlen(value) == 2)
	{
		if (!ft_strncmp(value, ">>", 2))
			return (TOKEN_REDIR_APPEND);
		if (!ft_strncmp(value, "<<", 2))
			return (TOKEN_HEREDOC);
	}
	else if (ft_strlen(value) == 1)
	{
		if (value[0] == '>')
			return (TOKEN_REDIROUT);
		if (value[0] == '<')
			return (TOKEN_REDIRIN);
		if (value[0] == '\'')
			return (TOKEN_SQUOTE);
		if (value[0] == '"')
			return (TOKEN_DQUOTE);
		if (value[0] == '$')
			return (TOKEN_ENV);
		if (value[0] == '|')
			return (TOKEN_PIPE);
	}
	return (TOKEN_TEXT);
}

int	get_token_len(char *input, int i)
{
	int	j;

	j = 0;
	while (input[i] != '|' && input[i] != '>' && input[i] != '<'
		&& input[i] != '$' && input[i] != '"' && input[i] != '\'' //ft_strncmp(input, "'", 1)
		&& !iswhitespace(input[i]) && input[i] != '\0')
	{
		
		j++;
		i++;
	}
	return (j);
}

char	*get_node_value(char *input, int *i)
{
	char	*node_value;
	
	if ((input[*i] == '>' && input[*i + 1] == '>') || (input[*i] == '<'
		&& input[*i + 1] == '<'))
		node_value = ft_substr(input, (*i)++, 2);
	else if (input[*i] == '|' || input[*i] == '>' || input[*i] == '<'
		|| input[*i] == '$' || input[*i] == '"' || input[*i] == '\'')
		node_value = ft_substr(input, *i, 1);
	else
	{
		node_value = ft_substr(input, *i, get_token_len(input, *i));
		(*i) += get_token_len(input, *i) - 1;
	}
	if (!node_value)
		return (NULL);
	return (node_value);
}

t_token	*get_token_list(char *input)
{
	t_token	*token_list;
	int		i;
	char	*node_value;
	t_token	*new_tok;
	
	new_tok = NULL;
	token_list = NULL;
	i = -1;
	while (input && input[++i])
	{
		if (!iswhitespace(input[i]))
		{
			node_value = get_node_value(input, &i);
			if (!node_value)
				return (NULL);
			new_tok = new_token(ft_strdup(node_value),
				get_token_type(node_value));
			if (!new_tok)
				return (NULL);
			add_back_token(&token_list, new_tok);
			free(node_value);
			node_value = NULL;
		}
	}
	return (token_list);
}

t_token	*lexer(char *input)
{
	t_token	*token_list;
	
	token_list = get_token_list(input);
	if (!token_list)
		return (NULL);
	return (token_list);
}
