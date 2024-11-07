/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/07 10:51:42 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// gets a node in a list that is $(type env) and processes following nodes that are type text
// if we have ' ' before, do not call this function
// $$ prints pid of current shell - probably do not have to do
// @returns char * that represents the output string
// void expand_env(t_mini *mini, t_token *token)
// {
// 	// it must all be in single text node following the env node, not more because that means there were spaces
// 	// so we check only the one following node after the env node
// 	t_token *to_process = token->next;
// 	char *temp = to_process->value;
// 	char *str_to_check;
// 	char *env;
// 	char *res;
// 	char *rest;
// 	(void)mini;
// 		if (to_process->type == 0)
// 		{
// 			// get value between {} and process this
// 			// check if there is { right after $, then collect characters before encountering }
// 			if (temp[0] == '{')
// 			{
// 				int i = 1;
// 				while (temp[i] && temp[i] != '}')
// 					i++;
// 				if (temp[i] == '}')
// 				{
// 					str_to_check = ft_substr(temp, 1, i - 1);
// 					if (getenv(str_to_check))
// 					{
// 						env = ft_strdup(getenv(str_to_check));
// 						rest = (temp + i + 1);
// 						res = ft_strjoin(env, rest);
// 					}
// 					else
// 						res = "\n";
// 					free(str_to_check);
// 				}
// 				else
// 					res = ft_strdup("minishell: bad substitution}");
// 			}
// 			else
// 			{
// 				if (getenv(temp))
// 					res = ft_strdup(getenv(temp)); // returns value of env or NULL if not found
// 				else
// 					res = ft_strdup("\n");
// 			}
// 		}
// 		// TODO: what if the $ has no text argument but there is rest of the code later eg. $ >> outfile.txt and what is the difference between $>> outfile.txt
// 		if (to_process->type == 5 || to_process->type == 6 || to_process->type == 7 || to_process->type == 2)
// 			res = ft_strdup("$"); //ft_strdup(token->value); // if there is o text following, store $ as text
// 		// TODO: what to do if the following node that should be text is not text?
// 		to_process->value = res;
// }

// void expand_envs(t_mini *mini, t_token **token_list)
// {
// 	t_token *temp = *token_list;
// 	t_token *prev = NULL;
// 	t_token *dollar;

// 	while (temp)
// 	{
// 		if (temp->type == TOKEN_ENV)
// 		{
// 			dollar = temp;  // Store the TOKEN_ENV node to remove it later
// 			expand_env(mini, temp);  // Perform expansion on the next node

// 			// Remove the TOKEN_ENV node from the list
// 			if (prev == NULL)
// 				*token_list = temp->next;  // Update the head if TOKEN_ENV was the first node
// 			else
// 				prev->next = temp->next;  // Link previous node to next of TOKEN_ENV

// 			temp = temp->next;  // Move temp to the next node after TOKEN_ENV

// 			// Free the TOKEN_ENV node
// 			free(dollar->value);
// 			free(dollar);

// 			// If head was updated, reset prev to NULL
// 			// if (prev == NULL)
// 			// 	prev = *token_list;
// 		}
// 		else
// 		{
// 			prev = temp;  // Update prev only if the current node is not TOKEN_ENV
// 			temp = temp->next;  // Move to the next node
// 		}
// 	}
// }

char *get_env_value(char *to_expand, int *i)
{
	char *str_to_check;
	char *env;
	char *res;
	char *rest;
	// remove the $ char and move to the next one
	if (to_expand[*i] == '{')
	{
		while (to_expand[*i] && to_expand[*i] != '}')
			i++;
		if (to_expand[*i] == '}')
		{
			str_to_check = ft_substr(to_expand, 1, *i - 1);
			if (getenv(str_to_check))
			{
				env = ft_strdup(getenv(str_to_check));
				rest = (to_expand + (*i) + 1);
				res = ft_strjoin(env, rest);
			}
			else
				res = "\n";
			free(str_to_check);
		}
		else
			res = ft_strdup("minishell: bad substitution}");
	}
	else
	{
		if (getenv(to_expand))
			res = ft_strdup(getenv(to_expand)); // returns value of env or NULL if not found
		else
			res = ft_strdup("\n");
		}
	return (res);
}

// expands envs as needed and removes unneeded quotes and $ (TODO: and {})
void	parse_envs_and_quotes(t_token *token)
{
	char	*text;
	int		i;
	char	*res;
	char	*env;

	int in_sq = 0;
	int in_dq = 0;
	i = 0;
	text = token->value;
	if (token->type != TOKEN_TEXT)
		return ; // if it was not text our work is done and we can successfully go out of this node

	// go over the text until we encounter ' or "
	// when we encounter one of them, we create a "first string" variable that stores string up to here
	// when encounter dq, do function handle_between_dq
	// this will have a while loop and will run until it encounters ", properly updating the index
	// it will expand the variables with expand_envs function
	// it will return a string that will be malloced and will not contain the last character, the i will stay on " or out of bounds
	// then we check if the current i is on " - we know the quote was ended
	// if the i is on NULL that means that the quote was not finished and we can throw error Unclosed quote 
	// in this outer function we check
	// if we encounter ', call function handle_between_sq

	while (text[i])
	{
		if (!in_dq && text[i] == '\'')
			in_sq = !in_sq;
		if (!in_sq && text[i] == '"')
			in_dq = !in_dq;
		if (in_sq && text[i] == '\'')
		{
			char *first_part = ft_substr(text, 0, i - 1);
			char *second_part = ft_substr(text, i + i, ft_strlen(text) - i - 1);
			res = ft_strjoin(first_part, second_part);
			free(first_part);
			free(second_part);
		}
		if (in_dq && text[i] == '"')
		{
			char *first_part = ft_substr(text, 0, i - 1);
			char *second_part = ft_substr(text, i + i, ft_strlen(text) - i - 1);
			res = ft_strjoin(first_part, second_part);
			free(first_part);
			free(second_part);
		}
		if (!in_sq && text[i] == '$')
		{
			if (text[i + 1])
			{
				char *first_part = ft_substr(text, 0, i - 1);
				i++;
				char *second_part = get_env_value(text, &i);
				res = ft_strjoin(first_part, second_part);
				free(first_part);
				free(second_part);
			}
			else // if there is nothing after dollar anymore we treat it as normal character
				env = ft_strdup("$");
			if (res)
			{
				char *temp;
				temp = res;
				res = ft_strjoin(temp, env);
			}
		}
		i++;
	}
	if (in_sq)
		error_msg("minishell: squote missing");
	if (in_dq)
		error_msg("minishell: dquote missing");
	token->value = res;
}
