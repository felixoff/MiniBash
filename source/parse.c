/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weambros <weambros@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 18:53:38 by weambros          #+#    #+#             */
/*   Updated: 2021/04/22 06:43:54 by weambros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_get_cmds_line_2(char **s, t_spec_chr *spec, char *tmp)
{
	char	*res;

	res = 0;
	while (**s)
	{
		ft_switch_quotes(spec, **s);
		if (!ft_check_spec(spec) && **s == ';')
		{
			**s = '\0';
			res = ft_strdup(tmp);
			if (!res)
				ft_exit_fatal(MSG_ERR_NO_MALLOC);
			(*s)++;
			return (res);
		}
		(*s)++;
	}
	return (0);
}

char	*ft_get_cmds_line(char **s)
{
	t_spec_chr	*spec;
	char		*res;
	char		*tmp;

	res = 0;
	tmp = *s;
	spec = ft_init_spec();
	res = ft_get_cmds_line_2(s, spec, tmp);
	if (res)
	{
		spec = ft_free(spec);
		return (res);
	}
	spec = ft_free(spec);
	res = ft_strdup(tmp);
	if (!res)
		ft_exit_fatal(MSG_ERR_NO_MALLOC);
	return (res);
}

t_parse	*ft_init_parse(void)
{
	t_parse	*p;

	p = malloc(sizeof(t_parse));
	if (!p)
		ft_exit_fatal(MSG_ERR_NO_MALLOC);
	p->j = 0;
	p->i = 0;
	p->token = -1;
	p->cmds = 0;
	p->cmds_line = 0;
	return (p);
}

void	ft_parse_2(t_parse *p)
{
	while (p->cmds_line[p->i])
	{
		ft_get_token(p->cmds_line, &p->i, &p->str_token, &p->token);
		ft_lstcmdsadd_back(&p->cmds, \
			ft_lstcmdsnew(ft_get_args(p->str_token), p->token));
		p->str_token = ft_free(p->str_token);
	}
	p->i = 0;
}

void	ft_parse(char *line)
{
	t_parse	*p;

	p = ft_init_parse();
	while (*line)
	{
		p->cmds_line = ft_get_cmds_line(&line);
		ft_insert_env_to_args(&p->cmds_line);
		if (!p->cmds_line || p->cmds_line[0] == '\0')
		{
			p->cmds_line = ft_free(p->cmds_line);
			break ;
		}
		ft_parse_2(p);
		ft_check(p->cmds);
		ft_run_command(p->cmds);
		ft_lstcmdsdel(&p->cmds);
		p->cmds_line = ft_free(p->cmds_line);
	}
	g_pid = 0;
	p = ft_free(p);
}
