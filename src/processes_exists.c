/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_exists.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 13:27:27 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/08 13:35:06 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

static char		*read_cmd_contents(int fd, int size)
{
	int		ret		= 0;
	char	*buffer	= NULL;

	if ((buffer = ft_strnew(size)) == NULL)
		return NULL;
	if ((ret = read(fd, buffer, size)) == -1)
	{
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

static char		*get_cmd_contents(const char *filename)
{
	int			fd;
	struct stat	st;
	char		*result;

	if (stat(filename, &st) == -1)
		return (ft_strnew(0));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		return (ft_strnew(0));
	}
	result = read_cmd_contents(fd, 1000);
	close(fd);
	return (result);
}

static char		**add_element(char **tab, char *element)
{
	char	**ntab;
	int		i;

	if (!(ntab = (char**)malloc(sizeof(char*)*(array_length(tab) + 2))))
		return (NULL);
	i = 0;
	while (tab != NULL && tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	ntab[i] = element;
	ntab[i + 1] = NULL;
	if (tab != NULL)
		free(tab);
	return (ntab);
}

static char		**get_cmdlines(char **files, char *start_path, BOOLEAN recurs)
{
	DIR				*directory = opendir(start_path);
	struct dirent	*d;
	char			*absolute_path;
	char			*file_path;
	char			**dirs;

	if (!directory)
		return (NULL);
	dirs = NULL;
	absolute_path = strdup(start_path);
	while ((d = readdir(directory)) != NULL)
	{
		if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
			continue ;
		file_path = ft_dstrjoin(ft_dstrjoin(absolute_path, "/", 0), d->d_name, 1);
		if (file_exists(file_path) == false)
		{
			free(file_path);
			continue ;
		}
		if (is_directory(file_path) == true && atoi(d->d_name) > 0)
		{
			if (recurs)
				dirs = add_element(dirs, strdup(file_path));
			free(file_path);
			continue ;
		}
		if (strcmp(d->d_name, "cmdline") == 0 && recurs == false)
		{
			files = add_element(files, strdup(file_path));
		}
		free(file_path);
	}
	int i = 0;
	while (i < (int)array_length(dirs))
	{
		files = get_cmdlines(files, dirs[i], false);
		free(dirs[i]);
		i++;
	}
	if (dirs != NULL)
		free(dirs);
	closedir(directory);
	return (files);
}

static int	get_process_pid(const char *filename)
{
	char **split = ft_split_string(filename, "/");
	int pid = -1;

	if (array_length(split) != 3)
		return (pid);
	pid = atoi(split[1]);
	free_array(split);
	return (pid);
}

BOOLEAN		processes_exists(char **names)
{
	char	**cmdlines = get_cmdlines(NULL, "/proc", true);
	int		i = 0;
	int		x = 0;
	BOOLEAN	result = false;
	int		current_pid = getpid();

	if (cmdlines)
	{
		while (cmdlines[i])
		{
			int pid = get_process_pid(cmdlines[i]);

			if (pid == -1 || pid == current_pid)
			{
				i++;
				continue ;
			}
			char *process_name = get_cmd_contents(cmdlines[i]);
			char *basename = file_base_name(process_name);
			x = 0;
			while (result == false && names && names[x]) {
				if (!strcmp(basename, names[x])) {
					result = true;
				}
				x++;
			}
			free(basename);
			free(process_name);
			free(cmdlines[i]);
			i++;
		}
		free(cmdlines);
	}
	return (result);
}

int			processes_exists_pid(char **names)
{
	char	**cmdlines = get_cmdlines(NULL, "/proc", true);
	int		i = 0;
	int		x = 0;
	int		result = -1;
	int		current_pid = getpid();

	if (cmdlines)
	{
		while (cmdlines[i])
		{
			int pid = get_process_pid(cmdlines[i]);

			if (pid == -1 || pid == current_pid)
			{
				i++;
				continue ;
			}
			char *process_name = get_cmd_contents(cmdlines[i]);
			char *basename = file_base_name(process_name);
			x = 0;
			while (result == -1 && names && names[x]) {
				if (!strcmp(basename, names[x])) {
					result = pid;
				}
				x++;
			}
			free(basename);
			free(process_name);
			free(cmdlines[i]);
			i++;
		}
		free(cmdlines);
	}
	return (result);
}
