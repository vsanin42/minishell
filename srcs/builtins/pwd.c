/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:54 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/15 13:08:02 by vsanin           ###   ########.fr       */
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
		return (mini_error(mini, create_msg("minishell",
					"pwd", "cwd not found", NULL), 2));
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}
