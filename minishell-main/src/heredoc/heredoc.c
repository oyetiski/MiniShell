/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olyetisk <olyetisk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:25:27 by olyetisk          #+#    #+#             */
/*   Updated: 2024/07/12 16:26:40 by olyetisk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void founder(t_token *token)
{
	t_token *tmp;
	
	tmp = token;
	while(tmp)
	{
		if (tmp->type == INPUT)
		{
			
		}
		if (tmp->type == HEREDOC)
		{
			
		}
	}
}
void found_input(t_token *token,t_shell *shell)
{
	t_token *tmp;

	tmp = token;
	shell->fd.fdin = open(token->text, O_RDONLY, S_IRWXU);
	if (shell->fd.fdin == -1)
	{	printf("%s","açılmadı ");
		exit(1);
	}
	dup2(shell->fd.fdin, STDIN_FILENO);
	while (tmp)
	{
		if (tmp->next->type == WORD && tmp->prev->type == WORD)
			printf("%s\n","input hatası");
	}
}
void found_output(t_shell *shell)
{
    t_token *tmp;

    tmp = &shell->token_list;
	 if (shell->fd.fdout != -1)
    close(shell->fd.fdout);
    while (tmp)
    {
        if (tmp->type == APPEND)
        {
            if (tmp->next)
            {
                shell->fd.fdout = open(tmp->text, O_CREAT | O_WRONLY | O_APPEND, 0644);
            }
        }
        else if (tmp->type == OUTPUT)
        {
            shell->fd.fdout = open(tmp->text, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        }

        if (shell->fd.fdout == -1)
        {
            perror("output error");
        }

        tmp = tmp->next;
    }
	dup2(shell->fd.fdout, STDOUT_FILENO) == -1;
}
