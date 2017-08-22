/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:56 by root              #+#    #+#             */
/*   Updated: 2017/08/16 04:01:33 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "ft_readline.h"
#include "lst.h"


int		set_manual_ttymode(void)
{
	struct termios termios;

	tcgetattr(STDIN_FILENO, &termios);
	g_otermios = termios;
	termios.c_oflag &= ~OPOST;
	termios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	return (tcsetattr(STDIN_FILENO, TCSANOW, &termios));
}

int		revert_manual_ttymode(void)
{
	return (tcsetattr(STDIN_FILENO, TCSANOW, &g_otermios));
}

/*********** screen *************/

int		update_winsize(void)
{
	struct winsize w;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
	g_winsize.col = w.ws_col;
	g_winsize.row = w.ws_row;
	return (0);
}

/********************************/

/********keyboard event**********/

void	place_holder(void)
{
	return ;
}

void	ctrl_event(uint64_t c)
{
	static void (*f[])() = {
		buff_head, buff_prev, place_holder, buff_delete, buff_end,
		buff_next, place_holder, place_holder, place_holder, exit_readline,
		buff_kill_next, buff_clear_content,
		place_holder, place_holder, place_holder,
		place_holder, place_holder, place_holder, place_holder, place_holder,
		buff_kill_prev, place_holder, buff_del_word, place_holder, buff_yankout,
		place_holder, place_holder, place_holder, place_holder, place_holder,
		buff_revert
	};

	f[c - 1]();
	last_action = f[c - 1];
}

/*
**f forward word
**b backward word
**r reset history and prompt
**u upper case
**l lower case
**c forward word upper first letter
*/

void	meta_event(void)
{
	uint64_t	c;

	c = 0;
	read(STDIN_FILENO, &c, sizeof(c));
	if (c == 'f')
		(last_action = buff_next_word)();
	else if (c == 'b')
		(last_action = buff_prev_word)();
	else if (c == 'u')
		(last_action = buff_lowtoup)();
	else if (c == 'l')
		(last_action = buff_uptolow)();
	else if (c == 'r')
		(last_action = buff_clear_line)();
	else if (c == 'c')
		(last_action = buff_capitalize)();
}

void	special_event(uint64_t c)
{
	static void (*special1[8])() = {
		place_holder, place_holder, buff_next, buff_prev,
		place_holder, buff_head, place_holder, buff_end};
	static void (*special2[4])() = {
		place_holder, place_holder, buff_next_word, buff_prev_word};
	static void (*special3[4])() = {
		buff_del_next, place_holder, place_holder, place_holder};

	c = c >> 16;
	if (c >= 0x41 && c <= 0x48)
	{
		(last_action = special1[c - 0x41])();
	}
	else if (IS_CTRL_ARROW(c))
	{
		(last_action = special2[(c >> 24) - 0x41])();
	}
	else if ((c & 0x7e30) == 0x7e30 &&
			((c & 0xf) > 2 && (c & 0xf) <= 6))
	{
		(last_action = special3[(c ^ 0x7e30) - 0x3])();
	}
}

void	keyboard_event(uint64_t	c, int r)
{
	if (IS_BACKSPACE_KEYCODE(c))
		buff_del_prev();
	else if (IS_SPECIAL_KEYCODE(c))
		special_event(c);
	else if (IS_META_MODIFIER(c))
		meta_event();
	else if (IS_CTRL_MODIFIER(c))
		ctrl_event(c);
	else
		buff_insert(&c, r);
	buff_yankin();
}

/********************************/

void	reload_line(int s)
{
	int	row;

	(void)s;
	update_winsize();
	row = (g_psize + g_cubuf) / g_winsize.col;
	while (row--)
		tputs(tgetstr("up", 0), 0, putchar_);
	tputs(tgetstr("cr", 0), 0, putchar_);
	tputs(tgetstr("cd", 0), 0, putchar_);
//write(STDERR_FILENO, g_prompt, strlen(g_prompt));
	buff_refresh(0, g_buffer.s, g_buffer.len);
	shift_cursor(g_buffer.len, g_cubuf);
}

int		init_readline(void)
{
	g_cubuf = 0;
	g_buffer.len = 0;
	return (buff_malloc(DEFAULT_BUFFER_SIZE));
}


void	exit_readline(void)
{
	buff_newline();
	g_buffer.msize = 0;
}

char	*ft_readline(char *prompt, size_t psize)
{
	uint64_t	c;
	int			r;
// is_atty
	if (init_readline())
		return ((char *)0);
	if (set_manual_ttymode())
	{
		free(g_buffer.s);
		tcsetattr(STDIN_FILENO, TCSADRAIN, &g_otermios);
		return ((char *)0);
	}
	tgetent(getenv("TERM"), NULL);
	update_winsize();
//	signal(SIGINT, );
	while (g_buffer.msize)
	{
		signal(SIGWINCH, reload_line);
		c = 0;
		r = read(STDIN_FILENO, &c, sizeof(c));
		keyboard_event(c, r);
	}
	revert_manual_ttymode();
	g_buffer.s[g_buffer.len] = 0;
	return (g_buffer.s);
}