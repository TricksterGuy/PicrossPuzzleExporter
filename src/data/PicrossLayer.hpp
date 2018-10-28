/******************************************************************************************************
 * Picross
 * Copyright (C) 2009-2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

#ifndef PICROSS_LAYER_HPP
#define PICROSS_LAYER_HPP

#include <vector>

class PicrossLayer
{
    public:
        PicrossLayer(int width_, int height_) : width(width_), height(height_), data(width_ * height_) {}
        ~PicrossLayer() {}
        const std::vector<unsigned short>& GetData() const {return data;}
        void Set(int x, int y, unsigned short cell_data) {data[y * width + x] = cell_data;}
        unsigned short Get(int x, int y) const {return data[y * width + x];}
        void Toggle(int layer, int tx, int ty) {data[ty * width + tx] ^= 1 << layer;}
        int GetWidth() const {return width;}
        int GetHeight() const {return height;}
    private:
        int width, height;
        std::vector<unsigned short> data;
};

#endif
