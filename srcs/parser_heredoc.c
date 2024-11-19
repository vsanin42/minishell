/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:55:45 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/19 21:52:22 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal = 0;

void	heredoc_handler(int sig) // WIP
{
	if (sig == SIGINT)
	{
		// g_signal = SIGINT;
		// write(STDOUT_FILENO, "\n", 1);
		
		// close(STDIN_FILENO);
		// write(STDERR_FILENO, "\n", 1);
		
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// frees ONLY S1 - to avoid using oldres and similar things
char	*str_append_nl(char *s1, char *s2)
{
	char	*tmp;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	tmp = ft_strjoin(s1, s2);
	free(s1);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, "\n");
	free(tmp);
	if (!res)
		return (NULL);
	return (res);
}

char	*heredoc_readline(char *limit)
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
		res = str_append_nl(res, input);
		if (!res)
			return (free(input), NULL); // null or break?
		free(input);
		input = readline("> ");
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			break ;
		}
	}
	free(input);
	return (res);
}

void	free_memo(void *mem_seg)
{
	if (mem_seg)
		free(mem_seg);
}

int	process_heredoc(t_token *token, char *limit, t_mini *mini)
{
	(void)mini;
	//int		fd;
	//fd = dup(STDIN_FILENO);
	//fd = open();
	// if (fd < 0)
	// 	return (error_msg("Error opening fd", mini, 0, 0)); // handle
	free(token->value);
	token->value = heredoc_readline(limit);
	//close(fd);
	// free_memo((void *)limit);
	return (0);
}

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
				return (error_msg("Error: expected delimiter after <<", mini, 0, 0)); // handle
			limit = ft_strdup(limit_token->value);
			process_heredoc(limit_token, limit, mini);
			free_memo((void *)limit);
		}
		temp = temp->next;
	}
	return (0);
}
