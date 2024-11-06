/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:35:40 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/06 19:50:34 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_ttokens_len(t_token	*token)
{
	t_token *temp;
	int	i;

	temp = token;
	i = 0;
	while (temp && temp->type != TOKEN_PIPE)
	{
		if (temp->type == TOKEN_TEXT)
			i++;
		temp = temp->next;
	}
	return (i);
}

void	init_cmd_node(t_cmd *node)
{
	node->cmd = NULL;
	node->args = NULL;
	node->redir = NULL;
	node->next = NULL;
}

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
	while (temp)
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

t_cmd	*new_cmd(t_token *token) // creating one command node
{
	t_cmd	*node;
	char	**args;
	char 	**atemp;

	args = NULL;
	atemp = NULL;
	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	init_cmd_node(node);
	node->redir = find_redirs(token);
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_TEXT) // upon finding the first text token
		{
			if (!node->cmd)
			{
				node->cmd = ft_strdup(token->value); // first value = command name, allocated
				if (get_ttokens_len(token->next) > 0)
				args = malloc(sizeof(char *) * (get_ttokens_len(token->next) + 1)); // allocate for the number of following text tokens
				if (!args)
					return (NULL);
				atemp = args;
			}
			else
			{
				*atemp = ft_strdup(token->value);
				atemp++;
			}
		}
		token = token->next;
	}
	if (atemp)
		*atemp = NULL; 
	node->args = args;
	return (node);
}

void	add_back_cmd(t_cmd **lst, t_cmd *new)
{
	t_cmd	*temp;

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

t_cmd	*parser(t_mini *mini, t_token *token_list)
{
	t_cmd	*parsed_list;
	t_cmd	*new_node;
	t_token *temp;

	(void)mini;
	parsed_list = NULL;
	temp = token_list;
	while (temp)
	{
		new_node = new_cmd(temp);
		if (!new_node)
			return (NULL);
		add_back_cmd(&parsed_list, new_node);
		while (temp && temp->type != TOKEN_PIPE) 
    		temp = temp->next;
		if (temp)
		 	temp = temp->next;
	}
	return (parsed_list);
}

// 1. ' : first change everything inside '' to plain text because it does not expand environment variables and treats all special characters as text as well
// 2. $ : expand envs to their true value and remove $ char (do it before checking for "" because within "" env value should be already expanded)
// 3. " : change everything between "" to plain text as now the envs have correct value and special characters are also treated as text
// 4. < > >> << : argument after redir operator is always a file, so set the type of following node to file (? and also check if it actually exists and the input is valid ?)
