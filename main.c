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

double hit_sphere(t_vec center, double radius, t_ray r, t_hit_point* hit_point)
{
    t_vec cen_to_og = vec_sub(2, center, r.origin);
    double a = dot_product(r.direction, r.direction);
    double h = dot_product(r.direction, cen_to_og);
    double c = dot_product(cen_to_og, cen_to_og) - radius*radius;
    double discriminant = pow(h,2) - (a*c);

	if(discriminant < 0)
		return (false); // false
	
	double root = (h - sqrt(discriminant)) / a;
	double ray_tmin = 0.001;
	double ray_tmax = INFINITY;

	// 	double closest_t = INFINITY;  for later
	// if (t > 0.001 && t < closest_t)
	// {
	// 	closest_t = t;
	// }
	if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrt(discriminant)) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }
	hit_point->t = root;
	hit_point->point = ray_fun(r, hit_point->t);
	hit_point->normal = vec_normalize(vec_sub(2, hit_point->point, center));
	hit_point->front_face = dot_product(r.direction, hit_point->normal) < 0;
	hit_point->normal = hit_point->front_face ? hit_point->normal : vec_scale(hit_point->normal, -1);

    return true;
}

uint32_t ray_color(t_ray r, t_vec center) {
	t_hit_point *hit_point = malloc(sizeof(t_hit_point));
	if (hit_point == NULL) {
		fprintf(stderr, "Memory allocation failed for hit_point\n");
		exit(EXIT_FAILURE);
	}
	// TODO change hit function to match object type
	hit_sphere(center, 0.5, r, hit_point);
	
	if(hit_point->t > 0.0)
	{
		t_vec N = vec_normalize(vec_sub(2, ray_fun(r, hit_point->t), center));
		// printf("color is (%f, %f, %f)\n", 0.5 * (N.x + 1), 0.5 * (N.y + 1), 0.5 * (N.z + 1));
		return color(0.5 * (N.x + 1), 0.5 * (N.y + 1), 0.5 * (N.z + 1) );
	}

	t_vec unit_direction = vec_normalize(r.direction);
    double a = 0.5*(unit_direction.y + 1.0);
	free(hit_point);
    return color((1.0-a), (1.0-a) ,(1.0-a)) + color(a * 0.5, a * 0.7, a * 1.0);
}

typedef struct s_viewport
{
	t_vec upper_left_corner;
	t_vec first_pixel;
	t_vec viewport_unit_rght;
	t_vec viewport_unit_bttm;
	t_vec camera_pos;
} t_viewport;

void initialize(t_viewport* v)
{
	v->camera_pos = vec_init(0, 0, 0);
	t_vec focal_point = vec_init(0, 0, 1);
	int viewport_height = 2;
	int viewport_width = viewport_height * (double)(WIDTH/HEIGHT);

	// the viewport is centered on the focal point, so we need to calculate 
	// the right and bottom vectors to get the upper left corner of the viewport
	t_vec viewport_rght = vec_init(viewport_width, 0, 0 );
	t_vec viewport_bttm = vec_init(0, -viewport_height, 0);

	// each unit vector is the direction from one pixel to the next on the viewport,
	//  used to calculate the position of each pixel on the viewport
	v->viewport_unit_rght = vec_scale(viewport_rght, 1.0 / WIDTH);
	v->viewport_unit_bttm = vec_scale(viewport_bttm, 1.0 / HEIGHT);

	// Calculate the location of the upper left pixel.
	v->upper_left_corner = vec_sub(4, v->camera_pos, focal_point, vec_scale(viewport_rght, 0.5 ), vec_scale(viewport_bttm, 0.5));
	v->first_pixel = vec_sum(3, v->upper_left_corner, vec_scale(v->viewport_unit_rght, 0.5 ), vec_scale(v->viewport_unit_bttm, 0.5 ));
}

// create a ray based on each pixel's position on the viewport
// the viewport is an imaginary plane in front of the camera that the rays will pass through
void shoot_ray(void* param)
{
	(void)param;
	t_viewport *v = malloc(sizeof(t_viewport));
	if (!v) {
		fprintf(stderr, "Memory allocation failed for viewport\n");
		exit(EXIT_FAILURE);
	}
	initialize(v);

	for (uint32_t y = 0; y < image->height; y++)
	{
		for (uint32_t x = 0; x < image->width; x++)
		{
			t_vec pixel_pos = vec_sum(3, v->first_pixel, vec_scale(v->viewport_unit_rght, x), vec_scale(v->viewport_unit_bttm, y));
			t_vec ray_dir = vec_normalize(vec_sub(2, pixel_pos, v->camera_pos));
			t_ray ray = {v->camera_pos, ray_dir};
			// printf("Ray direction for pixel (%d, %d): (%f, %f, %f)\n", x, y, ray_dir.x, ray_dir.y, ray_dir.z);
			// printf("Ray color for pixel (%d, %d): %d\n", x, y, ray_color(ray));
			
			// circle center defined here for now
			mlx_put_pixel(image, x, y, ray_color(ray, vec_init(0, 0, -1)));
		}
	}

}

void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
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
