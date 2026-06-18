#ifndef MINIRT_H
# define MINIRT_H

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


t_vec	vec_sum(const int sum_count, ...);
t_vec	vec_sub(const int sum_count, ...);
t_vec vec_scale(t_vec vec, double scalar);
t_vec vec_normalize(t_vec vec);
double  dot_product(t_vec vec1, t_vec vec2);
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
t_vec vec_init(int x, int y, int z);



#endif
