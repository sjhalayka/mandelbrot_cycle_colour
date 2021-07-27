#include <cmath>
#include <complex>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
using namespace std;


class grid_parameters
{
public:

    grid_parameters(float src_x_grid_min, float src_x_grid_max, float src_y_grid_min, float src_y_grid_max)
    {
        x_grid_min = src_x_grid_min;
        x_grid_max = src_x_grid_max;
        y_grid_min = src_y_grid_min;
        y_grid_max = src_y_grid_max;
    }

    float x_grid_max;
    float x_grid_min;
    float y_grid_max;
    float y_grid_min;
};


inline float iterate_mandelbrot(
    vector<complex<float>>& trajectory,
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


void get_mandelbrot_pixels(
    vector<unsigned char>& pixel_data,
    const size_t cap_size,
    const vector<RGB>& colours,
    const short unsigned int max_iterations,
    const float threshold,
    float x_grid_min,
    float y_grid_min,
    complex<float> x_step_size,
    complex<float> y_step_size,
    size_t x_res,
    size_t y_res)
{

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

}




