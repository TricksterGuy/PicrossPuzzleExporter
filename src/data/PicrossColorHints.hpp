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

#ifndef PICROSS_COLOR_HINTS_HPP
#define PICROSS_COLOR_HINTS_HPP

#include "Picross.hpp"
#include <vector>
#include <wx/bitmap.h>

class PicrossColorHints : public Picross
{
public:
    PicrossColorHints(const PicrossLayer& layer, const std::vector<wxColour>& colors) : Picross(PicrossPuzzle::TYPE_COLORED_HINT, layer, /*unused_bpc=*/1, /*layers=*/1) {palette = colors;}
    ~PicrossColorHints() {}
    void DrawBoard(wxDC& dc) const override;
    void Toggle(int layer, int tx, int ty) override {}
    unsigned int NumSet(int layer, int tx, int ty) const override {return data.Get(tx, ty);}
};

#endif