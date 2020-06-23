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

PicrossBW::PicrossBW(wxImage image_in, int width, int height) : Picross(width, height, 1, 1)
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

void PicrossBW::Draw(wxDC& dc)
{
    Picross::Draw(dc);
    wxRect rect;
    dc.GetClippingBox(rect);
    wxSize size = rect.GetSize();
    int cw = (size.GetWidth() - EXTRA_SOLUTIONS_WIDTH) / width ;
    int ch = (size.GetHeight() - EXTRA_SOLUTIONS_HEIGHT) / height;

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);
    if (showGrid) dc.SetPen(*wxBLACK_PEN);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (data.Get(x, y) == BLACK)
                dc.SetBrush(*wxBLACK_BRUSH);
            else
                dc.SetBrush(*wxWHITE_BRUSH);
            dc.DrawRectangle(x * cw + rect.GetX(), y * ch + rect.GetY(), cw, ch);
        }
    }
}
