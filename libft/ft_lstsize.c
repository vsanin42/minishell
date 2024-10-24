/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:56:06 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/24 16:22:48 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst != NULL)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

// int main()
// {
// 	t_list *lst;
// 	lst = NULL;
// 	printf("empty node length: %d\n", ft_lstsize(lst));
// 	lst = ft_lstnew("biba");
// 	printf("single node length: %d\n", ft_lstsize(lst));
// 	//ft_lstadd_front(&lst, ft_lstnew("boba"));
// 	lst->next = ft_lstnew("boba");
// 	printf("two node length: %d\n", ft_lstsize(lst));
// 	while (lst != NULL)
// 	{
// 		printf("%s\n", (char *)lst->content);
// 		lst = lst->next;
// 	}
// }