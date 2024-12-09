/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:16 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/09 13:42:48 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// returns value of $ENV, check if it really is ENV happens before it is called
// when calling if we are calling echo $non-existent-env,
// it prints newline if it is alone, and nothing if there is anything else
// handles $NAME and prints its value eg. "Zuzka"
// if called without prefix its from env builtin so print only envs with value
// if called with prefix its from export with no args so print prefix+all envs
int	env_builtin(t_mini *mini, t_cmd *cmd, char *prefix)
{
	int		i;
	char	**env_arr;

	i = 0;
	env_arr = mini->env;
	if (cmd->args[1])
		return (mini_error(mini, "env", cmd->args[1], "No such file or directory"), 127);
	printf("cmd: %s\n", cmd->cmd);
	while (env_arr[i])
	{
		if (prefix)
			write(1, prefix, ft_strlen(prefix));
		if (ft_strchr(env_arr[i], '=') || prefix)
		{
			write(1, env_arr[i], ft_strlen(env_arr[i]));
			write(1, "\n", 1);
		}
		env_arr++;
	}
	return (0);
}
