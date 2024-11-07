/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/07 17:27:42 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// we get optimal value WITHOUT $ that we just transform to env value or nothing if it doesnt match anything
// @returns expanded allocated value if found, NULL if not found, od allocated "$" if there was nothing
char	*process_env(char *env_to_process)
{
	char *res;

	if (!env_to_process)
		res = ft_strdup("$");
	else if (getenv(env_to_process))
	{
		res = ft_strdup(getenv(env_to_process));
	}
	else
		res = NULL;
	return (res); // return found str, NULL if not found, or $ if there was nothing after the $
}

// we only get a string between "" or not, if quotes are in the middle they are non-special
// eg. we can get $HOME, "$HOME", "$HOME'"
//TODO: WHAT IF LEN IS 0 MEANING THAT THERE IS NO VALID TEXT AFTER $
//TODO: WHAT ABOUT WHITESPACES
char *get_env_value_to_process(char *text)
{
	int i;
	int len;
	int start;
	char *to_append;
	char *res;
	char *env;

	i = 0;
	res = ft_strdup("");
	to_append = NULL;
	env = NULL;

	// WORDS WITHOUT $ - WORKS
	while (text[i])
	{
		start = i;
		len = 0;
		while (text[i] && text[i] != '$')	// collect what we find before $ to substring
		{
			i++;
			len++;
		}
		if (len)
		{
			to_append = ft_substr(text, start, len);
			res = ft_strjoin(res, to_append);
			free(to_append);
			to_append = NULL;
			len = 0;
		}

	// ENVS IN $
		if (text[i] == '$')
		{
			i++;
	// ENVS IN {}
			if (text[i] == '{') // collect everything until we encounter } or end of line
			{
				i++;
				start = i;
				while (text[i] && text[i] != '}')
				{
					len++;
					i++;
				}
				if (len)
				{
					to_append = ft_substr(text, start, len);
					env = process_env(to_append);
					if (env) // ELSE MAYBE ERROR zminishell: bas substitution? if we could not find the value in {} as it is done in zsh
						res = ft_strjoin(res, env);
					free(to_append);
					free(env);
					to_append = NULL;
					env = NULL;
					len = 0;
				}
				if (text[i] != '}') // means we did not end up on the } so the string must have ended
					break ;
			}
	// ENVS WITHOUT {}
			else // collect everything until encountering end of the line or the first non-alphanumeric character, the rest until whitespace is then appended after the expanded env, if not exist, it is null and we append to this
			{
				start = i;
				while (text[i] && isalnum(text[i]))
				{
					len++;
					i++;
				}
				if (len)
				{
					to_append = ft_substr(text, start, len);
					env = process_env(to_append);
					if (env)
						res = ft_strjoin(res, env);
					free(to_append);
					free(env);
					env = NULL;
					to_append = NULL;
					len = 0;
					i--;
				}
				if (!text[i]) // means we did  end up outside of the string so there  is nothing to collect so we will break
					break ;
			}
		}
		else // meaning we must have come to the end of string since we did not encounter $
			break ;
		i++;
	}
	return (res);
}


// will get a string without quotes as input, only quotes that are there are treates as normal text
// TODO: gets a string, if it has $, expands, if not, returns it back
char *expand_envs(char *str_to_expand)
{
	char *str_to_check;
	char *env;
	char *res;
	char *rest;
	int i = 0;
	int len = 0;

	while (str_to_expand[i])
	{
		if (str_to_expand[i] == '$')
		{
			i++;
			if (str_to_expand[i] == '{')
			{
				while (str_to_expand[i] && str_to_expand[i] != '}')
				{
					i++;
					len++;
				}
				if (str_to_expand[i] == '}')
				{
					str_to_check = ft_substr(str_to_expand, i, len - 1);
					if (getenv(str_to_check))
					{
						env = ft_strdup(getenv(str_to_check));
						rest = (str_to_expand + (i) + 1);
						res = ft_strjoin(env, rest);
					}
					else
						res = "\n";
					free(str_to_check);
				}
				else
					res = ft_strdup("minishell: bad substitution}");
				}
		}
		else
		{
			if (getenv(str_to_expand))
				res = ft_strdup(getenv(str_to_expand)); // returns value of env or NULL if not found
			else
				res = ft_strdup("\n");
			}
	}
	return (res);
}
