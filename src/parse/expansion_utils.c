/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olyetisk <olyetisk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:55:15 by olyetisk          #+#    #+#             */
/*   Updated: 2024/08/26 16:01:08 by olyetisk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_dollar2word(char **env, t_token *dollar)
{
	char	*temp;

	if (ft_strequ(dollar->text, "$"))
	{
		dollar->type = WORD;
		return ;
	}
	temp = get_env(env, dollar->text + 1);
	if (ft_strequ(temp, ""))
	{
		dollar->type = WHITESPACE;
		return ;
	}
	split_dollar(temp, dollar);
	if (dollar->text == NULL)
		dollar->text = ft_strdup("");
	free(temp);
}

void	create_joined_words(t_token *tokens)
{
	t_token	*temp;
	char	*temp_text;

	temp = tokens->next;
	while (temp->type == WORD)
	{
		temp_text = ft_strjoin(tokens->text, temp->text);
		if (temp_text == NULL)
			printf("Error encountered while word collaping.\n");
		destroy_token(remove_token(tokens, temp));
		free(tokens->text);
		tokens->text = temp_text;
		temp = tokens->next;
	}
}

void	token_dollar2exitcode(t_token *dollar)
{
	dollar->type = WORD;
	dollar->text = ft_itoa(g_global_exit);
}
