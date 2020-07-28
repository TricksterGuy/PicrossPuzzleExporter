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

#include "PicrossApp.hpp"
#include "PicrossFrame.hpp"
#include <wx/config.h>
#include <wx/dialog.h>

#include "PicrossValidator.hpp"
#include "ProtoExporter.hpp"
#ifdef ENABLE_XLSX
#include "XlsxExporter.hpp"
#endif
#include "PicrossFactory.hpp"

constexpr const char* FULL_VERSION_STRING = "0.2";

IMPLEMENT_APP(PicrossApp);

static const wxCmdLineEntryDesc command_line_desc [] =
{
     {wxCMD_LINE_SWITCH, "h", "help",          "Displays help on command line parameters",                      wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},

     // Required arguments for command line mode.
     {wxCMD_LINE_OPTION, "t", "type",           "Type of picross to make (Classic|Grayscale|Light|Painting|ColorHints)",  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "i", "image",          "Image filename",                                               wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "b", "bits_per_cell",  "Bits per cell (for type = Light|Grayscale)",                   wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "c", "colors",         "Number of colors (for type = ColorHints)",                     wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "e", "export",         "Export type (proto|xlsx)",                                     wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "f", "filename",       "Export filename (minus extension)",                            wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},

     // Optional arguments.
     {wxCMD_LINE_OPTION, "", "puzzle_name",         "Puzzle name",                      wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "puzzle_author",       "Puzzle author",                    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "puzzle_time",         "Puzzle time (seconds)",            wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "solution_image",      "Solution image",                   wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "solution_frames",     "Solution frames",                  wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "top_color",           "Background gradient top color",    wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "bottom_color",        "Background gradient bottom color", wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "background_type",     "Background type",                  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "background_image",    "Background image",                 wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
     {wxCMD_LINE_OPTION, "", "background_music",    "Background music",                 wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},

     {wxCMD_LINE_SWITCH, "v", "validate",       "Validate puzzle for unique solution" },
     {wxCMD_LINE_SWITCH, "s", "silent",         "Disables the GUI" },

     {wxCMD_LINE_NONE}
};

bool PicrossApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    wxInitAllImageHandlers();

    if (args.silent)
        return RunExport();
    else
        return RunGui();
}

bool PicrossApp::RunGui()
{
    frame = new PicrossFrame(NULL);
    frame->SetTitle(wxString::Format("Picross Puzzle Exporter v%s", FULL_VERSION_STRING));
    SetTopWindow(frame);
    frame->Centre();
    frame->Show();
    return true;
}

bool PicrossApp::RunExport()
{
    wxImage image;
    image.LoadFile(args.image);

    PicrossFactory::Options options;
    options.bpc = args.bits_per_cell;
    options.colors = args.colors;

    std::unique_ptr<Picross> picross(PicrossFactory::Create(image, args.type, options));
    if (!picross)
    {
        printf("Failed to create Picross Puzzle.\n");
        return false;
    }

    picross->Build();

    ExportParams params;
    params.author = args.puzzle_author;
    params.background_type = args.background_type;
    params.bg_image = args.background_image;
    params.bg_music = args.background_music;
    params.bottom_color = args.bottom_color;
    params.frames = args.solution_frames;
    params.name = args.puzzle_name;
    params.time = args.puzzle_time;
    params.top_color = args.top_color;

    if (args.validate)
    {
        if (!Validate(picross.get()))
        {
            /*const char* rgblayers[] = {"RED", "GREEN", "BLUE"};
            const char* rbylayers[] = {"BLACK", "WHITE", "RED", "BLUE", "YELLOW"};
            wxString message;
            if (args.type == 2)
                message = wxString::Format("Layer %s ", rgblayers[problem.layer]);
            else if (args.type == 3)
                message = wxString::Format("Layer %s ", rbylayers[problem.layer]);*/

            printf("Validation Error, Solution not unique\n");
            //printf("%sRows %d and %d can be swapped and puzzle will still be solved.", message.ToStdString().c_str(), problem.row1, problem.row2);
            return false;
        }
    }

    bool success = false;
    std::string filename;
    if (args.export_type == "proto")
    {
        args.filename.SetExt("picross");
        filename = args.filename.GetFullName().ToStdString();
        success = ExportProto(filename, *picross, params);
    }
    else if (args.export_type == "xlsx")
    {
#ifdef ENABLE_XLSX
        args.filename.SetExt("xlsx");
        filename = args.filename.GetFullName().ToStdString();
        success = ExportXlsx(filename, *picross);
#else

        printf("xlsx export not enabled in the program. Please compile with xlsx support enabled.\n");
        return false;
#endif
    }

    if (!success)
    {
        printf("Failed to export Picross Puzzle.\n");
        return false;
    }

    printf("Exported puzzle successfully!\nFilename: %s", filename.c_str());
    // Don't actually start the program.
    return false;
}

void PicrossApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc(command_line_desc);
    parser.SetSwitchChars("-");
}

bool PicrossApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    const std::unordered_map<std::string, int> valid_types = {{"classic", 0}, {"grayscale", 1}, {"light", 2}, {"painting", 3}, {"colorhints", 4}};
    const std::unordered_set<std::string> valid_exports = {"proto", "xlsx"};
    const std::unordered_map<std::string, int> valid_background_types = {{"stationary", 0}, {"stretched", 1}, {"tiled", 2}, {"gradient", 3}};
    args.silent = parser.Found("s");
    args.validate = parser.Found("v");

    wxString type;
    wxString filename;

    if (args.silent &&
        !(parser.Found("t", &type) &&
        parser.Found("i", &args.image) &&
        parser.Found("e", &args.export_type) &&
        parser.Found("f", &filename)))
    {
        printf(parser.GetUsageString().c_str());
        printf("All of --type --image --export_type and --filename are required if using the command line mode.\n");
        return false;
    }
    args.filename = wxFileName(filename);

    auto valid_type = valid_types.find(type.Lower().ToStdString());
    if (args.silent && valid_type == valid_types.end())
    {
        printf(parser.GetUsageString().c_str());
        printf("Invalid type given. Got: %s", type.ToStdString().c_str());
        return false;
    }
    else if (valid_type != valid_types.end())
    {
        args.type = valid_type->second;
    }

    parser.Found("b", &args.bits_per_cell);
    if (args.silent && (args.bits_per_cell > 3 || args.bits_per_cell < 1))
    {
        printf(parser.GetUsageString().c_str());
        printf("Invalid value given for --bits_per_cell. Must be from 1-3. Got %ld\n", args.bits_per_cell);
        return false;
    }

    if (args.type == 4)
    {
        if (!parser.Found("c", &args.colors))
        {
            printf("No value set for --colors it is required for --type=colorhints\n");
            return false;
        }
        if (args.colors < 2 || args.colors > 32)
        {
            printf("Invalid value for --colors, it should be between [2, 32]\n");
            return false;
        }
    }

    args.export_type.MakeLower();
    if (args.silent && valid_exports.find(args.export_type.ToStdString()) == valid_exports.end())
    {
        printf(parser.GetUsageString().c_str());
        printf("Invalid export type %s given.\n", args.export_type.ToStdString().c_str());
        return false;
    }

    wxString background_type;
    wxString top_color;
    wxString bottom_color;

    parser.Found("puzzle_name", &args.puzzle_name);
    parser.Found("puzzle_author", &args.puzzle_author);
    parser.Found("puzzle_time", &args.puzzle_time);
    parser.Found("solution_image", &args.solution_image);
    parser.Found("solution_frames", &args.solution_frames);
    if (parser.Found("top_color", &top_color))
        top_color.ToLong(&args.top_color, 16);
    if (parser.Found("bottom_color", &bottom_color))
        bottom_color.ToLong(&args.bottom_color, 16);
    parser.Found("background_type", &background_type);
    parser.Found("background_image", &args.background_image);
    parser.Found("background_music", &args.background_music);

    auto valid_background_type = valid_background_types.find(background_type.Lower().ToStdString());
    if (valid_background_type != valid_background_types.end())
        args.background_type = valid_background_type->second;

    return true;
}

