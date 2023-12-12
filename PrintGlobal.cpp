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

    for (auto& kv : hInst->ActiveProductData.TheoreticalRadius)
    {
        const std::vector<double>& valeurs = kv.second; // Accès direct à la valeur, qui est un std::vector<float>
        float somme = 0.0;
        for (double valeur : valeurs)
        {
            somme += valeur;
        }
        float moyenne = valeurs.empty() ? 0.0f : somme / static_cast<double>(valeurs.size()); // Vérification de la division par zéro et cast pour la taille
        std::cout << "Moyenne pour la clé " << kv.first << ": " << moyenne << std::endl; // Utilisation de .first pour obtenir la clé
    }

    // Cadre Principal
    dc->DrawLine(20, 30, 1100, 30);    // Haut
    dc->DrawLine(20, 760, 1100, 760);  // Bas
    dc->DrawLine(20, 30, 20, 760);     // Gauche
    dc->DrawLine(1100, 30, 1100, 760); // Droit

    dc->DrawLine(20, 138, 1100, 138); // Separateur Bordereaux Horizontal Bas
    dc->DrawLine(140, 66, 1100, 66);  // Separateur Bordereaux Horizontal Milieu
    dc->DrawLine(960, 102, 1100, 102);  // Separateur Bordereaux Horizontal Milieu Petit
    dc->DrawLine(140, 30, 140, 138);  // Separateur Bordereaux Vertical 0
    dc->DrawLine(680, 30, 680, 138);  // Separateur Bordereaux Vertical 1
    dc->DrawLine(960, 30, 960, 138);  // Separateur Bordereaux Vertical 2

    dc->DrawLine(800, 720, 1100, 720); // Separateur Signature Horizontal
    dc->DrawLine(800, 720, 800, 760);  // Separateur Signature Vertical

    dc->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("AIA/CP", wxRect(20, 30, 120, 36), wxALIGN_CENTRE);
    dc->DrawLabel("PROCES-VERBAL DE CONTROLE", wxRect(140, 30, 540, 36), wxALIGN_CENTRE);

    dc->SetFont(wxFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel(L"****", wxRect(20, 66, 120, 22), wxALIGN_CENTRE);
    dc->DrawLabel(L"Département\nde la Qualité\net du Contrôle", wxRect(20, 80, 120, 58), wxALIGN_CENTRE);
    dc->DrawLabel(L"Date de contrôle / Marque", wxRect(805, 724, 300, 30), wxALIGN_LEFT | wxALIGN_TOP);
    dc->DrawLabel(hInst->DP_Date->GetValue().Format("%d/%m/20%y"), wxRect(850, 740, 300, 30), wxALIGN_LEFT | wxALIGN_TOP);

    dc->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel(wxString::Format("Page %d/%d", pageIdx, this->ImpressNum), wxRect(960, 102, 140, 36), wxALIGN_CENTRE);
    dc->DrawLabel(L"Ind. " + hInst->ActiveProductData.ModificationIdx, wxRect(960, 30, 140, 36), wxALIGN_CENTRE);
    dc->DrawLabel(StringUtility::ReplaceWxString(hInst->C_JsonConfig.Posts[hInst->C_Post->GetCurrentSelection()], JsonLineSeparator, EndLineReplacer), wxRect(960, 66, 140, 36), wxALIGN_CENTRE);
    dc->DrawLabel(StringUtility::ReplaceWxString(hInst->C_JsonConfig.WorkCards[hInst->C_WorkCard->GetCurrentSelection()], JsonLineSeparator, EndLineReplacer), wxRect(680, 30, 280, 36), wxALIGN_CENTRE);
    dc->DrawLabel(StringUtility::ReplaceWxString(hInst->C_JsonConfig.Operators[hInst->C_Operator->GetCurrentSelection()], JsonLineSeparator, EndLineReplacer), wxRect(680, 66, 280, 72), wxALIGN_CENTRE);

    dc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel("Produit: " + hInst->TC_Product->GetValue(), wxRect(140, 66, 270, 72), wxALIGN_CENTRE);
    dc->DrawLabel("N° " + hInst->RadomeName, wxRect(410, 66, 270, 72), wxALIGN_CENTRE);
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
    dc->DrawLabel("Indice", wxRect(900, 30, 200, 20), wxALIGN_CENTRE);
    dc->DrawLabel(hInst->ActiveProductData.ModificationIdx, wxRect(900, 50, 200, 20), wxALIGN_CENTRE);
    dc->DrawLabel(wxString::Format("Page %d/%d", pageIdx, this->ImpressNum), wxRect(900, 70, 200, 50), wxALIGN_CENTRE);

    dc->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc->DrawLabel("Code opération", wxRect(680, 32, 220, 14), wxALIGN_CENTRE);
    dc->DrawLabel(hInst->ActiveProductData.OperationCode, wxRect(680, 44, 220, 26), wxALIGN_CENTRE);
    dc->DrawLabel("Approuvé par", wxRect(680, 72, 220, 10), wxALIGN_CENTRE);
    dc->DrawLabel(StringUtility::ReplaceWxString(hInst->C_JsonConfig.Operators[hInst->C_Operator->GetCurrentSelection()], JsonLineSeparator, EndLineReplacer), wxRect(680, 80, 220, 40), wxALIGN_CENTRE);

    dc->DrawLabel("Ensemble:", wxRect(144, 70, 136, 25), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    dc->DrawLabel("Sous ensemble:", wxRect(144, 95, 136, 25), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    dc->DrawLabel("N° " + hInst->RadomeName, wxRect(480, 70, 200, 25), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    dc->DrawLabel("N° " + hInst->RadomeName, wxRect(480, 95, 200, 25), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    dc->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc->DrawLabel(hInst->TC_Product->GetValue(), wxRect(280, 70, 200, 25), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    dc->DrawLabel("Radôme", wxRect(280, 95, 200, 25), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    dc->SetTextForeground(wxColour(224, 102, 102)); // Red
    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc->DrawLabel(L"Ce document est la propriété de l'AIA-CP, il ne peut être reproduit ou communiqué sans autorisation écrite.", wxRect(20, 760, 1080, 30), wxALIGN_CENTRE);

    dc->SetTextForeground(wxColour(0, 0, 0)); // Black
    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL));
    dc->DrawLabel(L"Annexe 1 à P/CP/E028 Ind A", wxRect(20, 10, 1080, 20), wxALIGN_CENTRE);
}
