/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:33:09 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/11 21:43:19 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// based on received token finds its type
// @returns type of token based on enum defined in header
// @param value textual value of token
t_token_type	get_token_type(char *value)
{
	if (ft_strlen(value) == 2)
	{
		if (!ft_strncmp(value, ">>", 2))
			return (TOKEN_APPEND);
		if (!ft_strncmp(value, "<<", 2))
			return (TOKEN_HEREDOC);
	}
	else if (ft_strlen(value) == 1)
	{
		if (value[0] == '>')
			return (TOKEN_REDIROUT);
		if (value[0] == '<')
			return (TOKEN_REDIRIN);
		if (value[0] == '|')
			return (TOKEN_PIPE);
	}
	return (TOKEN_TEXT);
}

char *get_node_value(char *input, int *i, int treat_specials_as_text_dq, int treat_specials_as_text_sq, int start)
{
	char *node_value;
	//int start;

	//start = *i;
	node_value = NULL;
	if (treat_specials_as_text_sq == 0 && treat_specials_as_text_dq == 0 && (input[*i] == '|' || input[*i] == '>' || input[*i] == '<' || iswhitespace(input[*i])))
	{
		node_value = ft_substr(input, start, *i - start);
		(*i)--;
		//return (node_value);
	}
	else if (treat_specials_as_text_sq == 0 && treat_specials_as_text_dq == 1 && (input[*i]  == '"'))
	{
		node_value = ft_substr(input, start, *i - start + 1);
		//return (node_value);
	}
	else if (treat_specials_as_text_sq == 1 && treat_specials_as_text_dq == 0 && (input[*i] == '\''))
	{
		node_value = ft_substr(input, start, *i - start + 1);
		//return (node_value);
	}
	//(*i)++; // else the character we are on is a basic printable character so we add it to the string
	//node_value = ft_strdup("NULL");
	return (node_value);
}

// separates text characters(non-delimiters) from readline input into tokens
// called as soon as we encounter first text char that is not operator
// runs until we are still on text characters, until encounters a delimiter
// @returns created text node and moves iteration pointer to correct character
// when encounter "/', treat other delimiters as normal characters, i is
// when encounter one of the delimiters, create node value and return it
// i is now at last el
// if normal text just move to the next char by i++
// at end decrease value to the last element of the collected string,
// because in outer function it will be increased again
char	*process_text(char *input, int *i)
{
	char	*node_value;
	int		treat_specials_as_text_sq;
	int		treat_specials_as_text_dq;
	int start;

	treat_specials_as_text_sq = 0;
	treat_specials_as_text_dq = 0;
	node_value = NULL;
	start = *i;
	while (input && input[(*i)])
	{
		if (input[*i] == '"' && treat_specials_as_text_sq == 0)
			treat_specials_as_text_dq = !treat_specials_as_text_dq;
		else if (input[*i] == '\''  && treat_specials_as_text_dq == 0)
			treat_specials_as_text_sq = !treat_specials_as_text_sq;
		// if it is one of the delimiters and it has its powers we are at the end of text input, i is now at that delimeter and we break out to the main loop
		// else if (treat_specials_as_text_sq == 0 && treat_specials_as_text_dq == 0 && (input[*i] == '|' || input[*i] == '>' || input[*i] == '<' || iswhitespace(input[*i])))
		// {
		// 	node_value = ft_substr(input, start, *i - start);
		// 	(*i)--;
		// 	return (node_value);
		// }
		// else if (treat_specials_as_text_sq == 0 && treat_specials_as_text_dq == 1 && (input[*i]  == '"'))
		// {
		// 	node_value = ft_substr(input, start, *i - start + 1);
		// 	return (node_value);
		// }
		// else if (treat_specials_as_text_sq == 1 && treat_specials_as_text_dq == 0 && (input[*i] == '\''))
		// {
		// 	node_value = ft_substr(input, start, *i - start + 1);
		// 	return (node_value);
		// }
		else
		{
			node_value = get_node_value(input, i, treat_specials_as_text_dq, treat_specials_as_text_sq, start);
			if (node_value)
				return (node_value);
		}
		(*i)++;
	}
	node_value = ft_substr(input, start, ft_strlen(input + start));
	(*i)--;
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
	i = 0;
	while (i <= ft_strlen(input) && input[i])
	{
		while (input[i] && iswhitespace(input[i]))
			i++;
		if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<'
			&& input[i + 1] == '<'))
			node_value = ft_substr(input, (i)++, 2);
		else if (input[i] == '|' || input[i] == '>' || input[i] == '<')
			node_value = ft_substr(input, i, 1);
		else // store text
			node_value = process_text(input, &i);
		if (node_value)
		{
			new_tok = new_token(ft_strdup(node_value),
				get_token_type(node_value));
			if (!new_tok)
				return (NULL);
			add_back_token(&token_list, new_tok);
			free(node_value);
			node_value = NULL;
		}
		i++;
	}
	return (token_list);
}

t_token	*lexer(char *input)
{
	t_token	*token_list;
	char *new_str;

	token_list = get_token_list(input);
	if (!token_list)
		return (NULL);
	free(input);
	input = NULL;
	t_token *temp = token_list;
	while (temp)
	{
		if (temp->type == TOKEN_TEXT)
		{
			new_str = str_from_array(parse_eq(temp));
			free(temp->value);
			temp->value = new_str;
		}
		temp = temp->next;
	}
	return (token_list);
}

