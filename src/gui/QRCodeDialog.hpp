///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class QRCodePanel;

#include <wx/scrolwin.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <qrencode.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class QRCodeDialog
///////////////////////////////////////////////////////////////////////////////
class QRCodeDialog : public wxDialog
{
	private:

	protected:
		QRCodePanel* m_scrolledWindow3;

	public:

		QRCodeDialog( QRcode* qr, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxT("QR Code"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 320,320 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER );
		~QRCodeDialog();

};

