/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:04:23 by markhali          #+#    #+#             */
/*   Updated: 2025/11/12 10:33:17 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_target(char **argv, t_env *env)
{
	char	*target;

	if (!argv[1] || !argv[1][0])
		target = get_env(env, "HOME");
	else if (ft_strcmp(argv[1], "-") == 0)
		target = get_env(env, "OLDPWD");
	else
		target = argv[1];
	return (target);
}

static int	print_error(char **argv, char *target, char *old_pwd)
{
	if (!target)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		if (!argv[1] || !argv[1][0])
			ft_putstr_fd("HOME", 2);
		else
			ft_putstr_fd("OLDPWD", 2);
		ft_putstr_fd(" not set\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(target, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	free(old_pwd);
	return (1);
}

int	builtin_cd(char **argv, t_env **env)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*target;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (1);
	target = get_target(argv, *env);
	if (!target)
		return (print_error(argv, target, old_pwd));
	if (chdir(target) != 0)
		return (print_error(argv, target, old_pwd));
	new_pwd = getcwd(NULL, 0);
	set_env(env, "OLDPWD", old_pwd);
	set_env(env, "PWD", new_pwd);
	if (argv[1] && ft_strcmp(argv[1], "-") == 0)
		printf("%s\n", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
}
