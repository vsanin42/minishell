/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:34:44 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/10 16:02:46 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// frees the program struct and possibly 2 strings so we save lines on norm
// writes error message to stderr
// @returns ERROR (1) as status code
// @param msg message describing the error
// @param mini struct containing all program-wide data needed to be freed
// @param str_to_free 3rd and 4th param that free a string and set it to NULL
int	error_msg(char *msg, t_mini *mini, char *str_to_free_1, char *str_to_free_2)
{
	if (mini)
	{
		if (mini->token_list)
			free_token_list(mini);
		if (mini->cmd_list)
			free_cmd_list(mini);
		mini->token_list = NULL;
		mini->cmd_list = NULL;
	}
	free(str_to_free_1);
	str_to_free_1 = NULL;
	free(str_to_free_2);
	str_to_free_2 = NULL;
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (ERROR);
}

void	s_error_msg(char *msg)
{
	printf("%s\n", msg);
}

void	validator_msg(t_mini *mini, char *object, char *msg)
{
	(void)mini;
	printf("minishell: %s: %s\n", object, msg);
}

int	mini_perror(t_mini *mini, char *msg)
{
	int	result;

	result = errno;
	mini->exit_status = errno;
	if (msg)
		perror(msg);
	else
		perror("minishell");
	free(msg);
	return (result);
}

void	mini_error(t_mini *mini, char *first, char *second, char *third)
{
	char	*msg;
	char	*old_msg;
	char	*new_part;

	(void)mini;
	msg = ft_strdup("minishell: ");
	if (first)
	{
		old_msg = msg;
		new_part = ft_strdup(first);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	if (second)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(second);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	if (third)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(third);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	free(msg);
}

// @returns error message that will be printed before perror message,must free!
char	*perror_msg(char *first, char *second, char *third, char *fourth)
{
	char *msg;
	char *old_msg;
	char *new_part;

	msg = "";
	old_msg = NULL;
	new_part = NULL;
	if (first)
	{
		new_part = ft_strdup(first);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
		}
	}
	if (second)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(second);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	if (third)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(third);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	if (fourth)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(fourth);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	return (msg);
}
