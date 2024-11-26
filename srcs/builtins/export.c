/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/26 14:34:17 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// appends variable into mini->env array
int	export_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**vars;
	char	**temp;
	char	*env_name;
	int		index;

	i = 1; // start at 1 because the 0th argument is the command itself
	temp = NULL;
	vars = cmd->args;
	while (vars[i])
	{
		env_name = get_env_name(vars[i]);
		if (!env_name)
			return (ERROR);
		index = get_env_index(mini->env, env_name);
		free(env_name);
		env_name = NULL;
		if (index > -1)
		{
			if (!change_arr_element(mini->env, vars[i], index))
				return (ERROR);
		}
		else
		{
			temp = add_back_array(mini->env, vars[i]);
			if (!temp)
				return (ERROR);
			free_arr(mini->env);
			mini->env = temp;
		}
		i++;
	}
	return (0);
}
