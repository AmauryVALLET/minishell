/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avallet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:03:13 by avallet          #+#    #+#             */
/*   Updated: 2023/08/31 19:32:41 by avallet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	flag_check(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	if (str[i] == '-' && str[i + 1])
	{
		i++;
		while (str[i] && str[i] == 'n')
			i++;
	}
	if (!str[i])
		return (1);
	else
		return (0);
}

int	ft_echo(int argc, char **argv)
{
	int		i;
	bool	is_option;

	i = 1;
	is_option = false;
	if (!argv || !argv[0])
		return (EXIT_FAILURE);
	if (argc > 1 && argv[i])
		is_option = flag_check(argv[i]);
	while (argv[i] && flag_check(argv[i]))
		i++;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!is_option)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
