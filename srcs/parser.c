/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:35:40 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/12 00:09:34 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// takes token from token list and counts following text tokens in that command
// @returns number of text tokens after token from parameter
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


char	**alloc_args(char **args, t_token *token)
{
	int len;

	len = get_ttokens_len(token->next);
	if (!args)
	{
		if (len > 0)
		{
			args = malloc(sizeof(char *) * (len + 1)); // allocate for the number of following text tokens
			if (!args)
				return (NULL);
			args[len] = NULL;
		}
	}
	return (args);
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

t_cmd	*new_cmd(t_token *token)
{
	t_cmd	*node;
	char	**args;
	char	**args_head;

	args = NULL;
	args_head = NULL;
	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	init_cmd_node(node);
	node->redir = find_redirs(token);
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_TEXT && !node->cmd)
		{
			node->cmd = ft_strdup(token->value);
			args = alloc_args(args, token);
			args_head = args;
		}
		else if (token->type == TOKEN_TEXT && node->cmd)
			*args++ = ft_strdup(token->value);
		token = token->next;
	}
	node->args = args_head;
	return (node);
}

t_cmd	*parser(t_mini *mini)
{
	t_cmd	*parsed_list;
	t_cmd	*new_node;
	t_token *temp;

	parsed_list = NULL;
	temp = mini->token_list;
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
	//free_token_list(mini->token_list); // can we free it in the process input function so all functions(lexer, parser, .., execution), assignment to mini, frees are called in one place?
	return (parsed_list);
}
