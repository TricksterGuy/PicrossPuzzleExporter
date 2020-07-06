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


#ifndef PICROSSRGY_HPP
#define PICROSSRGY_HPP

#include <wx/bitmap.h>
#include "Picross.hpp"
#include "reductionhelper.hpp"

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

class PicrossRBY : public Picross
{
    public:
        PicrossRBY(const PicrossLayer& layer) :  Picross(PicrossPuzzle::TYPE_RBY, layer, /*bpc=*/1, /*layers=*/5) {}
        ~PicrossRBY() {}
        void DrawBoard(wxDC& dc) const override;
};

#endif
