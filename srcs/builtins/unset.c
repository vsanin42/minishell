/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:34 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/26 14:39:38 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**remove_arr_element(char **env, int index) // env, 0, len=3
{
	char	**result;
	int		i;
	int		len;

	i = 0;
	len = get_arr_len(env);
	if (index >= len)
		return (env);
	result = malloc(sizeof(char *) * len);
	if (!result)
		return (NULL);
	while (env[i] && i < index)
	{
		result[i] = ft_strdup(env[i]);
		if (!result)
			return (NULL);
		i++;
	}
	while (env[i + 1] && i < len - 1)
	{
		result[i] = ft_strdup(env[i + 1]);
		if (!result)
			return (NULL);
		i++;
	}
	result[i] = NULL;
	return (result);
}

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
