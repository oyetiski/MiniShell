/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olyetisk <olyetisk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 18:05:16 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/08/21 15:47:56 by olyetisk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_heredoc(const char *delimiter, const char *temp_file_path) {
    char *line;
    int temp_fd;
    pid_t pid;
    
    temp_fd = open(temp_file_path, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (temp_fd < 0) {
        perror("Error opening temporary file");
        return;
    }
    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        close(temp_fd);
        return;
    } else if (pid == 0) {
        while (1) {
            write(STDOUT_FILENO, "> ", 2);
            line = (char *)malloc(1024);
            if (!line) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }

            ssize_t bytes_read = read(STDIN_FILENO, line, 1024);
            if (bytes_read < 0) {
                perror("Error reading line");
                free(line);
                exit(EXIT_FAILURE);
            }
            if (line[bytes_read - 1] == '\n') {
                line[bytes_read - 1] = '\0';
            }
            if (strcmp(line, delimiter) == 0) {
                free(line);
                break;
            }
            write(temp_fd, line, strlen(line));
            write(temp_fd, "\n", 1); 
            free(line);
        }
        close(temp_fd); 
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);

        temp_fd = open(temp_file_path, O_RDONLY);
        if (temp_fd == -1) {
            perror("Error reopening temporary file");
            return;
        }
        if (dup2(temp_fd, STDIN_FILENO) == -1) {
            perror("Error redirecting stdin");
        }
        close(temp_fd); 
    }
}

void found_input(t_shell *shell, t_token *temp) {
    char *del;
    char *temp_file = ft_strdup("/Users/olyetisk/Desktop/bugrasshell/b.txt");
    del = temp->next->text;
    if (shell->fdin != -1)
        close(shell->fdin);
    if (temp->type == HEREDOC) {
        handle_heredoc(del, temp_file);
    } else if (temp->type == INPUT) {
        shell->fdin = open(temp->next->text, O_RDONLY, 0644);
        if (shell->fdin == -1)
            perror("input error");
        dup2(shell->fdin, STDIN_FILENO);
    }
}


static void	found_output(t_shell *shell, t_token *temp)
{
	if (shell->fdout != -1)
		close(shell->fdout);
	if (temp->type == APPEND)
		shell->fdout = open(temp->next->text, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (temp->type == OUTPUT)
		shell->fdout = open(temp->next->text, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (shell->fdout == -1)
		perror("output error");
	dup2(shell->fdout, STDOUT_FILENO);
}

t_bool	apply_redirs(t_shell *shell, t_token *command)
{
	t_token	*temp;
	t_token	*temp2;

	temp = command;
	while (temp != NULL)
	{
		if ((temp->type == INPUT || temp->type == HEREDOC)
			|| (temp->type == OUTPUT || temp->type == APPEND))
		{
			if (temp->type == INPUT || temp->type == HEREDOC)
				found_input(shell, temp);
			if (temp->type == OUTPUT || temp->type == APPEND)
				found_output(shell, temp);
			temp2 = temp->next->next;
			remove_token(command, temp->next);
			remove_token(command, temp);
			temp = temp2;
			continue ;
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

void	clear_redirs(t_shell *shell)
{
	shell->fdin = STDIN_FILENO;
	shell->fdout = STDOUT_FILENO;
}