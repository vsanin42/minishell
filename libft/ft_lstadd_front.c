/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:44:36 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/24 17:00:04 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}

// int main()
// {
// 	t_list *lst;
// 	lst = ft_lstnew("biba");
// 	lst->next = ft_lstnew("boba");
// 	printf("%s", (char *)(ft_lstlast(lst))->content);
// }