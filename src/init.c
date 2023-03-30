/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlohmann <jlohmann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:20:31 by jlohmann          #+#    #+#             */
/*   Updated: 2023/03/30 19:01:48 by jlohmann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	mlx_panic(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void	map_init(struct s_map *map, uint16_t width, uint16_t height, char *file_path)
{
	(void)!file_path; // TODO: use for file parsing.
	map->width = width; map->height = height;
	map->data = malloc(sizeof(char) * width * height);
	if (map->data == NULL)
		mlx_panic(); // not a mlx_error. Call another function to panic.
	map->data = memcpy(map->data, (char[256]){
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,
		1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,1,
		1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,1,
		1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
		1,0,1,1,1,1,0,0,0,0,1,0,1,0,0,1,
		1,0,1,0,0,1,0,1,1,1,1,0,0,1,0,1,
		1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,1,
		1,0,1,0,0,0,0,1,0,0,0,1,0,0,1,1,
		1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	}, 256);
}

void	scene_init(struct s_scene *scene)
{
	scene->mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d", true);
	if (scene->mlx == NULL)
		mlx_panic();
	scene->screen = mlx_new_image(scene->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (scene->screen == NULL)
		mlx_panic();
	img_fill_color(scene->screen, 0x000000FF);
	if (mlx_image_to_window(scene->mlx, scene->screen, 0, 0) < 0)
		mlx_panic();
	// v-- TODO: set map with file parsing function
	map_init(&scene->map, 16, 16, NULL); // don't change these values until file parser is implemented!
	draw_map(scene);
	scene->player = (struct s_player){(t_point){300, 300}, (t_vec){0, 5}, M_PI_2, mlx_new_image(scene->mlx, 16, 16)};
	if (scene->player.image_2d == NULL)
		mlx_panic();
	if (mlx_image_to_window(scene->mlx, scene->player.image_2d, scene->player.pos.x, scene->player.pos.y) < 0)
		mlx_panic();
	draw_player(&scene->player);
}
