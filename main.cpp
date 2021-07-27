#include "main.h"

class image_info
{
public:
	size_t width;
	size_t height;
};

image_info save_mandelbrot_to_image(vector<unsigned char>& pixel_data, bool write_to_disk, const char* const filename, float x_grid_max, float x_grid_min, size_t x_res, float y_grid_max, float y_grid_min, const size_t cap_size, const vector<RGB>& colours)
{
	const size_t y_res = static_cast<size_t>(static_cast<float>(x_res) / ((x_grid_max - x_grid_min) / (y_grid_max - y_grid_min)));

	const complex<float> x_step_size((x_grid_max - x_grid_min) / (x_res - 1), 0);
	const complex<float> y_step_size(0, (y_grid_max - y_grid_min) / (y_res - 1));

	const unsigned short int max_iterations = 5285;
	const float threshold = 2.0f;

	pixel_data.resize(3 * x_res * y_res);

	get_mandelbrot_pixels(
		pixel_data,
		cap_size,
		colours,
		max_iterations,
		threshold,
		x_grid_min,
		y_grid_min,
		x_step_size,
		y_step_size,
		x_res,
		y_res);

	if (write_to_disk)
		write_RGB_tga(filename, x_res, y_res, pixel_data);

	image_info ii;

	ii.width = x_res;
	ii.height = y_res;

	return ii;
}





void write_region_of_interest_on_image(
	vector<unsigned char>& main_pixel_data,
	size_t main_x_res, size_t main_y_res,
	float main_x_grid_max, float main_x_grid_min,
	float main_y_grid_max, float main_y_grid_min,
	float roi_x_grid_max, float roi_x_grid_min,
	float roi_y_grid_max, float roi_y_grid_min)
{
	float total_x_dist = main_x_grid_max - main_x_grid_min;
	float actual_x_dist = main_x_grid_max - roi_x_grid_min;

	size_t x_begin = static_cast<size_t>(main_x_res * (1 - actual_x_dist / total_x_dist));

	float total_x_dist2 = main_x_grid_max - main_x_grid_min;
	float actual_x_dist2 = main_x_grid_max - roi_x_grid_max;

	size_t x_end = static_cast<size_t>(main_x_res * (1 - actual_x_dist2 / total_x_dist2));

	float total_y_dist = main_y_grid_max - main_y_grid_min;
	float actual_y_dist = main_y_grid_max - roi_y_grid_min;

	size_t y_begin = static_cast<size_t>(main_y_res * (1 - actual_y_dist / total_y_dist));

	float total_y_dist2 = main_y_grid_max - main_y_grid_min;
	float actual_y_dist2 = main_y_grid_max - roi_y_grid_max;

	size_t y_end = static_cast<size_t>(main_y_res * (1 - actual_y_dist2 / total_y_dist2));

	for (size_t i = 0; i < main_x_res; i++)
	{
		for (size_t j = 0; j < main_y_res; j++)
		{
			if ((i == x_begin || i == x_end) && (j >= y_begin && j <= y_end ) )
			{
				main_pixel_data[3 * (j * main_x_res + i) + 0] = 0;
				main_pixel_data[3 * (j * main_x_res + i) + 1] = 0;
				main_pixel_data[3 * (j * main_x_res + i) + 2] = 0;
			}
		}
	}

	for (size_t i = 0; i < main_x_res; i++)
	{
		for (size_t j = 0; j < main_y_res; j++)
		{
			if ((j == y_begin || j == y_end) && (i >= x_begin && i <= x_end))
			{
				main_pixel_data[3 * (j * main_x_res + i) + 0] = 0;
				main_pixel_data[3 * (j * main_x_res + i) + 1] = 0;
				main_pixel_data[3 * (j * main_x_res + i) + 2] = 0;
			}
		}
	}

}



int main(void)
{
	const size_t cap_size = 16;

	vector<RGB> colours(cap_size);

	for (size_t i = 0; i < cap_size; i++)
	{
		//colours[i].r = rand() % 256;
		//colours[i].g = rand() % 256;
		//colours[i].b = rand() % 256;

  //      double t = static_cast<float>(i + 1) / static_cast<float>(cap_size);

  //      RGB rgb = HSBtoRGB(static_cast<unsigned short>(300.f * t), 75, 100);

  //      colours[i].r = rgb.r;
		//colours[i].g = rgb.g;
		//colours[i].b = rgb.b;


		RGB Blue(0, 0, 255);
		RGB Green(0, 255, 0);
		RGB Cyan(0, 255, 255);
		RGB Red(255, 0, 0);
		RGB Magenta(255, 0, 255);
		RGB Brown(165, 42, 42);
		RGB LightGray(211, 211, 211);
		RGB DarkGray(169, 169, 169);
		RGB LightBlue(173, 216, 230);
		RGB LightGreen(144, 238, 144);
		RGB LightCyan(224, 255, 255);
		RGB LightRed(240, 128, 128);
		RGB LightMagenta(255, 128, 255);
		RGB Yellow(255, 255, 0);
		RGB Black(0, 0, 0);
		RGB White(255, 255, 255);

		colours[0].r = Blue.r;
		colours[0].g = Blue.g;
		colours[0].b = Blue.b;

		colours[1].r = Green.r;
		colours[1].g = Green.g;
		colours[1].b = Green.b;

		colours[2].r = Cyan.r;
		colours[2].g = Cyan.g;
		colours[2].b = Cyan.b;

		colours[3].r = Red.r;
		colours[3].g = Red.g;
		colours[3].b = Red.b;

		colours[4].r = Magenta.r;
		colours[4].g = Magenta.g;
		colours[4].b = Magenta.b;

		colours[5].r = Brown.r;
		colours[5].g = Brown.g;
		colours[5].b = Brown.b;

		colours[6].r = LightGray.r;
		colours[6].g = LightGray.g;
		colours[6].b = LightGray.b;

		colours[7].r = DarkGray.r;
		colours[7].g = DarkGray.g;
		colours[7].b = DarkGray.b;

		colours[8].r = LightBlue.r;
		colours[8].g = LightBlue.g;
		colours[8].b = LightBlue.b;

		colours[9].r = LightGreen.r;
		colours[9].g = LightGreen.g;
		colours[9].b = LightGreen.b;

		colours[10].r = LightCyan.r;
		colours[10].g = LightCyan.g;
		colours[10].b = LightCyan.b;

		colours[11].r = LightRed.r;
		colours[11].g = LightRed.g;
		colours[11].b = LightRed.b;

		colours[12].r = LightMagenta.r;
		colours[12].g = LightMagenta.g;
		colours[12].b = LightMagenta.b;

		colours[13].r = Yellow.r;
		colours[13].g = Yellow.g;
		colours[13].b = Yellow.b;

		colours[14].r = White.r;
		colours[14].g = White.g;
		colours[14].b = Black.b;

		colours[15].r = Black.r;
		colours[15].g = Black.g;
		colours[15].b = Black.b;
	}

	// Max TGA size is 65535x65535 pixels
	const unsigned short int res = 2000;

	const size_t x_res = res;


	image_info main_image_info;
	vector<unsigned char> main_pixel_data;

	grid_parameters main_gp(-2., 1.25, -1.25, 1.25);
	main_image_info = save_mandelbrot_to_image(main_pixel_data, false, "main.tga", main_gp.x_grid_max, main_gp.x_grid_min, x_res, main_gp.y_grid_max, main_gp.y_grid_min, cap_size, colours);

	vector<unsigned char> region_pixel_data;

	grid_parameters gp = grid_parameters(-0.42, 0.20, 0.568, 1.045);
	save_mandelbrot_to_image(region_pixel_data, true, "roi1.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data, 
		main_image_info.width, main_image_info.height, 
		main_gp.x_grid_max, main_gp.x_grid_min, 
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(0.13, 0.42, 0.427, 0.65);
	save_mandelbrot_to_image(region_pixel_data, true, "roi2.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);


	gp = grid_parameters(0.28, 0.48, 0.26, 0.414);
	save_mandelbrot_to_image(region_pixel_data, true, "roi3.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(0.35, 0.45, 0.17, 0.247);
	save_mandelbrot_to_image(region_pixel_data, true, "roi4.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(0.27, 0.39, 0.02, 0.112);
	save_mandelbrot_to_image(region_pixel_data, true, "roi5.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);	write_region_of_interest_on_image(
	main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(-0.65, -0.34, 0.46, 0.698);
	save_mandelbrot_to_image(region_pixel_data, true, "roi6.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(-0.69, -0.55, 0.38, 0.488);
	save_mandelbrot_to_image(region_pixel_data, true, "roi7.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(-0.96, -0.47, -0.02, 0.357);
	save_mandelbrot_to_image(region_pixel_data, true, "roi8.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(-1.07, -0.89, 0.19, 0.328);
	save_mandelbrot_to_image(region_pixel_data, true, "roi9.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(-1.23, -1.03, 0.16, 0.314);
	save_mandelbrot_to_image(region_pixel_data, true, "roi10.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(-1.47, -1.21, -0.09, 0.11);
	save_mandelbrot_to_image(region_pixel_data, true, "roi11.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);

	gp = grid_parameters(-1.79, -1.73, -0.02, 0.026);
	save_mandelbrot_to_image(region_pixel_data, true, "roi12.tga", gp.x_grid_max, gp.x_grid_min, x_res, gp.y_grid_max, gp.y_grid_min, cap_size, colours);
	write_region_of_interest_on_image(
		main_pixel_data,
		main_image_info.width, main_image_info.height,
		main_gp.x_grid_max, main_gp.x_grid_min,
		main_gp.y_grid_max, main_gp.y_grid_min,
		gp.x_grid_max, gp.x_grid_min,
		gp.y_grid_max, gp.y_grid_min);



	write_RGB_tga("main.tga", main_image_info.width, main_image_info.height, main_pixel_data);

















	return 0;
}