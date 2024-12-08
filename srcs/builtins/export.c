/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/09 20:52:21 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// gets index of env
// shouldn't return ERROR because it = 1 and it could be confused with index
// @returns index from env array,-2 on error in get_env_name to diff from ERROR
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

// checks if env we're attemping to add is alnum/_/doesn't start with a number
// checks if it contains a value after =
// @returns: 0 if OK
// @returns: ERROR if identifier is invalid
// @returns: -1 if found no '=' (don't do anything)
int	export_check_env(char *env)
{
	int	i;

	i = 0;
	if (env[i] >= '0' && env[i] <= '9')
		return (ERROR);
	while (env[i] && env[i] != '=')
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (ERROR);
		i++;
	}
	if (env[i] == '\0')
		return (-1);
	return (0);
}

// adds the env variable to the array and updates it
// if check_res is -1 meaning there was only name, we still want to keep in in env array, just do not print with env - it is handled there
int	export_add_back(t_mini *mini, char *env)
{
	char	**temp;
	int		check_res;

	temp = NULL;
	check_res = export_check_env(env);
	if (check_res == ERROR)
		return (ERROR);
	temp = add_back_array(mini->env, env);
	if (!temp)
		return (ERROR);
	free_arr(mini->env);
	mini->env = temp;
	return (0);
}

// appends variable into mini->env array
// if index is -1 it means it was just env name with no value
// in this case we want to keep the env in array, just not print it
// if there was env with the name anc value adn we wxport jut the same name, it does nothing
// if index returned is -2, error occured -> ERROR (preserves behaviour)
int	export_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**vars;
	int		index;

	i = 1;
	if (!cmd->args)
		return (ERROR);
	vars = cmd->args;
	if (vars[1] == NULL)
		return (env_builtin(mini, cmd, "declare -x "));
	while (vars[i])
	{
		index = export_get_index(mini, vars, i);
		if (index == -2)
			return (ERROR);
		if (index > -1)
		{
			if (export_check_env(vars[i]) == 0)
			{
				if (!change_arr_element(mini->env, vars[i], index))
					return (ERROR);
			}
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
