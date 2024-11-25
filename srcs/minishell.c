/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/25 13:57:58 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_input(char *input, t_mini *mini)
{
	mini->token_list = lexer(input);
	mini->token_list = remove_null_tokens(mini->token_list);
	//print_token_list(mini);
	parser_heredoc(mini);
	mini->cmd_list =  parser(mini);
	free_token_list(mini->token_list);
	//print_command_list(mini);
	if (evaluator(mini) == 0)
	{
		if (executor(mini) == ERROR)
			return (ERROR);
	}
	free_cmd_list(mini->cmd_list);
	return (0);
}

// called in loop to show a prompt and process input
int	show_prompt(t_mini *mini)
{
	char	*input;

	input = readline("\033[32mminishell \033[37m> ");
	if (!input) // if ctrl d was pressed, exit the process
		return (0); // needs proper exit in the future
	if (input[0] == '\0') // if empty input/enter
	{
		free(input);
		return (1); // if enter (empty input) was pressed, continue to the next iteration
	}
	add_history(input);
	if (check_input(input) == 1)
		return (free(input), 1);
	process_input(input, mini); // called without assigning, just for testing

	return (1);
}

void	set_termios() // terminal config editing to revent '^\' from being printed
{
	struct termios	termios;

	if (tcgetattr(0, &termios) == -1) // gets stdin settings, exits if fail
		exit(1);
	termios.c_cc[VQUIT] = _POSIX_VDISABLE; // only blocks '\' bc ctrl c prints ^C in bash
	if ((tcsetattr(0, TCSANOW, &termios)) == -1) // sets stdin settings immediately
		exit(1);
}

void	init_mini_env(t_mini *mini, char **env)
{
	int		i;
	int		j;
	char	**envs;

	i = 0;
	while (env[i])
		i++;
	envs = malloc(sizeof(char *) * (i + 1));
	if (!envs)
		return ;
	j = -1;
	while (++j < i)
	{
		envs[j] = ft_strdup(env[j]);
		if (!envs[j])
		{
			while (j > 0)
				free(envs[--j]);
			free(envs);
			envs = NULL;
			return ;
		}
	}
	envs[j] = NULL;
	mini->env = envs;
}

int main(int argc, char *argv[], char *env[])
{
	t_mini	mini;

	mini.token_list = NULL;
	mini.cmd_list = NULL;
	mini.error_msg = NULL;
	mini.env = NULL;
	init_mini_env(&mini, env);
	signal(SIGINT, sig_handler); // ctrl c
	signal(SIGQUIT, sig_handler); // ctrl '\'
	(void)argv; (void)env;
	set_termios();
	if (argc != 1)
		error_msg("Too many arguments. Use: ./minishell", NULL, NULL, NULL);
	while (1)
		if (show_prompt(&mini) == 0) // if ctrl d, break the loop, clear history and return
		{
			free_char_pp(mini.env);
			break ;
		}
	rl_clear_history();
	return (0);
}
