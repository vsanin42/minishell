/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 14:50:42 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/18 13:03:40 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ses_help(t_mini *mini, int *signaled, int *status, int *last_sig)
{
	if (WIFEXITED(*status))
		mini->exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		mini->exit_status = WTERMSIG(*status) + 128;
		*signaled = 1;
		*last_sig = WTERMSIG(*status);
	}
}

void	ses_init(int *signaled, int *i, int *status, int *last_sig)
{
	*signaled = 0;
	*i = 0;
	*status = 0;
	*last_sig = 0;
}

int	set_exit_status(int num_of_p, t_mini *mini, int *pids)
{
	int	i;
	int	status;
	int	signaled;
	int	last_sig;

	ses_init(&signaled, &i, &status, &last_sig);
	while (i < num_of_p)
	{
		waitpid(pids[i], &status, 0);
		ses_help(mini, &signaled, &status, &last_sig);
		i++;
	}
	if (signaled)
	{
		if (last_sig == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		else if (last_sig == SIGQUIT)
			write(STDERR_FILENO, "Quit\n", 5);
	}
	return (mini->exit_status);
}
