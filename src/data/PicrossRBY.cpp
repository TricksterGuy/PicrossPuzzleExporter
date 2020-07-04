/******************************************************************************************************
 * Picross
 * Copyright (C) 2009-2020 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * excluding commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    An acknowledgement in your documentation and link to the original version is required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ******************************************************************************************************/

#include "PicrossRBY.hpp"
#include "reductionhelper.hpp"
#include "color.hpp"

static inline int min(int x, int y) {return x < y ? x : y;}
static inline int min(int x, int y, int z) {return min(x, y) < z ? min(x, y) : z;}
static inline int max(int x, int y) {return x > y ? x : y;}
static inline int max(int x, int y, int z) {return max(x, y) > z ? max(x, y) : z;}
static inline bool between(int val, int min, int max) {return val >= min && val <= max;}
static inline bool bitsset(int val, int mask) {return (val & mask) == mask;}
static inline bool withintol(int val1, int val2, int tol) {return abs(val1 - val2) < tol;}
static inline bool outsidetol(int val1, int val2, int tol) {return abs(val1 - val2) > tol;}

#define TOL 32

enum CellRBY
{
    CLEAR = 0,
    BLACK = 1,
    WHITE = 2,

    RED = 4,
    BLUE = 8,
    YELLOW = 16,

    PURPLE = 12,
    ORANGE = 20,
    GREEN = 24,

    GRAY = 28,
};

std::shared_ptr<Palette> palette;
std::map<int, unsigned int> special_map;

void init_palette()
{
    palette.reset(new Palette());
    const int num_colors = 32;

    const unsigned short color_set[num_colors] = {
        0x0000,0x7fff,

        0x0008,0x2000,0x0108,0x0088,0x1004,0x0080,0x1084,
        0x001f,0x7c00,0x03ff,0x021f,0x4010,0x0200,0x4210,
        0x421f,0x7e10,0x63ff,0x431f,0x6218,0x4310,0x6318,

        // special green, magenta, cyanx2, pink, blue, dblue, lgray, dyellow
        0x03e0, 0x7c1f, 0x7fe0, 0x7e82, 0x529f, 0x4000, 0x1000,
        0x739c, 0x0672, //0x7e1c
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

class ColorMatch
{
    public:
        static wxColor GetColor(int value)
        {
            wxColor color;
            if (bitsset(value, GRAY))
                color.Set(128, 128, 128);
            else if (bitsset(value, PURPLE))
                color.Set(128, 0, 128);
            else if (bitsset(value, ORANGE))
                color.Set(255, 128, 0);
            else if (bitsset(value, GREEN))
                color.Set(0, 128, 0);
            else if (bitsset(value, RED))
                color.Set(255, 0, 0);
            else if (bitsset(value, BLUE))
                color.Set(0, 0, 255);
            else if (bitsset(value, YELLOW))
                color.Set(255, 255, 0);
            else if (value == WHITE)
                return wxColor(255, 255, 255);
            else if (value == BLACK)
                return wxColor(0, 0, 0);

            if (bitsset(value, WHITE))
                color.Set(color.Red() / 2 + 128, color.Green() / 2 + 128, color.Blue() / 2 + 128);
            if (bitsset(value, BLACK))
                color.Set(color.Red() / 2, color.Green() / 2, color.Blue() / 2);

            return color;
        }
};

PicrossRBY::PicrossRBY(const wxImage& image) : Picross(PicrossPuzzle::TYPE_RBY, image.GetWidth(), image.GetHeight(), 1, 5)
{
    // TODO converting the image to 16 bit color isn't necessary.
    Image16Bpp image16(image, "");
    image8.Set(image16, palette);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int value = 0;
            if (image.HasAlpha() && image.IsTransparent(x, y))
                value = CLEAR;
            else
            {
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
            }

            data.Set(x, y, value);
        }
    }
}

void PicrossRBY::DrawBoard(wxDC& dc)
{
    wxRect rect;
    dc.GetClippingBox(rect);
    wxSize size = rect.GetSize();

    auto [unused1, extra_hints_height, unused2, extra_hints_width] = CalculateHintBounds();
    // hints_width/height already removed via GetClippingBox.
    int w = std::min((size.GetWidth() - extra_hints_width) / width,
                     (size.GetHeight() - extra_hints_height) / height);

    dc.SetPen(*wxTRANSPARENT_PEN);
    if (showLayer) dc.SetBrush(*wxBLACK_BRUSH);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned int value = data.Get(x, y);
            if (value == (unsigned short) CLEAR) continue;
            if (!showLayer)
            {
                dc.SetBrush(wxBrush(ColorMatch::GetColor(value)));
                dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
            }
            else if (IsSet(layer, x, y))
                dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
        }
    }
}
