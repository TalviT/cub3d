/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlohmann <jlohmann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:06:21 by jlohmann          #+#    #+#             */
/*   Updated: 2023/05/04 17:14:18 by jlohmann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_background(mlx_image_t *bg, t_color ceiling, t_color floor)
{
	int		y;
	t_color	color;

	y = 0;
	while (y < 0.75 * SCREEN_HEIGHT)
	{
		color = color_dim(ceiling, 1 - (0.9 * y) / (0.75 * SCREEN_HEIGHT));
		uni_memset(
			bg->pixels + (y * bg->width * BPP),
			&color.pixel, BPP, bg->width);
		++y;
	}
	while (y < 1.5 * SCREEN_HEIGHT)
	{
		color = color_dim(floor, y / (0.75 * SCREEN_HEIGHT) - 0.9);
		uni_memset(
			bg->pixels + (y * bg->width * BPP),
			&color.pixel, BPP, bg->width);
		++y;
	}
}

bool	scene_init(t_scene *scene)
{
	scene->mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", false);
	if (scene->mlx == NULL)
		return (false);
	mlx_set_cursor_mode(scene->mlx, MLX_MOUSE_HIDDEN);
	scene->background = init_image(
			scene->mlx,
			(t_rect){0, -SCREEN_HEIGHT / 4, SCREEN_WIDTH, 1.5 * SCREEN_HEIGHT});
	if (scene->background == NULL)
		return (scene_destroy(scene, 0), false);
	set_background(scene->background, scene->tex.ceiling, scene->tex.floor);
	scene->screen = init_image(
			scene->mlx,
			(t_rect){0, -SCREEN_HEIGHT / 4, SCREEN_WIDTH, 1.5 * SCREEN_HEIGHT});
	if (scene->screen == NULL)
		return (scene_destroy(scene, 1), false);
	scene->fps = mlx_put_string(scene->mlx, "0  ", SCREEN_WIDTH - 100, 0);
	if (scene->fps == NULL)
		return (scene_destroy(scene, 2), false);
	if (!map_init(scene->mlx, &scene->map))
		return (scene_destroy(scene, 3), false);
	player_init(&scene->player);
	return (true);
}

void	scene_draw(t_scene *scene)
{
	int			screen_x;
	double		x;
	t_vec		ray_dir;
	t_hit_info	hit;

	draw_fill(scene->screen, (t_color){0x00000000});
	screen_x = 0;
	while (screen_x < SCREEN_WIDTH)
	{
		x = 2 * screen_x / (double)SCREEN_WIDTH - 1;
		ray_dir = vec_add(scene->player.dir, vec_scale(scene->player.plane, x));
		hit = ray_cast(&scene->player, &scene->map, ray_dir);
		draw_wall_line(scene, screen_x, hit);
		++screen_x;
	}
}

void	scene_destroy(t_scene *scene, int layer)
{
	if (scene->tex.t_north != NULL)
		mlx_delete_texture(scene->tex.t_north);
	if (scene->tex.t_west != NULL)
		mlx_delete_texture(scene->tex.t_west);
	if (scene->tex.t_south != NULL)
		mlx_delete_texture(scene->tex.t_south);
	if (scene->tex.t_east != NULL)
		mlx_delete_texture(scene->tex.t_east);
	if (layer >= 1)
		mlx_delete_image(scene->mlx, scene->background);
	if (layer >= 2)
		mlx_delete_image(scene->mlx, scene->screen);
	if (layer >= 3)
		mlx_delete_image(scene->mlx, scene->fps);
	if (layer >= 0)
		mlx_terminate(scene->mlx);
}
