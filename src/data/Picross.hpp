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

#ifndef PICROSS_HPP
#define PICROSS_HPP

#include <map>
#include <vector>
#include <wx/dc.h>
#include "PicrossLayer.hpp"
#include "Picross.pb.h"

struct Problem;

typedef std::vector<std::vector<int>> solutions;

struct ExportParams
{
    std::string name;
    std::string author;
    std::string image;
    int frames;
    int time;
    int background_type;
    std::string bg_image;
    uint32_t top_color;
    uint32_t bottom_color;
};

constexpr int SOLUTIONS_WIDTH = 100;
constexpr int SOLUTIONS_HEIGHT = 100;
constexpr int EXTRA_SOLUTIONS_WIDTH = 50;
constexpr int EXTRA_SOLUTIONS_HEIGHT = 50;

class Picross
{
    public:
        Picross(int width_, int height_, int bpc_, int num_layers) : data(width_, height_), layer(0), showLayer(false), showGrid(true),
                                                                     width(width_), height(height_), bpc(bpc_), max_layers(num_layers) {}
        virtual ~Picross() {}
        virtual PicrossPuzzle::Type GetType() const = 0;
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
        void SetShowGrid(bool grid) {showGrid = grid;}

        void TranslateToCoords(int x, int y, int w, int h, int& tx, int& ty) const;

        bool Validate() const;
        void Export(const wxString& file, const ExportParams& params) const;
        PicrossPuzzle Export(const ExportParams& params) const;
    protected:
        PicrossLayer data;
        int layer;
        bool showLayer;
        bool showGrid;
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
