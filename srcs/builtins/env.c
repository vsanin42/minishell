/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:16 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/03 13:27:22 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// returns value of $ENV, check if it really is ENV happens before it is called
// when calling if we are calling echo $non-existent-env,
// it prints newline if it is alone, and nothing if there is anything else
// handles $NAME and prints its value eg. "Zuzka"
int	env_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**env_arr;

	i = 0;
	(void)cmd;
	env_arr = mini->env;
	while (env_arr[i])
	{
		if (ft_strchr(env_arr[i], '='))
		{
			write(1, env_arr[i], ft_strlen(env_arr[i]));
			write(1, "\n", 1);
		}
		env_arr++;
	}
	return (0);
}
