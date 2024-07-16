/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcayyetiskin <olcayyetiskin@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 18:05:16 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/07/15 21:58:38 by olcayyetisk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void found_input(t_shell *shell)
{
    t_token *tmp;

    tmp = &shell->token_list; 
	if (tmp->next && (shell->fd.fdin = open("a.txt", O_RDWR)) == -1)
    {
        perror("inpusssst error");
        exit(1);
    }
    
  	if (dup2(shell->fd.fdin, STDIN_FILENO) == -1)
    {
        perror("dup2 error");
        exit(1);
    } 
    
    while (tmp)
    {
        if (tmp->type == HEREDOC)
        {
            if (tmp->next)
                shell->fd.fdin = open("a.txt", O_CREAT | O_WRONLY, 0644);
        }
        else if (tmp->type == INPUT)
            shell->fd.fdin = open("a.txt", O_CREAT | O_WRONLY, 0644);
        
        if (shell->fd.fdin == -1)
        {
            perror("input error");
            exit(1);
        }
        
        tmp = tmp->next;
    }
    
    if (dup2(shell->fd.fdin, STDIN_FILENO) == -1)
    {
        perror("dup2 output error");
        exit(1);
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
                shell->fd.fdout = open(tmp->next->text, O_CREAT | O_WRONLY | O_APPEND, 0644);
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
