#include "minirt.h"

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
	double magnitude;
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

double dot_product(t_vec vec1, t_vec vec2)
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