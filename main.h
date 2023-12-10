#pragma once

#include "pch.h"
#include "main.h"
#include "JsonConfig.h"

class Program : public wxApp
{
public:
	wxWindow* MWindow;

    wxStaticText* T_ApplicationName;
    wxStaticText* T_AffiliationPath;

    wxStaticText* T_Radome;
    wxChoice* C_Radome;

    wxStaticText* T_Product;
    wxTextCtrl* TC_Product;

    wxStaticText* T_WorkCard;
    wxChoice* C_WorkCard;

    wxStaticText* T_Operator;
    wxChoice* C_Operator;

    wxStaticText* T_Post;
    wxChoice* C_Post;

    wxStaticText* T_Date;
    wxDatePickerCtrl* DP_Date;

    wxButton* B_Print;

    JsonConfig C_JsonConfig;

    eRadomeType RadomeType;
    std::wstring RadomeName;
    std::wstring RadomeDescName;

    bool OnInit() override
    {
        DebugConsole();

        auto* window = new wxFrame(nullptr, wxID_ANY, "Editeur PVC", wxDefaultPosition);
        window->SetSizeHints(wxSize(MwWindowSizeWidth, MwWindowSizeHeight), wxSize(MwWindowSizeWidth, MwWindowSizeHeight));
        window->CenterOnScreen();

        MWindow = this->GetTopWindow();

        this->BuildWindowContent();
        this->GetConfigFile();

        // DEBUG ONLY
        this->GetRadomeType( "604" );
        wxCommandEvent devPrint(wxEVT_COMMAND_BUTTON_CLICKED, -1);
        this->Callback_OnPrintButtonPressed(devPrint);
        // DEBUG END

        window->Show();
        return true;
    }

    void BuildWindowContent();
    void GetConfigFile();
    void GetRadomeType(const std::string& fileName);

    void Callback_OnRadomeChange( wxCommandEvent& event );
    void Callback_OnPrintButtonPressed(wxCommandEvent& event);

    static void DebugConsole();
};
