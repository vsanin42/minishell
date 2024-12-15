/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:55:45 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/15 13:56:58 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// trims the limiter to make sure heredoc stops when it finds
// the limiter string without quotes just like bash
// also sets the expand flag to NO_EXP (don't expand)
// @returns: limiter without one of the two quote kinds
char	*trim_quotes_in_str(char *str, int *expand_flag, t_mini *mini)
{
	char	*new;
	char	*oldstr;

	oldstr = NULL;
	new = NULL;
	(void)mini;
	if (str[0] == '\'')
	{
		oldstr = str;
		new = ft_strtrim(str, "'");
		free(oldstr);
	}
	else if (str[0] == '"')
	{
		oldstr = str;
		new = ft_strtrim(str, "\"");
		free(oldstr);
	}
	*expand_flag = NO_EXP;
	return (new);
}

// takes the trimmed limiter and the corresponding flag
// sets its own signal handler each time it's called
// while the input is not EOF and doesn't match the limiter:
// -if the limiter was not trimmed, perform the expansion on each readline input
// -append the input to an existing string + append \n
// -free the old appended input and continue
// restores the signal handler back to normal behaviour once the input ends
// frees the input again because readline() is the last thing in the loop
// @returns: string of connected readline inputs joined by newlines
char	*heredoc_readline(t_mini *mini, char *limit, int *expand_flag)
{
	char	*res;
	char	*input;

	signal(SIGINT, heredoc_handler);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	input = readline("> ");
	while (input && ft_strncmp(input, limit, ft_strlen(limit) + 1))
	{
		if (*expand_flag == EXP)
			input = heredoc_expand(mini, input);
		res = str_append_nl(res, input);
		if (!res)
			return (free(input), NULL); // null or break?
		free(input);
		input = readline("> ");
	}
	signal(SIGINT, sig_handler);
	free(input);
	return (res);
}

// checks what the passed limit starts with
// if it starts with quotes, repeat the trimming process until none remain
// strlen check ensures no weird behaviour of strtrim
// '' and "" also technically need to be trimmed to empty string
// the resulting old limiter value is then strduped into new
// otherwise simply strdup the current value into new
// in both cases free the limit so it doesn't need freeing outside
// @returns: new limiter, either trimmed or with no changes
char	*make_new_limit(char *limit, int *expand_flag, t_mini *mini)
{
	char	*new;

	if (limit[0] == '\'' || limit[0] == '"')
	{
		while ((limit[0] == '\'' || limit[0] == '"') && ft_strlen(limit) > 1)
			limit = trim_quotes_in_str(limit, expand_flag, mini);
		new = ft_strdup(limit);
		free(limit);
	}
	else
	{
		new = ft_strdup(limit);
		free(limit);
	}
	return (new);
}

// the default expand flag is EXP (expand)
// this flag will only change if the limiter has quotes
// then the input shouldn't expand
// fd is the result of calling dup(STDIN_FILENO) to have a restore point
// new_limit is introduced to avoid double frees
// token->value is freed to get ready to be replaced by heredoc_readline()
// if ctrl+c is called, the handler will close STDIN,
// resulting in EBADF - bad file number error caused by readline not having
// anything to read from
// it is then restored via dup2()
// this will break if memory isn't allocated in ft_strtrim
int	process_heredoc(t_token *token, char *limit, t_mini *mini)
{
	int		fd;
	int		expand_flag;
	char	*new_limit;

	new_limit = NULL;
	expand_flag = EXP;
	fd = heredoc_dup(mini);
	new_limit = make_new_limit(limit, &expand_flag, mini);
	free(token->value);
	token->value = heredoc_readline(mini, new_limit, &expand_flag);
	free(new_limit);
	if (errno == EBADF)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			s_error_msg("Bad file descriptor"); // handle
		close(fd);
		return (ERROR);
	}
	close(fd);
	return (0);
}

// takes mini and its token list, traverses it until heredoc is found
// that means that the next token must be the delimiter
// extract the value from the delimiter token to compare against
// pass the token to process_heredoc() to have its value be replaced
// by user input based on the extracted delimiter
int	parser_heredoc(t_mini *mini)
{
	t_token	*temp;
	t_token	*limit_token;
	char	*limit;

	temp = mini->token_list;
	while (temp)
	{
		if (temp->type == TOKEN_HEREDOC)
		{
			limit_token = temp->next;
			if (!limit_token)
				return (ERROR);
			limit = ft_strdup(limit_token->value);
			process_heredoc(limit_token, limit, mini);
		}
		temp = temp->next;
	}
	return (0);
}
