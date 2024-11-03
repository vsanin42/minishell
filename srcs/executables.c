/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executables.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 21:49:15 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/03 15:16:17 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// will run for type EXECUTABLE at the end
// check relative, absolute, and env paths for the executable or absolute or relative paths for dir changing
// fork a child process
// parent waits for child, child executes the command

int	execute(char *ex)
{
	if (!ex)
		return (ERROR);
	// handle builtins: echo -n, cd relative, cd /absolute, pwd, export, unset, env, exit
	if (!ft_strncmp(ex, "pwd", 3))
		pwd_builtin();
	else if (!ft_strncmp(ex, "cd", 2))
		cd_builtin("../pipex"); // then change directory based on actual argument, if any
	else if (!ft_strncmp(ex, "exit", 4))
		exit_builtin("4"); // then change status based on actual argument, if any
	// check user created executables - here we create processes

	// check system included executables - here we create processes
	if (get_path_env)
		printf("path: %s\n", get_path_env(ex));
}
