/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 22:05:19 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/12 16:27:18 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// util function that can free 4 allocated strings and set them to NULL pointer
// helps us save space for norm
// if we do not hve all 4 strings to free, just pass in NULL as empty parameter
void	free_four_mallocs(char *s1, char *s2, char *s3, char *s4)
{
	free(s1);
	free(s2);
	free(s3);
	free(s4);
	s1 = NULL;
	s2 = NULL;
	s3 = NULL;
	s4 = NULL;
}

// frees the token list
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

// frees array of strings
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

// frees redir struct
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

// frees the command list
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
