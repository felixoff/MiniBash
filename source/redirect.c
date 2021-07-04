/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 03:20:03 by weambros          #+#    #+#             */
/*   Updated: 2021/04/24 18:53:21 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_run_r(t_lstcmds *cmds, t_lstcmds *prev)
{
	if (!prev)
		return ;
	if (prev->token == TOKEN_R_OUT)
		prev->fds[1] = \
		open(cmds->args[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (prev->token == TOKEN_R_D_OUT)
		prev->fds[1] = \
		open(cmds->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (prev->fds[1] < 0 && prev->fds[1] != -42)
	{
		ft_errno(1, SET);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
}

void	ft_run_r_in2(t_lstcmds *cmds, char *buf)
{
	int	byte;
	int	fd;

	fd = open(cmds->args[0], O_RDONLY);
	if (fd < 0)
	{
		ft_errno(1, SET);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return ;
	}
	byte = (int)read(fd, buf, 1);
	while (byte > 0)
	{
		if (write(1, buf, 1) < 0)
			ft_put_error(MSG_ERR_NO_WRITE);
		byte = (int)read(fd, buf, 1);
	}
	close(fd);
}

void	ft_run_r_in(t_lstcmds *cmds)
{
	char	*buf;
	int		ret;

	ret = 0;
	buf = malloc(1);
	if (!buf)
		ft_exit_fatal(MSG_ERR_NO_MALLOC);
	ft_run_r_in2(cmds, buf);
	buf = ft_free(buf);
	exit(ft_errno(0, GET));
}
