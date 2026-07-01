#ifndef MINIRT_H
# define MINIRT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include "MLX42.h"
#include <math.h>


#define WIDTH 1024
#define ASPECT_RATIO (16.0 / 9.0)
#define HEIGHT (int)(WIDTH / ASPECT_RATIO)
#define PI 3.14159265358979323846

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

typedef struct s_circle
{
	t_vec center;
	double radius;
} t_circle;

typedef struct s_hit_point
{
	t_vec point;
	t_vec normal;
	double t;
} t_hit_point;

typedef struct s_color
{
	double r;
	double g;
	double b;
} t_color;

// typedef struct s_object
// {
// 	(double) hit (t_ray r);
// } t_object;


t_vec	vec_sum(const int sum_count, ...);
t_vec	vec_sub(const int sum_count, ...);
t_vec vec_scale(t_vec vec, double scalar);
t_vec vec_normalize(t_vec vec);
double  dot_product(t_vec vec1, t_vec vec2);
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
t_vec vec_init(double x, double y, double z);
t_vec ray_fun(t_ray ray, double t);



#endif
