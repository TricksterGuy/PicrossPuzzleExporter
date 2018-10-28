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

#ifndef PICROSS_HPP
#define PICROSS_HPP

#include <map>
#include <vector>
#include <wx/dc.h>
#include "PicrossLayer.hpp"
#include "Picross.pb.h"

struct Problem;

typedef std::vector<std::vector<int>> solutions;
class Picross
{
    public:
        Picross(int width_, int height_, int bpc_, int num_layers) : data(width_, height_), layer(0), showLayer(false),
                                                                     width(width_), height(height_), bpc(bpc_), max_layers(num_layers) {}
        virtual ~Picross() {}
        virtual Type GetType() const = 0;
        virtual void Draw(wxDC& dc);
        virtual void Toggle(int layer, int tx, int ty) {data.Toggle(layer, tx, ty);}
        void Build();

        int GetWidth() const {return width;}
        int GetHeight() const {return height;}
        virtual bool IsSet(int layer, int tx, int ty);
        virtual unsigned int NumSet(int layer, int tx, int ty);
        const PicrossLayer& GetData() const {return data;}
        void SetLayer(int layer) {this->layer = layer;}
        void SetShowLayer(bool showLayer) {this->showLayer = showLayer;}

        bool Validate() const;
        void Export(const wxString& file) const;
        PicrossPuzzle Export() const;
    protected:
        PicrossLayer data;
        int layer;
        bool showLayer;
        int width, height;
        int bpc;
        int max_layers;
        std::map<int, solutions> rows;
        std::map<int, solutions> cols;
        std::map<int, std::vector<int>> total_rows;
        std::map<int, std::vector<int>> total_cols;
        friend bool Validate(const Picross* picross, Problem& problem);
};

#endif
