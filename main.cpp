#include "pch.h"
#include "main.h"

#include "PrintMirage.h"
#include "PrintRafale.h"

void Program::BuildWindowContent()
{
	const wxArrayString emptyArray( 1, "N/A" );

	this->T_ApplicationName = new wxStaticText( this->MWindow, wxID_ANY, wxT( "Editeur de PVC" ), wxPoint( 10, 10 ), wxSize( 300, 40 ), wxTEXT_ALIGNMENT_LEFT );
	this->T_ApplicationName->SetFont( GlobalFont );
	this->T_ApplicationName->SetForegroundColour( LabelColor );

	this->T_AffiliationPath = new wxStaticText( MWindow, wxID_ANY, wxT( "AEE / SIAE / AIACP - 2024" ), wxPoint( 936, 10 ), wxSize( 300, 40 ), wxTEXT_ALIGNMENT_RIGHT );
	this->T_AffiliationPath->SetFont( GlobalFont );
	this->T_AffiliationPath->SetForegroundColour( LabelColor );

	this->T_File = new wxStaticText( this->MWindow, wxID_ANY, wxT( "FICHIER CSV" ), wxPoint( 10, 60 ), LabelSize, wxTEXT_ALIGNMENT_LEFT );
	this->T_File->SetFont( GlobalFont );
	this->T_File->SetForegroundColour( LabelColor );

	this->C_File = new wxChoice( this->MWindow, wxID_ANY, wxPoint( 180, 56 ), ChoiceSizeLarge, emptyArray, 0 );
	this->C_File->SetFont( GlobalFont );

	this->T_Shell = new wxStaticText( this->MWindow, wxID_ANY, wxT( "COQUE N°" ), wxPoint( 740, 60 ), LabelSize, wxTEXT_ALIGNMENT_LEFT );
	this->T_Shell->SetFont( GlobalFont );
	this->T_Shell->SetForegroundColour( LabelColor );

	this->TC_Shell = new wxTextCtrl( this->MWindow, wxID_ANY, wxEmptyString, wxPoint( 910, 56 ), ChoiceSizeSmall, wxTE_PROCESS_ENTER );
	this->TC_Shell->SetFont( GlobalFont );

	this->T_Product = new wxStaticText( this->MWindow, wxID_ANY, wxT( "PRODUIT" ), wxPoint( 10, 110 ), LabelSize, wxTEXT_ALIGNMENT_LEFT );
	this->T_Product->SetFont( GlobalFont );
	this->T_Product->SetForegroundColour( LabelColor );

	this->C_Product = new wxChoice( this->MWindow, wxID_ANY, wxPoint( 180, 106 ), ChoiceSizeLarge, emptyArray, 0 );
	this->C_Product->SetFont( GlobalFont );

	this->T_Radome = new wxStaticText( this->MWindow, wxID_ANY, wxT( "RADOME N°" ), wxPoint( 740, 110 ), LabelSize, wxTEXT_ALIGNMENT_LEFT );
	this->T_Radome->SetFont( GlobalFont );
	this->T_Radome->SetForegroundColour( LabelColor );

	this->TC_Radome = new wxTextCtrl( this->MWindow, wxID_ANY, wxEmptyString, wxPoint( 910, 106 ), ChoiceSizeSmall, wxTE_PROCESS_ENTER );
	this->TC_Radome->SetFont( GlobalFont );

	this->T_WorkCard = new wxStaticText( this->MWindow, wxID_ANY, wxT( "GAMME" ), wxPoint( 10, 160 ), LabelSize, wxTEXT_ALIGNMENT_LEFT );
	this->T_WorkCard->SetFont( GlobalFont );
	this->T_WorkCard->SetForegroundColour( LabelColor );

	this->C_WorkCard = new wxChoice( this->MWindow, wxID_ANY, wxPoint( 180, 156 ), ChoiceSizeLarge, emptyArray, 0 );
	this->C_WorkCard->SetFont( GlobalFont );

	this->T_OperationCode = new wxStaticText( this->MWindow, wxID_ANY, wxT( "CODE OP." ), wxPoint( 740, 160 ), LabelSize, wxTEXT_ALIGNMENT_LEFT );
	this->T_OperationCode->SetFont( GlobalFont );
	this->T_OperationCode->SetForegroundColour( LabelColor );

	this->C_OperationCode = new wxChoice( this->MWindow, wxID_ANY, wxPoint( 910, 156 ), ChoiceSizeSmall, emptyArray, 0 );
	this->C_OperationCode->SetFont( GlobalFont );

	this->T_Operator = new wxStaticText( this->MWindow, wxID_ANY, wxT( "OPERATEUR" ), wxPoint( 10, 210 ), LabelSize, wxTEXT_ALIGNMENT_LEFT );
	this->T_Operator->SetFont( GlobalFont );
	this->T_Operator->SetForegroundColour( LabelColor );

	this->C_Operator = new wxChoice( this->MWindow, wxID_ANY, wxPoint( 180, 206 ), ChoiceSizeLarge, emptyArray, 0 );
	this->C_Operator->SetFont( GlobalFont );

	this->T_Post = new wxStaticText( this->MWindow, wxID_ANY, wxT( "POSTE" ), wxPoint( 740, 210 ), LabelSize, wxTEXT_ALIGNMENT_LEFT );
	this->T_Post->SetFont( GlobalFont );
	this->T_Post->SetForegroundColour( LabelColor );

	this->C_Post = new wxChoice( this->MWindow, wxID_ANY, wxPoint( 910, 206 ), ChoiceSizeSmall, emptyArray, 0 );
	this->C_Post->SetFont( GlobalFont );

	this->T_Date = new wxStaticText( this->MWindow, wxID_ANY, wxT( "DATE" ), wxPoint( 740, 260 ), wxSize( 80, 40 ), wxTEXT_ALIGNMENT_LEFT );
	this->T_Date->SetFont( GlobalFont );
	this->T_Date->SetForegroundColour( LabelColor );

	this->DP_Date = new wxDatePickerCtrl( this->MWindow, wxID_ANY, wxDefaultDateTime, wxPoint( 910, 256 ), ChoiceSizeSmall, wxDP_DEFAULT );
	this->DP_Date->SetFont( GlobalFont );

	this->B_Print = new wxButton( this->MWindow, wxID_ANY, "IMPRIMER", wxPoint( 10, 256 ), wxSize( 720, 40 ) );
	this->B_Print->SetFont( GlobalFont );
}

void Program::GetConfigFile()
{
	// Radome
	wxString csvFileName;
	wxString radomeName;
	wxString radomeNum;

	this->C_File->Clear();
	csvFileName.Clear();
	csvFileName = wxFindFirstFile( wxGetCwd() + "\\csv\\*.csv" );

	while ( !csvFileName.empty() )
	{
		radomeNum.Clear();
		radomeName.Clear();
		radomeName.Append( csvFileName.AfterLast( '\\' ) );
		radomeNum.Append( radomeName.BeforeFirst( '.' ) );

		this->C_File->Append( radomeNum );

		csvFileName = wxFindNextFile();
	}

	this->C_File->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Program::Callback_OnRadomeChange ), nullptr, this );
	wxCommandEvent radomeChangeInit( wxEVT_COMMAND_CHOICE_SELECTED, -1 );

	// Config Init
	this->C_JsonConfig = JsonConfig::LoadJsonConfig( ConfigPath );

	if ( this->C_File->GetCount() != 0 )
	{
		this->C_File->SetSelection( 0 );
		this->Callback_OnRadomeChange( radomeChangeInit );
	}

	// Print
	this->B_Print->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Program::Callback_OnPrintButtonPressed ), nullptr, this );
}

void Program::Callback_OnRadomeChange( wxCommandEvent& event )
{
	event.Skip();

	const auto fileName = static_cast < std::string >( this->C_File->GetString( C_File->GetCurrentSelection() ) );

	this->GetRadomeType( fileName );

	// Products
	this->C_Product->Clear();
	if ( !this->ActiveProductData.ProductNames.empty() )
	{
		for ( const std::string& product : this->ActiveProductData.ProductNames )
		{
			this->C_Product->Append( StringUtility::ReplaceWxString( StringUtility::StringToWString( product ), JsonLineSeparator, SpaceReplacer ) );
		}
	}
	else
	{
		this->C_Product->Append( "N/A" );
	}

	if ( this->C_Product->GetCount() != 0 )
	{
		this->C_Product->SetSelection( 0 );
	}

	// Work Cards
	this->C_WorkCard->Clear();
	if ( !this->ActiveProductData.WorkCards.empty() )
	{
		for ( const std::string& workCard : this->ActiveProductData.WorkCards )
		{
			this->C_WorkCard->Append( StringUtility::ReplaceWxString( workCard, JsonLineSeparator, SpaceReplacer ) );
		}
	}
	else
	{
		this->C_WorkCard->Append( "N/A" );
	}

	if ( this->C_WorkCard->GetCount() != 0 )
	{
		this->C_WorkCard->SetSelection( 0 );
	}

	// Operation Code
	this->C_OperationCode->Clear();
	if ( !this->ActiveProductData.OperationCodes.empty() )
	{
		for ( const std::string& workCard : this->ActiveProductData.OperationCodes )
		{
			this->C_OperationCode->Append( StringUtility::ReplaceWxString( workCard, JsonLineSeparator, SpaceReplacer ) );
		}
	}
	else
	{
		this->C_OperationCode->Append( "N/A" );
	}

	if ( this->C_OperationCode->GetCount() != 0 )
	{
		this->C_OperationCode->SetSelection( 0 );
	}

	// Operators
	this->C_Operator->Clear();
	for ( const std::string& operatorName : this->C_JsonConfig.Operators )
	{
		this->C_Operator->Append( StringUtility::ReplaceWxString( operatorName, JsonLineSeparator, SpaceReplacer ) );
	}

	if ( this->C_Operator->GetCount() != 0 )
	{
		this->C_Operator->SetSelection( 0 );
	}

	// Post
	this->C_Post->Clear();
	for ( const std::string& post : this->C_JsonConfig.Posts )
	{
		this->C_Post->Append( StringUtility::ReplaceWxString( post, JsonLineSeparator, SpaceReplacer ) );
	}

	if ( this->C_Post->GetCount() != 0 )
	{
		this->C_Post->SetSelection( 0 );
	}
}

void Program::Callback_OnPrintButtonPressed( wxCommandEvent& event )
{
	event.Skip();

	if ( ( this->ActiveProductData.RadomeType == Mirage_C || this->ActiveProductData.RadomeType == Rafale_C ) && this->TC_Radome->IsEmpty() )
	{
		wxMessageBox( "Entrez un numéro dans la case 'RADOME N°'", "N° Radome Invalide", wxOK | wxICON_WARNING );
		return;
	}

	if ( this->ActiveProductData.RadomeType == Rafale_C || this->ActiveProductData.RadomeType == Rafale_R )
	{
		PrintRafale::DrawRafalePage( this->MWindow );
	}
	else if ( this->ActiveProductData.RadomeType == Mirage_C || this->ActiveProductData.RadomeType == Mirage_R )
	{
		PrintMirage::DrawMiragePage( this->MWindow );
	}
}

void Program::GetRadomeType( const std::string& fileName )
{
	const std::string filePath = ExecutivePath + "\\csv\\" + fileName + ".csv";
	std::wifstream file( filePath );
	std::wstring w_line;
	int count = 0;
	while ( std::getline( file, w_line ) && count < 4 )
	{
		std::string line = StringUtility::WideStringToString( w_line );
		for ( const ProductData& productData : this->C_JsonConfig.Products )
		{
			if ( line.find( productData.ProgramIdx ) != std::string::npos )
			{
				this->ActiveProductData = productData;
				this->RadomeName        = StringUtility::KeepOnlyDigits( StringUtility::StringToWString( fileName ) );
				this->Calculation       = CalculationData( this, filePath );

				if ( productData.RadomeType == Mirage_C || productData.RadomeType == Rafale_C )
				{
					this->TC_Shell->SetValue( this->RadomeName );
					this->TC_Shell->SetEditable( true );
					this->TC_Radome->SetValue( "" );
				}
				else if ( productData.RadomeType == Mirage_R || productData.RadomeType == Rafale_R )
				{
					this->TC_Shell->SetValue( "Sans Objet" );
					this->TC_Shell->SetEditable( false );
					this->TC_Radome->SetValue( this->RadomeName );
				}

				return;
			}
		}

		++count;
	}
}

void Program::DebugConsole()
{
#ifdef _DEBUG
	AllocConsole();

	// Rediriger les flux standard vers la console
	FILE*fDummy;
	freopen_s( &fDummy, "CONOUT$", "w", stdout );
	freopen_s( &fDummy, "CONOUT$", "w", stderr );
	freopen_s( &fDummy, "CONIN$", "r", stdin );

	// Utiliser les flux de la bibliothèque C++ (std::cin, std::cout, std::cerr)
	std::cout.clear();
	std::cin.clear();
	std::cerr.clear();
	std::wcout.clear();
	std::wcin.clear();
	std::wcerr.clear();

	// Exemple d'écriture dans la console
	std::cout << "<< Console init" << std::endl;
#endif
}


wxIMPLEMENT_APP( Program );
