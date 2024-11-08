/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/08 21:53:46 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_normal_word(char *res, char *text, int *i)
{
	int		len;
	char	*to_append;
	char	*env;
	char	*oldres;
	
	oldres = NULL;
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
		oldres = res;
		res = ft_strjoin(res, to_append);
		free(oldres);
		free(to_append);
		oldres = NULL;
		to_append = NULL;
	}
	return (res);
}

char	*handle_env_in_braces(char *res, char *text, int *i)
{
	int		len;
	char	*to_append;
	char	*env;

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
	{
		free_four_mallocs(res, NULL, NULL, NULL);
		error_msg("minishell: missing }"); // 	MEMORY
	}
	if (len)
	{
		to_append = ft_substr(text, (*i) - len, len);
		if (!is_alnum(to_append))
		{
			free_four_mallocs(to_append, res, NULL, NULL);
			error_msg("minishell: bad substitution");	// MEMORY
		}
		env = process_env(to_append);
		if (env)
			res = ft_strjoin(res, env);
		free_four_mallocs(to_append, env, NULL, NULL);
	}
	return (res);
}

char *handle_env_without_braces(char *res, char *text, int *i)
{
	int len;
	char *to_append;
	char *env;
	char *oldres;
	
	oldres = NULL;
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
		{
			oldres = res; // work on this oldres to make space for 25 lines somehow
			res = ft_strjoin(res, env);
		}
		free_four_mallocs(to_append, env, oldres, NULL);
		(*i)--;
	}
	return (res);
}

char *handle_env(char *res, char *text, int *i)
{
	(*i)++;
	if (text[*i] == '{')
		res = handle_env_in_braces(res, text, i);
	else
		res = handle_env_without_braces(res, text, i);
	return (res);
}

// get string with or without ""/'' at  ends, quotes  in middle are non-special
// doesnt take into consderation if it is between "/', expands always
// thus check for whether it should be expanded must be in the calling function
// first checks the word before encountering $
// if encounters $ and after it is alphanumeric character or }, checks the env
// if encounters $ and after it isnt alphanumeric character treats it as $ char
// else we know we processed entire word and there is no $ and we are at end
// @returns allocated string back with envs expanded
char *get_env_value_to_process(char *text)
{
	int i;
	char *res;
	char *oldres;

	i = -1;
	res = ft_strdup("");
	while (text[++i])
	{
		oldres = res;
		res = handle_normal_word(res, text, &i);
		if (text[i] == '$' && (ft_isalnum(text[i + 1]) || text[i + 1] == '{'))
		{
			res = handle_env(res, text, &i);
			if (!text[i])
				break ;
		}
		else if (text[i] == '$' && (!text[i + 1] || ((text[i + 1] && !ft_isalnum(text[i + 1])))))
		{	
			res = ft_strjoin(res, ft_strdup("$"));
			free(oldres);
		}
		else
			break ;
	}
	return (res);
}
