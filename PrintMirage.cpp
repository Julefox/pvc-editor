
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
    Mirage_DrawMainPv(hInst, dc, 1);

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

    for (int i = 0; i < static_cast <int>(headers.size()); i++)
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
                //if (std::abs(hInst->Calculation.Height[i][idx] - UnassignedDoubleValue) > Epsilon)
                //{
                //    measure_height_up = hInst->Calculation.Height[i][idx] - 33.30;
                //    measure_height_down = hInst->Calculation.Height[i][idx];
                //    break;
                //}
            }
        }

        // Rayon Théorique
        double theoretical_radius = 0.0f;
        {
            double sum = 0.0f;

            //for (const double& rad : hInst->ActiveProductData.TheoreticalRadius[i])
            //{
            //    sum += rad;
            //}
            //
            //theoretical_radius = sum / std::size(hInst->ActiveProductData.TheoreticalRadius[i]);
        }

        if (i == 16)
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
                    //dc->DrawLabel(wxString::Format("%.2f", hInst->ActiveProductData.TheoreticalRadius[i][j]), wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small), wxALIGN_CENTER);

                    //const double rayDiff = hInst->Calculation.RayDifference[i][j];
                    //
                    //if (std::abs(rayDiff - UnassignedDoubleValue) > Epsilon)
                    //{
                    //    SetToleranceColor(dc, rayDiff, hInst->ActiveProductData.RadiusTolerance);
                    //    dc->DrawLabel(wxString::Format("%.2f", rayDiff), wxRect(x_start + case_w * (j + 4), y + case_h_small, case_w, case_h_small), wxALIGN_CENTER);
                    //}
                    //else
                    //{
                    //    dc->GradientFillLinear(wxRect(x_start + case_w * (j + 4), y + case_h_small, case_w, case_h_small + 1), GrayColor, GrayColor);
                    //}
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
                for (int j = 0; j < 8; j++)
                {
                    //const double rayDiff = hInst->Calculation.RayDifference[i][j];
                    //
                    //if (std::abs(rayDiff - UnassignedDoubleValue) > Epsilon)
                    //{
                    //    SetToleranceColor(dc, rayDiff, hInst->ActiveProductData.RadiusTolerance);
                    //    dc->DrawLabel(wxString::Format("%.2f", rayDiff), wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small), wxALIGN_CENTER);
                    //}
                    //else
                    //{
                    //    dc->GradientFillLinear(wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small + 1), GrayColor, GrayColor);
                    //}
                }

                dc->SetTextForeground(BlackColor);
            }
        }
        else if (i < 21)
        {
            y += case_h_large;
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
                    //dc->DrawLabel(wxString::Format("%.2f", hInst->ActiveProductData.TheoreticalRadius[i][j]), wxRect(x_start + case_w * (j + 4), y, case_w, case_h_small), wxALIGN_CENTER);

                    //const double rayDiff = hInst->Calculation.RayDifference[i][j];
                    //
                    //if (std::abs(rayDiff - UnassignedDoubleValue) > Epsilon)
                    //{
                    //    SetToleranceColor(dc, rayDiff, hInst->ActiveProductData.RadiusTolerance);
                    //    dc->DrawLabel(wxString::Format("%.2f", rayDiff), wxRect(x_start + case_w * (j + 4), y + case_h_small, case_w, case_h_small), wxALIGN_CENTER);
                    //}
                    //else
                    //{
                    //    dc->GradientFillLinear(wxRect(x_start + case_w * (j + 4), y + case_h_small, case_w, case_h_small), GrayColor, GrayColor);
                    //}
                }

                dc->SetTextForeground(BlackColor);
            }
        }
    }
}

void PrintMirage::MiragePage_02()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 2);
}

void PrintMirage::MiragePage_03()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 3);
    Mirage_DrawGraphic(hInst, dc, "H");
}

void PrintMirage::MiragePage_04()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 4);
    Mirage_DrawGraphic(hInst, dc, "HD");
}

void PrintMirage::MiragePage_05()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 5);
    Mirage_DrawGraphic(hInst, dc, "D");
}

void PrintMirage::MiragePage_06()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 6);
    Mirage_DrawGraphic(hInst, dc, "BD");
}

void PrintMirage::MiragePage_07()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 7);
    Mirage_DrawGraphic(hInst, dc, "B");
}

void PrintMirage::MiragePage_08()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 8);
    Mirage_DrawGraphic(hInst, dc, "BG");
}

void PrintMirage::MiragePage_09()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 9);
    Mirage_DrawGraphic(hInst, dc, "G");
}

void PrintMirage::MiragePage_10()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Mirage_DrawMainPv(hInst, dc, 10);
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

