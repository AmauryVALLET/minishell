/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avallet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:33:48 by avallet          #+#    #+#             */
/*   Updated: 2023/07/24 14:34:08 by avallet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/libft.h"

size_t	ft_strnlen(const char *str, size_t maxlen)
{
	const char	*ptr = str;
	size_t		len;

	if (!maxlen)
		return (0);
	len = 0;
	while (len < maxlen && *ptr)
	{
		ptr++;
		len++;
	}
	return (len);
}
