/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olyetisk <olyetisk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 17:13:02 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/08/26 14:57:16 by olyetisk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_print_err(char *path, char *err)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(err, STDERR_FILENO);
}

static void	cd_home(char **env)
{
	char	*home;
	char	*pwd;

	home = get_env(env, "HOME");
	if (home == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return ;
	}
	if (chdir(home + 5))
	{
		cd_print_err(home, strerror(errno));
		return ;
	}
	pwd = get_env(env, "PWD");
	set_env(env, "OLDPWD", (pwd + 4));
	set_env(env, "PWD", home + 5);
	free(home);
	free(pwd);
}

static void	cd_abs_path(char **env, char *path)
{
	char	*temp;
	char	*pwd;

	if (chdir(path))
	{
		cd_print_err(path, strerror(errno));
		return ;
	}
	pwd = get_env(env, "PWD");
	set_env(env, "OLDPWD", pwd + 4);
	temp = getcwd(NULL, 0);
	set_env(env, "PWD", temp);
	free(pwd);
	free(temp);
}

static void	cd_rel_path(char **env, char *path)
{
	char	*temp;
	char	*pwd;

	if (chdir(path))
	{
		cd_print_err(path, strerror(errno));
		return ;
	}
	pwd = get_env(env, "PWD");
	set_env(env, "OLDPWD", pwd + 4);
	temp = getcwd(NULL, 0);
	set_env(env, "PWD", temp);
	free(pwd);
	free(temp);
}

void	mini_cd(char **env, char *path)
{
	if (path == NULL)
		cd_home(env);
	else if (path[0] == '/')
		cd_abs_path(env, path);
	else
		cd_rel_path(env, path);
}
