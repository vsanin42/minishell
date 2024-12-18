/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:54:34 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/18 13:32:45 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_dup(t_mini *mini)
{
	int	fd;

	fd = dup(STDIN_FILENO);
	if (fd < 0)
		return (error_msg("minishell: Error opening fd", mini, 0, 0));
	return (fd);
}

// sighandler that gets enabled during the heredoc loop
// if ctrl+c is detected, the stdin is closed
// it is then restored via duplicated fd in process_heredoc()
void	heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		close(STDIN_FILENO);
		write(STDERR_FILENO, "\n", 1);
	}
}

// takes the input from heredoc readline and expands the value
// @returns: input with expanded variable regardless of quotes
char	*heredoc_expand(t_mini *mini, char *str)
{
	char	*tmp;

	tmp = str;
	str = get_env_value_to_process(mini, str);
	free(tmp);
	return (str);
}
