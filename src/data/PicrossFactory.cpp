#include "PicrossFactory.hpp"
#include "PicrossBW.hpp"
#include "PicrossGray.hpp"
#include "PicrossRGB.hpp"
#include "PicrossRBY.hpp"

#include "cpercep.hpp"

std::shared_ptr<Palette> PicrossFactory::palette;
std::unordered_map<int, unsigned int> PicrossFactory::special_map;

Picross* PicrossFactory::Create(const wxImage& image, int type, int bpc)
{
    Picross* picross = nullptr;
    switch(type)
    {
        case 0:
            picross = CreateBW(image);
            break;
        case 1:
            picross = CreateGray(image, bpc);
            break;
        case 2:
            picross = CreateRGB(image, bpc);
            break;
        case 3:
            picross = CreateRBY(image);
            break;
        default:
            break;
    }
    if (picross)
        picross->Build();
    return picross;
}

Picross* PicrossFactory::CreateBW(const wxImage& image)
{
    int width = image.GetWidth();
    int height = image.GetHeight();
    PicrossLayer data(width, height);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image.IsTransparent(x, y))
                continue;

            unsigned char r, g, b;
            r = image.GetRed(x, y);
            g = image.GetGreen(x, y);
            b = image.GetBlue(x, y);
            wxColour::MakeGrey(&r, &g, &b);
            data.Set(x, y, r < 128 ? 1 : 0);
        }
    }
    return new PicrossBW(std::move(data));
}

Picross* PicrossFactory::CreateGray(const wxImage& image, int bpc)
{
    int width = image.GetWidth();
    int height = image.GetHeight();
    PicrossLayer data(width, height);

    const int colorbits = 256 / (1 << bpc);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image.IsTransparent(x, y))
                continue;
            unsigned char r, g, b;
            r = image.GetRed(x, y);
            g = image.GetGreen(x, y);
            b = image.GetBlue(x, y);
            wxColour::MakeGrey(&r, &g, &b);
            data.Set(x, y, (255 - r) / colorbits);
        }
    }
    return new PicrossGray(std::move(data), bpc);
}

Picross* PicrossFactory::CreateRGB(const wxImage& image, int bpc)
{
    int width = image.GetWidth();
    int height = image.GetHeight();
    PicrossLayer data(width, height);

    const int colorbits = 256 / (1 << bpc);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image.IsTransparent(x, y))
                continue;
            int r, g, b;
            r = image.GetRed(x, y) / colorbits;
            g = image.GetGreen(x, y) / colorbits;
            b = image.GetBlue(x, y) / colorbits;
            data.Set(x, y, r | (g << bpc) | (b << bpc * 2));
        }
    }

    return new PicrossRGB(std::move(data), bpc);
}

Picross* PicrossFactory::CreateRBY(const wxImage& image)
{
    if (!palette)
        InitPalette();

    int width = image.GetWidth();
    int height = image.GetHeight();
    PicrossLayer data(width, height);

    // TODO converting the image to 16 bit color isn't necessary.
    Image8Bpp image8(Image16Bpp(image, ""), palette);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image.IsTransparent(x, y))
                continue;

            int value = CLEAR;
            int pindex = image8.pixels[y * image8.width + x];
            if (pindex == 0)
                value = BLACK;
            else if (pindex == 1)
                value = WHITE;
            else if (special_map.find(pindex) != special_map.end())
                value = special_map[pindex];
            else
            {
                pindex -= 2;
                if (pindex / 7 == 0)
                    value |= BLACK;
                if (pindex / 7 == 2)
                    value |= WHITE;
                pindex = pindex % 7 + 1;
                switch(pindex)
                {
                    case 1:
                        value |= RED;
                        break;
                    case 2:
                        value |= BLUE;
                        break;
                    case 3:
                        value |= YELLOW;
                        break;
                    case 4:
                        value |= ORANGE;
                        break;
                    case 5:
                        value |= PURPLE;
                        break;
                    case 6:
                        value |= GREEN;
                        break;
                    case 7:
                        value |= GRAY;
                        break;
                    default:
                        value = CLEAR;
                }
            }
            data.Set(x, y, value);
        }
    }

    return new PicrossRBY(std::move(data));
}

void PicrossFactory::InitPalette()
{
    cpercep_init();
    palette.reset(new Palette());
    const int num_colors = 32;

    const unsigned short color_set[num_colors] = {
        0x0000,0x7fff,

        0x0008,0x2000,0x0108,0x0088,0x1004,0x0080,0x1084,
        0x001f,0x7c00,0x03ff,0x021f,0x4010,0x0200,0x4210,
        0x421f,0x7e10,0x63ff,0x431f,0x6218,0x4310,0x6318,
        // special colormap
        //green, magenta, cyanx2,           pink,   blue,  dblue,  lgray, dyellow
         0x03e0,  0x7c1f, 0x7fe0, 0x7e82, 0x529f, 0x4000, 0x1000, 0x739c, 0x0672, //0x7e1c
    };

    special_map[23] = GREEN | WHITE;
    special_map[24] = PURPLE | WHITE;
    special_map[25] = BLUE | WHITE;
    special_map[26] = BLUE | WHITE;
    special_map[27] = RED | WHITE;
    special_map[28] = BLUE;
    special_map[29] = BLACK | BLUE;
    special_map[30] = WHITE | GRAY;
    special_map[31] = BLACK | YELLOW;
    //special_map[32] = WHITE | GRAY;

    std::vector<Color> colors;
    for (int i = 0; i < num_colors; i++)
        colors.push_back(color_set[i]);
    palette->Set(colors);
}