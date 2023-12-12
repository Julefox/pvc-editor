
#include "pch.h"
#include "main.h"

#include "PrintMirage.h"
#include "PrintRafale.h"

void Program::BuildWindowContent()
{
	this->T_ApplicationName = new wxStaticText(this->MWindow, wxID_ANY, wxT("Editeur de PVC"), wxPoint(10, 10), LabelSize, wxTEXT_ALIGNMENT_LEFT);
	this->T_ApplicationName->SetFont( GlobalFont );
	this->T_ApplicationName->SetForegroundColour(LabelColor);

	this->T_AffiliationPath = new wxStaticText(MWindow, wxID_ANY, wxT("AEE / SIAE / AIACP - 2024"), wxPoint(474, 10), wxSize(300, 40), wxTEXT_ALIGNMENT_RIGHT );
	this->T_AffiliationPath->SetFont( GlobalFont );
	this->T_AffiliationPath->SetForegroundColour(LabelColor);

	this->T_Radome = new wxStaticText(this->MWindow, wxID_ANY, wxT("S/N RADOME"), wxPoint(10, 60), LabelSize, wxTEXT_ALIGNMENT_LEFT);
	this->T_Radome->SetFont( GlobalFont );
	this->T_Radome->SetForegroundColour(LabelColor);

	const wxArrayString radomeArray;
	this->C_Radome = new wxChoice( this->MWindow, wxID_ANY, wxPoint(220, 56), ChoiceSize, radomeArray, 0 );
	this->C_Radome->SetFont( GlobalFont );

	this->T_Product = new wxStaticText(this->MWindow, wxID_ANY, wxT("PRODUIT"), wxPoint(10, 110), LabelSize, wxTEXT_ALIGNMENT_LEFT);
	this->T_Product->SetFont( GlobalFont );
	this->T_Product->SetForegroundColour(LabelColor);

	this->TC_Product = new wxTextCtrl(this->MWindow, wxID_ANY, wxEmptyString, wxPoint(220, 106), ChoiceSize, wxTE_READONLY);
	this->TC_Product->SetFont( GlobalFont );

	this->T_WorkCard = new wxStaticText(this->MWindow, wxID_ANY, wxT("GAMME"), wxPoint(10, 160), LabelSize, wxTEXT_ALIGNMENT_LEFT);
	this->T_WorkCard->SetFont(GlobalFont);
	this->T_WorkCard->SetForegroundColour(LabelColor);

	const wxArrayString workCardArray;
	this->C_WorkCard = new wxChoice(this->MWindow, wxID_ANY, wxPoint(220, 154), ChoiceSize, workCardArray, 0);
	this->C_WorkCard->SetFont(GlobalFont);

	this->T_Operator = new wxStaticText(this->MWindow, wxID_ANY, wxT("OPERATEUR"), wxPoint(10, 210), LabelSize, wxTEXT_ALIGNMENT_LEFT);
	this->T_Operator->SetFont(GlobalFont);
	this->T_Operator->SetForegroundColour(LabelColor);

	const wxArrayString operatorArray;
	this->C_Operator = new wxChoice(this->MWindow, wxID_ANY, wxPoint(220, 206), ChoiceSize, operatorArray, 0);
	this->C_Operator->SetFont(GlobalFont);

	this->T_Post = new wxStaticText(this->MWindow, wxID_ANY, wxT("POSTE"), wxPoint(10, 260), LabelSize, wxTEXT_ALIGNMENT_LEFT);
	this->T_Post->SetFont(GlobalFont);
	this->T_Post->SetForegroundColour(LabelColor);

	const wxArrayString postArray;
	this->C_Post = new wxChoice(this->MWindow, wxID_ANY, wxPoint(220, 256), wxSize(310, 40), postArray, 0);
	this->C_Post->SetFont(GlobalFont);

	this->T_Date = new wxStaticText(this->MWindow, wxID_ANY, wxT("DATE"), wxPoint(540, 260), wxSize(80, 40), wxTEXT_ALIGNMENT_LEFT);
	this->T_Date->SetFont(GlobalFont);

	this->DP_Date = new wxDatePickerCtrl(this->MWindow, wxID_ANY, wxDefaultDateTime, wxPoint(620, 254), wxSize(150, 40), wxDP_DEFAULT);
	this->DP_Date->SetFont(GlobalFont);

	this->B_Print = new wxButton(this->MWindow, wxID_ANY, "IMPRIMER", wxPoint(10, 310), wxSize(760, 50));
	this->B_Print->SetFont(GlobalFont);
}

void Program::GetConfigFile()
{
	// Radome
	wxString csvFileName;
	wxString radomeName;
	wxString radomeNum;

	this->C_Radome->Clear();
	csvFileName.Clear();
	csvFileName = wxFindFirstFile(wxGetCwd() + "\\csv\\*.csv" );

	while (!csvFileName.empty())
	{
		radomeNum.Clear();
		radomeName.Clear();
		radomeName.Append( csvFileName.AfterLast( '\\' ) );
		radomeNum.Append( radomeName.BeforeFirst( '.' ) );

		this->C_Radome->Append( radomeNum );

		csvFileName = wxFindNextFile();
	}

	this->C_Radome->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Program::Callback_OnRadomeChange ), nullptr, this);
	wxCommandEvent radomeChangeInit( wxEVT_COMMAND_CHOICE_SELECTED, -1 );

	// Config Init
	this->C_JsonConfig = JsonConfig::LoadJsonConfig(ConfigPath);

	// Radome but need init first
	if (this->C_Radome->GetCount() != 0)
	{
		this->C_Radome->SetSelection(0);
		this->Callback_OnRadomeChange( radomeChangeInit );
	}

	// Work Cards
	for ( const std::string& workCard : this->C_JsonConfig.WorkCards )
	{
		this->C_WorkCard->Append( StringUtility::ReplaceWxString(workCard, JsonLineSeparator, SpaceReplacer) );
	}

	if ( this->C_WorkCard->GetCount() != 0 )
	{
		this->C_WorkCard->SetSelection( 0 );
	}

	// Operators
	for ( const std::string& operatorName : this->C_JsonConfig.Operators )
	{
		this->C_Operator->Append( StringUtility::ReplaceWxString(operatorName, JsonLineSeparator, SpaceReplacer) );
	}

	if ( this->C_Operator->GetCount() != 0 )
	{
		this->C_Operator->SetSelection( 0 );
	}

	// Post
	for ( const std::string& post : this->C_JsonConfig.Posts )
	{
		this->C_Post->Append( StringUtility::ReplaceWxString( post, JsonLineSeparator, SpaceReplacer) );
	}

	if ( this->C_Post->GetCount() != 0 )
	{
		this->C_Post->SetSelection( 0 );
	}

	// Print
	this->B_Print->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Program::Callback_OnPrintButtonPressed), nullptr, this);
}

void Program::Callback_OnRadomeChange( wxCommandEvent& event )
{
	event.Skip();

	const auto fileName = (std::string)this->C_Radome->GetString( C_Radome->GetCurrentSelection() );

	this->GetRadomeType( fileName );
	this->TC_Product->SetLabelText( this->RadomeDescName );
}

void Program::Callback_OnPrintButtonPressed( wxCommandEvent& event )
{
	event.Skip();

	if ( this->ActiveProductData.RadomeType == Rafale_C || this->ActiveProductData.RadomeType == Rafale_R )
	{
		PrintRafale::DrawRafalePage( this->MWindow );
	}
	else if ( this->ActiveProductData.RadomeType == Mirage_C || this->ActiveProductData.RadomeType == Mirage_R )
	{
		PrintMirage::DrawMiragePage( this->MWindow );
	}
}

void Program::GetRadomeType(const std::string& fileName)
{
	const std::string filePath = ExecutivePath + "\\csv\\" + fileName + ".csv";
	std::wifstream file(filePath);
	std::wstring w_line;
	int count = 0;
	while (std::getline(file, w_line) && count < 4)
	{
		std::string line = StringUtility::WideStringToString(w_line);
		for (const ProductData& productData : this->C_JsonConfig.Products)
		{
			if (line.find(productData.ProgramIdx) != std::string::npos)
			{
				this->ActiveProductData = productData;
				this->RadomeName = StringUtility::StringToWString(fileName);
				this->RadomeDescName = StringUtility::StringToWString(productData.NameDescription);
				this->Calculation = CalculationData(this, filePath);
				return;
			}
		}

		++count;
	}

	this->RadomeDescName = L"...";
}

void Program::DebugConsole()
{
#ifdef _DEBUG
	AllocConsole();

	// Rediriger les flux standard vers la console
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);

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


wxIMPLEMENT_APP(Program);
