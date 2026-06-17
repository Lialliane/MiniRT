// sudo gcc main.c MLX42/build/libmlx42.a -I MLX42/include/MLX42 -I include -ldl -lglfw -pthread -lm

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include "MLX42.h"
#include <math.h>


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

t_vec	vec_sum(const int sum_count, ...)
{
	va_list	arguments;
	int		i;
	t_vec sum_vec = {0, 0, 0};
	t_vec temp_vec;
	va_start(arguments, sum_count);
	i = 0;
	while (i < sum_count)
	{
		temp_vec = va_arg(arguments, t_vec);
		sum_vec.x += temp_vec.x;
		sum_vec.y += temp_vec.y;
		sum_vec.z += temp_vec.z;
		i++;
	}
	va_end(arguments);
	return (sum_vec);
}

t_vec	vec_sub(const int sum_count, ...)
{
	va_list	arguments;
	int		i;
	t_vec sub_vec;
	t_vec temp_vec;

	va_start(arguments, sum_count);
	sub_vec = va_arg(arguments, t_vec);
	i = 1;
	while (i < sum_count)
	{
		temp_vec = va_arg(arguments, t_vec);
		sub_vec.x -= temp_vec.x;
		sub_vec.y -= temp_vec.y;
		sub_vec.z -= temp_vec.z;
		i++;
	}
	va_end(arguments);
	return (sub_vec);
}

t_vec vec_scale(t_vec vec, double scalar)
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

	magnitude = pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2);
	magnitude = sqrt(magnitude);
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


t_vec vec_init(int x, int y, int z)
{
    t_vec vec;

    vec.x = x;
    vec.y = y;
    vec.z = z;
    return (vec);
}

// returns a color based on the direction of the ray, just for testing purposes
uint32_t ray_color(t_ray r) 
{
	double a;
    t_vec dir = vec_normalize(r.direction);
    a = 0.5 * (dir.y + 1.0);

    int r1 = (1.0 - a) * 255 + a * 128; // white(255) → blue(128)
    int g1 = (1.0 - a) * 255 + a * 179;
    int b1 = (1.0 - a) * 255 + a * 255;

    return ft_pixel(r1, g1, b1, 255);
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

			int distance = pow(dx, 2) + pow(dy, 2);

			// if point is within raduis, it's in circle, draw white dot
			if (distance <= pow(radius, 2))
				mlx_put_pixel(image, x, y,
					ft_pixel(255, 255, 255, 255));
			else
				mlx_put_pixel(image, x, y,
					ft_pixel(0, 0, 0, 255));
		}
	}
}

// create a ray based on each pixel's position on the viewport
// the viewport is an imaginary plane in front of the camera that the rays will pass through
void shoot_ray(void* param)
{
	(void)param;
	t_vec camera_pos = vec_init(0, 0, 0);
	t_vec focal_point = vec_init(0, 0, 1);
	int viewport_width = 2;
	int viewport_height = 2;


	// the viewport is centered on the focal point, so we need to calculate 
	// the right and bottom vectors to get the upper left corner of the viewport
	t_vec viewport_rght = vec_init(viewport_width, 0, 0 );
	t_vec viewport_bttm = vec_init(0, -viewport_height, 0);

	// each unit vector is the direction from one pixel to the next on the viewport,
	//  used to calculate the position of each pixel on the viewport
	t_vec viewport_unit_rght = vec_scale(viewport_rght, 1.0 / WIDTH);
	t_vec viewport_unit_bttm = vec_scale(viewport_bttm, 1.0 / HEIGHT);


	t_vec upper_left_corner = vec_sub(4, camera_pos, focal_point, vec_scale(viewport_rght, 1/2 ), vec_scale(viewport_bttm, 1/2));
	t_vec first_pixel = vec_sum(3, upper_left_corner, vec_scale(viewport_unit_rght, 1/2 ), vec_scale(viewport_unit_bttm, 1/2 ));

	for (uint32_t y = 0; y < image->height; y++)
	{
		for (uint32_t x = 0; x < image->width; x++)
		{
			t_vec pixel_pos = vec_sum(3, first_pixel, vec_scale(viewport_unit_rght, x), vec_scale(viewport_unit_bttm, y));
			t_vec ray_dir = vec_normalize(vec_sub(2, pixel_pos, camera_pos));
			t_ray ray = {camera_pos, ray_dir};
			// printf("Ray direction for pixel (%d, %d): (%f, %f, %f)\n", x, y, ray_dir.x, ray_dir.y, ray_dir.z);
			// printf("Ray color for pixel (%d, %d): %d\n", x, y, ray_color(ray));
			mlx_put_pixel(image, x, y, ray_color(ray));
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
	
	mlx_loop_hook(mlx, shoot_ray, mlx);
	mlx_loop_hook(mlx, ft_hook, mlx);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}


// t_vec add();
// t_vec sub();
// double dot();
// t_vec normalize();