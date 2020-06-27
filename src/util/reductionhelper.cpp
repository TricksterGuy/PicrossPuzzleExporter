#include "reductionhelper.hpp"

#include <algorithm>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "cpercep.hpp"
#include "dither.hpp"

Image16Bpp::Image16Bpp(wxImage image, const std::string& _name) : width(image.GetWidth()), height(image.GetHeight()), name(_name), pixels(width * height)
{
    unsigned int num_pixels = width * height;
    const unsigned char* imageData = image.GetData();

    for (unsigned int i = 0; i < num_pixels; i++)
    {
        int x, y;
        x = i % width;
        y = i / width;

        int r, g, b;
        r = (imageData[3 * i] >> 3) & 0x1F;
        g = (imageData[3 * i + 1] >> 3) & 0x1F;
        b = (imageData[3 * i + 2] >> 3) & 0x1F;
        pixels[i] = r | g << 5 | b << 10;

        if (image.HasAlpha() && image.GetAlpha(x, y) == 0)
            pixels[i] = 0x8000;
    }
}

void Image16Bpp::GetColors(std::vector<Color>& colors) const
{
    for (unsigned int i = 0; i < width * height; i++)
    {
        short pix = pixels[i];
        if (pix != 0x8000)
            colors.push_back(Color(pix));
    }
}

Palette::Palette(const std::vector<Color>& _colors, const std::string& _name) : colors(_colors), name(_name)
{
};

void Palette::Set(const std::vector<Color>& _colors)
{
    colors = _colors;
}

int Palette::Search(const Color& a) const
{
    double bestd = DBL_MAX;
    int index = -1;

    if (colorIndexCache.find(a) != colorIndexCache.end())
        return colorIndexCache[a];


    for (unsigned int i = 0; i < colors.size(); i++)
    {
        double dist = 0;
        const Color& b = colors[i];
        dist = a.Distance(b);
        if (dist < bestd)
        {
            index = i;
            bestd = dist;
        }
    }

    colorIndexCache[a] = index;

    return index;
}

int Palette::Search(unsigned short color_data) const
{
    return Search(Color(color_data));
}

void Image8Bpp::Set(const Image16Bpp& image, std::shared_ptr<Palette> global_palette)
{
    width = image.width;
    height = image.height;
    name = image.name;
    pixels.resize(width * height);
    palette = global_palette;

    RiemersmaDither(image, *this, 0x8000, false, 0);
    /*for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = 0;
            unsigned short data = image.pixels[y * width + x];
            if (data != 0x8000)
                index = palette->Search(data);
            pixels[y * width + x] = index;
        }
    }*/
}
