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

#include "PicrossFrame.hpp"
#include "PicrossDataCanvas.hpp"
#include <wx/filedlg.h>
#include <sstream>

void PicrossFrame::OnLoadImage(wxCommandEvent& event)
{
    wxString image_path = wxLoadFileSelector("Image", "*", "", this);
    if (image_path.IsEmpty()) return;
    puzzleDataCanvas->OnChangeImage(image_path);
}

void PicrossFrame::OnValidate(wxCommandEvent& event)
{
    Picross* picross = puzzleDataCanvas->GetPuzzle();
    if (!picross) return;

    puzzleDataCanvas->OnValidate();
}

ExportParams PicrossFrame::GetExportParams() const
{
    ExportParams params;
    params.name = name->GetValue().ToStdString();
    params.author = author->GetValue().ToStdString();
    params.image = solutionImage->GetFileName().GetFullName().ToStdString();
    params.frames = solutionFrames->GetValue();
    params.time = puzzleTime->GetValue();
    params.background_type = backgroundType->GetSelection();
    params.bg_image = backgroundImage->GetFileName().GetFullName().ToStdString();
    params.top_color = topColor->GetColour().GetRGBA();
    params.bottom_color = bottomColor->GetColour().GetRGBA();
    return params;

}

void PicrossFrame::OnExportProtobuf(wxCommandEvent& event)
{
    wxString path = wxSaveFileSelector("Picross Puzzle", "picross", "", this);
    if (path.IsEmpty()) return;
    puzzleDataCanvas->OnExport(path, GetExportParams());
}

void PicrossFrame::OnChangePuzzleType(wxCommandEvent& event)
{
    int type = event.GetSelection();
    puzzleDataCanvas->OnChangeBpc(1);
    puzzleDataCanvas->OnChangeType(type);
    puzzleDataCanvas->OnChangeLayer(0);
    bitsPerCell->SetSelection(0);

    if (type == 0)
    {
        layerLabel->Hide();
        layersChoice->Hide();
        showCurrentLayerLabel->Hide();
        showOnlyLayer->Hide();
        bitsPerCellLabel->Hide();
        bitsPerCell->Hide();
    }
    else if (type == 1)
    {
        layerLabel->Hide();
        layersChoice->Hide();
        showCurrentLayerLabel->Hide();
        showOnlyLayer->Hide();
        bitsPerCellLabel->Show();
        bitsPerCell->Show();
    }
    else if (type == 2)
    {
        layerLabel->Show();
        layersChoice->Show();
        showCurrentLayerLabel->Show();
        showOnlyLayer->Show();
        bitsPerCellLabel->Show();
        bitsPerCell->Show();
    }
    else if (type == 3)
    {
        layerLabel->Show();
        layersChoice->Show();
        showCurrentLayerLabel->Show();
        showOnlyLayer->Show();
        bitsPerCellLabel->Hide();
        bitsPerCell->Hide();
    }

    if (layersChoice->IsShown())
    {
        layersChoice->Clear();
        if (type == 2)
        {
            layersChoice->Append("Red");
            layersChoice->Append("Green");
            layersChoice->Append("Blue");
        }
        else if (type == 3)
        {
            layersChoice->Append("Black");
            layersChoice->Append("White");
            layersChoice->Append("Red");
            layersChoice->Append("Blue");
            layersChoice->Append("Yellow");
        }
        layersChoice->Select(0);
    }

    contents->Layout();
}

void PicrossFrame::OnChangeBackgroundType(wxCommandEvent& event)
{
    int type = event.GetSelection();
    if (type != 3)
    {
        backgroundImageLabel->Show();
        backgroundImage->Show();
        topColorLabel->Hide();
        topColor->Hide();
        bottomColorLabel->Hide();
        bottomColor->Hide();
    }
    else
    {
        backgroundImageLabel->Hide();
        backgroundImage->Hide();
        topColorLabel->Show();
        topColor->Show();
        bottomColorLabel->Show();
        bottomColor->Show();
    }
    contents->Layout();
}

void PicrossFrame::OnLayerChange(wxCommandEvent& event)
{
    Picross* picross = puzzleDataCanvas->GetPuzzle();
    if (!picross) return;
    puzzleDataCanvas->OnChangeLayer(event.GetSelection());
}

void PicrossFrame::OnChangeBpc(wxCommandEvent& event)
{
    Picross* picross = puzzleDataCanvas->GetPuzzle();
    if (!picross) return;
    puzzleDataCanvas->OnChangeBpc(event.GetSelection() + 1);
}

void PicrossFrame::OnShowLayer(wxCommandEvent& event)
{
    Picross* picross = puzzleDataCanvas->GetPuzzle();
    if (!picross) return;
    puzzleDataCanvas->OnShowLayer(event.IsChecked());
}

void PicrossFrame::OnToggleGrid(wxCommandEvent& event)
{
    Picross* picross = puzzleDataCanvas->GetPuzzle();
    if (!picross) return;
    puzzleDataCanvas->OnShowGrid(event.IsChecked());
}
