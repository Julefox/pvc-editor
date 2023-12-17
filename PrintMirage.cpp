﻿
#include "PrintMirage.h"

#include "main.h"

void PrintMirage::DrawMiragePage(wxWindow* hInst)
{
    wxPrintDialogData g_printDialogData;
    wxPrintData g_printData;
    g_printData.SetOrientation( wxLANDSCAPE );
    g_printDialogData.SetPrintData( g_printData );

    auto* preview = new wxPrintPreview( new PrintMirage, new PrintMirage, &g_printDialogData );

    preview->SetZoom( 100 );

    if ( !preview->Ok() )
    {
        delete preview;
        wxMessageBox( wxT( "There was a problem previewing.\nPerhaps your current printer is not set correctly?" ), wxT( "Previewing" ), wxOK );
        return;
    }

    auto* frame = new wxPreviewFrame( preview, hInst, wxT( "Impression" ), wxPoint( -1, -1 ), wxSize( 1280, 880 ), wxDEFAULT_FRAME_STYLE | wxFRAME_SHAPED );
    frame->Centre( wxBOTH );
    frame->Initialize();
    frame->Show( true );
}

void PrintMirage::MiragePage_01()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc    = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 1);

    // Cadre Principal
    dc->DrawLine(119, 220, 1001, 220);  // Haut
    dc->DrawLine(119, 659, 1001, 659);  // Bas
    dc->DrawLine(119, 220, 119, 659);   // Gauche
    dc->DrawLine(1001, 220, 1001, 659); // Droit

    dc->DrawLine(119, 260, 1001, 260);  // Séparateur Titre

    dc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("TABLEAU DES ECARTS GENERATRICES", wxRect(119, 220, 882, 40), wxALIGN_CENTER);

    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    
    constexpr int case_h_small = 29, case_h_large = 51, case_w = 68, x_start = 119, y_start = 260;
    
    const std::vector<wxString> headers = { "Section", "Haut.", "Rayon T.", "Tol.", "H", "HD", "D", "BD", "B", "BG", "G", "HG", "Section" };
    
    for (int i = 0; i < static_cast <int>(headers.size()); i++)
    {
        const int headerPos = x_start + case_w * i;
        dc->DrawLabel(headers[i], wxRect(headerPos, y_start, case_w, case_h_small), wxALIGN_CENTER);
        dc->DrawLine(headerPos, y_start, headerPos, 659);
    }

    for (int i = 0; i < 13; i++)
    {
        dc->DrawLine(x_start, y_start + case_h_small * i, x_start + 880, y_start + case_h_small * i);
    }

    int y = y_start + case_h_small;

    for (int i = 12; i > 0; i--)
    {
        const int section = 12 - i;

        int height = 0;
        {
            for (int j = 0; j < 8; j++)
            {
                const PointMeasure point = hInst->Calculation.PointData[i][CalculationData::GetRafaleSide(j)];

                if (std::abs(point.Height - UnassignedDoubleValue) > Epsilon)
                {
                    height = point.Height;
                    break;
                }
            }
        }

        double theoretical_radius = 0.0f;
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

                    theoretical_radius = sum / 8;
                    break;
                }
            }
        }

        if(section == 0)
        {
            dc->DrawLabel("F", wxRect(x_start, y, case_w, case_h_small), wxALIGN_CENTER);
            dc->DrawLabel("F", wxRect(x_start + case_w * 12, y, case_w, case_h_small), wxALIGN_CENTER);
            dc->DrawLabel(wxString::Format("%.2f", height - 32.3f), wxRect(x_start + case_w, y, case_w, case_h_small), wxALIGN_CENTER);
            dc->DrawLabel(wxString::Format("%.2f", theoretical_radius), wxRect(x_start + case_w * 2, y, case_w, case_h_small), wxALIGN_CENTER);
            dc->DrawLabel(wxString::Format("± %.2f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_small), wxALIGN_CENTER);
        }
        else if (section == 11)
        {
            dc->DrawLabel(std::to_string(section), wxRect(x_start, y, case_w, case_h_large), wxALIGN_CENTER);
            dc->DrawLabel(std::to_string(section), wxRect(x_start + case_w * 12, y, case_w, case_h_large), wxALIGN_CENTER);
            dc->DrawLabel(wxString::Format("%.2f", height - 32.3f), wxRect(x_start + case_w, y, case_w, case_h_large), wxALIGN_CENTER);
            dc->DrawLabel(headers[2], wxRect(x_start + case_w * 2, y, case_w, case_h_large), wxALIGN_CENTER);
            dc->DrawLabel(wxString::Format("± %.2f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_large), wxALIGN_CENTER);
        }
        else
        {
            dc->DrawLabel(std::to_string(section), wxRect(x_start, y, case_w, case_h_small), wxALIGN_CENTER);
            dc->DrawLabel(std::to_string(section), wxRect(x_start + case_w * 12, y, case_w, case_h_small), wxALIGN_CENTER);
            dc->DrawLabel(wxString::Format("%.2f", height - 32.3f), wxRect(x_start + case_w, y, case_w, case_h_small), wxALIGN_CENTER);
            dc->DrawLabel(wxString::Format("%.2f", theoretical_radius), wxRect(x_start + case_w * 2, y, case_w, case_h_small), wxALIGN_CENTER);
            dc->DrawLabel(wxString::Format("± %.2f", hInst->ActiveProductData.RadiusTolerance), wxRect(x_start + case_w * 3, y, case_w, case_h_small), wxALIGN_CENTER);
        }

        if (i == 1)
        {
            for (int j = 0; j < 8; j++)
            {
                const eSideType side = CalculationData::GetMirageSide(j);
                const PointMeasure& point = hInst->Calculation.PointData[i][side];
                
                if (std::abs(point.RayDifference - UnassignedDoubleValue) > Epsilon && !((side == AND_BD && i == 1) || (side == AND_B && i > 4) || (side == AND_BG && i == 1)))
                {
                    dc->SetTextForeground(BlackColor);
                    dc->DrawLabel(wxString::Format("%.2f", hInst->ActiveProductData.TheoreticalRadius[point.Height][side]), wxRect(x_start + case_w * (side + 4), y, case_w, case_h_small), wxALIGN_CENTER);
                    SetToleranceColor(dc, point.RayDifference, hInst->ActiveProductData.RadiusTolerance);
                    dc->DrawLabel(wxString::Format("%.2f", point.RayDifference), wxRect(x_start + case_w * (side + 4), y + 26 , case_w, case_h_small), wxALIGN_CENTER);
                }
                else if (i == 1)
                {
                    dc->GradientFillLinear(wxRect(x_start + case_w * (side + 4), y, case_w, case_h_large), GrayColor, GrayColor);
                }
                else
                {
                    dc->GradientFillLinear(wxRect(x_start + case_w * (side + 4), y, case_w, case_h_small), GrayColor, GrayColor);
                }
            }

            dc->SetTextForeground(BlackColor);
        }
        else
        {
            for (int j = 0; j < 8; j++)
            {
                const eSideType side = CalculationData::GetMirageSide(j);
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
    }
}

void PrintMirage::MiragePage_02()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 2);
}

void PrintMirage::MiragePage_03()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 3);
    Mirage_DrawGraphic(hInst, dc, "H");
}

void PrintMirage::MiragePage_04()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 4);
    Mirage_DrawGraphic(hInst, dc, "HD");
}

void PrintMirage::MiragePage_05()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 5);
    Mirage_DrawGraphic(hInst, dc, "D");
}

void PrintMirage::MiragePage_06()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 6);
    Mirage_DrawGraphic(hInst, dc, "BD");
}

void PrintMirage::MiragePage_07()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 7);
    Mirage_DrawGraphic(hInst, dc, "B");
}

void PrintMirage::MiragePage_08()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 8);
    Mirage_DrawGraphic(hInst, dc, "BG");
}

void PrintMirage::MiragePage_09()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 9);
    Mirage_DrawGraphic(hInst, dc, "G");
}

void PrintMirage::MiragePage_10()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainHeader(hInst, dc, 10);
    Mirage_DrawGraphic(hInst, dc, "HG");
}

void PrintMirage::Mirage_DrawGraphic(Program* hInst, wxDC* dc, const wxString& gen)
{
    const wxPen pBigBlack(wxColour(0, 0, 0), 3);
    const wxPen pRed(wxColour(224, 102, 102));
    const wxPen pDotBlue(*wxBLUE, 1, wxPENSTYLE_DOT);

    dc->SetPen(*wxBLACK_PEN);
    dc->SetTextForeground(wxColour(0, 0, 0)); // Black

    // Cadre Principal
    dc->DrawLine(120, 220, 140, 220);   // Haut 1
    dc->DrawLine(208, 220, 1000, 220);  // Haut 2
    dc->DrawLine(120, 660, 1000, 660);  // Bas
    dc->DrawLine(120, 220, 120, 660);   // Gauche
    dc->DrawLine(1000, 220, 1000, 660); // Droit

    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc->DrawLabel(L"Ecart en mm", wxRect(142, 205, 64, 30), wxALIGN_CENTRE);

    // PM: 260 == Haut Y | 620 == Bas Y | 440 == Centre Y | 180 == Cote Gauche X | 1000 == Cote Droit X
    // Ligne de tolerance, de gauche a droite
    dc->SetPen(pDotBlue);
    dc->DrawLine(200, 440, 385, 260); // F
    dc->DrawLine(200, 440, 376, 610); // F // Leger offset pour ne pas cacher le chiffre 3

    dc->DrawLine(230, 440, 416, 260); // 1
    dc->DrawLine(230, 440, 416, 620); // 1

    dc->DrawLine(303, 440, 489, 260); // 2
    dc->DrawLine(303, 440, 489, 620); // 2

    dc->DrawLine(376, 440, 562, 260); // 3
    dc->DrawLine(376, 440, 562, 620); // 3

    dc->DrawLine(449, 440, 635, 260); // 4
    dc->DrawLine(449, 440, 635, 620); // 4

    dc->DrawLine(522, 440, 708, 260); // 5
    dc->DrawLine(522, 440, 708, 620); // 5

    dc->DrawLine(595, 440, 781, 260); // 6
    dc->DrawLine(595, 440, 781, 620); // 6

    dc->DrawLine(668, 440, 854, 260); // 7
    dc->DrawLine(668, 440, 854, 620); // 7

    if (gen != "B")
    {
        dc->DrawLine(741, 440, 834, 260); // 8
        dc->DrawLine(741, 440, 834, 620); // 8

        dc->DrawLine(814, 440, 907, 260); // 9
        dc->DrawLine(814, 440, 907, 620); // 9

        dc->DrawLine(887, 440, 980, 260); // 10
        dc->DrawLine(887, 440, 980, 620); // 10
    }

    // Cadre Tableau
    dc->SetPen(pBigBlack);
    dc->DrawLine(180, 260, 980, 260); // Haut
    dc->DrawLine(180, 260, 180, 620); // Gauche
    dc->DrawLine(980, 260, 980, 620); // Droit

    // Bas
    dc->DrawLine(180, 620, 190, 620);
    dc->DrawLabel(L"F", wxRect(190, 610, 20, 20), wxALIGN_CENTRE);
    dc->SetPen(*wxBLACK_PEN);
    dc->DrawLine(200, 260, 200, 610);
    dc->SetPen(pBigBlack);
    dc->DrawLine(210, 620, 220, 620);

    int x = 220;
    for ( int i = 1; i < 11; i++ )
    {
        dc->DrawLabel(wxString::Format(wxT("%d"), i), wxRect(x, 610, 20, 20), wxALIGN_CENTRE); // Affiche les chiffres en bas
        x += 10;
        dc->SetPen(*wxBLACK_PEN);
        dc->DrawLine(x, 260, x, 610); // Ligne verticale au dessus des chiffres
        dc->SetPen(pBigBlack);
        x += 10;
        const int forward = i == 10 ? x + 31 : x + 53;
        dc->DrawLine(x, 620, forward, 620); // Ligne du bas entre chaque chiffres
        x = forward;

        if (i == 8 && gen == "B")
            break;
    }

    if (gen != "B")
    {
        dc->DrawLabel(L"11", wxRect(928, 610, 20, 20), wxALIGN_CENTRE);
        dc->SetPen(*wxBLACK_PEN);
        dc->DrawLine(938, 260, 938, 610);
        dc->SetPen(pBigBlack);
        dc->DrawLine(950, 620, 980, 620);
    }
    else
    {
        dc->DrawLine(800, 620, 980, 620);
    }

    dc->SetPen(*wxBLACK_PEN);

    // Valeur cote gauche
    int y = 245;
    for (float i = 1.50f; i > -2.00f; i -= 0.50f)
    {
        dc->DrawLabel(wxString::Format(wxT("%.2f"), i), wxRect(120, y, 60, 30), wxALIGN_CENTRE);
        y += 60;
    }

    // Ligne Horizontale
    y = 290;
    for (int i = 0; i < 11; i++)
    {
        dc->DrawLine(180, y, 980, y);
        if (i == 1 || i == 10)
        {
            dc->SetPen(pRed);
            dc->DrawLine(200, y, 938, y);
            dc->SetPen(*wxBLACK_PEN);
        }
        y += 30;
    }

    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("N° de Section", wxRect(180, 620, 800, 40), wxALIGN_CENTRE);


    dc->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, true));
    dc->DrawLabel(L"Représentation graphique du profil: Génératrice " + gen, wxRect(120, 120, 880, 100), wxALIGN_CENTRE);
}

bool PrintMirage::OnPrintPage(const int page)
{
    if (GetDC())
    {
        switch (page)
        {
        case 1:
            this->MiragePage_01();
            break;
        case 2:
            this->MiragePage_02();
            break;
        case 3:
            this->MiragePage_03();
            break;
        case 4:
            this->MiragePage_04();
            break;
        case 5:
            this->MiragePage_05();
            break;
        case 6:
            this->MiragePage_06();
            break;
        case 7:
            this->MiragePage_07();
            break;
        case 8:
            this->MiragePage_08();
            break;
        case 9:
            this->MiragePage_09();
            break;
        case 10:
            this->MiragePage_10();
            break;
        default:
            break;
        }

        this->MapScreenSizeToPage();

        return true;
    }
    return false;
}

