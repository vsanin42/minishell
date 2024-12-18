/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/18 13:46:20 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// called on start of the input or after we go out of any env's scope
// processes the text we collected before encountering $ character
// @returns allocated collected string before it encountered $ or text end
// positions i on $ or right after the last character
char	*handle_word_no_env(char *res, char *text, int *i)
{
	int		len;
	char	*oldres;
	char	*to_append;

	len = 0;
	oldres = NULL;
	to_append = NULL;
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
		free_four_mallocs(to_append, oldres, NULL, NULL);
	}
	return (res);
}

// called when we encounter $ in input, i is at $ at start so we move it by one
// if there are braces { we move i one more character forward
// @returns expanded res, or the original res if could not expand
// sets i to the last element of the searched string - last alnum char or }
// if there are no braces, after calling the function we end up on next element
// after the string but it will be incremented after so we decrease by one
// we do not decrement by 1 with {} because then the i will end up on closing }
// which will then be incremented in loop so that is where we want the i to be
char	*handle_env(t_mini *mini, char *res, char *text, int *i)
{
	(*i)++;
	if (text[*i] == '{')
	{
		(*i)++;
		if (text[*i] == '?')
			res = handle_question(mini, res, text, i);
		else
			res = handle_env_in_braces(mini, res, text, i);
	}
	else
	{
		if (text[*i] == '?')
			res = handle_question(mini, res, text, i);
		else
			res = handle_env_without_braces(mini, res, text, i);
		(*i)--;
	}
	return (res);
}

// called when '${' is encountered
// pointer to i is now at '{' character, so we increment by 1 to be on a char
// @returns allocated res appended by expanded env, nothing if could not expand
// removed this from initialization as it will not be causing errors:
//	to_append = NULL; // remove if necessary, will not be used when not found
//	env = NULL; // remove if necessary, will not be used when not found
char	*handle_env_in_braces(t_mini *mini, char *res, char *text, int *i)
{
	int		len;
	char	*env;
	char	*oldres;
	char	*to_append;

	len = -1;
	oldres = res;
	while ((++len >= 0) && text[*i] && text[*i] != '}')
		(*i)++;
	if (len)
	{
		to_append = ft_substr(text, (*i) - len, len);
		env = process_local_env(mini, to_append);
		if (env)
		{
			res = ft_strjoin(res, env);
			free_four_mallocs(oldres, NULL, NULL, NULL);
		}
		free_four_mallocs(to_append, env, NULL, NULL);
	}
	return (res);
}

// called when $ followed by alnum character is encountered in input
// @returns allocated res appended by expanded env, nothing if could not expand
char	*handle_env_without_braces(t_mini *mini, char *res, char *text, int *i)
{
	int		len;
	char	*env;
	char	*oldres;
	char	*to_append;

	len = -1;
	env = NULL;
	oldres = NULL;
	to_append = NULL;
	while ((++len >= 0) && text[*i] && (ft_isalnum(text[*i])
			|| text[*i] == '_'))
		(*i)++;
	if (len)
	{
		to_append = ft_substr(text, (*i) - len, len);
		env = process_local_env(mini, to_append);
		oldres = res;
		if (env)
		{
			res = ft_strjoin(res, env);
			free_four_mallocs(oldres, NULL, NULL, NULL);
		}
		free_four_mallocs(to_append, env, NULL, NULL);
	}
	return (res);
}

// get string with or without ""/'' at  ends, quotes  in middle are non-special
// doesnt take into consderation if it is between "/', expands always
// thus check for whether it should be expanded must be in the calling function
// first stores everything word before encountering $ into string
// if encounters $ and after it is alphanumeric character or {, checks the env
// if encounters $ and after it isnt alnum character/{, treats it as '$' char
// else we know we processed entire word and there is no $ and we are at end
// @returns allocated string back with envs expanded
char	*get_env_value_to_process(t_mini *mini, char *text)
{
	int		i;
	char	*res;

	i = -1;
	res = ft_strdup("");
	while (text[++i])
	{
		res = handle_word_no_env(res, text, &i);
		if (text[i] == '$' && (ft_isalnum(text[i + 1]) || text[i + 1] == '_'
				|| text[i + 1] == '{' || text[i + 1] == '?'))
		{
			res = handle_env(mini, res, text, &i);
			if (!text[i])
				break ;
		}
		else if (text[i] == '$' && text[i + 1]
			&& (text[i + 1] == '\'' || text[i + 1] == '"'))
			continue ;
		else if (text[i] == '$' && (!text[i + 1]
				|| (text[i + 1] && !ft_isalnum(text[i + 1]))))
			res = quickjoin(res);
		else
			break ;
	}
	return (res);
}
