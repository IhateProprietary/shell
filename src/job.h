/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/22 10:44:01 by root              #+#    #+#             */
/*   Updated: 2017/10/04 19:42:52 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_H
# define JOB_H

# include "command.h"
# include "lst.h"

# include <unistd.h>

# define JTHANG   	000
# define JTNOHANG 	001
# define JTCOND	  	002
# define JTPIPE   	004

# define JTEXT		000
# define JTBUILTIN	010

# define JT_RUNNING		000
# define JT_DEAD		001
# define JT_BACKGROUND	002
# define JT_FOREGROUND	004
# define JT_SUSPENDED	010
# define JT_SIGNALED	020

# define JT_DEFAULT_SIZE 1024
# define JT_IS_NOHANG(x) (status == (127 << 16))

struct			s_jstat
{
	int		prev;
	int		cur;
	int		jnodecur;
	int		jnodesize;
	int		suspended;
	int		fg_status;
	int		exit;
};

typedef struct	s_process
{
	pid_t		pid;
	t_command	*c;
	int			flag;
	int			state;
}				t_process;

typedef struct	s_job
{
	t_lst		*proc;
	int			type;
}				t_job;

extern t_process		**g_jobs;
extern struct s_jstat	*g_js;

t_job					*job_create(t_lst **c);
void					job_exec(t_job *job);

void					job_setstatus(t_job *job, int status);
char					*job_getstatus(void);

int						test_execpath(char *c);
int						set_execpath(t_command *c);
void					transmute_av(t_command *c);
char					**set_envp(void);

void					job_restorefd(t_lst *redir);
t_lst					*job_openfd(t_lst *redir);
void					job_cond_fork(t_lst **c, int nohang);
void					job_fork_alone(t_lst **c, int nohang);
int						job_pipe_fork(t_lst **c, int nohang);
int						job_exec_process(t_process *p);
void					job_exec(t_job *job);
int						job_wait_control(t_process *proc, int options);
void					free_half_parsed_command(t_command *c);
void					free_full_parsed_command(t_command *c);

#endif
