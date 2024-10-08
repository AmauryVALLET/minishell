/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avallet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:31:42 by avallet          #+#    #+#             */
/*   Updated: 2023/08/25 14:22:02 by avallet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "exit.h"
#include "libft.h"

char	*get_env_value(t_env *env, char *name, int flag)
{
	char	*value;

	if (!env || !env->name || !name)
		return (NULL);
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			if (env->value)
			{
				if (flag == 0)
					return (env->value);
				value = ft_strdup(env->value);
				return (value);
			}
			else
				return (NULL);
		}
		env = env->next;
	}
	return (NULL);
}

bool	is_valid_variable_name(char	*name, char *var_str)
{
	if ((!ft_isalpha(name[0]) && name[0] != '_'))
	{
		printf("minishell: export: '%s': not a valid identifier\n", var_str);
		g_master.exit_status = 1;
		return (false);
	}
	name++;
	while (*name)
	{
		if (*name == '=')
			return (true);
		if (!ft_isalnum(*name) && *name != '_')
		{
			printf("minishell: export: '%s': not a valid identifier\n", var_str);
			g_master.exit_status = 1;
			return (false);
		}
		name++;
	}
	return (true);
}

bool	is_var_already_in_env(t_env **env_list, t_env *var)
{
	t_env	*current;

	current = *env_list;
	if (!current->name || !var->name)
	{
		return (false);
	}
	while (current)
	{
		if (!ft_strcmp(current->name, var->name))
		{
			free(current->value);
			current->value = var->value;
			free(var->name);
			return (true);
		}
		current = current->next;
	}
	return (false);
}

void	add_back_env_var(t_env **env_list, t_env *var)
{
	if (!env_list)
		env_list = &var;
	else
	{
		(*env_list)->last->next = var;
		(*env_list)->last = var;
	}
}

void	remove_var(t_master *master, t_env *env)
{
	t_env	*current;
	t_env	*previous;

	current = master->env_list;
	previous = NULL;
	while (current && ft_strcmp(current->name, env->name) != 0)
	{
		previous = current;
		current = current->next;
	}
	if (current)
	{
		if (previous)
			previous->next = current->next;
		else
			master->env_list = current->next;
		free(current->name);
		free(current->value);
		free(current);
	}
}
