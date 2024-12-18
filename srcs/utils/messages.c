/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:34:44 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/18 13:03:58 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// frees the program struct and possibly 2 strings so we save lines on norm
// writes error message to stderr
// @returns ERROR (1) as status code
// @param msg message describing the error
// @param mini struct containing all program-wide data needed to be freed
// @param str_to_free 3rd and 4th param that free a string and set it to NULL
int	error_msg(char *msg, t_mini *mini, char *str_1, char *str_2)
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
	free(str_1);
	str_1 = NULL;
	free(str_2);
	str_2 = NULL;
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	return (ERROR);
}

// takes old string, appends the new word, frees the old one
// @returns the new joined string
char	*append_word(char *msg, char *word)
{
	char	*old_msg;
	char	*new_part;

	old_msg = msg;
	new_part = ft_strdup(word);
	if (new_part)
	{
		msg = ft_strjoin(msg, new_part);
		free(new_part);
		new_part = NULL;
		free(old_msg);
		old_msg = NULL;
	}
	return (msg);
}

// @returns error message that will be printed before perror message,must free!
char	*create_msg(char *first, char *second, char *third, char *fourth)
{
	char	*msg;

	msg = ft_strdup("");
	if (first)
		msg = append_word(msg, first);
	if (second)
	{
		msg = append_word(msg, ": ");
		msg = append_word(msg, second);
	}
	if (third)
	{
		msg = append_word(msg, ": ");
		msg = append_word(msg, third);
	}
	if (fourth)
	{
		msg = append_word(msg, ": ");
		msg = append_word(msg, fourth);
	}
	return (msg);
}
