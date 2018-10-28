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

#include <wx/scrolwin.h>
#include <wx/bitmap.h>
#include <wx/dc.h>

#include <memory>
#include "Picross.hpp"

class PicrossDataCanvas : public wxScrolledWindow
{
    public:
        PicrossDataCanvas(wxWindow *parent, wxWindowID id=-1, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxHSCROLL|wxVSCROLL);
        ~PicrossDataCanvas();
        void OnDraw(wxDC& dc);
        void OnChangeImage(wxString& image_path);
        void OnChangeType(int new_type);
        void OnChangeLayer(int new_layer);
        void OnChangeBpc(int new_bpc);
        void OnClick(wxMouseEvent& event);
        void OnExport(wxString& path);
        void OnValidate();
        void OnShowLayer(bool show_layer);
        Picross* GetPuzzle() {return picross.get();}
    private:
        std::unique_ptr<Picross> picross;
        wxImage image;
        int size, type, layer, bpc;
        bool showLayer;
};
