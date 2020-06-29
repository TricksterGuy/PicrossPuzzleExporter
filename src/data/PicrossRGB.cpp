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

#include "PicrossRGB.hpp"


PicrossRGB::PicrossRGB(wxImage image, int width, int height, int bpc) : Picross(PicrossPuzzle::TYPE_RGB, width, height, bpc, 3)
{
    int colorbits = 256 / (1 << bpc);

    for (int y = 0; y < image.GetHeight(); y++)
    {
        for (int x = 0; x < image.GetWidth(); x++)
        {
            int value = 0;
            if (image.HasAlpha() && image.IsTransparent(x, y))
                value = 0;
            else
            {
                int r, g, b;
                r = image.GetRed(x, y) / colorbits;
                g = image.GetGreen(x, y) / colorbits;
                b = image.GetBlue(x, y) / colorbits;
                value = r | (g << bpc) | (b << bpc * 2);
            }

            data.Set(x, y, value);
        }
    }
}

void PicrossRGB::Toggle(int layer, int tx, int ty)
{
    unsigned int dmask = (1 << bpc) - 1;
    unsigned int value = data.Get(tx, ty);
    unsigned int numset = (NumSet(layer, tx, ty) + 1) & dmask;
    value = (value & ~(dmask << (bpc * layer))) | numset << (bpc * layer);
    data.Set(tx, ty, value);
    FlushCache(tx, ty);
}

void PicrossRGB::DrawBoard(wxDC& dc)
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

    int max = (1 << bpc) - 1;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (showLayer)
            {
                if (IsSet(layer, x, y))
                {
                    unsigned int c = 255 * NumSet(layer, x, y) / max;
                    dc.SetBrush(wxBrush(wxColour(c, c, c)));
                    dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
                }
            }
            else
            {
                unsigned int r = 255 * NumSet(0, x, y) / max;
                unsigned int g = 255 * NumSet(1, x, y) / max;
                unsigned int b = 255 * NumSet(2, x, y) / max;
                dc.SetBrush(wxBrush(wxColour(r, g, b)));
                dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
            }
        }
    }
}
