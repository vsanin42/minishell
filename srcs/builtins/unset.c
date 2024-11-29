/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:34 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/29 10:32:33 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// i starts at 1 because the 0th argument is the command itself
int	unset_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**vars;
	char 	**res;
	char	*env_name;
	int		index;

	i = 0;
	vars = cmd->args;
	while (vars[++i])
	{
		env_name = get_env_name(vars[i]);
		if (!env_name)
			return (ERROR);
		index = get_env_index(mini->env, env_name);
		free(env_name);
		env_name = NULL;
		if (index > -1)
		{
			res = remove_arr_element(mini->env, index);
			if (!res)
				return (ERROR);
			free_arr(mini->env);
			mini->env = res;
		}
	}
	return (0);
}
