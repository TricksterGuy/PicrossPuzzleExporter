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

#include "PicrossBW.hpp"

enum CellBW
{
    CLEAR = 0,
    BLACK = 1,
};

PicrossBW::PicrossBW(const wxImage& image) : Picross(PicrossPuzzle::TYPE_BW, image.GetWidth(), image.GetHeight(), 1, 1)
{
    int value = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image.HasAlpha() && image.IsTransparent(x, y))
            {
                value = CLEAR;
            }
            else
            {
                unsigned char r, g, b;
                r = image.GetRed(x, y);
                g = image.GetGreen(x, y);
                b = image.GetBlue(x, y);
                wxColour::MakeGrey(&r, &g, &b);
                value = r < 128 ? BLACK : CLEAR;
            }
            data.Set(x, y, value);
        }
    }
}

void PicrossBW::DrawBoard(wxDC& dc)
{
    wxRect rect;
    dc.GetClippingBox(rect);
    wxSize size = rect.GetSize();

    auto [unused1, extra_hints_height, unused2, extra_hints_width] = CalculateHintBounds();
    // hints_width/height already removed via GetClippingBox.
    int w = std::min((size.GetWidth() - extra_hints_width) / width,
                     (size.GetHeight() - extra_hints_height) / height);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (data.Get(x, y) == BLACK)
                dc.SetBrush(*wxBLACK_BRUSH);
            else
                dc.SetBrush(*wxWHITE_BRUSH);
            dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
        }
    }
}
