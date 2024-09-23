/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_filename.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:23:21 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 18:25:14 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	read_into_filename(int fd, char *buf, char *filename)
{
	int	i;

	i = 0;
	ft_strlcpy(filename, "/tmp/", 6);
	i = -1;
	while (i < 4)
	{
		if (read(fd, buf + i, 1) == -1)
			ft_exit_close(255);
		if (ft_isalnum(buf[i]))
			i++;
	}
	buf[i] = '\0';
	ft_strlcat(filename, buf, 15);
}

char	*get_random_filename(void)
{
	char	*filename;
	char	buf[5];
	int		fd;

	filename = malloc(sizeof(char) * 16);
	if (!filename || gbg_coll(filename, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (perror("open"), ft_exit_close(255), NULL);
	ft_strlcpy(filename, "/tmp/", 6);
	read_into_filename(fd, buf, filename);
	close(fd);
	return (filename);
}

char	*create_random_filename(void)
{
	char	*filename;

	filename = get_random_filename();
	if (access(filename, F_OK) == -1)
		return (filename);
	else
		return (create_random_filename());
}
