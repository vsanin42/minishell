/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/03 16:33:46 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// gets index of env
// shouldn't return ERROR because it = 1 and it could be confused with index
// @returns: -2 if error in get_env_name to differentiate from ERROR
// @returns: index from env array
int	export_get_index(t_mini *mini, char **vars, int i)
{
	char	*env_name;
	int		index;

	env_name = get_env_name(vars[i]);
	if (!env_name)
		return (-2);
	index = get_env_index(mini->env, env_name);
	free(env_name);
	return (index);
}

// adds the env variable to the array and updates it
int	export_add_back(t_mini *mini, char *env)
{
	char	**temp;

	temp = NULL;
	temp = add_back_array(mini->env, env);
	if (!temp)
		return (ERROR);
	free_arr(mini->env);
	mini->env = temp;
	return (0);
}

// appends variable into mini->env array
// if index returned is -2, error occured -> ERROR (preserves behaviour)
int	export_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**vars;
	int		index;

	i = 1;
	vars = cmd->args;
	while (vars[i])
	{
		index = export_get_index(mini, vars, i);
		if (index == -2)
			return (ERROR);
		if (index > -1)
		{
			if (!change_arr_element(mini->env, vars[i], index))
				return (ERROR);
		}
		else
		{
			if (export_add_back(mini, vars[i]) == ERROR)
				return (ERROR);
		}
		i++;
	}
	return (0);
}
