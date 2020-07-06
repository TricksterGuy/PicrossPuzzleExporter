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

#include "ProtoExporter.hpp"
#include <fstream>
#include <iostream>

bool ExportProto(const std::string& path, const Picross& picross, const ExportParams& params)
{
    std::ofstream file(path.c_str(), std::ios::binary);
    if (!file.good()) return false;

    PicrossPuzzle out;

    int width = picross.GetWidth();
    int height = picross.GetHeight();
    int bpc = picross.GetBpc();

    out.set_width(width);
    out.set_height(height);
    out.set_type(picross.GetType());
    out.set_bpc(bpc);

    // Write meta information from export params.
    auto* meta = out.mutable_meta();
    meta->set_name(params.name);
    meta->set_author(params.author);
    meta->set_time(params.time);
    auto* background = meta->mutable_background();
    background->set_type(static_cast<BackgroundInfo::Type>(params.background_type + 1));
    if (params.background_type != 3)
        background->set_image(params.bg_image);
    else
    {
        background->set_color1(params.top_color);
        background->set_color2(params.bottom_color);
    }
    background->set_music(params.bg_music);
    auto* solution_meta = meta->mutable_solution();
    solution_meta->set_image(params.image);
    solution_meta->set_frames(params.frames);


    const auto& rows = picross.GetRowHints();
    const auto& columns = picross.GetColumnHints();
    const auto& row_extra_hints = picross.GetRowExtraHints();
    const auto& column_extra_hints = picross.GetColumnExtraHints();
    const auto& shading_rows = picross.GetRowShadingHints();
    const auto& shading_columns = picross.GetColumnShadingHints();
    const auto& picross_layer = picross.GetData();

    for (int k = 0; k < picross.GetLayers(); k++)
    {
        auto* layer = out.add_layers();
        const auto& row_hints = rows.at(k);
        const auto& column_hints = columns.at(k);
        for (int i = 0; i < height; i++)
        {
            const auto& row_data = row_hints[i];
            auto* rows = layer->add_rows();
            for (const auto& elem : row_data)
                rows->add_data(elem);
            if (bpc > 1)
            {
                const auto& total_hints = row_extra_hints.at(k)[i];
                for (const auto& elem : total_hints)
                    rows->add_totals(elem);
                rows->set_shading(shading_rows.at(k)[i]);
            }
        }

        for (int j = 0; j < width; j++)
        {
            const auto& column_data = column_hints[j];
            auto* columns = layer->add_columns();
            for (const auto& elem : column_data)
                columns->add_data(elem);
            if (bpc > 1)
            {
                const auto& total_hints = column_extra_hints.at(k)[j];
                for (const auto& elem : total_hints)
                    columns->add_totals(elem);
                columns->set_shading(shading_columns.at(k)[j]);
            }
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
            out.add_solution(picross_layer.Get(x, y));
    }

    out.SerializeToOstream(&file);
    return true;
}