///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
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
	contents = new wxBoxSizer( wxHORIZONTAL );

	puzzleDataCanvas = new PicrossDataCanvas( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	puzzleDataCanvas->SetScrollRate( 5, 5 );
	contents->Add( puzzleDataCanvas, 1, wxALL|wxEXPAND, 0 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("Meta") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	name = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( name, 0, wxALL|wxALIGN_RIGHT|wxEXPAND, 5 );

	m_staticText11 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Author:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	author = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( author, 0, wxALL|wxALIGN_RIGHT|wxEXPAND, 5 );


	sbSizer2->Add( fgSizer1, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Solution Info") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText7 = new wxStaticText( sbSizer5->GetStaticBox(), wxID_ANY, wxT("Image:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer4->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	solutionImage = new wxFilePickerCtrl( sbSizer5->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_SMALL );
	fgSizer4->Add( solutionImage, 0, wxALL, 5 );

	m_staticText111 = new wxStaticText( sbSizer5->GetStaticBox(), wxID_ANY, wxT("Frames:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	fgSizer4->Add( m_staticText111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	solutionFrames = new wxSpinCtrl( sbSizer5->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 50, 1 );
	solutionFrames->Enable( false );

	fgSizer4->Add( solutionFrames, 0, wxALL, 5 );


	sbSizer5->Add( fgSizer4, 1, wxEXPAND, 5 );


	sbSizer2->Add( sbSizer5, 1, wxEXPAND, 5 );


	bSizer8->Add( sbSizer2, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("Puzzle Properties") ), wxHORIZONTAL );

	wxString puzzleTypeChoices[] = { wxT("Classic"), wxT("Grayscale"), wxT("Colors of Light"), wxT("Painting"), wxT("Color Hints") };
	int puzzleTypeNChoices = sizeof( puzzleTypeChoices ) / sizeof( wxString );
	puzzleType = new wxRadioBox( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, puzzleTypeNChoices, puzzleTypeChoices, 1, wxRA_SPECIFY_COLS );
	puzzleType->SetSelection( 0 );
	sbSizer3->Add( puzzleType, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText2 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer2->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	puzzleTime = new wxSpinCtrl( sbSizer3->GetStaticBox(), wxID_ANY, wxT("300"), wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, -1, 6039, 300 );
	fgSizer2->Add( puzzleTime, 0, wxALL|wxEXPAND, 5 );

	layerLabel = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Layer:"), wxDefaultPosition, wxDefaultSize, 0 );
	layerLabel->Wrap( -1 );
	layerLabel->Hide();

	fgSizer2->Add( layerLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString layersChoiceChoices;
	layersChoice = new wxChoice( sbSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, layersChoiceChoices, 0 );
	layersChoice->SetSelection( 0 );
	layersChoice->Hide();

	fgSizer2->Add( layersChoice, 0, wxALL|wxEXPAND, 5 );

	showCurrentLayerLabel = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Show Current Layer:"), wxDefaultPosition, wxDefaultSize, 0 );
	showCurrentLayerLabel->Wrap( -1 );
	showCurrentLayerLabel->Hide();

	fgSizer2->Add( showCurrentLayerLabel, 0, wxALL, 5 );

	showOnlyLayer = new wxCheckBox( sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	showOnlyLayer->Hide();
	showOnlyLayer->SetToolTip( wxT("Show layers output or full colored output") );

	fgSizer2->Add( showOnlyLayer, 0, wxALL|wxALIGN_RIGHT|wxEXPAND, 5 );

	bitsPerCellLabel = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Cell bits:"), wxDefaultPosition, wxDefaultSize, 0 );
	bitsPerCellLabel->Wrap( -1 );
	bitsPerCellLabel->Hide();

	fgSizer2->Add( bitsPerCellLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString bitsPerCellChoices[] = { wxT("1 Bit (2 colors)"), wxT("2 Bits (4 colors)"), wxT("3 Bits (8 colors)") };
	int bitsPerCellNChoices = sizeof( bitsPerCellChoices ) / sizeof( wxString );
	bitsPerCell = new wxChoice( sbSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, bitsPerCellNChoices, bitsPerCellChoices, 0 );
	bitsPerCell->SetSelection( 0 );
	bitsPerCell->Hide();

	fgSizer2->Add( bitsPerCell, 0, wxALL|wxEXPAND, 5 );

	colorsLabel = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Colors:"), wxDefaultPosition, wxDefaultSize, 0 );
	colorsLabel->Wrap( -1 );
	colorsLabel->Hide();

	fgSizer2->Add( colorsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	colors = new wxSpinCtrl( sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, 32, 32 );
	colors->Hide();

	fgSizer2->Add( colors, 0, wxALL, 5 );


	sbSizer3->Add( fgSizer2, 1, wxEXPAND, 5 );


	bSizer8->Add( sbSizer3, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* m_staticBoxSizer;
	m_staticBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("Background") ), wxHORIZONTAL );

	wxString backgroundTypeChoices[] = { wxT("Stationary"), wxT("Stretched"), wxT("Tiled"), wxT("Gradient") };
	int backgroundTypeNChoices = sizeof( backgroundTypeChoices ) / sizeof( wxString );
	backgroundType = new wxRadioBox( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, backgroundTypeNChoices, backgroundTypeChoices, 1, wxRA_SPECIFY_COLS );
	backgroundType->SetSelection( 0 );
	m_staticBoxSizer->Add( backgroundType, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	backgroundImageLabel = new wxStaticText( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("Image:"), wxDefaultPosition, wxDefaultSize, 0 );
	backgroundImageLabel->Wrap( -1 );
	fgSizer3->Add( backgroundImageLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	backgroundImage = new wxFilePickerCtrl( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_SMALL|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( backgroundImage, 0, wxALL, 5 );

	topColorLabel = new wxStaticText( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("Color 1 (Top)"), wxDefaultPosition, wxDefaultSize, 0 );
	topColorLabel->Wrap( -1 );
	topColorLabel->Hide();

	fgSizer3->Add( topColorLabel, 0, wxALL, 5 );

	topColor = new wxColourPickerCtrl( m_staticBoxSizer->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_SHOW_LABEL );
	topColor->Hide();

	fgSizer3->Add( topColor, 0, wxALL|wxEXPAND, 5 );

	bottomColorLabel = new wxStaticText( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("Color 2 (Bottom)"), wxDefaultPosition, wxDefaultSize, 0 );
	bottomColorLabel->Wrap( -1 );
	bottomColorLabel->Hide();

	fgSizer3->Add( bottomColorLabel, 0, wxALL, 5 );

	bottomColor = new wxColourPickerCtrl( m_staticBoxSizer->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_SHOW_LABEL );
	bottomColor->Hide();

	fgSizer3->Add( bottomColor, 0, wxALL|wxEXPAND, 5 );

	m_staticText12 = new wxStaticText( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("Music:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer3->Add( m_staticText12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	music = new wxFilePickerCtrl( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	fgSizer3->Add( music, 0, wxALL, 5 );


	m_staticBoxSizer->Add( fgSizer3, 1, wxEXPAND, 5 );


	bSizer8->Add( m_staticBoxSizer, 0, wxEXPAND, 5 );


	contents->Add( bSizer8, 0, wxEXPAND, 5 );


	m_panel4->SetSizer( contents );
	m_panel4->Layout();
	contents->Fit( m_panel4 );
	bSizer1->Add( m_panel4, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* loadMenuItem;
	loadMenuItem = new wxMenuItem( fileMenu, wxID_LOAD, wxString( wxT("Load") ) + wxT('\t') + wxT("Ctrl+O"), wxT("Load an image and convert into a puzzle."), wxITEM_NORMAL );
	fileMenu->Append( loadMenuItem );

	exportMenu = new wxMenu();
	wxMenuItem* exportMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxT("Export"), wxEmptyString, wxITEM_NORMAL, exportMenu );
	wxMenuItem* protobufMenuItem;
	protobufMenuItem = new wxMenuItem( exportMenu, ID_PROTOBUF, wxString( wxT("Protobuf") ) , wxT("Export puzzle to a protobuf."), wxITEM_NORMAL );
	exportMenu->Append( protobufMenuItem );

	fileMenu->Append( exportMenuItem );

	wxMenuItem* quitMenuItem;
	quitMenuItem = new wxMenuItem( fileMenu, wxID_QUIT, wxString( wxT("Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quits the program"), wxITEM_NORMAL );
	fileMenu->Append( quitMenuItem );

	m_menubar1->Append( fileMenu, wxT("File") );

	viewMenu = new wxMenu();
	wxMenuItem* showGridMenuItem;
	showGridMenuItem = new wxMenuItem( viewMenu, ID_SHOW_GRID, wxString( wxT("Show Grid") ) + wxT('\t') + wxT("Ctrl+G"), wxT("Toggles showing a grid."), wxITEM_CHECK );
	viewMenu->Append( showGridMenuItem );

	m_menubar1->Append( viewMenu, wxT("View") );

	validateMenu = new wxMenu();
	wxMenuItem* validateMenuItem;
	validateMenuItem = new wxMenuItem( validateMenu, ID_VALIDATE, wxString( wxT("Validate") ) + wxT('\t') + wxT("Ctrl+V"), wxT("Validates that the puzzle has a unique solution."), wxITEM_NORMAL );
	validateMenu->Append( validateMenuItem );

	m_menubar1->Append( validateMenu, wxT("Validate") );

	this->SetMenuBar( m_menubar1 );

	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PicrossGUI::OnResize ) );
	solutionImage->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( PicrossGUI::OnSolutionImageChanged ), NULL, this );
	puzzleType->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangePuzzleType ), NULL, this );
	layersChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PicrossGUI::OnLayerChange ), NULL, this );
	showOnlyLayer->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PicrossGUI::OnShowLayer ), NULL, this );
	bitsPerCell->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangeBpc ), NULL, this );
	colors->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( PicrossGUI::OnChangeNumColors ), NULL, this );
	backgroundType->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangeBackgroundType ), NULL, this );
	fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PicrossGUI::OnLoadImage ), this, loadMenuItem->GetId());
	exportMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PicrossGUI::OnExportProtobuf ), this, protobufMenuItem->GetId());
	fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PicrossGUI::OnQuit ), this, quitMenuItem->GetId());
	viewMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PicrossGUI::OnToggleGrid ), this, showGridMenuItem->GetId());
	validateMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PicrossGUI::OnValidate ), this, validateMenuItem->GetId());
}

PicrossGUI::~PicrossGUI()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PicrossGUI::OnResize ) );
	solutionImage->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( PicrossGUI::OnSolutionImageChanged ), NULL, this );
	puzzleType->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangePuzzleType ), NULL, this );
	layersChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PicrossGUI::OnLayerChange ), NULL, this );
	showOnlyLayer->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PicrossGUI::OnShowLayer ), NULL, this );
	bitsPerCell->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangeBpc ), NULL, this );
	colors->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( PicrossGUI::OnChangeNumColors ), NULL, this );
	backgroundType->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( PicrossGUI::OnChangeBackgroundType ), NULL, this );

}
