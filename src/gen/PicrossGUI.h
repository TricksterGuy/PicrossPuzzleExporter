///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 23 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PICROSSGUI_H__
#define __PICROSSGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class PicrossDataCanvas;

#include <wx/scrolwin.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/radiobox.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


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
		wxRadioBox* m_radioBox2;
		wxChoice* layersChoice;
		wxCheckBox* showOnlyLayer;
		wxChoice* bitsPerCell;
		wxButton* m_button1;
		wxButton* m_button3;
		wxButton* m_button2;
		wxButton* m_button4;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnResize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnChangeType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowLayer( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChangeBpc( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoadImage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnValidate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportImage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQRCode( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		PicrossGUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,480 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~PicrossGUI();
	
};

#endif //__PICROSSGUI_H__
