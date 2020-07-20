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

#include "PicrossDataCanvas.hpp"
#include "PicrossFactory.hpp"
#include "PicrossValidator.hpp"

#include <wx/msgdlg.h>

PicrossDataCanvas::PicrossDataCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) :
    wxScrolledWindow(parent, id, pos, size, style), image(20, 20), size(0), type(0), layer(0), bpc(1), colors(32), showLayer(false)
{
    picross.reset(NULL);
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PicrossDataCanvas::OnClick), NULL, this);
}

PicrossDataCanvas::~PicrossDataCanvas()
{
    Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PicrossDataCanvas::OnClick), NULL, this);
}

void PicrossDataCanvas::OnClick(wxMouseEvent& event)
{
    if (!picross)
    {
        event.Skip();
        return;
    }

    auto size = GetClientSize();
    int tx, ty;
    picross->TranslateToCoords(event.GetX(), event.GetY(), size.GetWidth(), size.GetHeight(), tx, ty);

    if (tx == -1 || ty == -1)
        return;

    picross->Toggle(layer, tx, ty);
    picross->Build();
    Refresh();
}

void PicrossDataCanvas::OnDraw(wxDC& dc)
{
    if (picross)
        picross->Draw(dc);
}

void PicrossDataCanvas::OnChangeImage(wxString& image_path)
{
    image.LoadFile(image_path);
    picross.reset(PicrossFactory::Create(image, type, ConstructOptions()));
    picross->SetLayer(layer);
    picross->SetShowLayer(showLayer);
    picross->SetShowGrid(showGrid);
    Refresh();
}

void PicrossDataCanvas::OnChangeType(int new_type)
{
    type = new_type;
    if (picross)
    {
        picross.reset(PicrossFactory::Create(image, type, ConstructOptions()));
        picross->SetLayer(layer);
        picross->SetShowLayer(showLayer);
        picross->SetShowGrid(showGrid);
    }
    Refresh();
}

void PicrossDataCanvas::OnChangeLayer(int new_layer)
{
    layer = new_layer;
    if (picross)
    {
        picross->SetLayer(layer);
        picross->SetShowLayer(showLayer);
        picross->SetShowGrid(showGrid);
    }
    Refresh();
}

void PicrossDataCanvas::OnChangeBpc(int new_bpc)
{
    bpc = new_bpc;
    if (picross)
    {
        picross.reset(PicrossFactory::Create(image, type, ConstructOptions()));
        picross->SetLayer(layer);
        picross->SetShowLayer(showLayer);
        picross->SetShowGrid(showGrid);
    }
    Refresh();
}

void PicrossDataCanvas::OnChangeNumColors(int new_colors)
{
    colors = new_colors;
    if (picross)
    {
        picross.reset(PicrossFactory::Create(image, type, ConstructOptions()));
        picross->SetLayer(layer);
        picross->SetShowLayer(showLayer);
        picross->SetShowGrid(showGrid);
    }
    Refresh();
}


void PicrossDataCanvas::OnValidate()
{
    if (picross)
    {
        Problem problem;
        bool validate = ::Validate(picross.get(), problem);
        if (!validate)
        {
            const char* rgblayers[] = {"RED", "GREEN", "BLUE"};
            const char* rbylayers[] = {"BLACK", "WHITE", "RED", "BLUE", "YELLOW"};
            wxString message;
            if (type == 2)
                message = wxString::Format("Layer %s", rgblayers[problem.layer]);
            else if (type == 3)
                message = wxString::Format("Layer %s", rbylayers[problem.layer]);

            wxMessageBox(wxString::Format("%s Rows %d and %d can be swapped and puzzle will still be solved.", message, problem.row1, problem.row2),
                         "Validation Error, Solution not unique", wxICON_ERROR);
        }
        else
            wxMessageBox("Validated Successfully");
    }
}

void PicrossDataCanvas::OnShowLayer(bool newShowLayer)
{
    showLayer = newShowLayer;
    picross->SetShowLayer(showLayer);
    Refresh();
}

void PicrossDataCanvas::OnShowGrid(bool newShowGrid)
{
    showGrid = newShowGrid;
    picross->SetShowGrid(showGrid);
    Refresh();
}
