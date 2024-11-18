/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:55:45 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/18 20:45:58 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_heredoc(char *limit, t_mini *mini)
{
	char	*input;
	int		len;
	int		fd;

	fd = dup(STDIN_FILENO);
	if (fd < 0)
		return (error_msg("Error opening fd", mini, 0, 0)); // handle
	len = ft_strlen(limit) + 1;
	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		if (!ft_strncmp(input, limit, len))
			break ;
		write(fd, input, ft_strlen(input));
		free(input);
	}
	close(fd);
	return (free(input), free(limit), 0);
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
			process_heredoc(limit, mini);
		}
		temp = temp->next;
	}
	free(limit);
	return (0);
}
