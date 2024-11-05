/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/05 14:32:54 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// gets a node in a list that is $(type env) and processes following nodes that are type text
// if we have ' ' before, do not call this function
// $$ prints pid of current shell - probably do not have to do
// @returns char * that represents the output string
char	*expand_env(t_mini *mini, t_token *token)
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
						env = getenv(str_to_check);
						rest = (temp + i + 1);
						res = ft_strjoin(env, rest);
					}
					else 
						res = "\n";
					free(str_to_check);
				}
				else
					res = ("Unclosed bracket: }");
			}
			else
				res = getenv(temp); // returns value of env or NULL if not found
		}
		// TODO: what if the $ has no text argument but there is rest of the code later eg. $ >> outfile.txt and what is the difference between $>> outfile.txt
		if (to_process->type == 5 || to_process->type == 6 || to_process->type == 7 || to_process->type == 2)
			res = "$"; //ft_strdup(token->value); // if there is o text following, store $ as text
		// TODO: what to do if the following node that should be text is not text?
		return (res);
}
