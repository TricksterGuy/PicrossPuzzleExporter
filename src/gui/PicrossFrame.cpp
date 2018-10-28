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

#include "PicrossFrame.hpp"
#include "PicrossDataCanvas.hpp"
#include "QRCodeDialog.hpp"
#include <wx/filedlg.h>
#include <qrencode.h>
#include <encode.h>
#include <sstream>

PicrossFrame::PicrossFrame(wxFrame* window) : PicrossGUI(window)
{
}

PicrossFrame::~PicrossFrame()
{
}

void PicrossFrame::OnLoadImage(wxCommandEvent& event)
{
    wxString image_path = wxLoadFileSelector("Image", "*", "", this);
    if (image_path.IsEmpty()) return;
    puzzleDataCanvas->OnChangeImage(image_path);
}

void PicrossFrame::OnValidate(wxCommandEvent& event)
{
    puzzleDataCanvas->OnValidate();
}

void PicrossFrame::OnExportImage(wxCommandEvent& event)
{
    wxString path = wxSaveFileSelector("Picross Puzzle", "picross", "", this);
    if (path.IsEmpty()) return;
    puzzleDataCanvas->OnExport(path);
}

void PicrossFrame::OnQRCode(wxCommandEvent& event)
{
    Picross* picross = puzzleDataCanvas->GetPuzzle();
    if (picross)
    {
        PicrossPuzzle puzzledata = picross->Export();
        std::string data;
        puzzledata.SerializeToString(&data);

        std::stringstream instream;
        std::stringstream outstream;
        instream.write(data.data(), data.size());
        base64::encoder encoder(data.size());
		encoder.encode(instream, outstream);

        std::string encoded = outstream.str();
        QRcode* qr = QRcode_encodeData(encoded.size(), (const unsigned char*)encoded.data(), 0, QR_ECLEVEL_H);
        QRCodeDialog* dialog = new QRCodeDialog(qr);
        dialog->ShowModal();
        delete dialog;
    }
}

void PicrossFrame::OnChangeType(wxCommandEvent& event)
{
    int type = event.GetSelection();
    puzzleDataCanvas->OnChangeType(type);
    puzzleDataCanvas->OnChangeLayer(0);

    if (type == 0)
    {
        layersChoice->Hide();
        showOnlyLayer->Hide();
        bitsPerCell->Hide();
    }
    else if (type == 1)
    {
        layersChoice->Hide();
        showOnlyLayer->Hide();
        bitsPerCell->Show();
    }
    else if (type == 2)
    {
        layersChoice->Show();
        showOnlyLayer->Show();
        bitsPerCell->Show();
    }
    else if (type == 3)
    {
        layersChoice->Show();
        showOnlyLayer->Show();
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

void PicrossFrame::OnLayerChange(wxCommandEvent& event)
{
    puzzleDataCanvas->OnChangeLayer(event.GetSelection());
}

void PicrossFrame::OnChangeBpc(wxCommandEvent& event)
{
    puzzleDataCanvas->OnChangeBpc(event.GetSelection() + 1);
}

void PicrossFrame::OnShowLayer(wxCommandEvent& event)
{
    puzzleDataCanvas->OnShowLayer(event.IsChecked());
}
