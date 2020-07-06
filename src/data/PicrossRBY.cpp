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

static inline bool bitsset(int val, int mask) {return (val & mask) == mask;}

static wxColor GetRBYColor(int value)
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

void PicrossRBY::DrawBoard(wxDC& dc) const
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
            if (!value) continue;
            if (!showLayer)
            {
                dc.SetBrush(wxBrush(GetRBYColor(value)));
                dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
            }
            else if (IsSet(layer, x, y))
                dc.DrawRectangle(x * w + rect.GetX(), y * w + rect.GetY(), w, w);
        }
    }
}
