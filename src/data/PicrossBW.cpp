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

PicrossBW::PicrossBW(wxImage image_in, int width, int height) : Picross(PicrossPuzzle::TYPE_BW, width, height, 1, 1)
{
    wxImage image = image_in.ConvertToGreyscale();
    for (int y = 0; y < image.GetHeight(); y++)
    {
        for (int x = 0; x < image.GetWidth(); x++)
        {
            int value;
            if (image.HasAlpha() && image.IsTransparent(x, y))
                value = CLEAR;
            else
                value = image.GetRed(x, y) < 128 ? BLACK : CLEAR;
            data.Set(x, y, value);
        }
    }
}

void PicrossBW::DrawBoard(wxDC& dc)
{
    wxRect rect;
    dc.GetClippingBox(rect);
    wxSize size = rect.GetSize();

    auto [unused1, extra_solutions_height, unused2, extra_solutions_width] = CalculateSolutionBounds();
    int w = std::min((size.GetWidth() - extra_solutions_width) / width,
                     (size.GetHeight() - extra_solutions_height) / height);

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
