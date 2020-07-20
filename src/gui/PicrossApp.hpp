/******************************************************************************************************
 * Picross
 * Copyright (C) 2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

#ifndef PICROSSAPP_HPP
#define PICROSSAPP_HPP

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/filename.h>
#include "PicrossFrame.hpp"

struct ProgramArgs
{
    bool silent;
    bool validate;

    int type;

    wxString image;
    long bits_per_cell = 1;
    long colors = 0;
    wxString export_type;
    wxFileName filename;
    wxString puzzle_name;
    wxString puzzle_author;
    long puzzle_time = 0;
    wxString solution_image;
    long solution_frames = 0;
    int background_type = 0;
    wxString background_image;
    long top_color = 0;
    long bottom_color = 0;
    wxString background_music;
};

class PicrossApp : public wxApp
{
    public:
        bool OnInit() override;
        void OnInitCmdLine(wxCmdLineParser& parser) override;
        bool OnCmdLineParsed(wxCmdLineParser& parser) override;
        PicrossFrame* GetFrame() {return frame;}
    private:
        PicrossFrame* frame;
        ProgramArgs args;

        bool RunGui();
        bool RunExport();

};

DECLARE_APP(PicrossApp);

#endif

