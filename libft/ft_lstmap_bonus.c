/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:59:56 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/29 17:17:14 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*cre(t_list *lst, void *(*f)(void *), void (*del)(void *), t_list **map)
{
	void	*new_cont;

	if (!lst || !f || !del)
		return (NULL);
	new_cont = f(lst->content);
	if (!new_cont)
	{
		ft_lstclear(map, del);
		return (NULL);
	}
	return (new_cont);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*map;
	t_list	*temp;
	void	*new_cont;

	map = NULL;
	while (lst)
	{
		new_cont = cre(lst, f, del, &map);
		if (!new_cont)
			return (NULL);
		temp = ft_lstnew(new_cont);
		if (!temp)
		{
			ft_lstclear(&map, del);
			del(new_cont);
			return (NULL);
		}
		ft_lstadd_back(&map, temp);
		lst = lst->next;
	}
	return (map);
}
