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

#include "PicrossValidator.hpp"
#include <map>
#include <vector>
#include <set>

#define ContainsKey(map, val) (map.find(val) != map.end())
#define IsEmptySolution(row_col) (row_col.size() == 1 && row_col[0] == 0)
#define IsFilledSolution(row_col, width) (row_col.size() == 1 && row_col[0] == width)

std::set<int> GetMarkedColumns(const PicrossLayer& data, int r1, int r2, int layer)
{
    int width = data.GetWidth();
    int mask = 1 << layer;
    std::set<int> marks;
    for (int i = 0; i < width; i++)
    {
        int val = data.Get(i, r1);
        if ((val & mask) && val != -1)
            marks.insert(i);
        val = data.Get(i, r2);
        if ((val & mask) && val != -1)
            marks.insert(i);
    }
    return marks;
}

PicrossLayer SwapRows(PicrossLayer& data, int r1, int r2)
{
    PicrossLayer newLayer = data;
    int width = data.GetWidth();
    for (int i = 0; i < width; i++)
    {
        unsigned int tmp = newLayer.Get(i, r1);
        newLayer.Set(i, r1, newLayer.Get(i, r2));
        newLayer.Set(i, r2, tmp);
    }
    return newLayer;
}

bool RowsEqual(PicrossLayer& data, int r1, int r2, int layer)
{
    int width = data.GetWidth();
    int mask = 1 << layer;
    for (int i = 0; i < width; i++)
    {
        int val = data.Get(i, r1);
        int val2 = data.Get(i, r2);
        if (((val & mask) && val != -1) != ((val2 & mask) && val2 != -1))
            return false;
    }
    return true;
}

std::map<int, std::vector<int>> BuildPartialSolution(PicrossLayer& data, std::set<int>& columns, int layer)
{
    std::map<int, std::vector<int>> partial_solution;
    unsigned int mask = 1 << layer;
    int height = data.GetHeight();
    for (int col_id : columns)
    {
        std::vector<int> col;
        col.reserve(height / 2);
        int marks = 0;
        for (int y = 0; y < height; y++)
        {
            int val = data.Get(col_id, y);
            if ((val & mask) && val != -1)
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
        partial_solution[col_id] = col;
    }
    return partial_solution;
}

bool Validate(const Picross* picross, Problem& problem)
{
    std::map<int, solutions> rows = picross->rows;
    std::map<int, solutions> columns = picross->cols;
    PicrossLayer data = picross->data;
    int max_layers = picross->max_layers;
    int width = picross->width;
    int height = picross->height;

    for (int layer = 0; layer < max_layers; layer++)
    {
        solutions& row_solutions = rows[layer];
        solutions& col_solutions = columns[layer];
        int current_id = 0;
        // Map from solution to unique id
        std::map<std::vector<int>, int> found_sets;
        // Map from unique id to rows containing solution.
        std::map<int, std::set<int>> id_members;
        for (int i = 0; i < height; i++)
        {
            std::vector<int>& row = row_solutions[i];
            // Don't care about rows with 0's or filled rows with all marks.
            if (IsEmptySolution(row) || IsFilledSolution(row, width))
                continue;
            if (!ContainsKey(found_sets, row))
            {
                found_sets[row] = current_id;
                id_members[current_id] = std::set<int>();
                current_id++;
            }
            int id = found_sets[row];
            id_members[id].insert(i);
        }
        for (const auto& id_memberset : id_members)
        {
            // Only one row/column had this sequence
            if (id_memberset.second.size() <= 1)
                continue;

            const std::set<int>& members = id_memberset.second;
            for (int id1 : members)
            {
                for (int id2 : members)
                {
                    if (id1 == id2) continue;
                    if (RowsEqual(data, id1, id2, layer)) continue;

                    std::set<int> columns = GetMarkedColumns(data, id1, id2, layer);
                    PicrossLayer swapped = SwapRows(data, id1, id2);
                    std::map<int, std::vector<int>> partial_solution = BuildPartialSolution(swapped, columns, layer);
                    bool notchanged = true;
                    for (const auto& id_solution : partial_solution)
                    {
                        // if there is a difference we can break
                        if (id_solution.second != col_solutions[id_solution.first])
                        {
                            notchanged = false;
                            break;
                        }
                    }
                    // Not unique!
                    if (notchanged)
                    {
                        problem.layer = layer;
                        problem.row1 = id1;
                        problem.row2 = id2;
                        return false;
                    }
                }
            }
        }
    }

    problem.layer = -1;
    problem.row1 = -1;
    problem.row2 = -1;
    return true;
}
