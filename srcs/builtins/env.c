/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:16 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/15 13:07:41 by vsanin           ###   ########.fr       */
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
	char	**envs;

	i = 0;
	envs = mini->env;
	if (cmd->args[1])
		return (mini_error(mini, create_msg("env", cmd->args[1],
					"No such file or directory", NULL), 127));
	while (envs[i])
	{
		if (prefix)
			write(1, prefix, ft_strlen(prefix));
		if (has_env_value(envs[i]) || ft_strchr(envs[i], '=') || prefix)
		{
			write(1, envs[i], ft_strlen(envs[i]));
		}
		if (prefix && !has_env_value(envs[i]) && (ft_strchr(envs[i], '=')))
			write(1, "\"\"", 2);
		write(1, "\n", 1);
		envs++;
	}
	return (0);
}
