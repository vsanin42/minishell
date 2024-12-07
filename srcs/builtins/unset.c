/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:34 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/07 11:29:16 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	unset_strdup(char **result, char **env, int *i, int flag)
{
	if (flag == 1)
		result[*i] = ft_strdup(env[*i]);
	else if (flag == 2)
		result[*i] = ft_strdup(env[*i + 1]);
	if (!result[*i])
		return (ERROR);
	(*i)++;
	return (0);
}

// removes an array element by duplicating it and skipping said element
// @returns allocated array without removed element
// old array is freed in the caller
char	**unset_arr_element(char **env, int index, int len)
{
	char	**result;
	int		i;

	i = 0;
	if (index >= len)
		return (env);
	result = malloc(sizeof(char *) * len);
	if (!result)
		return (NULL);
	while (env[i] && i < index)
	{
		if (unset_strdup(result, env, &i, 1) == ERROR)
			return (NULL);
	}
	while (env[i + 1] && i < len - 1)
	{
		if (unset_strdup(result, env, &i, 2) == ERROR)
			return (NULL);
	}
	result[i] = NULL;
	return (result);
}

// unsets an env variable
// @returns: ERROR if index was not found - same logic as export
// @returns: 0 if successfully removed the variable and updated env list
// no arguments = return 0 - correct bash behaviour
int	unset_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**vars;
	char	**res;
	int		index;

	i = 0;
	vars = cmd->args;
	while (vars[++i])
	{
		index = export_get_index(mini, vars, i);
		if (index == -2)
			return (ERROR);
		if (index > -1)
		{
			res = unset_arr_element(mini->env, index, get_arr_len(mini->env));
			if (!res)
				return (ERROR);
			free_arr(mini->env);
			mini->env = res;
		}
	}
	return (0);
}
