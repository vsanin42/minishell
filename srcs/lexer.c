/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:33:09 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/08 15:20:05 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

// operates on input string from readline and separates it into tokens
// called as soon as we encounter first text char that is not operator
// @returns created text node and moves iteration pointer to correct character
char	*process_text(char *input, int *i)
{
	char	*node_value;
	int		start;
	int		treat_specials_as_text_sq;
	int		treat_specials_as_text_dq;

	treat_specials_as_text_sq = 0;
	treat_specials_as_text_dq = 0;
	start = *i;
	node_value = NULL;
	while (input && input[*i])
	{
		if (input[*i] == '"' && treat_specials_as_text_sq == 0) // when encounter "/', start/stop treating specials as text
			treat_specials_as_text_dq = !treat_specials_as_text_dq;
		else if (input[*i] == '\''  && treat_specials_as_text_dq == 0)
			treat_specials_as_text_sq = !treat_specials_as_text_sq;
		// if it is one of the delimeters and it has its powers we are at the end of text input, i is now at that delimeter and we break out to the main loop
		else if (treat_specials_as_text_sq == 0 && treat_specials_as_text_dq == 0 && (input[*i] == '|' || input[*i] == '>' || input[*i] == '<' || iswhitespace(input[*i])))
		{
			node_value = ft_substr(input, start, *i - start);
			(*i)--;
			return (node_value);
		}
		else if (treat_specials_as_text_sq == 0 && treat_specials_as_text_dq == 1 && (input[*i]  == '"' /*|| input[*i] == '|' || input[*i] == '>' || input[*i] == '<' || iswhitespace(input[*i]) */))
		{
			node_value = ft_substr(input, start, *i - start + 1);
			return (node_value);
		}
		else if (treat_specials_as_text_sq == 1 && treat_specials_as_text_dq == 0 && (input[*i] == '\''/*  || input[*i] == '|' || input[*i] == '>' || input[*i] == '<' || iswhitespace(input[*i]) */))
		{
			node_value = ft_substr(input, start, *i - start + 1);
			return (node_value);
		}
		(*i)++; // else the character we are on is a basic printable character so we add it to the string
	}
	// if it is whitespace at the end it continues as a node with value of space
	node_value = ft_substr(input, start, ft_strlen(input + start));
	(*i)--; // decrease value to the last element of the collected string, because in outer funciton it will be increased again
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
	while (/* input &&  */input[i])
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
		new_str = parse_eq(temp);
		free(temp->value);
		temp->value = new_str;
		temp = temp->next;
	}
	return (token_list);
}

