/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:50:38 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/12 20:32:30 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

// prints the token list in format: value	type
void	print_token_list(t_mini *mini)
{
	t_token *temp;

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
void print_command_list(t_mini *mini)
{
	printf("commands:\n\n");
	t_cmd *temp = mini->cmd_list;
	char **atemp2;
	t_redir *aredir;
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
