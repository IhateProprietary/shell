/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:12 by jye               #+#    #+#             */
/*   Updated: 2017/08/27 20:28:18 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hashlib.h"
#include "htvar.h"
#include "lst.h"

//
#include <string.h>

t_hashtable		*g_htvar;
int				g_envpsize;
t_lst			*g_envp;

int		vhash_init(void)
{
	if ((g_htvar = init_hashtable(HT_DEFAULT_BUCKET)) == 0)
		return (1);
	else
		return (0);
}

int		vhash_insert(t_var *var)
{
	t_bucket	*item;
	t_var		*c;

	if ((item = hash_insert(g_htvar, var->key, HT_SEARCH)) == 0)
		return (1);
	if (item->c)
	{
		c = (t_var *)item->c;
		free(c->value);
		c->value = var->value;
		free(var->key);
		if (var->lock_)
		{
			pop_lst__(&var->lock_, NULL);
			g_envpsize -= 1;
		}
		free(var);
	}
	else
	{
		item->c = var;
	}
	return (0);
}

void	vhash_pop(char *key)
{
	t_bucket	*item;
	t_var		*var;

	if ((item = hash_search(g_htvar, key)) == 0)
		return ;
	var = (t_var *)item->c;
	free(var->key);
	free(var->value);
	if (var->lock_)
	{
		pop_lst__(&var->lock_, NULL);
		g_envpsize -= 1;
	}
	hash_popentry(g_htvar, key, free);
}

char	*vhash_search(char *key)
{
	t_bucket	*item;
	t_var		*v;

	item = hash_search(g_htvar, key);
	if (item == 0)
		return (0);
	v = item->c;
	return (v->value);
}

////////

int		frag_var(char *s, char **key, char **value)
{
	char	*ptr;

	if ((ptr = strchr(s, '=')) == 0)
		return (1);
	*ptr = 0;
	*value =  ptr[1] == 0 ? 0 : strdup(ptr + 1);
	if ((*key = strdup(s)) == 0)
	{
		free(*value);
		return (1);
	}
	*ptr = '=';
	return (0);
}

char	*defrag_var(t_var *var)
{
	size_t	ks;
	size_t	vs;
	char	*s;

	ks = strlen(var->key);
	vs = strlen(var->value);
	if ((s = malloc(ks + vs + 2)) == 0)
		return (0);
	memcpy(s, var->key, ks);
	s[ks] = '=';
	memcpy(s + ks + 1, var->value, vs);
	s[ks + vs + 1] = 0;
	return (s);
}

t_var	*init_var(char *s, int envp)
{
	char	*key;
	char	*value;
	t_var	*v;

	if (frag_var(s, &key, &value))
		return ((t_var *)0);
	if ((v = malloc(sizeof(*v))) == 0)
	{
		free(key);
		free(value);
		return ((t_var *)0);
	}
	v->key = key;
	v->value = value;
	if (envp)
	{
		push_lst__(&g_envp, v);
		v->lock_ = g_envp;
		g_envpsize += 1;
	}
	else
		v->lock_ = 0;
	return (v);
}

int		init_htvar(char **envp)
{
	t_var	*v;

	if (vhash_init())
		return (1);
	while (*envp)
	{
		if ((v = init_var(*envp, 1)))
			vhash_insert(v);
		++envp;
	}
	return (0);
}
