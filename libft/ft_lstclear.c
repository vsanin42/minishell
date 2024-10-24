/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 17:13:14 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/24 18:07:36 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*current;

	if (lst != NULL)
	{
		current = *lst;
		while (current)
		{
			temp = current;
			if (del != NULL)
				del(current->content);
			current = current->next;
			free(temp);
		}
		*lst = NULL;
	}
}

// void del(void *content)
// 	{
//         free(content);
// 	}
// // Function to test ft_lstclear
// void test_ft_lstclear()
// {
//     // Create a linked list
//     t_list *head = ft_lstnew(strdup("Node 1"));
//     head->next = ft_lstnew(strdup("Node 2"));
//     head->next->next = ft_lstnew(strdup("Node 3"));
//     // Call ft_lstclear to delete and free all nodes in the list
//     ft_lstclear(&head, del);
//     // Check if the list is empty (head is NULL)
//     if (head == NULL)
//     {
//         printf("List cleared successfully.\n");
//     }
//     else
//     {
//         printf("List not cleared properly.\n");
//     }
// }
// int main()
// {
//     // Test ft_lstclear
//     test_ft_lstclear();
//     return 0;
// }