/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_long.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 11:55:45 by root              #+#    #+#             */
/*   Updated: 2017/09/23 23:40:27 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt_long.h"

#include <string.h>
#include <stdio.h>

#define OPT_ISOPT(opt) ((opt)[0] == '-' && (opt)[1])
#define OPT_ISLONG(opt) (!strncmp((opt), "--", 2) && (opt)[2] != 0)
#define OPT_ISEND(opt) (!strcmp((opt), "--"))

int			g_optind_;
char		*g_optarg_;
int			g_opterr_ = 1;

struct s_options	*ft_opt_getstruct_long(char *s,
											struct s_options *longopt,
											char *pname)
{
	char	*has_arg;
	size_t	ncmp;

	has_arg = strchr(s, '=');
	if (has_arg)
		g_optarg_ = has_arg + 1;
	ncmp = has_arg ? has_arg - s : strlen(s);
	while (longopt->s != NULL)
	{
		if (ncmp == strlen(longopt->s) &&
			!strncmp(s, longopt->s, ncmp))
			break ;
		longopt++;
	}
	if (longopt->s == NULL)
	{
		if (g_opterr_)
			dprintf(2, "%s: unrecognized options '--%*s'\n", pname, ncmp, s);
		return (NULL);
	}
	return (longopt);
}

struct s_options	*ft_opt_getstruct(char s, struct s_options *longopt)
{
	while (longopt->s != NULL)
	{
		if (s == longopt->val)
			break ;
		longopt++;
	}
	return (longopt->s ? longopt : NULL);
}

static int			ft_opt_end(char **nextchar)
{
	*nextchar = NULL;
	return (-1);
}

static int			ft_getopt_(char **nextchar)
{
	lopt = ft_opt_getstruct(*nextchar, longopt);
	if (lopt->has_arg == req_arg && nextchar[1] == 0)
		g_optarg_ = av[g_optind_++];
	return (*(*nextchar)++);
}

int					ft_getopt_long(int ac, char **av,
									char *optstr, struct s_options *longopt)
{
	static char			*nextchar;
	static char			*pname;
	struct s_options	*lopt;

	g_optarg_ = 0;
	if (g_optind_ == 0)
		nextchar = ft_getopt_init();
	if (nextchar == NULL)
		return (-1);
	if ((*nextchar == 0 && g_optind_ >= ac) || OPT_ISEND(av[optind_]))
		return (ft_opt_end(&nextchar));
	else if (*nextchar == 0 && OPT_ISLONG(av[g_optind_]))
		return (ft_getopt_long_long(
					ft_opt_getstruct_long(av[g_optind_] + 2, longopt), pname));
	else if (*nextchar == 0 && OPT_ISOPT(av[g_optind_]))
		nextchar = av[g_optind_++] + 1;
	else if (*nextchar == 0)
		return (ft_opt_end(&nextchar));
	if (strchr(optstr, *nextchar))
		return (ft_getopt_());
	if (g_opterr_)
		dprintf(2, "%s: unrecognized option '%c'\n", g_pname, *nextchar++);
	return ('?');
}
