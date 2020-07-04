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

#include "PicrossGray.hpp"

PicrossGray::PicrossGray(const wxImage& image, int bpc) : Picross(PicrossPuzzle::TYPE_GRAY, image.GetWidth(), image.GetHeight(), bpc, 1)
{
    const int colorbits = 256 / (1 << bpc);
    int value = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image.HasAlpha() && image.IsTransparent(x, y))
            {
                value = 0;
            }
            else
            {
                unsigned char r, g, b;
                r = image.GetRed(x, y);
                g = image.GetGreen(x, y);
                b = image.GetBlue(x, y);
                wxColour::MakeGrey(&r, &g, &b);
                value = (255 - r) / colorbits;
            }
            data.Set(x, y, value);
        }
    }
}

void PicrossGray::Toggle(int layer, int tx, int ty)
{
    unsigned int dmask = (1 << bpc) - 1;
    unsigned int value = data.Get(tx, ty);
    data.Set(tx, ty, (value - 1) & dmask);
    FlushCache(tx, ty);
}

void PicrossGray::DrawBoard(wxDC& dc) const
{

    wxRect rect;
    dc.GetClippingBox(rect);
    wxSize size = rect.GetSize();

    auto [unused1, extra_hints_height, unused2, extra_hints_width] = CalculateHintBounds();
    // hints_width/height already removed via GetClippingBox.
    int w = std::min((size.GetWidth() - extra_hints_width) / width,
                     (size.GetHeight() - extra_hints_height) / height);

    int max = (1 << bpc) - 1;

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned int cval = 255 * (max - data.Get(x, y)) / max;
            dc.SetBrush(wxBrush(wxColour(cval, cval, cval)));
            dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
        }
    }
}
