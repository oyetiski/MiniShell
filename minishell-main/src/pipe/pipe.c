/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olyetisk <olyetisk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:17:45 by olyetisk          #+#    #+#             */
/*   Updated: 2024/07/12 13:20:06 by olyetisk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	found_pipe(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (tmp->next->type == WORD && tmp->prev->type == WORD)
			{
				printf("%s\n","sytax error pipe için");
				exit(1);
			}
		}
		tmp = tmp->next;
	}
}