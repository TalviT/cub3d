/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 11:33:48 by jlohmann          #+#    #+#             */
/*   Updated: 2023/04/14 02:37:34 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"
# include "graphics.h"
# include "cub3d_error.h"
# include "get_next_line.h"

# define SCREEN_WIDTH 1920
# define SCREEN_HEIGHT 1080
# define FOV 0.666
# define MAP_WIDTH 300
# define MAP_HEIGHT 300
# define MOVE_SPEED 0.05
# define ROT_SPEED 0.04
# define MOUSE_SENSITIVITY 0.002

// --- Game object structs --- //

typedef struct s_player {
	t_vec		pos;
	t_vec		dir;
	t_vec		plane;
	mlx_image_t	*img;
}	t_player;

typedef struct s_map {
	uint16_t	width;
	uint16_t	height;
	char		*data;
	mlx_image_t	*img;
}	t_map;

typedef struct s_tex {
	mlx_texture_t	*t_north;
	mlx_texture_t	*t_south;
	mlx_texture_t	*t_west;
	mlx_texture_t	*t_east;
	int32_t			floor;
	int32_t			ceiling;
}	t_tex;

typedef struct s_scene {
	mlx_t		*mlx;
	mlx_image_t	*background;
	mlx_image_t	*screen;
	t_map		map;
	t_tex		tex;
	t_player	player;
}	t_scene;

// --- Helper structs --- //

typedef struct s_keystate {
	bool	w;
	bool	a;
	bool	s;
	bool	d;
	bool	left;
	bool	right;
	bool	shift;
}	t_keystate;

typedef struct s_dda_params {
	double	camera_x;
	t_vec	ray_dir;
	t_point	block_pos;
	t_point	step_dir;
	t_vec	dist;
	t_vec	delta;
}	t_dda_params;

typedef struct s_hit_info {
	double	dist;
	t_vec	pos;
	t_point	block_pos;
	int		side;
}	t_hit_info;

// parsing
int			tex_init(t_scene *scene);
char		*convert_map(char **map, t_scene *scene);
int			find_start(int start[2], t_scene *scene);
void		*create_vector(int x, int y);
int			check_map(char **map, t_scene *scene);
void		clean_pars(t_scene *scene);
int			get_cf_colors(int fd, t_scene *scene);
int			get_map(int fd, t_scene *scene);
int			get_textures(int fd, t_scene *scene);
int			parse_map(char **map);
int			pars(char *path, t_scene *scene);
char		*skip_space(char *line);
char		*first_word(char *line);
char		*skip_number(char *line);
char		*cub_trim(char *line);

// init_utils.c
mlx_image_t	*init_image(mlx_t *mlx, int32_t x, int32_t y, uint32_t width, uint32_t height);
// scene.c
void		scene_init(t_scene *scene);
void		scene_draw(t_scene *scene);
void		scene_destroy(t_scene *scene);
// map.c
void		map_init(mlx_t *mlx, t_map *map, char *file_path);
void		map_draw(t_map *map);
void		map_destroy(mlx_t *mlx, t_map *map);
// player.c
void		player_init(mlx_t *mlx, t_player *player, t_vec pos, t_vec dir);
void		player_update(mlx_t *mlx, t_player *player, t_map *map);
void		player_draw(t_player *player, t_map *map);
void		player_destroy(mlx_t *mlx, t_player *player);
// player_utils.c
void		player_move(t_keystate *state, t_player *player, t_map *map);
void		player_rotate(t_keystate *state, t_player *player);
// hooks.c
void		key_hook(mlx_key_data_t keydata, void *param);
void		cursor_hook(double xpos, double ypos, void *param);
void		update(void *param);
// input.c
t_keystate	input_get_keys(mlx_t *mlx);
// raycaster.c
t_hit_info	ray_cast(int x, t_player *player, t_map *map);
void		draw_wall_line(mlx_image_t *screen, int32_t x, double dist, uint32_t color);
// common_utils.c
t_vec		vec_add(t_vec a, t_vec b);
t_vec		vec_scale(t_vec vec, double factor);
t_vec		vec_rotate(t_vec vec, double angle);
// error_handling.c
//void		print_help(void);
void		panic(const char *msg);
void		mlx_panic(void);

#endif
