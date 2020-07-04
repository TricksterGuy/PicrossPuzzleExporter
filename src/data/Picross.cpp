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

#include "Picross.hpp"
#include <wx/settings.h>
#include <wx/dcmemory.h>
#include <fstream>
#include <iostream>

void Picross::Build()
{
    // TODO write Picross::Clear which erases all hints from puzzle.
    for (int k = 0; k < max_layers; k++)
    {
        shading_rows[k].clear();
        shading_columns[k].clear();

        // Build row hints info.
        rows[k] = Hints(height);
        for (int y = 0; y < height; y++)
        {
            std::vector<int>& row = rows[k][y];
            row.reserve(width / 2);
            int marks = 0;
            for (int x = 0; x < width; x++)
            {
                if (IsSet(k, x, y))
                {
                    marks++;
                }
                else if (marks)
                {
                    row.push_back(marks);
                    marks = 0;
                }
            }
            if (row.empty() || marks > 0)
            {
                row.push_back(marks);
            }
        }

        // Build column hints info.
        columns[k] = Hints(width);
        for (int x = 0; x < width; x++)
        {
            std::vector<int>& column = columns[k][x];
            column.reserve(height / 2);
            int marks = 0;
            for (int y = 0; y < height; y++)
            {
                if (IsSet(k, x, y))
                {
                    marks++;
                }
                else if (marks)
                {
                    column.push_back(marks);
                    marks = 0;
                }
            }
            if (column.empty() || marks > 0)
                column.push_back(marks);
        }

        if (bpc <= 1) continue;

        // Build row shading hints info.
        total_rows[k] = Hints(height);
        for (int y = 0; y < height; y++)
        {
            std::vector<int>& row = total_rows[k][y];
            row.resize((1 << bpc) - 1);
            int shadings = 1;
            int last_shade = -1;

            for (int x = 0; x < width; x++)
            {
                int num = NumSet(k, x, y);
                if (num == 0) continue;
                row[num - 1]++;
                if (last_shade == -1)
                    last_shade = num;
                if (last_shade != num)
                {
                    shadings++;
                    last_shade = num;
                }
            }
            shading_rows[k].push_back(shadings);
        }

        total_columns[k] = Hints(width);
        for (int x = 0; x < width; x++)
        {
            std::vector<int>& column = total_columns[k][x];
            column.resize((1 << bpc) - 1);
            int shadings = 1;
            int last_shade = -1;

            for (int y = 0; y < height; y++)
            {
                int num = NumSet(k, x, y);
                if (num == 0) continue;
                column[num - 1]++;
                if (last_shade == -1)
                    last_shade = num;
                if (last_shade != num)
                {
                    shadings++;
                    last_shade = num;
                }
            }
            shading_columns[k].push_back(shadings);
        }
    }
    FlushCache();
}

bool Picross::IsSet(int layer, int tx, int ty) const
{
    unsigned short dmask = (1 << bpc) - 1;
    unsigned short mask = dmask << (layer * bpc);
    unsigned short value = data.Get(tx, ty);
    return (value & mask) != 0 && (value != static_cast<unsigned short>(-1));
}

unsigned int Picross::NumSet(int layer, int tx, int ty) const
{
    unsigned short dmask = (1 << bpc) - 1;
    unsigned short value = data.Get(tx, ty);
    if (value == static_cast<unsigned short>(-1)) return 0;
    return (value >> (layer * bpc)) & dmask;
}

void Picross::Draw(wxDC& dc) const
{
    wxSize size = dc.GetSize();
    dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
    auto [hints_height, extra_hints_height, hints_width, extra_hints_width] = CalculateHintBounds();

    int w = std::min((size.GetWidth() - hints_width - extra_hints_width) / width,
                     (size.GetHeight() - hints_height - extra_hints_height) / height);

    for (int i = 0; i < height; i++)
    {
        auto [hints, extra_hints] = GetRowHintsString(i);
        auto text_size = dc.GetMultiLineTextExtent(hints);
        dc.DrawText(hints, 0, i * w + hints_height + (w - text_size.GetHeight()) / 2);

        if (bpc > 1)
        {
            text_size = dc.GetMultiLineTextExtent(extra_hints);
            dc.DrawText(extra_hints, hints_width + w * width, i * w + hints_height + (w - text_size.GetHeight()) / 2);
        }
    }

    for (int i = 0; i < width; i++)
    {
        auto [hints, extra_hints] = GetColumnHintsString(i);
        auto text_size = dc.GetMultiLineTextExtent(hints);
        dc.DrawText(hints, i * w + hints_width + (w - text_size.GetWidth()) / 2, 0);
        if (bpc > 1)
        {
            text_size = dc.GetMultiLineTextExtent(extra_hints);
            dc.DrawText(extra_hints, i * w + hints_width + (w - text_size.GetWidth()) / 2, hints_height + w * height);
        }
    }

    dc.SetClippingRegion(hints_width, hints_height, size.GetWidth() - hints_width, size.GetHeight() - hints_height);

    DrawBoard(dc);

    if (showGrid)
    {
        dc.SetPen(*wxGREY_PEN);
        for (int i = 0; i <= width; i++)
            dc.DrawLine(i * w + hints_width, hints_height, i * w + hints_width, height * w + hints_height);
        for (int i = 0; i <= height; i++)
            dc.DrawLine(hints_width, i * w + hints_height, width * w + hints_width, i * w + hints_height);
    }
}

void Picross::TranslateToCoords(int x, int y, int w, int h, int& tx, int& ty) const
{
    tx = -1;
    ty = -1;

    auto [hints_height, extra_hints_height, hints_width, extra_hints_width] = CalculateHintBounds();

    x -= hints_width;
    y -= hints_height;

    if (x < 0 || y < 0) return;

    int cw = std::min((w - hints_width - extra_hints_width) / width,
                      (h - hints_height - extra_hints_height) / height);

    tx = x / cw;
    ty = y / cw;

    if (tx >= width || ty >= height)
    {
        tx = -1;
        ty = -1;
    }
}

void Picross::Export(const wxString& file, const ExportParams& params) const
{
    std::ofstream out(file.ToStdString(), std::ios::binary);
    if (!out.good()) return;

    PicrossPuzzle puzzle = Export(params);
    puzzle.SerializeToOstream(&out);
}

// TODO move into its own file like the xlsx exporter.
PicrossPuzzle Picross::Export(const ExportParams& params) const
{
    PicrossPuzzle out;

    out.set_width(width);
    out.set_height(height);
    out.set_type(GetType());
    out.set_bpc(bpc);

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

    for (int k = 0; k < max_layers; k++)
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
                const auto& total_hints = total_rows.at(k)[i];
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
                const auto& total_hints = total_columns.at(k)[j];
                for (const auto& elem : total_hints)
                    columns->add_totals(elem);
                columns->set_shading(shading_columns.at(k)[j]);
            }
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
            out.add_solution(data.Get(x, y));
    }

    return out;
}

std::pair<wxString, wxString> Picross::GetRowHintsString(int row) const
{
    if (row_hints_cache.find(row) == row_hints_cache.end())
    {
        wxString row_text;
        const auto& row_hints = rows.at(layer);
        for (const auto& elem : row_hints[row])
            row_text << elem << " ";

        wxString row_text_extra;
        if (bpc > 1)
        {
            const auto& row_total = total_rows.at(layer).at(row);
            for (unsigned int j = 0; j < row_total.size(); j++)
            {
                int num = row_total[j];
                if (num == 0) continue;
                row_text_extra << wxString::Format("%d:%d ", j+1, num);
            }
            row_text_extra << wxString::Format("%d", shading_rows.at(layer)[row]);
        }

        row_hints_cache[row] = row_text;
        row_extra_hints_cache[row] = row_text_extra;
    }
    return {row_hints_cache[row], row_extra_hints_cache[row]};
}

std::pair<wxString, wxString> Picross::GetColumnHintsString(int c) const
{
    if (column_hints_cache.find(c) == column_hints_cache.end())
    {
        wxString column_text;
        const auto& column_hints = columns.at(layer);
        for (const auto& elem : column_hints[c])
            column_text << elem << "\n";

        wxString column_text_extra;
        if (bpc > 1)
        {
            const auto& column_total = total_columns.at(layer)[c];
            for (unsigned int j = 0; j < column_total.size(); j++)
            {
                int num = column_total[j];
                if (num == 0) continue;
                column_text_extra << wxString::Format("%d:%d\n", j+1, num);
            }
            column_text_extra << wxString::Format(" %d ", shading_columns.at(layer)[c]);
        }

        column_hints_cache[c] = column_text;
        column_extra_hints_cache[c] = column_text_extra;
    }
    return {column_hints_cache[c], column_extra_hints_cache[c]};
}

std::tuple<int, int, int, int> Picross::CalculateHintBounds() const
{
    if (std::get<0>(calculated_sizes) != 0)
        return calculated_sizes;

    wxMemoryDC dc;
    dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));

    std::array<wxSize, 4> arr;
    for (int i = 0; i < width; i++)
    {
        auto [hints, extra] = GetColumnHintsString(i);
        auto size = dc.GetMultiLineTextExtent(hints);
        if (arr[0].GetHeight() < size.GetHeight())
            arr[0] = size;
        size = dc.GetMultiLineTextExtent(extra);
        if (arr[1].GetHeight() < size.GetHeight())
            arr[1] = size;
    }
    for (int i = 0; i < height; i++)
    {
        auto [hints, extra] = GetRowHintsString(i);
        auto size = dc.GetMultiLineTextExtent(hints);
        if (arr[2].GetWidth() < size.GetWidth())
            arr[2] = size;
        size = dc.GetMultiLineTextExtent(extra);
        if (arr[3].GetWidth() < size.GetWidth())
            arr[3] = size;
    }

    calculated_sizes = {
        arr[0].GetHeight(),
        arr[1].GetHeight(),
        arr[2].GetWidth(),
        arr[3].GetWidth()};

    return calculated_sizes;
}

