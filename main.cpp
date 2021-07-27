#include "main.h"

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
        colours[2].b =  Cyan.b;

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
    const unsigned short int res = 100;

    const float x_grid_max = 1.25;
    const float x_grid_min = -2;
    const size_t x_res = res;
    const complex<float> x_step_size((x_grid_max - x_grid_min) / (x_res - 1), 0);

    const float y_grid_max = 1.25;
    const float y_grid_min = -1.25;
    const size_t y_res = static_cast<size_t>(static_cast<float>(x_res) / ((x_grid_max - x_grid_min) / (y_grid_max - y_grid_min)));
    const complex<float> y_step_size(0, (y_grid_max - y_grid_min) / (y_res - 1));

    const unsigned short int max_iterations = 5285;
    const float threshold = 2.0f;

    vector<unsigned char> pixel_data(3 * x_res * y_res, 0);

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


    write_RGB_tga("out.tga", x_res, y_res, pixel_data);

    return 0;
}