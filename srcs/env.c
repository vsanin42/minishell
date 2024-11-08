/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/08 14:06:03 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *handle_normal_word(char *res, char *text, int *i)
{
	int	len;
	char *to_append;
	char *env;

	len = 0;
	to_append = NULL;
	env = NULL;
	while (text[*i] && text[*i] != '$')
	{
		(*i)++;
		len++;
	}
	if (len)
	{
		to_append = ft_substr(text, (*i) - len, len);
		res = ft_strjoin(res, to_append);
		free(to_append);
		to_append = NULL;
	}
	return (res);
}

char *handle_env_in_braces(char *res, char *text, int *i)
{
	int len;
	char *to_append;
	char *env;

	len = 0;
	to_append = NULL;
	env = NULL;
	(*i)++;
	while (text[*i] && text[*i] != '}')
	{
		len++;
		(*i)++;
	}
	if (text[*i] != '}')
		error_msg("minishell: missing }"); // 	MEMORY
	if (len)
	{
		to_append = ft_substr(text, (*i) - len, len);
		if (!is_alnum(to_append))
		{
			free(to_append);
			to_append = NULL;
			error_msg("minishell: bad substitution");	// MEMORY
		}
		env = process_env(to_append);
		if (env)
			res = ft_strjoin(res, env);
		free(to_append);
		to_append = NULL;
		free(env);
		env = NULL;
	}
	return (res);
}

char *handle_env_without_braces(char *res, char *text, int *i)
{
	int len;
	char *to_append;
	char *env;

	len = 0;
	to_append = NULL;
	env = NULL;
	while (text[*i] && ft_isalnum(text[*i]))
	{
		len++;
		(*i)++;
	}
	if (len)
	{
		to_append = ft_substr(text, (*i) - len, len);
		env = process_env(to_append);
		if (env)
			res = ft_strjoin(res, env);
		free(to_append);
		free(env);
		env = NULL;
		to_append = NULL;
		(*i)--;
	}
	return (res);
}

char *process_end_of_text(char *res)
{
	char *to_append;

	to_append = ft_strdup("$");
	res = ft_strjoin(res, to_append);
	free(to_append);
	to_append = NULL;
	return (res);
}

// get string with or without ""/'' at  ends, quotes  in middle are non-special
// doesnt take into consderation if it is between "/', expands always
// thus check for whether it should be expanded must be in the calling function
// @returns allocated string back with envs expanded
char *get_env_value_to_process(char *text)
{
	int i;
	char *res;

	i = -1;
	res = ft_strdup("");
	while (text[++i])
	{
		res = handle_normal_word(res, text, &i); 	// WORDS WITHOUT $
		if (text[i] == '$' && (ft_isalnum(text[i + 1]) || text[i + 1] == '{')) 	// ENVS (WITH $)
		{
			i++;
			if (text[i] == '{') // ENVS IN {}
				res = handle_env_in_braces(res, text, &i);
			else	// ENVS WITHOUT {}
			{
				res = handle_env_without_braces(res, text, &i);
				if (!text[i])
					break ;
			}
		}
		else if (text[i] == '$' && (!text[i + 1] || ((text[i + 1] && !ft_isalnum(text[i + 1]))))) 	// ENCOUNTERED END OF STRING AND FOUND NO $ OF FOUND JUST $ AS LAST CHARACTER
			res = process_end_of_text(res);
		else
			break ;
	}
	return (res);
}
