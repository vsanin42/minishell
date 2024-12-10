/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/10 16:43:41 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// adds string env into mini->env array
// checks if string is already there -> change it
// if string is not there -> append it
// if name is in wrong format, print error
// if it is only name without value and exists, do not exchange, keep original
// if it is only name without value and does not exist, append to array
// but it is not printed with "env", only with "export"
int	export_each_arg(t_mini *mini, char *env, char *env_name)
{
	int		index;
	char	**temp;

	temp = NULL;
	index = get_env_index_by_name(mini->env, env_name);
	if (index > -1)
	{
		if (has_env_value(env))
			temp = change_arr_element(mini->env, env, index);
	}
	else
	{
		temp = add_back_array(mini->env, env);
		if (!temp)
			return (ERROR);
		free_arr(mini->env);
		mini->env = temp;
	}
	return (0);
}

// iterates over arguments passed to export and adds them to env array
// with function export_each_arg and conditions there
// @returns 0 on success, 1 on error adn prints error message
int	export_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**arg;
	char	*env_name;

	i = 1;
	if (!cmd->args)
		return (ERROR);
	arg = cmd->args;
	if (arg[1] == NULL)
		return (env_builtin(mini, cmd, "declare -x "));
	while (arg[i])
	{
		env_name = extract_env_name(arg[i]);
		if (!env_name)
			return (ERROR);
		if (check_env_name(env_name) == ERROR)
			return (mini->exit_status = 1, free(env_name), mini_error(mini,
					mini->cmd_list->cmd, arg[i], "not a valid identifier"), 1);
		if (export_each_arg(mini, arg[i], env_name) == ERROR)
			return (free(env_name), ERROR);
		free(env_name);
		i++;
	}
	return (0);
}
