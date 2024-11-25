/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:54 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/25 19:45:35 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// finds cwd and stores it in malloced string, return must be freed - WORKS
// @returns malloced cwd string, NULL on failure
char	*get_current_directory(void)
{
	char	*cwd;

	cwd = (char*)malloc(PATH_MAX * sizeof(char)); // allocate memory for buffer where getcwd stores output
	if (!cwd)
		return (NULL);
	if (getcwd(cwd, PATH_MAX) == NULL)	// getcwd stores current working directory in buffer that we have to allocate
	{
		free(cwd); // free memory on failure
		return (NULL);
	}
	return (cwd);
}

// duplicates pwd - WORKS
// call for type EXECUTABLE
// @returns 0 on success, 1 on error
int	pwd_builtin(t_mini *mini, t_cmd *cmd)
{
	char	*cwd;

	(void)cmd;
	cwd = get_current_directory(); // testing finding a path when we will be expecting path type
	if (!cwd)
	{
		set_executor_error_msg(mini, "pwd", "cwd not found", NULL);
		return (ERROR);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}
