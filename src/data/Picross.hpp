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
#include <tuple>
#include <vector>
#include <wx/dc.h>
#include "PicrossLayer.hpp"
#include "Picross.pb.h"

struct Problem;

typedef std::vector<std::vector<int>> solutions;
typedef std::map<int, solutions> layer_solutions;

struct ExportParams
{
    std::string name;
    std::string author;
    std::string image;
    int frames;
    int time;
    int background_type;
    std::string bg_image;
    std::string bg_music;
    uint32_t top_color;
    uint32_t bottom_color;
};

class Picross
{
    public:
        Picross(PicrossPuzzle::Type type_, int width_, int height_, int bpc_, int num_layers) : type(type_), data(width_, height_), layer(0), showLayer(false), showGrid(true),
                                                                                                width(width_), height(height_), bpc(bpc_), max_layers(num_layers), shading_rows(max_layers),
                                                                                                shading_cols(max_layers) {}
        virtual ~Picross() {}
        PicrossPuzzle::Type GetType() const {return type;}
        virtual void Draw(wxDC& dc);
        virtual void Toggle(int layer, int tx, int ty)
        {
            data.Toggle(layer, tx, ty);
            FlushCache(tx, ty);
        }
        void Build();

        int GetWidth() const {return width;}
        int GetHeight() const {return height;}
        virtual bool IsSet(int layer, int tx, int ty);
        virtual unsigned int NumSet(int layer, int tx, int ty);
        const PicrossLayer& GetData() const {return data;}
        void SetLayer(int layer_id)
        {
            layer = layer_id;
            FlushCache();
        }
        void SetShowLayer(bool showLayer) {this->showLayer = showLayer;}
        void SetShowGrid(bool grid) {showGrid = grid;}

        void TranslateToCoords(int x, int y, int w, int h, int& tx, int& ty) const;

        bool Validate() const;
        void Export(const wxString& file, const ExportParams& params) const;
        PicrossPuzzle Export(const ExportParams& params) const;
    protected:
        std::tuple<int, int, int, int> CalculateSolutionBounds() const;

        void FlushCache()
        {
            row_solutions_cache.clear();
            col_solutions_cache.clear();
            row_extra_solutions_cache.clear();
            col_extra_solutions_cache.clear();
            calculated_sizes = {0, 0, 0, 0};
        }
        void FlushCache(int row, int col)
        {
            if (row != -1)
            {
                row_solutions_cache.erase(row);
                row_extra_solutions_cache.erase(row);
            }
            if (col != -1)
            {
                col_solutions_cache.erase(col);
                col_extra_solutions_cache.erase(col);
            }
            calculated_sizes = {0, 0, 0, 0};
        }

        PicrossPuzzle::Type type;
        PicrossLayer data;
        int layer;
        bool showLayer;
        bool showGrid;
        int width, height;
        int bpc;
        int max_layers;
        // Numbers at the top and left of picross puzzle per layer.
        // Map layer_id -> Array of hints.
        layer_solutions rows;
        layer_solutions cols;
        // Numbers at the bottom and right for bpc puzzle puzzles per layer.
        // Map layer_id -> Array sized 2 ** bpc with totals for each bit combination.
        layer_solutions total_rows;
        layer_solutions total_cols;
        // Shading hints for bpc > 1
        // Map layer_id -> Array sized width/height with number of changes in shading.
        solutions shading_rows;
        solutions shading_cols;

        friend bool Validate(const Picross* picross, Problem& problem);
    private:
        std::pair<wxString, wxString> getRowHints(int row) const;
        std::pair<wxString, wxString> getColHints(int col) const;

        mutable std::unordered_map<int, wxString> row_solutions_cache;
        mutable std::unordered_map<int, wxString> col_solutions_cache;
        mutable std::unordered_map<int, wxString> row_extra_solutions_cache;
        mutable std::unordered_map<int, wxString> col_extra_solutions_cache;
        mutable std::tuple<int, int, int, int> calculated_sizes;
};

#endif
