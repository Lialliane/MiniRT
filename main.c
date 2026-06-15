// sudo gcc main.c MLX42/build/libmlx42.a -I MLX42/include/MLX42 -I include -ldl -lglfw -pthread -lm

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MLX42.h"


#define WIDTH 512
#define HEIGHT 512

typedef struct s_vector
{
	double x;
	double y;
	double z;
} t_vec;

typedef struct s_ray
{
	t_vec origin;
	t_vec direction;
} t_ray;

static mlx_image_t* image;

t_vec vec_sum(t_vec vec1, t_vec vec2)
{
	t_vec sum_vec;

	sum_vec.x = vec1.x + vec2.x;
	sum_vec.y = vec1.y + vec2.y;
	sum_vec.z = vec1.z + vec2.z;
	return (sum_vec);
}

t_vec vec_sub(t_vec vec1, t_vec vec2)
{
	t_vec sub_vec;

	sub_vec.x = vec1.x - vec2.x;
	sub_vec.y = vec1.y - vec2.y;
	sub_vec.z = vec1.z - vec2.z;
	return (sub_vec);
}

t_vec vec_scale(t_vec vec, int scalar)
{
	t_vec mult_vec;

	mult_vec.x = vec.x * scalar;
	mult_vec.y = vec.y * scalar;
	mult_vec.z = vec.z * scalar;
	return (mult_vec);
}

// returns a unit vector in the same direction as the input vector
// done to make comparison of vectors direction easier 
t_vec vec_normalize(t_vec vec)
{
	int magnitude;
	t_vec norm_vec;

	magnitude = ft_pow(vec.x, 2) + ft_pow(vec.y, 2) + ft_pow(vec.z, 2);
	if (magnitude == 0)
		return ((t_vec){0, 0, 0});
	norm_vec.x = vec.x / magnitude;
	norm_vec.y = vec.y / magnitude;
	norm_vec.z = vec.z / magnitude;
	return (norm_vec);
}

int dot_product(t_vec vec1, t_vec vec2)
{
	return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
}


int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

int	ft_pow(int nb, int power)
{
	int	result;

	result = nb;
	if (power < 0)
		return (0);
	if (power == 0)
		return (1);
	else if (power == 1)
		return (nb);
	else
	{
		result *= ft_pow(nb, power - 1);
		return (result);
	}
}

t_vec vec_init(int x, int y, int z)
{
    t_vec vec;

    vec.x = x;
    vec.y = y;
    vec.z = z;
    return (vec);
}

void ft_draw_sphere(void* param)
{
	(void)param;

	int	center_x = image->width / 2;
	int	center_y = image->height / 2;
	int	radius = 40;

	for (uint32_t y = 0; y < image->height; y++)
	{
		for (uint32_t x = 0; x < image->width; x++)
		{
			// distance from center of image
			int dx = (int)x - center_x;
			int dy = (int)y - center_y;

			int distance = ft_pow(dx, 2) + ft_pow(dy, 2);

			// if point is within raduis, it's in circle, draw white dot
			if (distance <= ft_pow(radius, 2))
				mlx_put_pixel(image, x, y,
					ft_pixel(255, 255, 255, 255));
			else
				mlx_put_pixel(image, x, y,
					ft_pixel(0, 0, 0, 255));
		}
	}
}

void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	// if (mlx_is_key_down(mlx, MLX_KEY_UP))
	// 	image->instances[0].y -= 5;
	// if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
	// 	image->instances[0].y += 5;
	// if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	// 	image->instances[0].x -= 5;
	// if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
	// 	image->instances[0].x += 5;
}

// -----------------------------------------------------------------------------

int32_t main(void)
{
	mlx_t* mlx;

	// Gotta error check this stuff
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(image = mlx_new_image(mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	
	mlx_loop_hook(mlx, ft_draw_sphere, mlx);
	mlx_loop_hook(mlx, ft_hook, mlx);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}


// t_vec add();
// t_vec sub();
// double dot();
// t_vec normalize();