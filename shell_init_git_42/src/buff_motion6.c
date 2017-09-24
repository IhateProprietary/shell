/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:09:00 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 14:03:24 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

void	buff_newline(void)
{
	int	diff;

	diff = ((g_psize + g_buffer.len) / g_winsize.col) + 1;
	diff -= (g_psize + g_buffer.cu) / g_winsize.col;
	while (diff--)
		write(STDERR_FILENO, "\n", 1);
	write(STDERR_FILENO, "\r", 1);
}

void	buff_delete(void)
{
	if (g_buffer.len == 0)
	{
		free(g_buffer.s);
		g_buffer.s = NULL;
		exit_readline();
	}
	else
	{
		buff_del_next();
	}
}

void	buff_insert(void *c, int r)
{
	if (g_buffer.len + r >= g_buffer.msize)
	{
		if (buff_realloc(g_buffer.msize + DEFAULT_BUFFER_SIZE))
			return ;
	}
	ft_memmove(g_buffer.s + g_buffer.cu + r,
				g_buffer.s + g_buffer.cu,
				g_buffer.len - g_buffer.cu);
	ft_memcpy(g_buffer.s + g_buffer.cu, c, r);
	g_buffer.len += r;
	buff_refresh(g_buffer.cu, g_buffer.s + g_buffer.cu,
					g_buffer.len - g_buffer.cu);
	g_buffer.cu += r;
	buff_record(g_buffer.cu, -r, RL_ACTION_DELETE);
	shift_cursor(g_buffer.len, g_buffer.cu);
}
