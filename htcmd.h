/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 16:21:35 by jye               #+#    #+#             */
/*   Updated: 2017/08/20 12:49:16 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTCMD_H
# define HTCMD_H

# include "hashlib.h"

# define C_SHELL_BUILTIN	0x0
# define C_SHELL_EXT		0x1

typedef int	(*t_builtin)(int ac, char **av, char **envp);

union				u_ccty
{
	char		*path;
	t_builtin	*func;
};

typedef struct		s_ccsh
{
	char			*key;
	int				type;
	union u_ccty	c;
}					t_ccsh;

int		chash_init(void);
t_ccsh	*chash_lookup(char *key, char *envp);
t_ccsh	*chash_search(char *key, char *envp);
void	chash_flush(void);
void	free_ccsh(t_ccsh *z);

#endif
