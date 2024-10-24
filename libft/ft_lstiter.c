/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:11:09 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/24 18:19:27 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst && f)
	{
		while (lst != NULL)
		{
			f(lst->content);
			lst = lst->next;
		}
	}
}

// void print_content(void *content)
// {
//     printf("%s\n", (char *)content);
// }

// // Function to test ft_lstiter
// void test_ft_lstiter()
// {
//     // Create a linked list
//     t_list *head = ft_lstnew("Node 1");
//     head->next = ft_lstnew("Node 2");
//     head->next->next = ft_lstnew("Node 3");

//     // Call ft_lstiter to iterate the list and print node content
//     ft_lstiter(head, print_content);
// }

// int main()
// {
//     // Test ft_lstiter
//     test_ft_lstiter();
//     return 0;
// }