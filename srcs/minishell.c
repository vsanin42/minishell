/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/10/27 23:49:26 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_received = 0; // global variable to track received signal (1 max)

// default handler function definition in sigaction struct, must return void
void	sighandle(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line(rl_line_buffer, 0);
		rl_redisplay();
	}
}

int main(int argc, char *argv[], char *env[])
{
    char	*input;
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sighandle;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL); // ctrl c
	sigaction(SIGQUIT, &sa, NULL); // ctrl '\'
	(void)argc; (void)argv; (void)env; 
	while (1)
	{
		input = readline("minishell > ");
		if (!input || input[0] == '\0') // !input == EOF (ctrl d), or if empty input/enter
		{
			free(input);
			if (!input)
				break ;
			continue ;
		}
		printf("%s\n", input);
		add_history(input);
		free(input);
	}
	rl_clear_history();
	return (0);
}