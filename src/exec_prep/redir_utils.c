/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avallet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by avallet          #+#    #+#             */
/*   Updated: 2023/08/31 17:24:45 by avallet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

int	count_redir(t_exec *exec)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	count = 0;
	while (exec->argv[i])
	{
		j = 0;
		while (exec->argv[i][j])
		{
			if (exec->argv[i][j] == '<' || exec->argv[i][j] == '>')
				count++;
			j++;
		}
		i++;
	}
	return (count);
}

int	check_redir_return(int identifier, int i)
{
	if (identifier == 1)
	{
		if (i == 0)
			return (4);
		else
			return (3);
	}
	else if (identifier == 2)
	{
		if (i == 0)
			return (5);
		else
			return (2);
	}
	return (-1);
}

int	check_op_char(char **argv, int i, int j)
{
	if (argv[i][j] == '<' && (!(single_q_cntr(g_master.readline_av[i],
		argv[i][j], spec_char_cnter(argv[i], argv[i][j], j)) % 2 != 0))
		&& (!(double_q_cnter(g_master.readline_av[i], argv[i][j],
		spec_char_cnter(argv[i], argv[i][j], j)) % 2 != 0)))
		return (1);
	if (argv[i][j] == '>' && !(single_q_cntr(g_master.readline_av[i],
		argv[i][j], spec_char_cnter(argv[i], argv[i][j], j)) % 2 != 0)
		&& (!(double_q_cnter(g_master.readline_av[i], argv[i][j],
		spec_char_cnter(argv[i], argv[i][j], j)) % 2 != 0)))
		return (2);
	return (0);
}

int	check_redir(char **argv)
{
	int		i;
	size_t	j;
	int		pos;

	pos = 0;
	i = -1;
	while (argv[++i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (check_op_char(argv, i, j) == 1)
				return (1);
			if (check_op_char(argv, i, j) == 2)
			{
				if (argv[i][j + 1] && argv[i][j + 1] == '>')
					return (check_redir_return(1, i));
				return (check_redir_return(2, i));
			}
			j++;
		}
	}
	return (-1);
}

int	clean_argv(t_exec *exec)
{
	int		i;
	int		j;
	int		k;
	char	**new_argv;

	i = 0;
	k = 0;
	j = find_redirection(exec->argv);
	if (j == -1)
		return (EXIT_FAILURE);
	new_argv = malloc(sizeof(char *) * (exec->argc + 1));
	while (exec->argv[i] && i < j)
		new_argv[k++] = ft_strdup(exec->argv[i++]);
	i = i + 2;
	exec->argc = i;
	while (exec->argv[i])
		new_argv[k++] = ft_strdup(exec->argv[i++]);
	new_argv[k] = NULL;
	free_double_ptr(exec->argv);
	exec->argv = new_argv;
	exec->argc = k;
	if (!exec->argv[0])
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
