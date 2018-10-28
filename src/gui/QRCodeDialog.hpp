#ifndef __QRCODEDIALOG_H__
#define __QRCODEDIALOG_H__

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

class QRCodeDialog : public wxDialog
{
	public:
		QRCodeDialog(QRcode* qr, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxT("QR Code"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~QRCodeDialog();
	protected:
		QRCodePanel* m_scrolledWindow3;
};

#endif
