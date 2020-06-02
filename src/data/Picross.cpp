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

#include "Picross.hpp"
#include <fstream>
#include <iostream>

void Picross::Build()
{
    for (int k = 0; k < max_layers; k++)
    {
        rows[k] = solutions(height);
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
    }
    for (int k = 0; k < max_layers; k++)
    {
        cols[k] = solutions(width);
        for (int x = 0; x < width; x++)
        {
            std::vector<int>& col = cols[k][x];
            col.reserve(height / 2);
            int marks = 0;
            for (int y = 0; y < height; y++)
            {
                if (IsSet(k, x, y))
                {
                    marks++;
                }
                else if (marks)
                {
                    col.push_back(marks);
                    marks = 0;
                }
            }
            if (col.empty() or marks > 0)
                col.push_back(marks);
        }
    }

    if (bpc > 1)
    {
        for (int k = 0; k < max_layers; k++)
        {
            std::vector<int> rows(height);
            std::vector<int> cols(width);
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int num = NumSet(k, x, y);
                    rows[y] += num;
                    cols[x] += num;
                }
            }
            total_rows[k] = rows;
            total_cols[k] = cols;
        }
    }
}

bool Picross::IsSet(int layer, int tx, int ty)
{
    unsigned short dmask = (1 << bpc) - 1;
    unsigned short mask = dmask << (layer * bpc);
    unsigned short value = data.Get(tx, ty);
    return (value & mask) != 0 && (value != (unsigned short)-1);
}

unsigned int Picross::NumSet(int layer, int tx, int ty)
{
    unsigned short dmask = (1 << bpc) - 1;
    unsigned short value = data.Get(tx, ty);
    if (value == (unsigned short) -1) return 0;
    return (value >> (layer * bpc)) & dmask;
}

void Picross::Draw(wxDC& dc)
{
    wxSize size = dc.GetSize();
    int cw = (size.GetWidth() - 132) / width;
    int ch = (size.GetHeight() - 132) / height;
    solutions& row = rows[layer];
    solutions& col = cols[layer];
    for (int i = 0; i < height; i++)
    {
        wxString row_text;
        for (unsigned int j = 0; j < row[i].size(); j++)
        {
            row_text << row[i][j] << " ";
        }
        dc.DrawText(row_text, 0, i * ch + 100 + ch / 2);
    }

    for (int i = 0; i < width; i++)
    {
        wxString col_text;
        for (unsigned int j = 0; j < col[i].size(); j++)
        {
            col_text << col[i][j] << "\n";
        }
        dc.DrawText(col_text, i * cw + 100 + cw / 2, 0);
    }
    dc.SetClippingRegion(100, 100, size.GetWidth() - 100, size.GetHeight() - 100);

    if (bpc > 1)
    {
        std::vector<int>& row_total = total_rows[layer];
        std::vector<int>& col_total = total_cols[layer];

        for (unsigned int i = 0; i < row_total.size(); i++)
        {
            dc.DrawText(wxString::Format("%d", row_total[i]), size.GetWidth() - 16, i * ch + 100 + ch / 2);
        }
        for (unsigned int i = 0; i < col_total.size(); i++)
        {
            dc.DrawText(wxString::Format("%d", col_total[i]), i * cw + 100 + cw / 2, size.GetHeight() - 16);
        }
    }
}

void Picross::Export(const wxString& file) const
{
    std::ofstream out(file.ToStdString());
    if (!out.good()) return;

    PicrossPuzzle puzzle = Export();

    puzzle.SerializeToOstream(&out);
}

PicrossPuzzle Picross::Export() const
{
    PicrossPuzzle out;

    out.set_width(width);
    out.set_height(height);
    out.set_type(GetType());
    out.set_bpc(bpc);

    for (int k = 0; k < max_layers; k++)
    {
        SolutionLayer* layer = out.add_layers();
        const solutions& row_solutions = rows.at(k);
        const solutions& col_solutions = cols.at(k);

        for (int i = 0; i < height; i++)
        {
            const std::vector<int>& row_data = row_solutions[i];
            Solution* rows = layer->add_rows();
            for (const auto& elem : row_data)
                rows->add_data(elem);
            if (bpc > 1)
                rows->set_total(total_rows.at(k)[i]);
        }

        for (int j = 0; j < width; j++)
        {
            const std::vector<int>& col_data = col_solutions[j];
            Solution* cols = layer->add_cols();
            for (const auto& elem : col_data)
                cols->add_data(elem);
            if (bpc > 1)
                cols->set_total(total_cols.at(k)[j]);
        }

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                out.add_data(data.Get(x, y));
            }
        }
    }

    return out;
}
