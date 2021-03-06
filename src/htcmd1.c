/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 21:51:18 by jye               #+#    #+#             */
/*   Updated: 2017/11/08 20:36:46 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "lst.h"
#include "htcmd.h"

#include "cd.h"
#include "echo.h"
#include "exit_.h"
#include "setenv.h"
#include "unset.h"
#include "fg.h"
#include "bg.h"
#include "job_.h"
#include "disown.h"
#include "env.h"

#include <unistd.h>

#include <string.h>
#include <stdlib.h>

#define BUILTIN_NO 11

int		init_builtin_table(void)
{
	static t_ccsh	bi[BUILTIN_NO] = {
		{"cd", C_SHELL_BUILTIN, ft_cd},
		{"echo", C_SHELL_BUILTIN, ft_echo},
		{"exit", C_SHELL_BUILTIN, ft_exitsh},
		{"setenv", C_SHELL_BUILTIN, ft_setenv},
		{"unset", C_SHELL_BUILTIN, ft_unset},
		{"unsetenv", C_SHELL_BUILTIN, ft_unset},
		{"env", C_SHELL_BUILTIN, ft_env},
		{"fg", C_SHELL_BUILTIN, ft_fg},
		{"bg", C_SHELL_BUILTIN, ft_bg},
		{"disown", C_SHELL_BUILTIN, ft_disown},
		{"jobs", C_SHELL_BUILTIN, ft_job},
	};
	int				i;
	t_bucket		*c;

	if ((g_htbi = init_hashtable(96)) == 0)
		return (1);
	i = 0;
	while (i < BUILTIN_NO)
	{
		c = hash_insert(g_htbi, bi[i].key, HT_NOSEARCH);
		c->c = &bi[i++];
	}
	return (0);
}

int		chash_init(void)
{
	if ((g_htext = init_hashtable(HT_DEFAULT_BUCKET)) == 0 ||
		(init_builtin_table()))
	{
		free(g_htext);
		free(g_htbi);
		return (1);
	}
	else
		return (0);
}

void	free_ccsh(t_ccsh *c)
{
	free(c->c);
	free(c->key);
	free(c);
}

t_ccsh	*chash_lookup(char *key, char *envp)
{
	static char	*cpath;

	if (key == (char *)0)
		return ((t_ccsh *)0);
	if (cpath != envp)
	{
		cpath = envp;
		chash_flush();
	}
	return (chash_search(key, cpath));
}

t_ccsh	*chash_search(char *key, char *envp)
{
	char		*cpath;
	t_bucket	*item;
	t_ccsh		*cc;

	if (!(item = hash_search(g_htbi, key)) &&
		!(item = hash_search(g_htext, key)))
	{
		if ((cpath = path_lookup(key, envp)) == 0)
			return (0);
		if ((cc = init_ccsh(key, cpath, C_SHELL_EXT)) == 0)
			return (0);
		if ((item = hash_insert(g_htext, key, HT_NOSEARCH)) == 0)
		{
			free(cc->c);
			free(cc->key);
			free(cc);
			return (0);
		}
		item->c = cc;
	}
	return (((t_ccsh *)item->c));
}
