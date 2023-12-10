#include "PrintGlobal.h"

#include "main.h"

bool PrintGlobal::OnBeginDocument( const int startPage, const int endPage )
{
    if (!wxPrintout::OnBeginDocument(startPage, endPage))
        return false;

    return true;
}

void PrintGlobal::GetPageInfo( int* minPage, int* maxPage, int* pageFrom, int* pageTo )
{
    *minPage = 1; *maxPage = ImpressNum;
    *pageFrom = 1; *pageTo = ImpressNum;
}

bool PrintGlobal::HasPage( const int page )
{
    return ( page >= 1 && page <= ImpressNum );
}


void PrintGlobal::SetDcScale( wxPrintout* print, wxDC* dc )
{
    // You might use THIS code to set the printer DC to roughly
    // reflect the screen text size. This page also draws lines of
    // actual length 5cm on the page.

    // Get the logical pixels per inch of screen and printer
    int ppiScreenX, ppiScreenY;
    print->GetPPIScreen(&ppiScreenX, &ppiScreenY);
    int ppiPrinterX, ppiPrinterY;
    print->GetPPIPrinter(&ppiPrinterX, &ppiPrinterY);

    // This scales the DC so that the printout roughly represents the
    // the screen scaling.
    float scale = (float)ppiPrinterX / (float)ppiScreenX;

    // Now we have to check in case our real page size is reduced
    // (e.g. because we're drawing to a print preview memory DC)
    int pageWidth, pageHeight;
    int w, h;
    dc->GetSize(&w, &h);
    print->GetPageSizePixels(&pageWidth, &pageHeight);

    // If printer pageWidth == current DC width, then this doesn't
    // change. But w might be the preview bitmap width,
    // so scale down.
    float overallScale = scale * (float)(w / (float)pageWidth);
    dc->SetUserScale(overallScale, overallScale);

    // Calculate conversion factor for converting millimetres into
    // logical units.
    // There are approx. 25.4 mm to the inch. There are ppi
    // device units to the inch. Therefore 1 mm corresponds to
    // ppi/25.4 device units. We also divide by the
    // screen-to-printer scaling factor, because we need to
    // unscale to pass logical units to DrawLine.

    // Draw 50 mm by 50 mm L shape
    /*float logUnitsFactor = (float)(ppiPrinterX/(scale*25.4));
    float logUnits = (float)(50*logUnitsFactor);*/
}

void PrintGlobal::Rafale_DrawMainPv( Program* hInst, wxDC*dc, const int pageIdx )
{
    dc->SetPen(*wxBLACK_PEN);

    // Cadre Principal
    dc->DrawLine(20, 30, 1100, 30);    // Haut
    dc->DrawLine(20, 760, 1100, 760);  // Bas
    dc->DrawLine(20, 30, 20, 760);     // Gauche
    dc->DrawLine(1100, 30, 1100, 760); // Droit

    dc->DrawLine(20, 120, 1100, 120); // Separateur Bordereaux Horizontal Bas
    dc->DrawLine(140, 64, 960, 64);   // Separateur Bordereaux Horizontal Milieu
    dc->DrawLine(960, 91, 1100, 91);  // Separateur Bordereaux Horizontal Milieu Petit
    dc->DrawLine(140, 30, 140, 120);  // Separateur Bordereaux Vertical 0
    dc->DrawLine(680, 30, 680, 120);  // Separateur Bordereaux Vertical 1
    dc->DrawLine(960, 30, 960, 120);  // Separateur Bordereaux Vertical 2

    dc->DrawLine(800, 720, 1100, 720); // Separateur Date Horizontal
    dc->DrawLine(800, 720, 800, 760);  // Separateur Date Vertical

    dc->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("AIA/CP", wxRect(20, 32, 120, 30), wxALIGN_CENTRE);
    dc->DrawLabel("PROCES-VERBAL DE CONTROLE", wxRect(140, 32, 540, 30), wxALIGN_CENTRE);

    dc->SetFont(wxFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel(L"****\nDépartement\nde la Qualité\net du Contrôle", wxRect(20, 60, 120, 60), wxALIGN_CENTRE);
    dc->DrawLabel(L"Date de contrôle / Marque", wxRect(805, 724, 300, 30), wxALIGN_LEFT | wxALIGN_TOP);
    dc->DrawLabel(hInst->DP_Date->GetValue().Format("%d/%m/20%y"), wxRect(850, 740, 300, 30), wxALIGN_LEFT | wxALIGN_TOP);

    dc->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel(wxString::Format("Page %d/%d", pageIdx, this->ImpressNum), wxRect(960, 91, 140, 29), wxALIGN_CENTRE);
    dc->DrawLabel(StringUtility::ReplaceWxString(hInst->C_JsonConfig.Posts[hInst->C_Post->GetCurrentSelection()], JsonLineSeparator, EndLineReplacer), wxRect(960, 30, 140, 61), wxALIGN_CENTRE);
    dc->DrawLabel(StringUtility::ReplaceWxString(hInst->C_JsonConfig.WorkCards[hInst->C_WorkCard->GetCurrentSelection()], JsonLineSeparator, EndLineReplacer), wxRect(680, 30, 280, 34), wxALIGN_CENTRE);
    dc->DrawLabel(StringUtility::ReplaceWxString(hInst->C_JsonConfig.Operators[hInst->C_Operator->GetCurrentSelection()], JsonLineSeparator, EndLineReplacer), wxRect(680, 64, 280, 58), wxALIGN_CENTRE);

    dc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("Produit: " + hInst->TC_Product->GetValue(), wxRect(140, 64, 270, 56), wxALIGN_CENTRE);
    dc->DrawLabel("N° " + hInst->RadomeName, wxRect(410, 64, 270, 56), wxALIGN_CENTRE);
}

void PrintGlobal::Mirage_DrawMainPv( Program* hInst, wxDC* dc, const int pageIdx )
{
    dc->SetPen(*wxBLACK_PEN);

    // Cadre Principal
    dc->DrawLine(20, 30, 1100, 30);    // Haut
    dc->DrawLine(20, 760, 1100, 760);  // Bas
    dc->DrawLine(20, 30, 20, 760);     // Gauche
    dc->DrawLine(1100, 30, 1100, 760); // Droit

    dc->DrawLine(20, 120, 1100, 120); // Separateur Bordereaux Horizontal Bas
    dc->DrawLine(140, 70, 1100, 70);  // Separateur Bordereaux Horizontal Milieu
    dc->DrawLine(140, 30, 140, 120);  // Separateur Bordereaux Vertical 0
    dc->DrawLine(680, 30, 680, 120);  // Separateur Bordereaux Vertical 1
    dc->DrawLine(900, 30, 900, 120);  // Separateur Bordereaux Vertical 2

    dc->DrawLine(800, 700, 1100, 700); // Separateur Date Horizontal
    dc->DrawLine(800, 700, 800, 760);  // Separateur Date Vertical

    dc->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("AIA/CP", wxRect(20, 32, 120, 30), wxALIGN_CENTRE);
    dc->DrawLabel("PROCES-VERBAL DE CONTROLE", wxRect(140, 32, 540, 40), wxALIGN_CENTRE);

    dc->SetFont(wxFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel(L"****\nDépartement\nde la Qualité\net du Contrôle", wxRect(20, 60, 120, 60), wxALIGN_CENTRE);
    dc->DrawLabel(L"Date de contrôle / Marque", wxRect(805, 704, 300, 30), wxALIGN_LEFT | wxALIGN_TOP);
    dc->DrawLabel(hInst->DP_Date->GetValue().Format("%d/%m/20%y"), wxRect(850, 720, 300, 30), wxALIGN_LEFT | wxALIGN_TOP);

    dc->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel(wxString::Format("Page %d/%d", pageIdx, this->ImpressNum), wxRect(900, 70, 200, 50), wxALIGN_CENTRE);

    dc->SetTextForeground(wxColour(224, 102, 102)); // Red
    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc->DrawLabel(L"Ce document est la propriété de l'AIA-CP, il ne peut être reproduit ou communiqué sans autorisation écrite.", wxRect(20, 760, 1080, 30), wxALIGN_CENTRE);
    dc->SetTextForeground(wxColour(0, 0, 0)); // Black
}
