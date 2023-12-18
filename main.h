#pragma once

#include "pch.h"
#include "main.h"
#include "JsonConfig.h"
#include "CalculationData.h"

class Program : public wxApp
{
public:
	wxWindow* MWindow;

	wxStaticText* T_ApplicationName;
	wxStaticText* T_AffiliationPath;

	wxStaticText* T_File;
	wxChoice* C_File;

	wxStaticText* T_Product;
	wxChoice* C_Product;

	wxStaticText* T_Shell;
	wxTextCtrl* TC_Shell;

	wxStaticText* T_Radome;
	wxTextCtrl* TC_Radome;

	wxStaticText* T_WorkCard;
	wxChoice* C_WorkCard;

	wxStaticText* T_OperationCode;
	wxChoice* C_OperationCode;

	wxStaticText* T_Operator;
	wxChoice* C_Operator;

	wxStaticText* T_Post;
	wxChoice* C_Post;

	wxStaticText* T_Mounting;
	wxChoice* C_Mounting;

	wxStaticText* T_Date;
	wxDatePickerCtrl* DP_Date;

	wxButton* B_Print;

	JsonConfig C_JsonConfig;

	std::wstring RadomeName;

	ProductData ActiveProductData;

	CalculationData Calculation;

	bool OnInit() override
	{
		DebugConsole();

		auto* window = new wxFrame( nullptr, wxID_ANY, "Editeur PVC", wxDefaultPosition, MwWindowSize );
		window->SetSizeHints( MwWindowSize, MwWindowSize );
		window->SetBackgroundColour( MainColor );
		window->EnableMaximizeButton( false );

		MWindow = this->GetTopWindow();

		this->BuildWindowContent();
		this->GetConfigFile();

		// DEBUG ONLY
		//this->GetRadomeType("3252C");
		//this->TC_Radome->SetValue("3252C");
		this->GetRadomeType( "C475" );
		this->TC_Radome->SetValue( "C475" );
		wxCommandEvent devPrint( wxEVT_COMMAND_BUTTON_CLICKED, -1 );
		this->Callback_OnPrintButtonPressed( devPrint );
		// DEBUG END

		window->CenterOnScreen();
		window->Show();
		return true;
	}

	void BuildWindowContent();
	void GetConfigFile();
	void GetRadomeType( const std::string& fileName );

	void Callback_OnRadomeChange( wxCommandEvent& event );
	void Callback_OnPrintButtonPressed( wxCommandEvent& event );

	static void DebugConsole();
};
