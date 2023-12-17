
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
    Rafale_DrawMainHeader(hInst, dc, 1);

    constexpr int case_h_small = 19, case_h_large = 37, case_w = 80, x_start = 40, y_start = 190;

    dc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    DrawRectangle(dc, x_start, 150, 1040, 40, "Contrôle profil (Ecart entre rayon théorique et rayon mesuré)");

    const std::vector<wxString> headers = { "Section", "Haut./Pointe", "Rayon", "Tol.", "H + 2°", "HD", "D", "BD", "B + 2°", "BG", "G", "HG", "Haut./Base" };

	dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    DrawArray(dc, x_start, y_start, case_w, 13, case_h_small, 16, headers);
    DrawArray(dc, x_start, y_start + case_h_small * 16, case_w, 13, case_h_large, 5);

    int x = x_start, y = y_start + case_h_small;

    for (int i = 1; i < 16; i++)
    {
        CalculateHeight(hInst, i);
        CalculateTheoreticalRadius(hInst, i);
        dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h_small), wxALIGN_CENTER); // 1e Colonne
        dc->DrawLabel(wxString::Format("%d", UpperHeight), wxRect(x_start + case_w * 1, y, case_w, case_h_small), wxALIGN_CENTER); // 2e Colonne
        dc->DrawLabel(wxString::Format("%.2f", TheoreticalRadius), wxRect(x_start + case_w * 2, y, case_w, case_h_small), wxALIGN_CENTER); // 3e Colonne
        dc->DrawLabel(wxString::Format("± %.1f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_small), wxALIGN_CENTER); // 4e Colonne
        dc->DrawLabel(std::to_wstring(LowerHeight), wxRect(x_start + case_w * 12, y, case_w, case_h_small), wxALIGN_CENTER); // 13e Colonne

        for (int j = 0; j < 8; j++)
        {
            const eSideType side = CalculationData::GetRafaleSide(j);
            const PointMeasure& point = hInst->Calculation.PointData[i][side];

            if (std::abs(point.RayDifference - UnassignedDoubleValue) > Epsilon)
            {
                SetToleranceColor(dc, point.RayDifference, hInst->ActiveProductData.RadiusTolerance);
                dc->DrawLabel(wxString::Format("%.2f", point.RayDifference), wxRect(x_start + case_w * (side + 4), y, case_w, case_h_small), wxALIGN_CENTER);
            }
            else
            {
                dc->GradientFillLinear(wxRect(x_start + case_w * (side + 4), y, case_w, case_h_small), GrayColor, GrayColor);
            }
        }

        dc->SetTextForeground(BlackColor);

        y += case_h_small;
    }

    for (int i = 16; i < 21; i++)
    {
        CalculateHeight(hInst, i);
        dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h_large), wxALIGN_CENTER); // 1e Colonne

        if (hInst->ActiveProductData.RadomeType == Rafale_C)
        {
            dc->DrawLabel(wxString::Format("%d", UpperHeight), wxRect(x_start + case_w * 1, y, case_w, case_h_large), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel(headers[2], wxRect(x_start + case_w * 2, y, case_w, case_h_large), wxALIGN_CENTER); // 3e Colonne
            dc->DrawLabel(wxString::Format("± %.1f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_large), wxALIGN_CENTER); // 4e Colonne
            dc->DrawLabel(std::to_wstring(LowerHeight), wxRect(x_start + case_w * 12, y, case_w, case_h_large), wxALIGN_CENTER); // 13e Colonne

            for (int j = 0; j < 8; j++)
            {
                const eSideType side = CalculationData::GetRafaleSide(j);
                const PointMeasure& point = hInst->Calculation.PointData[i][side];

                dc->SetTextForeground(BlackColor);
                dc->DrawLabel(wxString::Format("%.2f", hInst->ActiveProductData.TheoreticalRadius[LowerHeight][side]), wxRect(x_start + case_w * (side + 4), y, case_w, case_h_small), wxALIGN_CENTER);

                if (side == RBE_H && UpperHeight > 1800) // Ignore les points sur le cache syst. SPECTRA
                {
                    dc->SetTextForeground(BlueColor);
                    dc->DrawLabel("CAPOT", wxRect(x_start + case_w * (side + 4), y + case_h_small, case_w, case_h_small), wxALIGN_CENTER);
                }
                else if (std::abs(point.RayDifference - UnassignedDoubleValue) > Epsilon)
                {
                    SetToleranceColor(dc, point.RayDifference, hInst->ActiveProductData.RadiusTolerance);
                    dc->DrawLabel(wxString::Format("%.2f", point.RayDifference), wxRect(x_start + case_w * (side + 4), y + case_h_small, case_w, case_h_small), wxALIGN_CENTER);
                }
                else
                {
                    dc->GradientFillLinear(wxRect(x_start + case_w * (side + 4), y + case_h_small - 1, case_w, case_h_small), GrayColor, GrayColor);
                }
            }
        }
        else
        {
            dc->DrawLabel("...", wxRect(x_start + case_w * 1, y, case_w, case_h_large), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel("...", wxRect(x_start + case_w * 2, y, case_w, case_h_large), wxALIGN_CENTER); // 3e Colonne
            dc->DrawLabel("...", wxRect(x_start + case_w * 3, y, case_w, case_h_large), wxALIGN_CENTER); // 4e Colonne
            dc->DrawLabel("...", wxRect(x_start + case_w * 12, y, case_w, case_h_large), wxALIGN_CENTER); // 13e Colonne

            for (int j = 0; j < 8; j++)
            {
                const eSideType side = CalculationData::GetRafaleSide(j);
                const PointMeasure& point = hInst->Calculation.PointData[i][side];

                dc->SetTextForeground(BlackColor);
                dc->DrawLabel("...", wxRect(x_start + case_w * (side + 4), y, case_w, case_h_small), wxALIGN_CENTER);
				dc->GradientFillLinear(wxRect(x_start + case_w * (side + 4), y + case_h_small - 1, case_w, case_h_small), GrayColor, GrayColor);
            }
        }
        
        dc->SetTextForeground(BlackColor);

        y += case_h_large;
    }
}

void PrintRafale::RafalePage_02()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Rafale_DrawMainHeader(hInst, dc, 2);

    constexpr int case_h = 23, case_w = 94, x_start = 43, y_start = 190;

    dc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    DrawRectangle(dc, x_start, 150, 1034, 40, "Contrôle profil (ondulations)");

    const wxString h =  wxString::Format("H + 2° %.1f", hInst->ActiveProductData.UndulationTolerance[RBE_H]) + "%";
    const wxString hd = wxString::Format("HD %.1f", hInst->ActiveProductData.UndulationTolerance[RBE_HD]) + "%";
    const wxString d =  wxString::Format("D %.1f", hInst->ActiveProductData.UndulationTolerance[RBE_D]) + "%";
    const wxString bd = wxString::Format("BD %.1f", hInst->ActiveProductData.UndulationTolerance[RBE_BD]) + "%";
    const wxString b =  wxString::Format("B + 2° %.1f", hInst->ActiveProductData.UndulationTolerance[RBE_B]) + "%";
    const wxString bg = wxString::Format("BG %.1f", hInst->ActiveProductData.UndulationTolerance[RBE_BG]) + "%";
    const wxString g =  wxString::Format("G %.1f", hInst->ActiveProductData.UndulationTolerance[RBE_G]) + "%";
    const wxString hg = wxString::Format("HG %.1f", hInst->ActiveProductData.UndulationTolerance[RBE_HG]) + "%";
    
    const std::vector<wxString> headers = { "Section", "Haut./Pointe", h, hd, d, bd, b, bg, g, hg, "Haut./Base"};

    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    DrawArray(dc, x_start, y_start, case_w, 11, case_h, 21, headers);

    int x = x_start, y = y_start + case_h;

    for (int i = 1; i < 21; i++)
    {
        CalculateHeight(hInst, i);
        CalculateTheoreticalRadius(hInst, i);
        dc->DrawLabel(std::to_wstring(i), wxRect(40, y, case_w, case_h), wxALIGN_CENTER); // 1e Colonne

        if (hInst->ActiveProductData.RadomeType == Rafale_R && i > 15)
        {
            dc->DrawLabel("...", wxRect(x_start + case_w * 1, y, case_w, case_h), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel("...", wxRect(x_start + case_w * 10, y, case_w, case_h), wxALIGN_CENTER); // 13e Colonne
        }
        else
        {
            dc->DrawLabel(wxString::Format("%d", UpperHeight), wxRect(x_start + case_w * 1, y, case_w, case_h), wxALIGN_CENTER); // 2e Colonne
            dc->DrawLabel(std::to_wstring(LowerHeight), wxRect(x_start + case_w * 10, y, case_w, case_h), wxALIGN_CENTER); // 13e Colonne
        }

        for (int j = 0; j < 8; j++)
        {
            const eSideType side = CalculationData::GetRafaleSide(j);
            const PointMeasure& point = hInst->Calculation.PointData[i][side];

            if (std::abs(point.Undulation - UnassignedDoubleValue) > Epsilon && !(side == RBE_H && UpperHeight > 1800)) // Ignore les points sur le cache syst. SPECTRA
            {
                SetToleranceColor(dc, point.Undulation, point.UndulationTolerance);
                dc->DrawLabel(wxString::Format("%.2f", point.Undulation) + "%", wxRect(x_start + case_w * (side + 2), y, case_w, case_h), wxALIGN_CENTER);
            }
            else
            {
                dc->GradientFillLinear(wxRect(x_start + case_w * (side + 2), y, case_w, case_h + 1), GrayColor, GrayColor);
            }
        }

        dc->SetTextForeground(BlackColor);

        y += case_h;
    }
}

void PrintRafale::RafalePage_03()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Rafale_DrawMainHeader(hInst, dc, 3);

    dc->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    dc->DrawLabel("Contrôle du montage et des formes extérieures", wxRect(20, 138, 1100, 80), wxALIGN_CENTER);

    dc->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    int x_right = 320;

    if ( hInst->ActiveProductData.RadomeType == Rafale_C )
    {
        // Cadre Gauche
        DrawRectangle(dc, 80, 380, 440, 30, "Interface au niveau cadre avion");
        DrawRectangle(dc, 80, 410, 440, 80, StringUtility::ReplaceWxString(StringUtility::StringToWString(hInst->C_JsonConfig.RafaleFrameLeftContent), JsonLineSeparator, SpaceReplacer));
        DrawRectangle(dc, 80, 490, 220, 30, "Conforme");
        DrawRectangle(dc, 300, 490, 220, 30, "Non conforme");

        // Askip c'est toujours conforme
        dc->DrawLine(300, 490, 520, 520);
        dc->DrawLine(300, 520, 520, 490);

        x_right = 600;
    }

    // Cadre Droit
    DrawRectangle(dc, x_right, 380, 440, 30, "Longueur");
    DrawRectangle(dc, x_right, 410, 440, 80, StringUtility::ReplaceWxString(StringUtility::StringToWString(hInst->C_JsonConfig.RafaleFrameRightContent), JsonLineSeparator, EndLineReplacer));
    DrawRectangle(dc, x_right, 490, 220, 30, wxString::Format("L. théorique: %.2f ± %.1f", hInst->ActiveProductData.TheoreticalHeight, hInst->ActiveProductData.HeightTolerance));
    SetToleranceColor(dc, hInst->Calculation.RadomeHeight - hInst->ActiveProductData.TheoreticalHeight, hInst->ActiveProductData.HeightTolerance);
    DrawRectangle(dc, x_right + 220, 490, 220, 30, wxString::Format("L. mesurée: %.2f mm", hInst->Calculation.RadomeHeight));
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

void PrintRafale::CalculateHeight(Program* hInst, int i)
{
    // Hauteur Radome // Petit hack pour trouver les valeurs qui ne sont pas trouver (en particulier la 20e ligne)
    for (int idx = 0; idx < 8; idx++)
    {
        const PointMeasure& point = hInst->Calculation.PointData[i][CalculationData::GetRafaleSide(idx)];

        if (std::abs(point.Height - UnassignedDoubleValue) > Epsilon)
        {
            UpperHeight = 2114 - point.Height; // 2e Colonne
            LowerHeight = point.Height;        // 13e Colonne
            break;
        }
    }
}

void PrintRafale::CalculateTheoreticalRadius(Program* hInst, const int i)
{
    double sum = 0.0f;
    for (int side = 0; side < 8; side++)
    {
        PointMeasure point = hInst->Calculation.PointData[i][CalculationData::GetRafaleSide(side)];
    
        if (std::abs(point.Height - UnassignedDoubleValue) > Epsilon)
        {
            for (int rad = 0; rad < 8; rad++)
            {
                sum += hInst->ActiveProductData.TheoreticalRadius[point.Height][CalculationData::GetRafaleSide(rad)];
            }
    
            TheoreticalRadius = sum / 8;
            break;
        }
    }
}
