#include <cmath>
#include <complex>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
using namespace std;


inline float iterate_mandelbrot(
    vector<complex<float>> &trajectory,
    const complex<float> C,
    const short unsigned int max_iterations,
    const float threshold)
{
    complex<float> Z(0, 0);

    trajectory.push_back(Z);

    for (short unsigned int i = 0; i < max_iterations; i++)
    {
        Z = Z * Z + C;

        trajectory.push_back(Z);

        if (abs(Z) >= threshold)
            break;
    }

    return abs(Z);
}

class RGB
{
public:

    RGB(void)
    {
        r = 0;
        g = 0;
        b = 0;
    }

    RGB(unsigned int src_r, unsigned int src_g, unsigned int src_b)
    {
        r = src_r;
        g = src_g;
        b = src_b;

    }

    unsigned char r, g, b;
};

RGB HSBtoRGB(unsigned short int hue_degree, unsigned char sat_percent, unsigned char bri_percent)
{
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;

    if (hue_degree > 359)
        hue_degree = 359;

    if (sat_percent > 100)
        sat_percent = 100;

    if (bri_percent > 100)
        bri_percent = 100;

    float hue_pos = 6.0f - ((static_cast<float>(hue_degree) / 359.0f) * 6.0f);

    if (hue_pos >= 0.0f && hue_pos < 1.0f)
    {
        R = 255.0f;
        G = 0.0f;
        B = 255.0f * hue_pos;
    }
    else if (hue_pos >= 1.0f && hue_pos < 2.0f)
    {
        hue_pos -= 1.0f;

        R = 255.0f - (255.0f * hue_pos);
        G = 0.0f;
        B = 255.0f;
    }
    else if (hue_pos >= 2.0f && hue_pos < 3.0f)
    {
        hue_pos -= 2.0f;

        R = 0.0f;
        G = 255.0f * hue_pos;
        B = 255.0f;
    }
    else if (hue_pos >= 3.0f && hue_pos < 4.0f)
    {
        hue_pos -= 3.0f;

        R = 0.0f;
        G = 255.0f;
        B = 255.0f - (255.0f * hue_pos);
    }
    else if (hue_pos >= 4.0f && hue_pos < 5.0f)
    {
        hue_pos -= 4.0f;

        R = 255.0f * hue_pos;
        G = 255.0f;
        B = 0.0f;
    }
    else
    {
        hue_pos -= 5.0f;

        R = 255.0f;
        G = 255.0f - (255.0f * hue_pos);
        B = 0.0f;
    }

    if (100 != sat_percent)
    {
        if (0 == sat_percent)
        {
            R = 255.0f;
            G = 255.0f;
            B = 255.0f;
        }
        else
        {
            if (255.0f != R)
                R += ((255.0f - R) / 100.0f) * (100.0f - sat_percent);
            if (255.0f != G)
                G += ((255.0f - G) / 100.0f) * (100.0f - sat_percent);
            if (255.0f != B)
                B += ((255.0f - B) / 100.0f) * (100.0f - sat_percent);
        }
    }

    if (100 != bri_percent)
    {
        if (0 == bri_percent)
        {
            R = 0.0f;
            G = 0.0f;
            B = 0.0f;
        }
        else
        {
            if (0.0f != R)
                R *= static_cast<float>(bri_percent) / 100.0f;
            if (0.0f != G)
                G *= static_cast<float>(bri_percent) / 100.0f;
            if (0.0f != B)
                B *= static_cast<float>(bri_percent) / 100.0f;
        }
    }

    if (R < 0.0f)
        R = 0.0f;
    else if (R > 255.0f)
        R = 255.0f;

    if (G < 0.0f)
        G = 0.0f;
    else if (G > 255.0f)
        G = 255.0f;

    if (B < 0.0f)
        B = 0.0f;
    else if (B > 255.0f)
        B = 255.0f;

    RGB rgb(0, 0, 0);

    rgb.r = static_cast<unsigned char>(R);
    rgb.g = static_cast<unsigned char>(G);
    rgb.b = static_cast<unsigned char>(B);

    return rgb;
}


void write_RGB_tga(const char* const filename, const size_t width, const size_t height, vector<unsigned char> output_pixels)
{
    if (output_pixels.size() == 0)
        return;

    // Set up Targa TGA image data.
    unsigned char  idlength = 0;
    unsigned char  colourmaptype = 0;
    unsigned char  datatypecode = 2;
    unsigned short int colourmaporigin = 0;
    unsigned short int colourmaplength = 0;
    unsigned char  colourmapdepth = 0;
    unsigned short int x_origin = 0;
    unsigned short int y_origin = 0;

    unsigned short int px = static_cast<unsigned short int>(width);
    unsigned short int py = static_cast<unsigned short int>(height);
    unsigned char  bitsperpixel = 24;
    unsigned char  imagedescriptor = 0;
    vector<char> idstring;

    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            size_t index = 3 * (j * width + i);

            unsigned char temp_char;
            temp_char = output_pixels[index + 0];
            output_pixels[index + 0] = output_pixels[index + 2];
            output_pixels[index + 2] = temp_char;
        }
    }

    // Write Targa TGA file to disk.
    ofstream out(filename, ios::binary);

    if (!out.is_open())
    {
        cout << "Failed to open TGA file for writing: " << filename << endl;
        return;
    }

    out.write(reinterpret_cast<char*>(&idlength), 1);
    out.write(reinterpret_cast<char*>(&colourmaptype), 1);
    out.write(reinterpret_cast<char*>(&datatypecode), 1);
    out.write(reinterpret_cast<char*>(&colourmaporigin), 2);
    out.write(reinterpret_cast<char*>(&colourmaplength), 2);
    out.write(reinterpret_cast<char*>(&colourmapdepth), 1);
    out.write(reinterpret_cast<char*>(&x_origin), 2);
    out.write(reinterpret_cast<char*>(&y_origin), 2);
    out.write(reinterpret_cast<char*>(&px), 2);
    out.write(reinterpret_cast<char*>(&py), 2);
    out.write(reinterpret_cast<char*>(&bitsperpixel), 1);
    out.write(reinterpret_cast<char*>(&imagedescriptor), 1);

    out.write(reinterpret_cast<char*>(&output_pixels[0]), width * height * 3 * sizeof(unsigned char));

    out.close();
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
    const unsigned short int res = 1000;

    const float x_grid_max = 2;
    const float x_grid_min = -2;
    const size_t x_res = res;
    const complex<float> x_step_size((x_grid_max - x_grid_min) / (x_res - 1), 0);

    const float y_grid_max = 2;
    const float y_grid_min = -2;
    const size_t y_res = res;
    const complex<float> y_step_size(0, (y_grid_max - y_grid_min) / (y_res - 1));

    const unsigned short int max_iterations = 5285;
    const float threshold = 2.0f;

    vector<unsigned char> pixel_data(3 * x_res * y_res, 0);

    // Calculate Mandelbrot set
    complex<float> pos(x_grid_min, y_grid_min);

    for (size_t x = 0; x < x_res; x++, pos += x_step_size)
    {
        cout << x << " of " << x_res << endl;

        pos = complex<float>(pos.real(), y_grid_min);

        for (size_t y = 0; y < y_res; y++, pos += y_step_size)
        {
            vector<complex<float>> trajectory;

            float magnitude = iterate_mandelbrot(trajectory, pos, max_iterations, threshold);

            if (magnitude >= threshold)
            {
                pixel_data[3 * (y * x_res + x) + 0] = 255;
				pixel_data[3 * (y * x_res + x) + 1] = 255;
				pixel_data[3 * (y * x_res + x) + 2] = 255;
			}
			else
			{
				trajectory.erase(trajectory.begin(), trajectory.begin() + (trajectory.size() / 10));

                vector<complex<float>> new_points;

                for (size_t i = 0; i < trajectory.size(); i++)
                    if (std::find(new_points.begin(), new_points.end(), trajectory[i]) == new_points.end())
                        new_points.push_back(trajectory[i]);

                trajectory = new_points;

                if (trajectory.size() > cap_size)
                    trajectory.resize(cap_size);

                pixel_data[3 * (y * x_res + x) + 0] = colours[trajectory.size() - 1].r;
                pixel_data[3 * (y * x_res + x) + 1] = colours[trajectory.size() - 1].g;
                pixel_data[3 * (y * x_res + x) + 2] = colours[trajectory.size() - 1].b;
			}
        }
    }

    write_RGB_tga("out.tga", x_res, y_res, pixel_data);

    return 0;
}