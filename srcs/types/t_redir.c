/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_redir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:34:38 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/25 20:02:06 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks if we encountered redirection operator and can create new redir node
// @returns 1 if we are on operator token and have text token after it, 0 if no
int	new_redir_condition(t_token *token)
{
	if 	((token->type == TOKEN_REDIRIN
		|| token->type == TOKEN_REDIROUT
		|| token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC)
		&& token->next
		&& token->next->type == TOKEN_TEXT)
		return (1);
	return (0);
}

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
// and sets TOKEN_TYPE to FILE to not interfere with TEXT tokens
// if meets pipe, sets it as WR end in redir list last element and exits
// @returns collected redir list for each commmand
// @param token token from which we start collecting redir info
// @param previous exists if we had a pipe before our new command
t_redir	*find_redirs(t_token *token)
{
	t_token	*n;
	t_redir	*head_redir;

	n = token;
	head_redir = NULL;
	while (n)
	{
		if (new_redir_condition(n))
		{
			add_back_redir(&head_redir, create_redir(n->type, n->next->value));
			n->next->type = TOKEN_FILE;
		}
		if (n->type == TOKEN_PIPE)
			break ;
		n = n->next;
	}
	return (head_redir);
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
