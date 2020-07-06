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

#ifndef PICROSS_FACTORY_HPP
#define PICROSS_FACTORY_HPP

#include <unordered_map>
#include <wx/bitmap.h>
#include "Picross.hpp"
#include "reductionhelper.hpp"

class PicrossFactory
{
public:
    static Picross* Create(const wxImage& image, int type, int bpc);

private:
    PicrossFactory() = delete;
    ~PicrossFactory() = delete;

    static Picross* CreateBW(const wxImage& image);
    static Picross* CreateGray(const wxImage& image, int bpc);
    static Picross* CreateRGB(const wxImage& image, int bpc);
    static Picross* CreateRBY(const wxImage& image);

    static void InitPalette();

    static std::shared_ptr<Palette> palette;
    static std::unordered_map<int, unsigned int> special_map;
};


#endif
