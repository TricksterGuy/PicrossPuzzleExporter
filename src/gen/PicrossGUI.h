///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class PicrossDataCanvas;

#include <wx/scrolwin.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/radiobox.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_LOAD 1000
#define ID_PROTOBUF 1001
#define wxID_QUIT 1002
#define ID_SHOW_GRID 1003
#define ID_VALIDATE 1004

///////////////////////////////////////////////////////////////////////////////
/// Class PicrossGUI
///////////////////////////////////////////////////////////////////////////////
class PicrossGUI : public wxFrame
{
	private:

	protected:
		wxPanel* m_panel4;
		wxBoxSizer* contents;
		PicrossDataCanvas* puzzleDataCanvas;
		wxStaticText* m_staticText1;
		wxTextCtrl* name;
		wxStaticText* m_staticText11;
		wxTextCtrl* author;
		wxStaticText* m_staticText7;
		wxFilePickerCtrl* solutionImage;
		wxStaticText* m_staticText111;
		wxSpinCtrl* solutionFrames;
		wxRadioBox* puzzleType;
		wxStaticText* m_staticText2;
		wxSpinCtrl* puzzleTime;
		wxStaticText* layerLabel;
		wxChoice* layersChoice;
		wxStaticText* showCurrentLayerLabel;
		wxCheckBox* showOnlyLayer;
		wxStaticText* bitsPerCellLabel;
		wxChoice* bitsPerCell;
		wxRadioBox* backgroundType;
		wxStaticText* backgroundImageLabel;
		wxFilePickerCtrl* backgroundImage;
		wxStaticText* topColorLabel;
		wxColourPickerCtrl* topColor;
		wxStaticText* bottomColorLabel;
		wxColourPickerCtrl* bottomColor;
		wxMenuBar* m_menubar1;
		wxMenu* fileMenu;
		wxMenu* exportMenu;
		wxMenu* viewMenu;
		wxMenu* validateMenu;
		wxStatusBar* m_statusBar1;

		// Virtual event handlers, overide them in your derived class
		virtual void OnResize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnChangePuzzleType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowLayer( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChangeBpc( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChangeBackgroundType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoadImage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportProtobuf( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToggleGrid( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnValidate( wxCommandEvent& event ) { event.Skip(); }


	public:

		PicrossGUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Picross Puzzle Exporter"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 960,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~PicrossGUI();

};

