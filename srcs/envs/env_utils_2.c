/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:11:13 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/17 15:06:04 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_paths(char **paths, int *i)
{
	*i = -1;
	while (paths[++(*i)])
		free(paths[*i]);
}

// checks if passed in env contains also value or only name
// (possibly ending in =)
// @returns 1 if has value, 0 if not
int	has_env_value(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	if (env[i] == '\0' || (env[i] == '=' && env[i + 1] == '\0'))
		return (0);
	return (1);
}

// checks whether env has valid name-only alnum or _, starting with letter or _
// @returns 0 if valid name, 1 if not
int	check_env_name(char *env)
{
	int	i;

	i = 0;
	if ((env[i] >= '0' && env[i] <= '9'))
		return (ERROR);
	while (env[i] && env[i] != '=')
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (ERROR);
		i++;
	}
	return (0);
}
