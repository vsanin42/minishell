/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:35:40 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/08 18:22:32 by vsanin           ###   ########.fr       */
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

char	**alloc_args(char **args, t_token *token)
{
	int len;
	
	len = get_ttokens_len(token->next);
	printf("number of text nodes ahead: %d\n", len);
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
	char **args_head;

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
