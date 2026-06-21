// gcc main.c utils.c MLX42/build/libmlx42.a -I MLX42/include/MLX42 -I include -ldl -lglfw -pthread -lm

#include "minirt.h"

static mlx_image_t* image;

int32_t color(double r, double g, double b)
{
	int red = r * 255;
	int green = g * 255;
	int blue = b * 255;

	return ft_pixel(red, green, blue, 255);
}

double hit_sphere(t_vec center, double radius, t_ray r) {
    t_vec cen_to_og= vec_sub(2, center, r.origin);
    double a = dot_product(r.direction, r.direction);
    double h = dot_product(r.direction, cen_to_og);
    double c = dot_product(cen_to_og, cen_to_og) - radius*radius;
    double discriminant = pow(h,2) - (a*c);

	if(discriminant < 0)
		return (-1.0); // false
	
	// double sqrtd = sqrt(discriminant);
	// double root = (h - sqrtd) / a;
	// double ray_tmin = 0.001;
	// double ray_tmax = INFINITY; 
	// if (root <= ray_tmin || ray_tmax <= root) {
    //         root = (h + sqrtd) / a;
    //         if (root <= ray_tmin || ray_tmax <= root)
    //             return false;
    //     }
	// double t = root;
	// t_vec hit_point = vec_sum(2, r.origin, vec_scale(r.direction, t));
	// t_vec normal = vec_normalize(vec_sub(2, hit_point, center));

    return (h - sqrt(discriminant) ) / a;
}

uint32_t ray_color(t_ray r) {
	t_vec center = vec_init(0, 0, -1);
	double t = hit_sphere(center, 0.5, r);
	if(t > 0.0)
	{
		t_vec N = vec_normalize(vec_sub(2, vec_sum(2, r.origin, vec_scale(r.direction, t)), center));
		// printf("color is (%f, %f, %f)\n", 0.5 * (N.x + 1), 0.5 * (N.y + 1), 0.5 * (N.z + 1));
		return color(0.5 * (N.x + 1), 0.5 * (N.y + 1), 0.5 * (N.z + 1) );
	}

	t_vec unit_direction = vec_normalize(r.direction);
    double a = 0.5*(unit_direction.y + 1.0);
    return color((1.0-a), (1.0-a) ,(1.0-a)) + color(a * 0.5, a * 0.7, a * 1.0);
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

	t_vec upper_left_corner = vec_sub(4, camera_pos, focal_point, vec_scale(viewport_rght, 0.5 ), vec_scale(viewport_bttm, 0.5));
	t_vec first_pixel = vec_sum(3, upper_left_corner, vec_scale(viewport_unit_rght, 0.5 ), vec_scale(viewport_unit_bttm, 0.5 ));

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
