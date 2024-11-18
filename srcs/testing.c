/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:50:38 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/18 15:47:51 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// prints the token list in format: value	type
void	print_token_list(t_mini *mini)
{
	t_token	*temp;

	temp = mini->token_list;
	printf("tokens from input: \n");
	while (temp)
	{
		printf("%s\t", temp->value);
		printf("type: %d\n", temp->type);
		temp = temp->next;
	}
}

// prints the command list in format: cmd\n, arguments\n, redir\n, redir->type\n, /...new_command/
void	print_command_list(t_mini *mini)
{
	t_cmd	*temp;
	t_redir	*aredir;
	char	**atemp2;

	temp = mini->cmd_list;
	printf("\ncommands:\n");
	while (temp)
	{
		printf("cmd name:\t%s\n", temp->cmd);
		atemp2 = temp->args;
		aredir = temp->redir;
		while (atemp2 && *atemp2)
		{
			printf("argument:\t%s\n", *atemp2);
			atemp2++;
		}
		while (aredir)
		{
			printf("redir file:\t%s\n", aredir->file);
			printf("redir type:\t%d\n", aredir->type);
			aredir = aredir->next;
		}
		printf("---------------------------\n");
		temp = temp->next;
	}
}

	/* 	testing current directory - WORKS */
	/* char *cwd = get_current_directory(); // testing finding a path when we will be expecting path type
		printf("cwd: %s\n", cwd);
		free(cwd); */

	/*	testing getting command paths - WORKS */
	/* 	char *path = get_path_env(input);
	if (path)
		printf("path: %s\n", path);
	else
		printf("Path does not exist"); */

	/* testing cd_builtin - WORKS  */
	/* 	printf("cd: %d\n", cd_builtin(input));
	char *cwd = get_current_directory(); // testing finding a path when we will be expecting path type
	printf("cwd: %s\n", cwd);
	free(cwd); */


	/* 	testing builtin pwd with no options - WORKS
	pwd_builtin(); */

	/* 	testing exit  - WORKS */
	/* 	if (input)
		exit_builtin(input); */


	/* testing is_executable/is_readable - WORKS
	printf("exec: %d\n", is_readable_file(input)); */

	/* testing if input is being redirected properly - WORKS  */
	/* 	if (redirect_input(input) == -1)
	{
		perror("Error redirecting input hh");
		return 1;  // Exit or handle error appropriately
	}
	char buff[50];
	ssize_t bytesRead = read(STDIN_FILENO, buff, sizeof(buff) - 1);
	if (bytesRead == -1) {
		perror("Error reading from redirected input");
		return 1;  // Handle the read error appropriately
	}
	buff[bytesRead] = '\0';  // Null-terminate the string to safely print
	printf("buff: %s\n", buff); */
