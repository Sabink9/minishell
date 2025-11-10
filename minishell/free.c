/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:33:26 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/10 14:36:02 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

void	free_partial(char **tab, int count)
{
	int	k;

	k = 0;
	while (k < count)
	{
		free(tab[k]);
		k++;
	}
	free(tab);
}

void	free_split(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	free_dup_until(char **out, int i)
{
	while (i > 0)
	{
		free(out[i - 1]);
		i--;
	}
	free(out);
}

void	free_cmdv(char ***cmdv)
{
	int	i;
	int	j;

	i = 0;
	while (cmdv && cmdv[i])
	{
		j = 0;
		while (cmdv[i][j])
			free(cmdv[i][j++]);
		free(cmdv[i]);
		i++;
	}
	free(cmdv);
}
