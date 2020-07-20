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

void Picross::Build()
{
    // TODO write Picross::Clear which erases all hints from puzzle.
    for (int k = 0; k < max_layers; k++)
    {
        // Build row hints info.
        rows[k] = Hints(height);
        color_rows[k] = Hints(height);
        for (int y = 0; y < height; y++)
        {
            auto& row = rows[k][y];
            auto& color_row = color_rows[k][y];
            row.reserve(width / 2);
            color_row.reserve(width / 2);
            int marks = 0;
            int color = -1;
            for (int x = 0; x < width; x++)
            {
                int num = NumSet(k, x, y);
                if (color == -1 && num != 0)
                {
                    color = num;
                }

                if (num == color)
                {
                    marks++;
                }
                else if (marks)
                {
                    row.push_back(marks);
                    color_row.push_back(color);
                    marks = num == 0 ? 0 : 1;
                    color = num == 0 ? -1 : num;
                }
            }
            if (row.empty() || marks > 0)
            {
                row.push_back(marks);
                color_row.push_back(color == -1 ? 0 : color);
            }
        }

        // Build column hints info.
        columns[k] = Hints(width);
        color_columns[k] = Hints(width);
        for (int x = 0; x < width; x++)
        {
            auto& column = columns[k][x];
            auto& color_column = color_columns[k][x];
            column.reserve(height / 2);
            color_column.reserve(height / 2);
            int marks = 0;
            int color = -1;
            for (int y = 0; y < height; y++)
            {
                int num = NumSet(k, x, y);
                if (color == -1 && num != 0)
                {
                    color = num;
                }

                if (num == color)
                {
                    marks++;
                }
                else if (marks)
                {
                    column.push_back(marks);
                    color_column.push_back(color);
                    marks = num == 0 ? 0 : 1;
                    color = num == 0 ? -1 : num;
                }
            }
            if (column.empty() || marks > 0)
            {
                column.push_back(marks);
                color_column.push_back(color == -1 ? 0 : color);
            }
        }

        if (bpc <= 1) continue;

        // Build row shading hints info.
        total_rows[k] = Hints(height);
        shading_rows[k].clear();
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
        shading_columns[k].clear();
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

    /*for (int i = 0; i < height; i++)
    {
        auto [hints, extra_hints] = GetRowHintsBitmap(i);
        auto text_size = hints.GetSize();
        dc.DrawBitmap(hints, 0, i * w + hints_height + (w - text_size.GetHeight()) / 2);

        if (bpc > 1)
        {
            text_size = extra_hints.GetSize();
            dc.DrawBitmap(extra_hints, hints_width + w * width, i * w + hints_height + (w - text_size.GetHeight()) / 2);
        }
    }

    for (int i = 0; i < width; i++)
    {
        auto [hints, extra_hints] = GetColumnHintsBitmap(i);
        auto text_size = hints.GetSize();
        dc.DrawBitmap(hints, i * w + hints_width + (w - text_size.GetWidth()) / 2, 0);
        if (bpc > 1)
        {
            text_size = extra_hints.GetSize();
            dc.DrawBitmap(extra_hints, i * w + hints_width + (w - text_size.GetWidth()) / 2, hints_height + w * height);
        }
    }*/

    dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
    dc.SetPen(*wxBLACK_PEN);

    for (int row = 0; row < height; row++)
    {
        auto [hints, extra] = GetRowHintsString(row);

        const auto& row_hints = rows.at(layer)[row];
        const auto& color_row_hints = color_rows.at(layer)[row];

        int x = 0;
        for (unsigned int i = 0; i < row_hints.size(); i++)
        {
            wxString text = wxString::Format("%d ", row_hints[i]);
            auto size = dc.GetMultiLineTextExtent(text);
            if (type == PicrossPuzzle::TYPE_COLORED_HINT)
            {
                int index = color_row_hints[i] - 1;
                wxColour color = (index >= 0) ? palette[index] : *wxBLACK;
                dc.SetTextForeground(color);
            }

            dc.DrawText(text, x, row * w + hints_height + (w - size.GetHeight()) / 2);
            x += size.GetWidth();
        }

        if (bpc > 1)
        {
            auto text_size = dc.GetMultiLineTextExtent(extra);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawText(extra, hints_width + w * width, row * w + hints_height + (w - text_size.GetHeight()) / 2);
        }
    }

    for (int column = 0; column < width; column++)
    {
        auto [hints, extra] = GetColumnHintsString(column);

        const auto& column_hints = columns.at(layer)[column];
        const auto& color_column_hints = color_columns.at(layer)[column];

        int y = 0;
        for (unsigned int i = 0; i < column_hints.size(); i++)
        {
            wxString text = wxString::Format("%d", column_hints[i]);
            auto size = dc.GetMultiLineTextExtent(text);
            if (type == PicrossPuzzle::TYPE_COLORED_HINT)
            {
                int index = color_column_hints[i] - 1;
                wxColour color = (index >= 0) ? palette[index] : *wxBLACK;
                dc.SetTextForeground(color);
            }

            dc.DrawText(text,  column * w + hints_width + (w - size.GetWidth()) / 2, y);
            y += size.GetHeight();
        }

        if (bpc > 1)
        {
            auto text_size = dc.GetMultiLineTextExtent(extra);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawText(extra, column * w + hints_width + (w - text_size.GetWidth()) / 2, hints_height + w * height);
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

std::pair<wxBitmap, wxBitmap> Picross::GetRowHintsBitmap(int row) const
{
    if (row_hints_cache.find(row) == row_hints_cache.end())
    {
        wxMemoryDC dc;
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));

        auto [hints, extra] = GetRowHintsString(row);
        auto size = dc.GetMultiLineTextExtent(hints);

        wxBitmap row_hints_bitmap(size);
        dc.SelectObject(row_hints_bitmap);

        const auto& row_hints = rows.at(layer)[row];
        const auto& color_row_hints = color_rows.at(layer)[row];

        int x = 0;
        for (unsigned int i = 0; i < row_hints.size(); i++)
        {
            wxString text = wxString::Format("%d ", row_hints[i]);
            dc.SetPen(palette[color_row_hints[i] - 1]);
            dc.DrawText(text, x, 0);
            x += dc.GetMultiLineTextExtent(text).GetWidth();
        }

        wxBitmap extra_row_hints_bitmap;
        if (bpc > 1)
        {
            auto size = dc.GetMultiLineTextExtent(hints);
            extra_row_hints_bitmap.Create(size);

            dc.SelectObject(extra_row_hints_bitmap);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawText(extra, 0, 0);
        }
        dc.SelectObject(wxNullBitmap);

        row_hints_cache[row] = row_hints_bitmap;
        row_extra_hints_cache[row] = extra_row_hints_bitmap;
    }
    return {row_hints_cache[row], row_extra_hints_cache[row]};
}

std::pair<wxString, wxString> Picross::GetRowHintsString(int row) const
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

    return {row_text, row_text_extra};
}

std::pair<wxBitmap, wxBitmap> Picross::GetColumnHintsBitmap(int column) const
{
    if (column_hints_cache.find(column) == column_hints_cache.end())
    {
        wxMemoryDC dc;
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));

        auto [hints, extra] = GetColumnHintsString(column);
        auto size = dc.GetMultiLineTextExtent(hints);

        wxBitmap column_hints_bitmap(size);
        dc.SelectObject(column_hints_bitmap);

        const auto& column_hints = columns.at(layer)[column];
        const auto& color_column_hints = color_columns.at(layer)[column];

        int y = 0;
        for (unsigned int i = 0; i < column_hints.size(); i++)
        {
            dc.SetPen(palette[color_column_hints[i] - 1]);
            dc.DrawText(wxString::Format("%d", column_hints[i]), 0, y);
            y += dc.GetCharHeight();
        }

        wxBitmap extra_column_hints_bitmap;
        if (bpc > 1)
        {
            auto size = dc.GetMultiLineTextExtent(extra);
            extra_column_hints_bitmap.Create(size);

            dc.SelectObject(extra_column_hints_bitmap);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawText(extra, 0, 0);
        }
        dc.SelectObject(wxNullBitmap);

        column_hints_cache[column] = column_hints_bitmap;
        column_extra_hints_cache[column] = extra_column_hints_bitmap;
    }
    return {column_hints_cache[column], column_extra_hints_cache[column]};
}

std::pair<wxString, wxString> Picross::GetColumnHintsString(int c) const
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
    return {column_text, column_text_extra};
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
        arr[3].GetWidth()
    };

    return calculated_sizes;
}

