/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job8.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 22:42:01 by root              #+#    #+#             */
/*   Updated: 2017/10/15 17:52:06 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "htvar.h"
#include "command.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

char		*job_getstatus(void)
{
	int			l;
	char		*itoa_ptr;
	static char	status[4];

	if (!g_js.pstat)
	{
		status[0] = 0x30;
		status[1] = 0;
		return (status);
	}
	l = g_js.pstat;
	itoa_ptr = status;
	while (l)
	{
		l /= 10;
		itoa_ptr++;
	}
	*itoa_ptr-- = 0;
	l = g_js.pstat;
	while (l)
	{
		*itoa_ptr-- = 0x30 + (l % 10);
		l /= 10;
	}
	return (status);
}

int		job_wait_control_(pid_t pid, int options)
{
	int					status;

	status = 127 << 16;
	if (pid > 0)
		waitpid(pid, &status, options);
	return (status);
}

int			job_wait_control(t_process *proc, int options)
{
	int		status;

	status = job_wait_control_(proc->pid, options);
	proc->status = status;
	proc->state =
		((JT_IS_NOHANG(proc->status) * JT_BACKGROUND) |
		(WIFSTOPPED(proc->status) * JT_SUSPENDED) |
		(JT_IS_NOHANG(proc->status) * JT_FOREGROUND));
	if (proc->state)
		job_insert(proc);
	else
	{
		free_full_parsed_command(proc->c);
		free(proc);
	}
	if ((options & WNOHANG))
		return (-1);
	g_js.exit = 0;
	if (WIFSIGNALED(status))
		return (g_js.pstat = WTERMSIG(status) + 128);
	else if (WIFSTOPPED(status))
		return (g_js.pstat = WSTOPSIG(status) + 128);
	return (g_js.pstat = WEXITSTATUS(status));
}

void		job_exec(t_job *job)
{
	if (job->type & JTCOND)
	{
		job_cond_fork(&job->proc, job->type & JTNOHANG);
	}
	else if (job->type & JTPIPE)
	{
		job_pipe_fork(&job->proc, job->type & JTNOHANG);
	}
	else
	{
		job_fork_alone(&job->proc, job->type & JTNOHANG);
	}
	free(job);
}