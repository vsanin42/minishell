/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 22:05:19 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/25 20:02:13 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// free with auto check in case it hasn't been malloced
// specific case when a var is malloced as initialization
// shoutout to Nikita
void	free_memo(void *mem_seg)
{
	if (mem_seg)
		free(mem_seg);
}

// util function that can free 4 allocated strings and set them to NULL pointer
// helps us save space for norm
// if we do not hve all 4 strings to free, just pass in NULL as empty parameter
void	free_four_mallocs(char *s1, char *s2, char *s3, char *s4)
{
	free(s1);
	free(s2);
	free(s3);
	free(s4);
	s1 = NULL;
	s2 = NULL;
	s3 = NULL;
	s4 = NULL;
}
