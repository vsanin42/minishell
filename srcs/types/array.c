/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:30:33 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/28 11:03:14 by zpiarova         ###   ########.fr       */
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

int	array_char_len(char **head)
{
	int	i;

	i = 0;
	while (head && *head)
	{
		i += ft_strlen(*head);
		head++;
	}
	return (i);
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
		{
			free_arr(res);
			return (NULL);
		}
		j++;
	}
	res[i] = ft_strdup(new_el);
	if (!res[i])
	{
		free_arr(res);
		return (NULL);
	}
	res[i + 1] = NULL;
	return (res);
}

// TODO: UPDATE HEAD OF ARRAY IF WE ARE EXCHANGING TEH FIRST ELEMENT OF ARRAY ?
char **change_arr_element(char **arr, char *new_el, int	index)
{
	char	**rest_of_arr;
	int		i;

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
	free(arr[index]);
	arr[index] = ft_strdup(new_el);
	if (!arr[index])
		return (free_arr(arr), free_arr(rest_of_arr), NULL);
	return (arr);
}
