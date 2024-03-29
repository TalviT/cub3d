/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlohmann <jlohmann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 10:30:50 by pharbst           #+#    #+#             */
/*   Updated: 2023/05/03 02:20:16 by jlohmann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_error.h"

char	*convert_map(char **map, t_scene *scene)
{
	char	*ret_map;
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (map[++i])
		if ((int)ft_strlen(map[i]) > j)
			j = ft_strlen(map[i]);
	scene->map.width = j;
	ret_map = ft_calloc(j * scene->map.height + 1, sizeof(char));
	if (!ret_map)
		return (cub_errno(WRITE, ERALLOC), NULL);
	i = -1;
	while (map[++i])
	{
		ft_memcpy(ret_map + (i * j), map[i], ft_strlen(map[i]));
		if ((int)ft_strlen(map[i]) < j)
			ft_memset(ret_map + (i * j) + ft_strlen(map[i]),
				'0', j - ft_strlen(map[i]));
		free(map[i]);
	}
	free(map);
	return (ret_map);
}

static int	start_assistant(t_scene *scene, int coords[2], int start[2])
{
	if (ft_strchr("NSEW", scene->map.data[coords[0]
				+ coords[1] * scene->map.width]))
	{
		if (start[0] != -1)
			return (cub_errno(WRITE, ERMULTIPLAY), 1);
		if (scene->map.data[coords[0] + coords[1] * scene->map.width] == 'N')
			scene->player.dir = (t_vec){0, -1};
		if (scene->map.data[coords[0] + coords[1] * scene->map.width] == 'S')
			scene->player.dir = (t_vec){0, 1};
		if (scene->map.data[coords[0] + coords[1] * scene->map.width] == 'W')
			scene->player.dir = (t_vec){-1, 0};
		if (scene->map.data[coords[0] + coords[1] * scene->map.width] == 'E')
			scene->player.dir = (t_vec){1, 0};
		start[0] = coords[0];
		start[1] = coords[1];
		scene->player.pos = (t_vec){coords[0] + 0.5, coords[1] + 0.5};
	}
	coords[0]++;
	if (coords[0] == scene->map.width)
	{
		coords[0] = 0;
		coords[1]++;
	}
	return (0);
}

int	find_start(int start[2], t_scene *scene)
{
	int	coords[2];

	coords[0] = 0;
	coords[1] = 0;
	start[0] = -1;
	start[1] = -1;
	while (scene->map.data[coords[0] + coords[1] * scene->map.width])
		if (start_assistant(scene, coords, start))
			return (1);
	if (start[0] == -1)
		return (cub_errno(WRITE, ERNOPLAY), 1);
	return (0);
}

void	*create_vector(int x, int y)
{
	int	*vector;

	vector = malloc(sizeof(int *));
	if (!vector)
		return (cub_errno(WRITE, ERALLOC), NULL);
	vector[0] = x;
	vector[1] = y;
	return (vector);
}
