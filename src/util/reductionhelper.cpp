#include "reductionhelper.hpp"

#include <algorithm>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "mediancut.hpp"

Image16Bpp::Image16Bpp(wxImage image) : width(image.GetWidth()), height(image.GetHeight()), pixels(width * height)
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
        pixels[i] = Color16(r, g, b, 0);

        if (image.IsTransparent(x, y))
            pixels[i] = Color16(0, 0, 0, 1);
    }
}

Palette::Palette(const std::vector<Color16>& _colors) : colors(_colors), colorSet(colors.begin(), colors.end())
{
    labColors.reserve(colors.size());
    for (const auto& color : colors)
        labColors.push_back(ColorLAB(color));
}

void Palette::Clear()
{
    labColors.clear();
    colors.clear();
    colorSet.clear();
    paletteEntryStats.clear();
    colorEntryStats.clear();
}

void Palette::Set(const std::vector<Color16>& _colors)
{
    Clear();
    for (const auto& color : _colors)
        Add(color);
}

bool Palette::Set(unsigned int index, const Color16& color)
{
    if (index >= 256)
        return false;

    if (index >= colors.size())
    {
        colors.resize(index + 1);
        labColors.resize(index + 1);
        colorSet.insert(colors.begin(), colors.end());
    }

    Color16 old = colors[index];

    if (old == color)
        return true;

    if (colorSet.find(color) != colorSet.end())
        return false;

    colors[index] = color;
    colorSet.erase(old);
    colorSet.insert(color);
    labColors[index] = ColorLAB(color);

    return true;
}

int Palette::Search(const Color16& color) const
{
    unsigned long bestd = 0x7FFFFFFF;
    int index = -1;

    if (colorEntryStats.find(color) != colorEntryStats.end())
    {
        int index = colorEntryStats[color].index;
        colorEntryStats[color].count += 1;
        paletteEntryStats[index].used_count += 1;
        paletteEntryStats[index].error += colorEntryStats[color].error;
        if (colorEntryStats[color].error == 0)
            paletteEntryStats[index].perfect_count += 1;
        return index;
    }

    ColorLAB a(color);
    for (unsigned int i = 0; i < labColors.size(); i++)
    {
        const ColorLAB& b = labColors[i];
        unsigned long dist = a.Distance(b);
        if (dist <= bestd)
        {
            index = i;
            bestd = dist;

            if (bestd == 0)
            {
                paletteEntryStats[index].perfect_count += 1;
                break;
            }
        }
    }

    colorEntryStats[color].index = index;
    colorEntryStats[color].count += 1;
    colorEntryStats[color].error = bestd;

    paletteEntryStats[index].used_count += 1;
    paletteEntryStats[index].error += bestd;

    /*if (bestd != 0)
    {
        printf("Color remap: Color (%d %d %d) (%d %d %d) given to palette not an exact match. palette entry: %d - (%d %d %d) (%d %d %d).  dist: %ld.\n",
                   color.r, color.g, color.b, a.l, a.a, a.b, index, colors[index].r, colors[index].g, colors[index].b,
                   labColors[index].l, labColors[index].a, labColors[index].b, bestd);
    }*/

    return index;
}

void Palette::Add(const Color16& c)
{
    if (colorSet.find(c) == colorSet.end())
    {
        colorSet.insert(c);
        colors.push_back(c);
        labColors.push_back(ColorLAB(c));
    }
}

Image8Bpp::Image8Bpp(const Image16Bpp& image, int num_colors) : pixels(width * height)
{
    palette.reset(new Palette());
    GetPalette(image, num_colors, *palette);
    Set(image, palette);
}

void Image8Bpp::Set(const Image16Bpp& image, std::shared_ptr<Palette> global_palette)
{
    width = image.width;
    height = image.height;
    pixels.resize(width * height);
    palette = global_palette;

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            int index = 0;
            const Color16& color = image.pixels[y * width + x];
            pixels[y * width + x] = (color.a) ? 0 :  index = palette->Search(color);
        }
    }
}
