/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:24:31 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/24 17:12:22 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst != NULL)
	{
		if (del != NULL)
			del(lst->content);
		free(lst);
	}
}

// void del_content(void *content) {
//     // Assume content points to dynamically allocated memory
//     free(content);
// }
// int main()
// {
// 	t_list *lst;
// 	lst = ft_lstnew("biba");
// 	ft_lstdelone(lst, del_content);
// 	printf("%s", (char *)(lst->content));
// }