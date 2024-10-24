/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:34:51 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/24 16:22:38 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

// int main()
// {
// 	t_list *lst;
// 	lst = ft_lstnew("biba");
// 	lst->next = ft_lstnew("boba");
// 	printf("%s", (char *)(ft_lstlast(lst))->content);
// }