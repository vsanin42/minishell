/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:34:38 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/08 17:54:49 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_back_redir(t_redir **lst, t_redir *new)
{
	t_redir	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_redir	*create_redir(t_token_type type, char *value)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(value);
	redir->next = NULL;
	return (redir);
}

t_redir	*find_redirs(t_token *token)
{
	t_token	*temp;
	t_redir	*head_redir;
	t_redir	*new_redir;

	temp = token;
	head_redir = NULL;
	new_redir = NULL;
	while (temp && temp->type != TOKEN_PIPE)
	{
		if ((temp->type == TOKEN_REDIRIN
			|| temp->type == TOKEN_REDIROUT || temp->type == TOKEN_APPEND)
			&& temp->next && temp->next->type == TOKEN_TEXT)
		{
			new_redir = create_redir(temp->type, temp->next->value);
			add_back_redir(&head_redir, new_redir);
			temp->next->type = TOKEN_FILE;
		}
		temp = temp->next;
	}
	return (head_redir);
}

void	init_cmd_node(t_cmd *node)
{
	node->cmd = NULL;
	node->args = NULL;
	node->redir = NULL;
	node->next = NULL;
}
