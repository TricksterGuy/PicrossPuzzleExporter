///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "QRCodePanel.hpp"

#include "QRCodeDialog.hpp"

///////////////////////////////////////////////////////////////////////////

QRCodeDialog::QRCodeDialog( QRcode* qr, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( 320, 320 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_scrolledWindow3 = new QRCodePanel( qr, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow3->SetScrollRate( 5, 5 );
	bSizer7->Add( m_scrolledWindow3, 1, wxEXPAND | wxALL, 5 );
	SetClientSize(m_scrolledWindow3->GetSize());

	this->SetSizer( bSizer7 );
	this->Layout();

	this->Centre( wxBOTH );
}

QRCodeDialog::~QRCodeDialog()
{
}
