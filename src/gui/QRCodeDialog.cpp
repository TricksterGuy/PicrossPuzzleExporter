#include "QRCodePanel.hpp"

#include "QRCodeDialog.hpp"

QRCodeDialog::QRCodeDialog( QRcode* qr, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( 320, 320);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_scrolledWindow3 = new QRCodePanel( qr, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	bSizer7->Add( m_scrolledWindow3, 1, wxEXPAND | wxALL, 5 );
	SetClientSize(m_scrolledWindow3->GetSize());

	this->SetSizer( bSizer7 );
	this->Layout();
	bSizer7->Fit( this );

	this->Centre( wxBOTH );
}

QRCodeDialog::~QRCodeDialog()
{
}
