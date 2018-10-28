///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 23 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PicrossDataCanvas.hpp"

#include "PicrossGUI.h"

///////////////////////////////////////////////////////////////////////////

PicrossGUI::PicrossGUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	contents = new wxBoxSizer( wxHORIZONTAL );
	
	puzzleDataCanvas = new PicrossDataCanvas( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	puzzleDataCanvas->SetScrollRate( 5, 5 );
	contents->Add( puzzleDataCanvas, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_radioBox2Choices[] = { wxT("Black"), wxT("Grayscale"), wxT("RGB"), wxT("RBY-BW") };
	int m_radioBox2NChoices = sizeof( m_radioBox2Choices ) / sizeof( wxString );
	m_radioBox2 = new wxRadioBox( m_panel4, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, m_radioBox2NChoices, m_radioBox2Choices, 1, wxRA_SPECIFY_COLS );
	m_radioBox2->SetSelection( 0 );
	bSizer8->Add( m_radioBox2, 0, wxALL|wxEXPAND, 5 );
	
	wxArrayString layersChoiceChoices;
	layersChoice = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, layersChoiceChoices, 0 );
	layersChoice->SetSelection( 0 );
	layersChoice->Hide();
	
	bSizer8->Add( layersChoice, 0, wxALL, 5 );
	
	showOnlyLayer = new wxCheckBox( m_panel4, wxID_ANY, wxT("Show Layer"), wxDefaultPosition, wxDefaultSize, 0 );
	showOnlyLayer->Hide();
	showOnlyLayer->SetToolTip( wxT("Show layers output or full colored output") );
	
	bSizer8->Add( showOnlyLayer, 0, wxALL, 5 );
	
	wxString bitsPerCellChoices[] = { wxT("1 Bit (2 colors)"), wxT("2 Bits (4 colors)"), wxT("3 Bits (8 colors)") };
	int bitsPerCellNChoices = sizeof( bitsPerCellChoices ) / sizeof( wxString );
	bitsPerCell = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, bitsPerCellNChoices, bitsPerCellChoices, 0 );
	bitsPerCell->SetSelection( 0 );
	bitsPerCell->Hide();
	
	bSizer8->Add( bitsPerCell, 0, wxALL, 5 );
	
	
	contents->Add( bSizer8, 0, wxEXPAND, 5 );
	
	
	bSizer3->Add( contents, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button1 = new wxButton( m_panel4, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button1, 0, wxALL, 5 );
	
	m_button3 = new wxButton( m_panel4, wxID_ANY, wxT("Validate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button3, 0, wxALL, 5 );
	
	m_button2 = new wxButton( m_panel4, wxID_ANY, wxT("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button2, 0, wxALL, 5 );
	
	m_button4 = new wxButton( m_panel4, wxID_ANY, wxT("QR Code"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button4, 0, wxALL, 5 );
	
	
	bSizer3->Add( bSizer6, 0, wxEXPAND|wxALIGN_RIGHT, 0 );
	
	
	m_panel4->SetSizer( bSizer3 );
	m_panel4->Layout();
	bSizer3->Fit( m_panel4 );
	bSizer1->Add( m_panel4, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PicrossGUI::OnResize ) );
	m_radioBox2->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangeType ), NULL, this );
	layersChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PicrossGUI::OnLayerChange ), NULL, this );
	showOnlyLayer->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PicrossGUI::OnShowLayer ), NULL, this );
	bitsPerCell->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangeBpc ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PicrossGUI::OnLoadImage ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PicrossGUI::OnValidate ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PicrossGUI::OnExportImage ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PicrossGUI::OnQRCode ), NULL, this );
}

PicrossGUI::~PicrossGUI()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PicrossGUI::OnResize ) );
	m_radioBox2->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangeType ), NULL, this );
	layersChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PicrossGUI::OnLayerChange ), NULL, this );
	showOnlyLayer->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PicrossGUI::OnShowLayer ), NULL, this );
	bitsPerCell->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangeBpc ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PicrossGUI::OnLoadImage ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PicrossGUI::OnValidate ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PicrossGUI::OnExportImage ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PicrossGUI::OnQRCode ), NULL, this );
	
}
