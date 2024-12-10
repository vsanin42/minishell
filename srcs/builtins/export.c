/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/10 14:28:58 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// adds string env into mini->env array
// checks if string is already there -> change it
// if string is not there -> append it
// if name is in wrong format, print error
// if it is only name without value and exists, do not exchange keep the original env
// if it is only name without value and does not exist, append to array
// but it is not printed with "env", only with "export"
// @returns 0 on success, 1 on error adn prints error message
int	export_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**vars;
	int		index;
	char	*env_name;
	char	**temp;

	temp = NULL;
	i = 1;
	if (!cmd->args)
		return (ERROR);
	vars = cmd->args;
	if (vars[1] == NULL)
		return (env_builtin(mini, cmd, "declare -x "));
	while (vars[i])
	{
		env_name = extract_env_name(vars[i]);
		if (!env_name)
			return (ERROR);
		if (check_env_name(env_name) == ERROR)
			return (mini_error(mini, cmd->cmd, vars[i], "not a valid identifier"), mini->exit_status = 1, 1);
		index = get_env_index_by_name(mini->env, env_name);
		free(env_name);
		if (index > -1)
		{
			if (has_env_value(vars[i]) == 0)
				temp = change_arr_element(mini->env, vars[i], index);
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
