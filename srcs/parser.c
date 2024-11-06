/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:35:40 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/05 23:16:40 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_ttokens_len(t_token	*token)
{
	t_token *temp;
	int	i;

	temp = token;
	i = 0;
	while (temp && temp->type == TOKEN_TEXT)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

int	set_to_text_sq(t_token	*token)
{
	t_token *temp;

	temp = token;
	while (temp && temp->type != TOKEN_SQUOTE)
	{
		temp->type = TOKEN_TEXT;
		temp = temp->next;
	}
	return (0);
}

int	set_to_text_dq(t_token	*token)
{
	t_token *temp;

	temp = token;
	while (temp && temp->type != TOKEN_DQUOTE)
	{
		temp->type = TOKEN_TEXT;
		temp = temp->next;
	}
	return (0);
}

// 1. ' : first change everything inside '' to plain text because it does not expand environment variables and treats all special characters as text as well
// 2. $ : expand envs to their true value and remove $ char (do it before checking for "" because within "" env value should be already expanded)
// 3. " : change everything between "" to plain text as now the envs have correct value and special characters are also treated as text
// 4. < > >> << : argument after redir operator is always a file, so set the type of following node to file (? and also check if it actually exists and the input is valid ?)
t_cmd	*parser(t_mini *mini, t_token *token_list)
{
	t_cmd	*parsed_list;
	//t_cmd	*new_node;
	t_token *temp;
	char	**args;
	//char	*cmd;
	//int		flag;

	parsed_list = NULL;
	temp = token_list;
	//flag = 0;
	while (temp)
	{
		if (temp->type == TOKEN_SQUOTE)
		{
			set_to_text_sq(temp->next);
		}
		expand_envs(mini, &temp);
		token_list = temp;
		if (temp->type == TOKEN_DQUOTE)
		{
			set_to_text_dq(temp->next);
		}
		if (temp->next && temp->next->type == TOKEN_TEXT && (temp->type == TOKEN_REDIRIN
			|| temp->type == TOKEN_REDIROUT || temp->type == TOKEN_APPEND))
			temp->next->type = TOKEN_FILE;





		if (temp->type == TOKEN_TEXT)
		{
			char **atemp;
			args = malloc(sizeof(char *) * (get_ttokens_len(temp) + 1));
			atemp = args;
			while (temp && temp->type == TOKEN_TEXT)
			{
				*atemp = temp->value;
				temp = temp->next;
				atemp++;
			}
			*atemp = NULL;
			//new_node = make_node();
			//cmd_add_back(&parsed_list, new_node);
			/* if (!flag)
			{
				cmd = temp->value;
				temp = temp->next;
				flag = 1;
				continue ;
			}
			else
			{
				args = malloc(sizeof(char *) * (get_ttokens_len(temp) + 1));
				while (temp && temp->type == TOKEN_TEXT)
				{
					*args++ = temp->value;
					temp = temp->next;
				}
				*args = NULL;
				//new_node = make_node();
				//cmd_add_back(&parsed_list, new_node);
				flag = 0;
			} */
		}
		// char **atemp2 = args;
		// printf("args collected from a text block:\n");
		// while (atemp2 && *atemp2)
		// {
		// 	printf("%s\n", *atemp2);
		// 	atemp2++;
		// }
		if (temp)
			temp = temp->next;
	}
	return (parsed_list);
}
