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

#ifndef PICROSSFRAME_HPP
#define PICROSSFRAME_HPP

#include <wx/docview.h>

#include "Picross.hpp"
#include "PicrossGUI.h"

/**
  *  Represents the main class of the application
  */
class PicrossFrame : public PicrossGUI
{
    public:
        PicrossFrame(wxFrame* window) : PicrossGUI(window) {}
        ~PicrossFrame() {}
		void OnChangePuzzleType(wxCommandEvent& event) override;
		void OnChangeBackgroundType(wxCommandEvent& event) override;
		void OnLayerChange(wxCommandEvent& event) override;
		void OnChangeBpc(wxCommandEvent& event) override;
		void OnLoadImage(wxCommandEvent& event) override;
		void OnValidate(wxCommandEvent& event) override;
		void OnExportProtobuf(wxCommandEvent& event) override;
		void OnShowLayer(wxCommandEvent& event) override;
		void OnToggleGrid(wxCommandEvent& event) override;
		void OnQuit(wxCommandEvent& event) override {Destroy();}
		void OnResize(wxSizeEvent& event) {Refresh(); event.Skip();}
    private:
        ExportParams GetExportParams() const;
};

#endif
