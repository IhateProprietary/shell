/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 04:00:33 by jye               #+#    #+#             */
/*   Updated: 2017/08/19 15:41:35 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "ft_readline.h"
#include "token.h"
#include "parser.h"
#include "lst.h"


#include <stdio.h>

int		main(int ac, char **av)
{
	char	*s;
	t_lst	*t;
	t_command	*c;
	t_lst	*z;

	s = ft_readline("", 0);
	init_tokenizer();
	t = tokenize(s);
	t = parse_token(t);
	c = t->data;
	z = c->av.lav;
/*****process parsed bullshit in a fork or not******/
	
	return (0);
}
