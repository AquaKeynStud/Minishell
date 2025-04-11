/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 11:06:23 by abouclie          #+#    #+#             */
/*   Updated: 2024/11/25 15:06:21 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_obj;
	void	*cnt;

	new_list = NULL;
	while (lst)
	{
		cnt = f(lst->content);
		if (!cnt)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		new_obj = ft_lstnew(cnt);
		if (!new_obj)
		{
			del(cnt);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_obj);
		lst = lst->next;
	}
	return (new_list);
}
