
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

    const int case_h_small = 19 /* Doit etre 18.85f */, case_h_large = 37 /* Doit etre 37.69f */, case_w = 80, x_start = 40, y_start = 190;

    std::vector<wxString> headers = { "Section", "Haut/Pointe", "Rayon", "Tol.", "H + 2°", "HD", "D", "BD", "B + 2°", "BG", "G", "HG", "Haut/Base" };

    for (int i = 0; i < 13; i++)
    {
        dc->DrawLabel(headers[i], wxRect(x_start + case_w * i, y_start, case_w, case_h_small), wxALIGN_CENTER);
    }

    float x = x_start, y = y_start;
    for (int i = 1; i <= 21; i++)
    {
        dc->DrawLine(40, y, 1080, y);

        if (i <= 13)
        {
            x += case_w;
            dc->DrawLine(x, 190, x, 680);
        }

        // Hauteur Radome
        const int measure_height = 2114 - hInst->Calculation.Height[i][7];

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
            dc->DrawLabel(wxString::Format("%d", measure_height), wxRect(x_start + case_w * 1, y, case_w, case_h_large), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel(wxString::Format("%.2f", theoretical_radius), wxRect(x_start + case_w * 2, y, case_w, case_h_large), wxALIGN_CENTER); // 3e Colonne
            dc->DrawLabel(wxString::Format("+/- %.1f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_large), wxALIGN_CENTER); // 4e Colonne
            dc->DrawLabel(std::to_wstring(hInst->Calculation.Height[i][7]), wxRect(x_start + case_w * 12, y, case_w, case_h_large), wxALIGN_CENTER); // 13e Colonne
        }
        else if (i < 17)
        {
            y += case_h_small;
            dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h_small), wxALIGN_CENTER); // 1e Colonne
            dc->DrawLabel(wxString::Format("%d", measure_height), wxRect(x_start + case_w * 1, y, case_w, case_h_small), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel(wxString::Format("%.2f", theoretical_radius), wxRect(x_start + case_w * 2, y, case_w, case_h_small), wxALIGN_CENTER); // 3e Colonne
            dc->DrawLabel(wxString::Format("+/- %.1f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_small), wxALIGN_CENTER); // 4e Colonne
            dc->DrawLabel(std::to_wstring( hInst->Calculation.Height[i][7] ), wxRect(x_start + case_w * 12, y, case_w, case_h_small), wxALIGN_CENTER); // 13e Colonne

            // Ecart Rayon
            {
	            for ( int j = 0; j < 8; j++ )
	            {
                    double rayDiff = hInst->Calculation.RayDifference[i][j];

                    if (std::abs(rayDiff - UnassignedDoubleValue) > Epsilon)
                    {
                        this->SetToleranceColor( dc, rayDiff, hInst->ActiveProductData.RadiusTolerance );
                        dc->DrawLabel(wxString::Format("%.2f", rayDiff), wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small), wxALIGN_CENTER);
                    }
                    else
                    {
                        dc->GradientFillLinear(wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small), GrayColor, GrayColor);
                    }
	            }

                dc->SetTextForeground(BlackColor);
            }
        }
        else if ( i < 21 )
        {
            y += case_h_large;
            dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h_large), wxALIGN_CENTER); // 1e Colonne
            dc->DrawLabel(wxString::Format("%d", measure_height), wxRect(x_start + case_w * 1, y, case_w, case_h_large), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel(wxString::Format("%.2f", theoretical_radius), wxRect(x_start + case_w * 2, y, case_w, case_h_large), wxALIGN_CENTER); // 3e Colonne
            dc->DrawLabel(wxString::Format("+/- %.1f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w , case_h_large), wxALIGN_CENTER); // 4e Colonne
            dc->DrawLabel(std::to_wstring(hInst->Calculation.Height[i][7]), wxRect(x_start + case_w * 12, y, case_w, case_h_large), wxALIGN_CENTER); // 13e Colonne
        }
    }
}

void PrintRafale::RafalePage_02()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Rafale_DrawMainPv(hInst, dc, 2);
}

void PrintRafale::RafalePage_03()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Rafale_DrawMainPv(hInst, dc, 3);
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
