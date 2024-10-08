/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avallet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:18:49 by avallet          #+#    #+#             */
/*   Updated: 2023/08/31 16:49:02 by avallet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

void	execve_execute_command(t_exec *exec, t_env *env_list,
			t_builtin_type type)
{
	char	**envp;

	envp = env_list_to_array(env_list);
	if (type == T_OTHERS)
		execve(exec->pathname, exec->argv, envp);
	else
	{
		execute_builtin(exec, type);
		free_double_ptr(envp);
		return ;
	}
	free_double_ptr(envp);
	free_executable();
	perror("execve (execute_command)");
}

void	launch_dup_child_process(t_master *master, t_token *token,
	t_builtin_type type)
{
	(void)master;
	if (token->next && token->next->type == T_PIPE)
	{
		fd_close(g_master.pipefd[0]);
		dup2(g_master.pipefd[1], STDOUT_FILENO);
		fd_close(g_master.pipefd[1]);
	}
	else if (g_master.first_cmd == false)
	{
		fd_close(g_master.pipefd[1]);
		dup2(g_master.pipefd[0], STDIN_FILENO);
		fd_close(g_master.pipefd[0]);
	}
	else if (g_master.first_cmd == true && type != T_OTHERS)
	{
		fd_close(g_master.pipefd[0]);
		dup2(g_master.pipefd[1], STDOUT_FILENO);
		fd_close(g_master.pipefd[1]);
	}
}

void	child_process_execution(t_master *master, t_token *token, t_exec *exec,
			t_builtin_type type)
{
	(void)master;
	exec = g_master.exec;
	if (g_master.pid == 0)
	{
		dup2(g_master.tmp_fd, STDIN_FILENO);
		fd_close(g_master.tmp_fd);
		launch_dup_child_process(master, token, type);
		if (launch_redirection(g_master.exec) == EXIT_FAILURE)
		{
			g_master.exit_status = 1;
			ft_free_child();
			exit(g_master.exit_status);
		}
		execve_execute_command(g_master.exec, g_master.env_list, type);
		ft_free_child();
		exit(g_master.exit_status);
	}
}

void	parent_process_execution(t_master *master, t_token **token)
{
	(void)master;
	if (g_master.exit_status != 127 && g_master.pid > 0)
	{
		g_master.child_pid[g_master.count_pid++] = g_master.pid;
		if ((*token)->next && (*token)->next->type == T_PIPE)
			g_master.first_cmd = false;
		else
			g_master.first_cmd = true;
		fd_close(g_master.tmp_fd);
		if (g_master.pipefd[1] != -1)
			fd_close(g_master.pipefd[1]);
		g_master.tmp_fd = g_master.pipefd[0];
		if ((*token)->next)
			*token = (*token)->next->next;
		else
			*token = NULL;
		free_executable();
	}
}
