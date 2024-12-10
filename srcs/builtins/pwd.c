/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:54 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/10 15:54:08 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// finds cwd and stores it in malloced string, return must be freed - WORKS
// @returns malloced cwd string, NULL on failure
char	*get_current_directory(void)
{
	char	*cwd;

	cwd = (char *)malloc(PATH_MAX * sizeof(char));
	if (!cwd)
		return (NULL);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		free(cwd);
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
	cwd = get_current_directory();
	if (!cwd)
	{
		mini_error(mini, "pwd", "cwd not found", NULL);
		return (ERROR);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}
