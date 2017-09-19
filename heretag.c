/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heretag.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 12:36:28 by root              #+#    #+#             */
/*   Updated: 2017/09/15 00:06:36 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "command.h"
#include "ft_readline.h"

int		is_heretag(char *s, char *heretag)
{
	int		r;
	size_t	sl;

	r = 0;
	while (*s && *s == ' ')
		s++;
	sl = strlen(heretag);
	if (!strncmp(s, heretag, sl))
	{
		s += sl;
		r = 1;
	}
	while (*s && *s == ' ')
		s++;
	if (*s != 0)
	{
		r = 0;
	}
	return (r);
}

void	here_tag(t_rd *rd)
{
	char	*s;
	int		fd[2];

	rd->heretag = -1 + pipe(fd);
	if (rd->heretag == -2)
		return ;
	while ((s = ft_readline("heretag> ", strlen("heretag> "))) &&
		   s != (char *)-1)
	{
		if (is_heretag(s, rd->s))
			break ;
		write(fd[1], s, strlen(s));
		write(fd[1], "\n", 1);
		free(s);
	}
	close(fd[1]);
	free(rd->s);
	rd->s = 0;
	if (s == (char *)-1)
	{
		close(fd[0]);
		return ;
	}
	free(s);
	rd->heretag = fd[0];
}