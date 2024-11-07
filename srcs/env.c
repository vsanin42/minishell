/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/07 17:13:01 by zpiarova         ###   ########.fr       */
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

// it gets a string that ends at the end of string or whitespace and doesnt contain any special quotes
// @returns expanded env with ${} removed
// can contain multiple $ but not in {}
// there cannot be another $ in
// $HOME returns /zuzana/piarova/home
// $HOMEa returns nothing
// ${HOME} returns /zuzama/piarova/home
// ${HOME}a returns /zuzana/piarova/homea
// a$HOME returns a
// a$HOMEa$HOME returns a/zuzana/piarova/home

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

// get what is between quote trims both the quotes and returns allocated string
// when encounter quote, this function goes over the string untill it finds another quote or end and returns the text it collected
// gets the text with the first quote and continues until it finds the last quote, gets the value inside
// gets: "abcdef" or "abcdef (unclosed)
char *get_what_is_between_quotes(char quote_type, char *text, int *i)
{
	char *what_is_between_quotes;
	char *res;
	int len = 0;

	while (text[*i] && text[*i] != quote_type)
	{
		(*i)++;
		len++;
	}
	what_is_between_quotes = ft_substr(text, 1, len - 1); // now we have a trimmed string
	if (quote_type == '"')
	{
		res = expand_envs(what_is_between_quotes);// now expand envs based on quote_type
		free(what_is_between_quotes);
	}
	else
		res = what_is_between_quotes;
	return (res);

}

// expands envs as needed and removes unneeded quotes and $ (TODO: and {})
void	parse_envs_and_quotes(t_token *token)
{
	char	*text;
	int		i;
	char	*res;

	i = 0;
	text = token->value;
	res = NULL;
	if (token->type != TOKEN_TEXT)
		return ; // if it was not text our work is done and we can successfully go out of this node
	// go over the text until we encounter ' or "
	while (text[i])
	{
		while (text[i] !='\'' && text[i] != '"')
		{
			i++;
		}
		if (text[i] == '\'' || text[i] == '"') // this stotext what is inside the quotes as string and moves the index to the correct location (last string or at the end of the string)
		{
			// we append what we got before meeting quotes into text
			// TODO: handle what it does if it starts with "' so there is no first part
			char *first_part = ft_substr(text, 0, i); // store what is before the quote into a string
			char *expanded_first_part = expand_envs(first_part); // the one that is not among quotes so it definitelly will be expanded
			res = ft_strjoin(res, expanded_first_part);
			free(first_part);
			//free(expanded_first_part);
			first_part = NULL;
			expanded_first_part = NULL;
			// get what is between quote trims both the quotes and returns allocated string
			char *second_part = get_what_is_between_quotes(text[i], text, &i); // when encounter quote, this function goes over the string untill it finds another quote or end and returns the text it collected
			// if i points to null we know it was not terminated quote and can throw error
			if (!text[i]) // if when we exited the string without finding the closing quote
			{
				// free everything
				error_msg("minishell: unclosed quotes");
			}
			// if i points to the closing quote, we know it is ended and can join what was before and the value
			if (text[i] == '\'' || text[i] == '"')
			{
				// if there is something after the closing quote
				res = ft_strjoin(res, second_part);
				free(second_part);
				second_part = NULL;
				if (text[i + 1])
				{
					// set pointer to beginning of the text to what is right after the closing quote
					text = text + i + 1;
					i = 0;
				}
				// if there isnt anything after the closing quote
				else
					break ;
			}
		}
		i++;
	}

	free(token->value);
	token->value = NULL;
	token->value = res;
	// when we encounter one of them, we create a "first string" variable that stores string up to here
	// when encounter dq, do function handle_between_dq
	// this will have a while loop and will run until it encounters ", properly updating the index
	// it will expand the variables with expand_envs function
	// it will return a string that will be malloced and will not contain the last character, the i will stay on " or out of bounds
	// then we check if the current i is on " - we know the quote was ended
	// if the i is on NULL that means that the quote was not finished and we can throw error Unclosed quote
	// in this outer function we check
	// if we encounter ', call function handle_between_sq
}
