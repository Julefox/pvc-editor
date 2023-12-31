﻿#include "PrintGlobal.h"

#include "main.h"

bool PrintGlobal::OnBeginDocument( const int startPage, const int endPage )
{
	if ( !wxPrintout::OnBeginDocument( startPage, endPage ) )
		return false;

	return true;
}

void PrintGlobal::GetPageInfo( int* minPage, int* maxPage, int* pageFrom, int* pageTo )
{
	*minPage  = 1;
	*maxPage  = ImpressNum;
	*pageFrom = 1;
	*pageTo   = ImpressNum;
}

bool PrintGlobal::HasPage( const int page )
{
	return ( page >= 1 && page <= ImpressNum );
}

void PrintGlobal::SetToleranceColor( wxDC* dc, const double value, const double tolerance )
{
	if ( MathUtility::RoundValue( fabs( value ), 2 ) > tolerance )
	{
		dc->SetTextForeground( RedColor );
	}
	else
	{
		dc->SetTextForeground( BlueColor );
	}
}

void PrintGlobal::SetDcScale( wxPrintout* print, wxDC* dc )
{
	// You might use THIS code to set the printer DC to roughly
	// reflect the screen text size. This page also draws lines of
	// actual length 5cm on the page.

	// Get the logical pixels per inch of screen and printer
	int ppiScreenX, ppiScreenY;
	print->GetPPIScreen( &ppiScreenX, &ppiScreenY );
	int ppiPrinterX, ppiPrinterY;
	print->GetPPIPrinter( &ppiPrinterX, &ppiPrinterY );

	// This scales the DC so that the printout roughly represents the
	// the screen scaling.
	float scale = static_cast < float >( ppiPrinterX ) / static_cast < float >( ppiScreenX );

	// Now we have to check in case our real page size is reduced
	// (e.g. because we're drawing to a print preview memory DC)
	int pageWidth, pageHeight;
	int w, h;
	dc->GetSize( &w, &h );
	print->GetPageSizePixels( &pageWidth, &pageHeight );

	// If printer pageWidth == current DC width, then this doesn't
	// change. But w might be the preview bitmap width,
	// so scale down.
	float overallScale = scale * ( w / static_cast < float >( pageWidth ) );
	dc->SetUserScale( overallScale, overallScale );

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

void PrintGlobal::Rafale_DrawMainHeader( Program* hInst, wxDC* dc, const int pageIdx )
{
	dc->SetPen( *wxBLACK_PEN );

	// Cadre Principal
	dc->DrawLine( 20, 30, 1100, 30 );    // Haut
	dc->DrawLine( 20, 760, 1100, 760 );  // Bas
	dc->DrawLine( 20, 30, 20, 760 );     // Gauche
	dc->DrawLine( 1100, 30, 1100, 760 ); // Droit

	dc->DrawLine( 20, 138, 1100, 138 );  // Separateur Bordereaux Horizontal Bas
	dc->DrawLine( 140, 66, 1100, 66 );   // Separateur Bordereaux Horizontal Milieu
	dc->DrawLine( 960, 102, 1100, 102 ); // Separateur Bordereaux Horizontal Milieu Petit
	dc->DrawLine( 140, 30, 140, 138 );   // Separateur Bordereaux Vertical 0
	dc->DrawLine( 680, 30, 680, 138 );   // Separateur Bordereaux Vertical 1
	dc->DrawLine( 960, 30, 960, 138 );   // Separateur Bordereaux Vertical 2

	dc->DrawLine( 800, 700, 1100, 700 ); // Separateur Signature Horizontal
	dc->DrawLine( 800, 700, 800, 760 );  // Separateur Signature Vertical

	dc->SetFont( wxFont( 16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( "AIA/CP", wxRect( 20, 30, 120, 36 ), wxALIGN_CENTRE );

	const wxString title = hInst->ActiveProductData.RadomeType == Rafale_C
		                       ? "PROCES-VERBAL DE CONTROLE"
		                       : "FEUILLE DE RELEVE";

	dc->DrawLabel( title, wxRect( 140, 30, 540, 36 ), wxALIGN_CENTRE );

	dc->SetFont( wxFont( 9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( L"****", wxRect( 20, 66, 120, 22 ), wxALIGN_CENTRE );
	dc->DrawLabel( L"Département\nde la Qualité\net du Contrôle", wxRect( 20, 80, 120, 58 ), wxALIGN_CENTRE );
	dc->DrawLabel( L"Date de contrôle / Marque", wxRect( 805, 704, 300, 30 ), wxALIGN_LEFT | wxALIGN_TOP );

	dc->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( hInst->DP_Date->GetValue().Format( "%d/%m/20%y" ), wxRect( 820, 728, 300, 30 ), wxALIGN_LEFT | wxALIGN_TOP );

	dc->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( wxString::Format( "Page %d/%d", pageIdx, this->ImpressNum ), wxRect( 960, 102, 140, 36 ), wxALIGN_CENTRE );
	dc->DrawLabel( L"Ind. " + hInst->C_OperationCode->GetString( hInst->C_OperationCode->GetCurrentSelection() ).AfterLast( '_' ), wxRect( 960, 30, 140, 36 ), wxALIGN_CENTRE );
	dc->DrawLabel( StringUtility::ReplaceWxString( hInst->C_JsonConfig.Posts[ hInst->C_Post->GetCurrentSelection() ], JsonLineSeparator, EndLineReplacer ), wxRect( 960, 66, 140, 36 ), wxALIGN_CENTRE );
	dc->DrawLabel( hInst->C_WorkCard->GetString( hInst->C_WorkCard->GetCurrentSelection() ) + "\n" + hInst->C_OperationCode->GetString( hInst->C_OperationCode->GetCurrentSelection() ).BeforeLast( '_' ), wxRect( 680, 30, 280, 36 ), wxALIGN_CENTRE );
	dc->DrawLabel( StringUtility::ReplaceWxString( hInst->C_JsonConfig.Operators[ hInst->C_Operator->GetCurrentSelection() ], JsonLineSeparator, EndLineReplacer ), wxRect( 680, 66, 280, 72 ), wxALIGN_CENTRE );

	dc->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	if ( hInst->ActiveProductData.RadomeType == Rafale_C )
	{
		dc->DrawLabel( "Ensemble:", wxRect( 150, 66, 200, 36 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "N° " + hInst->TC_Shell->GetValue(), wxRect( 550, 66, 200, 36 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "Sous ensemble:", wxRect( 150, 102, 200, 36 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "N° " + hInst->TC_Radome->GetValue(), wxRect( 550, 102, 200, 36 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( hInst->C_Product->GetString( hInst->C_Product->GetCurrentSelection() ), wxRect( 350, 66, 200, 36 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "Radôme", wxRect( 350, 102, 200, 36 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
	}
	else
	{
		dc->DrawLabel( "Ensemble:", wxRect( 150, 66, 200, 72 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( hInst->C_Product->GetString( hInst->C_Product->GetCurrentSelection() ), wxRect( 350, 66, 200, 72 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "N° " + hInst->TC_Radome->GetValue(), wxRect( 550, 66, 200, 72 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
	}

	dc->SetTextForeground( RedColor );
	dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
	dc->DrawLabel( Copyright, wxRect( 20, 760, 1080, 30 ), wxALIGN_CENTRE );

	dc->SetTextForeground( BlackColor );
	dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
	dc->DrawLabel( StringUtility::StringToWString( hInst->C_JsonConfig.RafaleAnnexHeader ), wxRect( 20, 10, 1080, 20 ), wxALIGN_CENTRE );
}

void PrintGlobal::Mirage_DrawMainHeader( Program* hInst, wxDC* dc, const int pageIdx )
{
	dc->SetPen( *wxBLACK_PEN );

	// Cadre Principal
	dc->DrawLine( 20, 30, 1100, 30 );    // Haut
	dc->DrawLine( 20, 760, 1100, 760 );  // Bas
	dc->DrawLine( 20, 30, 20, 760 );     // Gauche
	dc->DrawLine( 1100, 30, 1100, 760 ); // Droit

	dc->DrawLine( 20, 120, 1100, 120 ); // Separateur Bordereaux Horizontal Bas
	dc->DrawLine( 140, 70, 1100, 70 );  // Separateur Bordereaux Horizontal Milieu
	dc->DrawLine( 140, 30, 140, 120 );  // Separateur Bordereaux Vertical 0
	dc->DrawLine( 680, 30, 680, 120 );  // Separateur Bordereaux Vertical 1
	dc->DrawLine( 900, 30, 900, 120 );  // Separateur Bordereaux Vertical 2

	dc->DrawLine( 800, 700, 1100, 700 ); // Separateur Date Horizontal
	dc->DrawLine( 800, 700, 800, 760 );  // Separateur Date Vertical

	dc->SetFont( wxFont( 16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( "AIA/CP", wxRect( 20, 32, 120, 30 ), wxALIGN_CENTRE );

	const wxString title = hInst->ActiveProductData.RadomeType == Mirage_C
		                       ? "PROCES-VERBAL DE CONTROLE"
		                       : "FEUILLE DE RELEVE";

	dc->DrawLabel( title, wxRect( 140, 32, 540, 40 ), wxALIGN_CENTRE );

	dc->SetFont( wxFont( 9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( L"****\nDépartement\nde la Qualité\net du Contrôle", wxRect( 20, 60, 120, 60 ), wxALIGN_CENTRE );
	dc->DrawLabel( L"Date de contrôle / Marque", wxRect( 805, 704, 300, 30 ), wxALIGN_LEFT | wxALIGN_TOP );

	dc->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( hInst->DP_Date->GetValue().Format( "%d/%m/20%y" ), wxRect( 820, 728, 300, 30 ), wxALIGN_LEFT | wxALIGN_TOP );

	dc->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( "Indice", wxRect( 900, 30, 200, 20 ), wxALIGN_CENTRE );
	dc->DrawLabel( hInst->C_OperationCode->GetString( hInst->C_OperationCode->GetCurrentSelection() ).AfterLast( '_' ), wxRect( 900, 50, 200, 20 ), wxALIGN_CENTRE );
	dc->DrawLabel( wxString::Format( "Page %d/%d", pageIdx, this->ImpressNum ), wxRect( 900, 70, 200, 50 ), wxALIGN_CENTRE );
	dc->DrawLabel( "Code opération", wxRect( 680, 32, 220, 14 ), wxALIGN_CENTRE );
	dc->DrawLabel( hInst->C_OperationCode->GetString( hInst->C_OperationCode->GetCurrentSelection() ).BeforeLast( '_' ), wxRect( 680, 44, 220, 26 ), wxALIGN_CENTRE );
	dc->DrawLabel( "Approuvé par", wxRect( 680, 72, 220, 10 ), wxALIGN_CENTRE );
	dc->DrawLabel( StringUtility::ReplaceWxString( hInst->C_JsonConfig.Operators[ hInst->C_Operator->GetCurrentSelection() ], JsonLineSeparator, EndLineReplacer ), wxRect( 680, 80, 220, 40 ), wxALIGN_CENTRE );

	dc->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	if ( hInst->ActiveProductData.RadomeType == Mirage_C )
	{
		dc->DrawLabel( "Ensemble:", wxRect( 144, 70, 136, 25 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "N° " + hInst->TC_Shell->GetValue(), wxRect( 480, 70, 200, 25 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "Sous ensemble:", wxRect( 144, 95, 136, 25 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "N° " + hInst->TC_Radome->GetValue(), wxRect( 480, 95, 200, 25 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( hInst->C_Product->GetString( hInst->C_Product->GetCurrentSelection() ), wxRect( 280, 70, 200, 25 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "Radôme", wxRect( 280, 95, 200, 25 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
	}
	else
	{
		dc->DrawLabel( "Ensemble:", wxRect( 144, 70, 136, 50 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( hInst->C_Product->GetString( hInst->C_Product->GetCurrentSelection() ), wxRect( 280, 70, 200, 50 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
		dc->DrawLabel( "N° " + hInst->TC_Radome->GetValue(), wxRect( 480, 70, 200, 50 ), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL );
	}

	dc->SetTextForeground( RedColor );
	dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
	dc->DrawLabel( Copyright, wxRect( 20, 760, 1080, 30 ), wxALIGN_CENTRE );

	dc->SetTextForeground( BlackColor );
	dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
	dc->DrawLabel( StringUtility::StringToWString( hInst->C_JsonConfig.MirageAnnexHeader ), wxRect( 20, 10, 1080, 20 ), wxALIGN_CENTRE );
}

void PrintGlobal::DrawArray( wxDC* dc, const int x, const int y, const int w, const int wIdx, const int h, const int hIdx, const std::vector < wxString >& headers )
{
	const int headerLength = static_cast < int >( headers.size() );

	for ( int i = 0; i <= wIdx; i++ )
	{
		dc->DrawLine( x + w * i, y, x + w * i, y + h * hIdx );

		if ( i < headerLength && i < wIdx )
		{
			dc->DrawLabel( headers[ i ], wxRect( x + w * i, y, w, h ), wxALIGN_CENTRE );
		}
	}

	for ( int i = 0; i <= hIdx; i++ )
	{
		dc->DrawLine( x, y + h * i, x + w * wIdx, y + h * i );
	}
}

void PrintGlobal::DrawRectangle( wxDC* dc, const int x, const int y, const int w, const int h, const wxString& title, const bool absoluteCoords )
{
	const int x_end = absoluteCoords
		                  ? w
		                  : x + w;
	const int y_end = absoluteCoords
		                  ? h
		                  : y + h;
	const int x_t_end = absoluteCoords
		                    ? w - x
		                    : w;
	const int y_t_end = absoluteCoords
		                    ? h - y
		                    : h;

	dc->DrawLine( x, y, x_end, y );         // Haut
	dc->DrawLine( x, y_end, x_end, y_end ); // Bas
	dc->DrawLine( x, y, x, y_end );         // Gauche
	dc->DrawLine( x_end, y, x_end, y_end ); // Droit

	if ( !title.empty() )
	{
		dc->DrawLabel( title, wxRect( x, y, x_t_end, y_t_end ), wxALIGN_CENTRE );
	}
}

void PrintGlobal::DrawWhiteLabel(wxDC* dc, const int x, const int y, const int w, const int h, const wxString& title, const bool absoluteCoords)
{
	const int x_t_end = absoluteCoords
		? w - x
		: w;
	const int y_t_end = absoluteCoords
		? h - y
		: h;

	const wxRect rect(x, y, x_t_end, y_t_end);

	dc->SetBrush(*wxWHITE_BRUSH);
	dc->SetPen(*wxWHITE_PEN);
	dc->DrawRectangle(rect);
	dc->DrawLabel(title, rect, wxALIGN_CENTER);
}
