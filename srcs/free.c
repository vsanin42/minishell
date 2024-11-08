/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 22:05:19 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/08 22:06:53 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_token_list(t_token *token)
{
	t_token *temp;

	while (token)
	{
		temp = token;
		free(token->value);
		token = token->next;
		free(temp);
	}
	token = NULL;
}

void	free_char_pp(char **arr)
{
	char	*temp;
	char	**head;

	head = arr;
	while (*arr)
	{
		temp = *arr;
		free(temp);
		arr++;
	}
	free(head);
}

void	free_redir(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir;
		if (redir->file)
			free(redir->file);
		redir = redir->next;
		free(temp);
	}
}

void	free_cmd_list(t_cmd *node)
{
	t_cmd	*temp;

	while (node)
	{
		temp = node;
		if (node->cmd)
			free(node->cmd);
		if (node->args)
			free_char_pp(node->args);
		if (node->redir)
			free_redir(node->redir);
		node = node->next;
		free(temp);
	}
}
