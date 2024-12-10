/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:30:33 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/10 17:05:54 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// frees array of strings
void	free_arr(char **arr)
{
	char	**head;

	if (!arr)
		return ;
	head = arr;
	while (*arr)
	{
		free(*arr);
		*arr = NULL;
		arr++;
	}
	free(head);
	head = NULL;
}

int	get_arr_len(char **arr)
{
	int		i;

	if (!arr || !(*arr))
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

// allocates new array of size + 2 (for new el and NULL terminator)
// duplicates all elements, appends the new one to the back
// @returns new allocated array
char	**add_back_array(char **arr, char *new_el)
{
	int		i;
	int		j;
	char	**res;

	if (!arr || !(*arr) || !new_el)
		return (NULL);
	i = 0;
	while (arr[i])
		i++;
	res = malloc(sizeof(char *) * (i + 2));
	if (!res)
		return (NULL);
	j = 0;
	while (j < i)
	{
		res[j] = ft_strdup(arr[j]);
		if (!res[j])
			return (free_arr(res), NULL);
		j++;
	}
	res[i] = ft_strdup(new_el);
	if (!res[i])
		return (free_arr(res), NULL);
	res[i + 1] = NULL;
	return (res);
}

char	**change_arr_element(char **arr, char *new_el, int index)
{
	char	**rest_of_arr;
	int		i;
	char	*temp;

	if (!arr || !(*arr) || !new_el)
		return (NULL);
	i = 0;
	rest_of_arr = arr;
	while (*rest_of_arr && i < index)
	{
		rest_of_arr++;
		i++;
	}
	if (rest_of_arr == NULL)
		return (NULL);
	rest_of_arr++;
	temp = ft_strdup(new_el);
	if (!temp)
		return (NULL);
	free(arr[index]);
	arr[index] = temp;
	return (arr);
}

// called to initialize our env array that will be stored in the mini struct
// @returns alocated array of strings representing one env with name and value
char	**dup_array(char **arr)
{
	int		i;
	int		j;
	char	**res;

	i = 0;
	while (arr[i])
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
		return (NULL);
	j = -1;
	while (++j < i)
	{
		res[j] = ft_strdup(arr[j]);
		if (!res[j])
		{
			while (j > 0)
				free(res[--j]);
			free(res);
			res = NULL;
			return (NULL);
		}
	}
	res[j] = NULL;
	return (res);
}
