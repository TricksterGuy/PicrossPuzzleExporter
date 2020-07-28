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
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

DFA line(const std::vector<int>& hints)
{
    REG r0(0), r1(1);
    REG border = *r0;
    REG between = +r0;
    REG r = border;
    if (!hints.empty())
    {
        r += r1(hints[0], hints[0]);
        for (int i = 1; i < hints.size(); i++)
            r += between + r1(hints[i], hints[i]);
    }
    return r + border;
}

struct NonogramOptions : public Options
{
    NonogramOptions(const Picross* p) : Options("Nonogram"), picross(p) {}
    const Picross* picross;
};

enum
{
    BRANCH_NONE, ///< Branch on rows/columns in order
    BRANCH_AFC,  ///< Use AFC for branching
};

class Nonogram : public Script
{
public:
    Nonogram(const NonogramOptions& opt);
    Nonogram(Nonogram& s) : Script(s), spec(s.spec)
    {
        b.update(*this, s.b);
    }
    Space* copy(void) override {return new Nonogram(*this);}
    bool IsSolution() const;
protected:
    int width(void) const {return spec->GetWidth();}
    int height(void) const {return spec->GetHeight();}
};

Nonogram::Nonogram(const NonogramOptions& opt) : Script(opt), spec(opt.picross), b(*this,width() * height(), 0, 1)
{
    const auto& row_hints = spec->GetRowHints().at(0);
    const auto& column_hints = spec->GetColumnHints().at(0);

    Matrix<BoolVarArray> m(b, width(), height());

    // Post constraints
    for (int w = 0; w < width(); w++)
        extensional(*this, m.col(w), line(column_hints[w]));
    for (int h = 0; h < height(); h++)
        extensional(*this, m.row(h), line(row_hints[h]));

    switch (opt.branching())
    {
        case BRANCH_NONE:
        {
            /*
             * The following branches either by columns or rows, depending on
             * whether there are more hints relative to the height or width
             * for columns or rows.
             *
             * This idea is due to Pascal Van Hentenryck and has been suggested
             * to us by Hakan Kjellerstrand.
             */

            // Number of hints for columns
            int cols = 0;
            // Number of hints for rows
            int rows = 0;

            for (int w = 0; w < width(); w++)
                cols += column_hints[w].size();
            for (int h = 0; h < height(); h++)
                rows += row_hints[h].size();

            if (rows * width() > cols * height())
            {
                for (int w = 0; w < width(); w++)
                    branch(*this, m.col(w), BOOL_VAR_NONE(), BOOL_VAL_MAX());
            }
            else
            {
                for (int h = 0; h < height(); h++)
                    branch(*this, m.row(h), BOOL_VAR_NONE(), BOOL_VAL_MAX());
            }
        }
        break;
        case BRANCH_AFC:
            /*
             * The following just uses the AFC for branching. This is
             * equivalent to SIZE/AFC in this case since the variables are
             * binary.
             */
            branch(*this, b, BOOL_VAR_AFC_MAX(opt.decay()), BOOL_VAL_MAX());
            break;
    }
}

bool Nonogram::IsSolution() const
{
    Matrix<BoolVarArray> m(b, width(), height());
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            if (m(x, y).val() != spec->IsSet(0, x, y))
                return false;
        }
    }
    return true;
}

bool Validate(const Picross* picross)
{
    NonogramOptions opt(picross);
    opt.branching(BRANCH_AFC);
    opt.branching(BRANCH_NONE, "none", "Branch on rows/columns in order");
    opt.branching(BRANCH_AFC, "afc", "Use AFC for branching");

    //Script::run<Nonogram,DFS,NonogramOptions>(opt);

    DFS<Nonogram> e(new Nonogram(opt));

    Nonogram* n = e.next();
    if (!n->IsSolution())
    {
        //FindDifferences(n);
        delete n;
        return false;
    }

    Nonogram* n2 = e.next();
    if (n2)
    {
        //FindDifferences(n, n2);
        delete n;
        delete n2;
        return false;
    }

    delete n;
    delete n2;
    return true;

    /*int solutions = 0;
    while (Nonogram* n = e.next())
    {
        solutions++;
        printf("%d\n", solutions);
        //n->print(std::cout);
        delete n;
    }*/
}
