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

#include <unordered_map>
#include <tuple>
#include <vector>
#include <wx/dc.h>
#include "PicrossLayer.hpp"
#include "Picross.pb.h"

struct Problem;

typedef std::vector<std::vector<int>> Hints;
typedef std::vector<std::vector<int>> ShadingHints;
typedef std::unordered_map<int, Hints> LayerHints;

class Picross
{
    public:
        Picross(PicrossPuzzle::Type type_, const PicrossLayer& layer, int bpc_, int num_layers) : type(type_), data(layer), layer(0), showLayer(false), showGrid(true),
                                                                                                  width(layer.GetWidth()), height(layer.GetHeight()), bpc(bpc_),
                                                                                                  max_layers(num_layers), shading_rows(max_layers), shading_columns(max_layers), palette(1, *wxBLACK) {}
        virtual ~Picross() {}
        PicrossPuzzle::Type GetType() const {return type;}
        void Draw(wxDC& dc) const;
        virtual void DrawBoard(wxDC& dc) const {}
        virtual void Toggle(int layer, int tx, int ty) {data.Toggle(layer, tx, ty);}
        void Build();

        int GetWidth() const {return width;}
        int GetHeight() const {return height;}
        int GetBpc() const {return bpc;}
        int GetLayers() const {return max_layers;}
        virtual bool IsSet(int layer, int tx, int ty) const;
        virtual unsigned int NumSet(int layer, int tx, int ty) const;
        const PicrossLayer& GetData() const {return data;}
        const LayerHints& GetRowHints() const {return rows;}
        const LayerHints& GetRowColorHints() const {return color_rows;}
        const LayerHints& GetRowExtraHints() const {return total_rows;}
        const ShadingHints& GetRowShadingHints() const {return shading_rows;}
        const LayerHints& GetColumnHints() const {return columns;}
        const LayerHints& GetColumnColorHints() const {return color_columns;}
        const LayerHints& GetColumnExtraHints() const {return total_columns;}
        const ShadingHints& GetColumnShadingHints() const {return shading_columns;}
        const std::vector<wxColour>& GetPalette() const {return palette;}
        void SetLayer(int layer_id)
        {
            layer = layer_id;
            FlushCache();
        }
        void SetShowLayer(bool showLayer) {this->showLayer = showLayer;}
        void SetShowGrid(bool grid) {showGrid = grid;}

        void TranslateToCoords(int x, int y, int w, int h, int& tx, int& ty) const;

        bool Validate() const;
    protected:
        std::tuple<int, int, int, int> CalculateHintBounds() const;

        void FlushCache()
        {
            row_hints_cache.clear();
            column_hints_cache.clear();
            row_extra_hints_cache.clear();
            column_extra_hints_cache.clear();
            calculated_sizes = {0, 0, 0, 0};
        }
        void FlushCache(int row, int column)
        {
            if (row != -1)
            {
                row_hints_cache.erase(row);
                row_extra_hints_cache.erase(row);
            }
            if (column != -1)
            {
                column_hints_cache.erase(column);
                column_extra_hints_cache.erase(column);
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
        LayerHints rows;
        LayerHints columns;
        // Hint Colors for type = ColorHints
        LayerHints color_rows;
        LayerHints color_columns;
        // Numbers at the bottom and right for bpc puzzle puzzles per layer.
        // Map layer_id -> Array sized 2 ** bpc with totals for each bit combination.
        LayerHints total_rows;
        LayerHints total_columns;
        // Shading hints for bpc > 1
        // Map layer_id -> Array sized width/height with number of changes in shading.
        ShadingHints shading_rows;
        ShadingHints shading_columns;
        std::vector<wxColour> palette;

        friend bool Validate(const Picross* picross, Problem& problem);
    private:
        // Returns string form of both hints and extra hints.
        std::pair<wxString, wxString> GetRowHintsString(int row) const;
        std::pair<wxString, wxString> GetColumnHintsString(int column) const;

        std::pair<wxBitmap, wxBitmap> GetRowHintsBitmap(int row) const;
        std::pair<wxBitmap, wxBitmap> GetColumnHintsBitmap(int column) const;

        mutable std::unordered_map<int, wxBitmap> row_hints_cache;
        mutable std::unordered_map<int, wxBitmap> column_hints_cache;
        mutable std::unordered_map<int, wxBitmap> row_extra_hints_cache;
        mutable std::unordered_map<int, wxBitmap> column_extra_hints_cache;
        mutable std::tuple<int, int, int, int> calculated_sizes;
};

#endif
