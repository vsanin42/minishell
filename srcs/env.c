/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/05 22:54:57 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// gets a node in a list that is $(type env) and processes following nodes that are type text
// if we have ' ' before, do not call this function
// $$ prints pid of current shell - probably do not have to do
// @returns char * that represents the output string
void expand_env(t_mini *mini, t_token *token)
{
	// it must all be in single text node following the env node, not more because that means there were spaces
	// so we check only the one following node after the env node
	t_token *to_process = token->next;
	char *temp = to_process->value;
	char *str_to_check;
	char *env;
	char *res;
	char *rest;
	(void)mini;
		if (to_process->type == 0)
		{
			// get value between {} and process this
			// check if there is { right after $, then collect characters before encountering }
			if (temp[0] == '{')
			{
				int i = 1;
				while (temp[i] && temp[i] != '}')
					i++;
				if (temp[i] == '}')
				{
					str_to_check = ft_substr(temp, 1, i - 1);
					if (getenv(str_to_check))
					{
						env = ft_strdup(getenv(str_to_check));
						rest = (temp + i + 1);
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
				if (getenv(temp))
					res = ft_strdup(getenv(temp)); // returns value of env or NULL if not found
				else
					res = ft_strdup("\n");
			}
		}
		// TODO: what if the $ has no text argument but there is rest of the code later eg. $ >> outfile.txt and what is the difference between $>> outfile.txt
		if (to_process->type == 5 || to_process->type == 6 || to_process->type == 7 || to_process->type == 2)
			res = ft_strdup("$"); //ft_strdup(token->value); // if there is o text following, store $ as text
		// TODO: what to do if the following node that should be text is not text?
		to_process->value = res;
}

// void	expand_envs(t_mini *mini, t_token *token_list)
// {
// 	t_token *temp = token_list;
// 	t_token *prev;
// 	t_token *dollar;

// 	prev = NULL;
// 	dollar = NULL;
// 	while (temp)
// 	{
// 		if (temp->type != TOKEN_ENV && temp->next && temp->next->type == TOKEN_ENV)
// 			prev = temp;
// 		if (temp->type == TOKEN_ENV)
// 		{
// 			dollar = temp;
// 			expand_env(mini, temp);
// 			if (prev)
// 				prev->next = temp->next;
// 			dollar->next = NULL;
// 			temp = temp->next;
// 			free(dollar->value);
// 			free(dollar);
// 			continue ;
// 		}
// 		//prev = prev->next;
// 		temp = temp->next;
// 	}
// }
void expand_envs(t_mini *mini, t_token **token_list)
{
	t_token *temp = *token_list;
	t_token *prev = NULL;
	t_token *dollar;

	while (temp)
	{
		if (temp->type == TOKEN_ENV)
		{
			dollar = temp;  // Store the TOKEN_ENV node to remove it later
			expand_env(mini, temp);  // Perform expansion on the next node

			// Remove the TOKEN_ENV node from the list
			if (prev == NULL)
				*token_list = temp->next;  // Update the head if TOKEN_ENV was the first node
			else
				prev->next = temp->next;  // Link previous node to next of TOKEN_ENV

			temp = temp->next;  // Move temp to the next node after TOKEN_ENV

			// Free the TOKEN_ENV node
			free(dollar->value);
			free(dollar);

			// If head was updated, reset prev to NULL
			// if (prev == NULL)
			// 	prev = *token_list;
		}
		else
		{
			prev = temp;  // Update prev only if the current node is not TOKEN_ENV
			temp = temp->next;  // Move to the next node
		}
	}
}
