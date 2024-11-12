/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:34:38 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/12 20:24:35 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// adjusted ft_lstnew = allocates new redir struct and assigns its values
// @returns created redir node
t_redir	*create_redir(t_type type, char *value)
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

// adjusted ft_lstadd_back = appends created node to end of redir list
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

// stores tokens after redirection operators into redir list until end or pipe
// sets their type to TOKEN_FILE to not interfere with TEXT tokens
// @returns collected redir list
t_redir	*find_redirs(t_token *token)
{
	t_token	*temp;
	t_redir	*head_redir;
	t_redir	*new_redir;

	temp = token;
	head_redir = NULL;
	new_redir = NULL;
	while (temp && temp->type != TOKEN_PIPE) // probably lets do while temp and break when encountering a pipe, setting its WR/RD ends as redirs
	{
		if ((temp->type == TOKEN_REDIRIN
			|| temp->type == TOKEN_REDIROUT
			|| temp->type == TOKEN_APPEND)
			&& temp->next
			&& temp->next->type == TOKEN_TEXT)
		{
			new_redir = create_redir(temp->type, temp->next->value);
			add_back_redir(&head_redir, new_redir);
			temp->next->type = TOKEN_FILE;
		}
		temp = temp->next;
	}
	return (head_redir);
}
