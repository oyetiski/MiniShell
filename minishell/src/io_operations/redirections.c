/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olyetisk <olyetisk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 18:05:16 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/07/17 12:49:57 by olyetisk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void found_input(t_shell *shell)
{
    t_token *tmp;

    tmp = &shell->token_list; 
	if (shell->fd.fdin != -1)
        close (shell->fd.fdin);
    while (tmp)
    {
        if (tmp->type == HEREDOC)
        {
            if (tmp->next)
                shell->fd.fdin = open(tmp->next->next, O_RDONLY, 0644);
        }
        else if (tmp->type == INPUT)
            shell->fd.fdin = open(tmp->next->text, O_RDONLY, 0644);
        
        if (shell->fd.fdin == -1)
            perror ("input error");
        tmp = tmp->next;
    }
    dup2(shell->fd.fdin, STDIN_FILENO) == -1;
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
                shell->fd.fdout = open(tmp->next->text, O_CREAT | O_WRONLY | O_APPEND, 0644);
            }
        }
        else if (tmp->type == OUTPUT)
        {
            shell->fd.fdout = open(tmp->next->text, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        }

        if (shell->fd.fdout == -1)
        {
            perror("output error");
        }
        tmp = tmp->next;
    }
	dup2(shell->fd.fdout, STDOUT_FILENO) == -1;
}