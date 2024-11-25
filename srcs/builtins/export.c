/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/25 20:30:37 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	change_env(char **envs, char *new_env, char *new_env_name)
// {
// 	int		i;
// 	char	*curr_env_name;
// 	//char	**second_part_of_arr;

// 	if (!envs || !(*envs) || !new_env)
// 		return (ERROR);
// 	i = 0;
// 	printf("exchanging\n");
// 	while (envs[i])
// 	{
// 		curr_env_name = get_env_name(envs[i]);
// 		if (!curr_env_name)
// 			return (ERROR);
// 		if (!ft_strncmp(curr_env_name, new_env_name, ft_strlen(curr_env_name)))
// 		{
// 			free(curr_env_name);
// 			curr_env_name = NULL;
// 			//second_part_of_arr = envs;
// 			//second_part_of_arr++;
// 			free(envs[i]);
// 			envs[i] = ft_strdup(new_env);
// 			if (!envs[i]) // Handle strdup failure
// 			{
// 				free_arr(envs);
// 				//free_arr(second_part_of_arr);
// 				return (ERROR);
// 			}
// 			return (0); // Successfully replaced the variable
// 		}
// 		free(curr_env_name);
// 		i++;
// 	}
// 	return (ERROR);
// }

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
		if (index > -1)
		{
			if (!change_arr_element(mini->env, vars[i], index))
			{
				free(env_name);
				env_name = NULL;
				return (ERROR);
			}
		}
		else
		{
			temp = add_back_array(mini->env, vars[i]);
			if (temp)
			{
				free_arr(mini->env);
				mini->env = temp;
			}
		}
		free(env_name);
		env_name = NULL;
		i++;
	}
	return (0);
}
