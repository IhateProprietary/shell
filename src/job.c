/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/10/15 15:07:39 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

t_process		**g_jobs;
struct s_jstat	g_js = {-1, -1, 0, 0, 0, 0, 0};
pid_t			g_orgid;
pid_t			g_shgid;
char			*g_sig_[NOSIG];
