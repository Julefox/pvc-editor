
#include "PrintRafale.h"

#include "main.h"

void PrintRafale::DrawRafalePage( wxWindow* hInst )
{
    wxPrintDialogData g_printDialogData;
    wxPrintData g_printData;
    g_printData.SetOrientation( wxLANDSCAPE );
    g_printDialogData.SetPrintData( g_printData );

    auto* preview = new wxPrintPreview( new PrintRafale, new PrintRafale, &g_printDialogData );

    preview->SetZoom( 100 );

    if ( !preview->Ok() )
    {
        delete preview;
        wxMessageBox(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"), wxT("Previewing"), wxOK);
        return;
    }

    auto* frame = new wxPreviewFrame( preview, hInst, wxT("Impression"), wxPoint(-1, -1), wxSize(1280, 880), wxDEFAULT_FRAME_STYLE | wxFRAME_SHAPED);
    frame->Centre( wxBOTH );
    frame->Initialize();
    frame->Show( true );
}

void PrintRafale::RafalePage_01()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale( this, dc );
    Rafale_DrawMainPv(hInst, dc, 1);

    // Cadre Principal
    dc->DrawLine(40, 158, 1080, 158);   // Haut
    dc->DrawLine(40, 680, 1080, 680);   // Bas
    dc->DrawLine(40, 158, 40, 680);     // Gauche
    dc->DrawLine(1080, 158, 1080, 680); // Droit

    dc->DrawLine(40, 190, 1080, 190);   // Séparateur Titre
    dc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("Contrôle profil (Ecart entre rayon théorique et rayon mesuré)", wxRect(40, 158, 1040, 32), wxALIGN_CENTER);

    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    constexpr int case_h_small = 19 /* Doit etre 18.85f */, case_h_large = 37 /* Doit etre 37.69f */, case_w = 80, x_start = 40, y_start = 190;

    const std::vector<wxString> headers = { "Section", "Haut./Pointe", "Rayon", "Tol.", "H + 2°", "HD", "D", "BD", "B + 2°", "BG", "G", "HG", "Haut./Base" };

    for (int i = 0; i < static_cast < int >(headers.size()); i++)
    {
        dc->DrawLabel(headers[i], wxRect(x_start + case_w * i, y_start, case_w, case_h_small), wxALIGN_CENTER);
    }

    // Pour certaines raisons, dessiner TOUTE les lignes avant les valeurs
    float x = x_start, y = y_start;
    for (int i = 1; i <= 21; i++)
    {
        dc->DrawLine(40, y, 1080, y);
    
        if (i <= 13)
        {
            x += case_w;
            dc->DrawLine(x, 190, x, 680);
        }
    }

    x = x_start, y = y_start;
    for (int i = 1; i <= 21; i++)
    {
        dc->DrawLine(40, y, 1080, y);

        if (i <= 13)
        {
            x += case_w;
            dc->DrawLine(x, 190, x, 680);
        }

        // Hauteur Radome // Petit hack pour trouver les valeurs qui ne sont pas trouver (en particulier la 20e ligne)
        int measure_height_up = 0;   // 2e Colonne
        int measure_height_down = 0; // 13e Colonne
        {
            for (int idx = 0; idx < 8; idx++)
            {
                if (std::abs(hInst->Calculation.Height[i][idx] - UnassignedDoubleValue) > Epsilon)
                {
                    measure_height_up = 2114 - hInst->Calculation.Height[i][idx];
                    measure_height_down = hInst->Calculation.Height[i][idx];
                    break;
                }
            }
        }

        // Rayon Théorique
        double theoretical_radius = 0.0f;
        {
            double sum = 0.0f;

            for (const double& rad : hInst->ActiveProductData.TheoreticalRadius[i])
            {
                sum += rad;
            }

            theoretical_radius = sum / std::size(hInst->ActiveProductData.TheoreticalRadius[i]);
        }

        if(i == 16)
        {
            y += case_h_small;
            dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h_large), wxALIGN_CENTER); // 1e Colonne
            dc->DrawLabel(wxString::Format("%d", measure_height_up), wxRect(x_start + case_w * 1, y, case_w, case_h_large), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel(wxString::Format("%.2f", theoretical_radius), wxRect(x_start + case_w * 2, y, case_w, case_h_large), wxALIGN_CENTER); // 3e Colonne
            dc->DrawLabel(wxString::Format("+/- %.1f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_large), wxALIGN_CENTER); // 4e Colonne
            dc->DrawLabel(std::to_wstring(measure_height_down), wxRect(x_start + case_w * 12, y, case_w, case_h_large), wxALIGN_CENTER); // 13e Colonne

            // Ecart Rayon
            {
                for (int j = 0; j < 8; j++)
                {
                    dc->SetTextForeground(BlackColor);
                    dc->DrawLabel(wxString::Format("%.2f", hInst->ActiveProductData.TheoreticalRadius[i][j]), wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small), wxALIGN_CENTER);

                    const double rayDiff = hInst->Calculation.RayDifference[i][j];

                    if (std::abs(rayDiff - UnassignedDoubleValue) > Epsilon)
                    {
                        SetToleranceColor(dc, rayDiff, hInst->ActiveProductData.RadiusTolerance);
                        dc->DrawLabel(wxString::Format("%.2f", rayDiff), wxRect(x_start + case_w * (j + 4), y + case_h_small, case_w, case_h_small), wxALIGN_CENTER);
                    }
                    else
                    {
                        dc->GradientFillLinear(wxRect(x_start + case_w * (j + 4), y + case_h_small, case_w, case_h_small + 1), GrayColor, GrayColor);
                    }
                }

                dc->SetTextForeground(BlackColor);
            }
        }
        else if (i < 17)
        {
            y += case_h_small;
            dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h_small), wxALIGN_CENTER); // 1e Colonne
            dc->DrawLabel(wxString::Format("%d", measure_height_up), wxRect(x_start + case_w * 1, y, case_w, case_h_small), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel(wxString::Format("%.2f", theoretical_radius), wxRect(x_start + case_w * 2, y, case_w, case_h_small), wxALIGN_CENTER); // 3e Colonne
            dc->DrawLabel(wxString::Format("+/- %.1f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_small), wxALIGN_CENTER); // 4e Colonne
            dc->DrawLabel(std::to_wstring(measure_height_down), wxRect(x_start + case_w * 12, y, case_w, case_h_small), wxALIGN_CENTER); // 13e Colonne

            // Ecart Rayon
            {
            	for ( int j = 0; j < 8; j++ )
	            {
                    const double rayDiff = hInst->Calculation.RayDifference[i][j];
                
                    if (std::abs(rayDiff - UnassignedDoubleValue) > Epsilon)
                    {
                        SetToleranceColor( dc, rayDiff, hInst->ActiveProductData.RadiusTolerance );
                        dc->DrawLabel(wxString::Format("%.2f", rayDiff), wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small), wxALIGN_CENTER);
                    }
                    else
                    {
                        dc->GradientFillLinear(wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small + 1), GrayColor, GrayColor);
                    }
	            }

                dc->SetTextForeground(BlackColor);
            }
        }
        else if ( i < 21 )
        {
            y += case_h_large;
            dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h_large), wxALIGN_CENTER); // 1e Colonne
            dc->DrawLabel(wxString::Format("%d", measure_height_up), wxRect(x_start + case_w * 1, y, case_w, case_h_large), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel(wxString::Format("%.2f", theoretical_radius), wxRect(x_start + case_w * 2, y, case_w, case_h_large), wxALIGN_CENTER); // 3e Colonne
            dc->DrawLabel(wxString::Format("+/- %.1f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w , case_h_large), wxALIGN_CENTER); // 4e Colonne
            dc->DrawLabel(std::to_wstring(measure_height_down), wxRect(x_start + case_w * 12, y, case_w, case_h_large), wxALIGN_CENTER); // 13e Colonne

            // Ecart Rayon
            {
                for (int j = 0; j < 8; j++)
                {
                    dc->SetTextForeground(BlackColor);
                    dc->DrawLabel(wxString::Format("%.2f", hInst->ActiveProductData.TheoreticalRadius[i][j]), wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small), wxALIGN_CENTER);

                    const double rayDiff = hInst->Calculation.RayDifference[i][j];

                    if (std::abs(rayDiff - UnassignedDoubleValue) > Epsilon)
                    {
                        SetToleranceColor(dc, rayDiff, hInst->ActiveProductData.RadiusTolerance);
                        dc->DrawLabel(wxString::Format("%.2f", rayDiff), wxRect(x_start + case_w * (j + 4), y + case_h_small, case_w, case_h_small), wxALIGN_CENTER);
                    }
                    else
                    {
                        dc->GradientFillLinear(wxRect(x_start + case_w * (j + 4), y + case_h_small, case_w, case_h_small), GrayColor, GrayColor);
                    }
                }

                dc->SetTextForeground(BlackColor);
            }
        }
    }
}

void PrintRafale::RafalePage_02()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Rafale_DrawMainPv(hInst, dc, 2);

    // Cadre Principal
    dc->DrawLine(40, 158, 1080, 158);   // Haut
    dc->DrawLine(40, 673, 1080, 673);   // Bas
    dc->DrawLine(40, 158, 40, 673);     // Gauche
    dc->DrawLine(1080, 158, 1080, 673); // Droit

    dc->DrawLine(40, 190, 1080, 190);   // Séparateur Titre
    dc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("Contrôle profil (ondulations)", wxRect(40, 158, 1040, 32), wxALIGN_CENTER);

    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    constexpr int case_h = 23 /* Doit etre 23.3f */, case_w = 95, x_start = 40, y_start = 190;

    const wxString h =  wxString::Format("H + 2° %.1f", hInst->Calculation.UndulationTolerance[0]) + "%";
    const wxString hd = wxString::Format("HD %.1f", hInst->Calculation.UndulationTolerance[1]) + "%";
    const wxString d =  wxString::Format("D %.1f", hInst->Calculation.UndulationTolerance[2]) + "%";
    const wxString bd = wxString::Format("BD %.1f", hInst->Calculation.UndulationTolerance[3]) + "%";
    const wxString b =  wxString::Format("B + 2° %.1f", hInst->Calculation.UndulationTolerance[4]) + "%";
    const wxString bg = wxString::Format("BG %.1f", hInst->Calculation.UndulationTolerance[5]) + "%";
    const wxString g =  wxString::Format("G %.1f", hInst->Calculation.UndulationTolerance[6]) + "%";
    const wxString hg = wxString::Format("HG %.1f", hInst->Calculation.UndulationTolerance[7]) + "%";

    const std::vector<wxString> headers = { "Section", "Haut./Pointe", h, hd, d, bd, b, bg, g, hg, "Haut./Base"};

    for (int i = 0; i < static_cast <int>(headers.size()); i++)
    {
        dc->DrawLabel(headers[i], wxRect(x_start + case_w * i, y_start, case_w, case_h), wxALIGN_CENTER);
    }

    // Pour certaines raisons, dessiner TOUTE les lignes avant les valeurs
    float x = x_start, y = y_start;
    for (int i = 1; i < 21; i++)
    {
        y += case_h;

        dc->DrawLine(40, y, 1080, y);

        if (i <= 10)
        {
            x += case_w;
            dc->DrawLine(x, 190, x, 673);
        }
    }

    x = x_start, y = y_start;
    for (int i = 1; i < 21; i++)
    {
        y += case_h;

        // Hauteur Radome // Petit hack pour trouver les valeurs qui ne sont pas trouver (en particulier la 20e ligne)
        int measure_height_up = 0;   // 2e Colonne
        int measure_height_down = 0; // 13e Colonne
        {
            for (int idx = 0; idx < 8; idx++)
            {
                if (std::abs(hInst->Calculation.Height[i][idx] - UnassignedDoubleValue) > Epsilon)
                {
                    measure_height_up = 2114 - hInst->Calculation.Height[i][idx];
                    measure_height_down = hInst->Calculation.Height[i][idx];
                    break;
                }
            }
        }

        dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h), wxALIGN_CENTER); // 1e Colonne
        dc->DrawLabel(wxString::Format("%d", measure_height_up), wxRect(x_start + case_w * 1, y, case_w, case_h), wxALIGN_CENTER); // 2e Colonne
        dc->DrawLabel(std::to_wstring(measure_height_down), wxRect(x_start + case_w * 10, y, 1080 - (x_start + case_w * 10), case_h), wxALIGN_CENTER); // 11e Colonne

        // Ondulations
        {
            for (int j = 0; j < 8; j++)
            {
                const double undulationDiff = hInst->Calculation.Undulation[i][j];
                std::string key;
                for (const auto& pair : RadomeIndexPosition)
                {
                    if (pair.second == j)
                    {
                        key = pair.first;
                    }
                }

                if (std::abs(undulationDiff - UnassignedDoubleValue) > Epsilon && hInst->ActiveProductData.UndulationTolerance.find( key ) != hInst->ActiveProductData.UndulationTolerance.end())
                {
                    SetToleranceColor(dc, undulationDiff, hInst->ActiveProductData.UndulationTolerance[key]);
                    dc->DrawLabel(wxString::Format("%.2f", hInst->Calculation.Undulation[i][j] * 100) + "%", wxRect(x_start + case_w * (j + 2), y, case_w, case_h), wxALIGN_CENTER);
                }
                else
                {
                    dc->GradientFillLinear(wxRect(x_start + case_w * (j + 2), y, case_w, case_h + 1), GrayColor, GrayColor);
                }
            }

            dc->SetTextForeground(BlackColor);
        }
    }
}

void PrintRafale::RafalePage_03()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Rafale_DrawMainPv(hInst, dc, 3);

    dc->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    dc->DrawLabel("Contrôle du montage et des formes extérieures", wxRect(20, 138, 1100, 80), wxALIGN_CENTER);

    dc->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // Cadre Gauche
    if ( hInst->ActiveProductData.RadomeType == Rafale_C )
    {
        dc->DrawLine(80, 380, 520, 380);  // Haut
        dc->DrawLine(80, 520, 520, 520);  // Bas
        dc->DrawLine(80, 380, 80, 520);   // Gauche
        dc->DrawLine(520, 380, 520, 520); // Droit

        dc->DrawLine(80, 410, 520, 410); // Séparateur Titre

        // Séparateur Conforme/Non Conforme
        dc->DrawLine(80, 490, 520, 490); // Vertical
        dc->DrawLine(300, 490, 300, 520); // Horizontal

        dc->DrawLabel("Interface au niveau cadre avion", wxRect(80, 380, 440, 30), wxALIGN_CENTER);
        dc->DrawLabel(StringUtility::ReplaceWxString(StringUtility::StringToWString(hInst->C_JsonConfig.RafaleFrameLeftContent), JsonLineSeparator, SpaceReplacer), wxRect(80, 410, 440, 80), wxALIGN_CENTER);
        dc->DrawLabel("Conforme", wxRect(80, 490, 220, 30), wxALIGN_CENTER);
        dc->DrawLabel("Non conforme", wxRect(300, 490, 220, 30), wxALIGN_CENTER);

        // Askip c'est toujours conforme
        dc->DrawLine(300, 490, 520, 520);
        dc->DrawLine(300, 520, 520, 490);
    }

    // Cadre Droit
    dc->DrawLine(600, 380, 1040, 380);  // Haut
    dc->DrawLine(600, 520, 1040, 520);  // Bas
    dc->DrawLine(600, 380, 600, 520);   // Gauche
    dc->DrawLine(1040, 380, 1040, 520); // Droit
    
    dc->DrawLine(600, 410, 1040, 410); // Séparateur Titre

    // Séparateur
    dc->DrawLine(600, 490, 1040, 490); // Vertical
    dc->DrawLine(820, 490, 820, 520); // Horizontal
    
    dc->DrawLabel("Longueur", wxRect(600, 380, 440, 30), wxALIGN_CENTER);
    dc->DrawLabel(StringUtility::ReplaceWxString(StringUtility::StringToWString(hInst->C_JsonConfig.RafaleFrameRightContent), JsonLineSeparator, EndLineReplacer), wxRect(600, 410, 440, 80), wxALIGN_CENTER);
    dc->DrawLabel(wxString::Format( "L. théorique: %.2f ± %.1f", hInst->ActiveProductData.TheoreticalHeight, hInst->ActiveProductData.HeightTolerance), wxRect(600, 490, 220, 30), wxALIGN_CENTER);
    SetToleranceColor(dc, hInst->Calculation.RadomeHeight - hInst->ActiveProductData.TheoreticalHeight, hInst->ActiveProductData.HeightTolerance);
    dc->DrawLabel(wxString::Format("L. mesurée: %.2f mm", hInst->Calculation.RadomeHeight), wxRect(820, 490, 220, 30), wxALIGN_CENTER);
}

bool PrintRafale::OnPrintPage(const int page)
{
    if (GetDC())
    {
        switch (page)
        {
        case 1:
            this->RafalePage_01();
            break;
        case 2:
            this->RafalePage_02();
            break;
        case 3:
            this->RafalePage_03();
            break;
        default:
            break;
        }

        this->MapScreenSizeToPage();

        return true;
    }
    return false;
}
