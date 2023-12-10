
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

    // DEV ONLY
    Mirage_DrawGraphic(hInst, dc, "H");
    // DEV END
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

    dc->DrawLine(741, 440, 834, 260); // 8
    dc->DrawLine(741, 440, 834, 620); // 8

    dc->DrawLine(814, 440, 907, 260); // 9
    dc->DrawLine(814, 440, 907, 620); // 9

    dc->DrawLine(887, 440, 980, 260); // 10
    dc->DrawLine(887, 440, 980, 620); // 10

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
    }

    dc->DrawLabel(L"11", wxRect(928, 610, 20, 20), wxALIGN_CENTRE);
    dc->SetPen(*wxBLACK_PEN);
    dc->DrawLine(938, 260, 938, 610);
    dc->SetPen(pBigBlack);
    dc->DrawLine(950, 620, 980, 620);

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

