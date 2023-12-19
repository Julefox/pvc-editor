#include "PrintMirage.h"

#include "main.h"

void PrintMirage::DrawMiragePage( wxWindow* hInst )
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
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 1 );

// DEBUG
	Mirage_DrawGraphic(hInst, dc, "HG", AND_HG);
	return;
//

	dc->SetFont( wxFont( 11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, wxFONTFLAG_UNDERLINED ) );
	dc->DrawLabel( "Vérification géométrique", wxRect( 20, 130, 1080, 30 ), wxALIGN_CENTER );
	dc->SetFont( wxFont( 9, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD ) );
	dc->DrawLabel( "NE PAS JOINDRE AU RIC", wxRect( 20, 160, 1080, 30 ), wxALIGN_CENTER );

	dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	DrawRectangle( dc, 900, 130, 180, 30, hInst->C_WorkCard->GetString( hInst->C_WorkCard->GetCurrentSelection() ) );

	constexpr int case_h_small = 30, case_h_large = 60, case_w = 80, x_start = 40, y_start = 210;

	dc->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	DrawRectangle( dc, x_start, y_start, 1040, 40, "TABLEAU DES ECARTS GENERATRICES" );

	const std::vector < wxString > headers = { "Section", "Haut.", "Rayon T.", "Tol.", "H", "HD", "D", "BD", "B", "BG", "G", "HG", "Section" };

	int x = x_start, y = y_start + 40;

	dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	DrawArray( dc, x, y, case_w, 13, case_h_small, 12, headers );
	DrawArray( dc, x, y + case_h_small * 12, case_w, 13, case_h_large, 1 );

	y += case_h_small;

	for ( int i = 12; i > 0; i-- )
	{
		CalculateHeight( hInst, i );
		CalculateTheoreticalRadius( hInst, i );

		const int section = 12 - i;
		const int size    = section == 11
			                    ? case_h_large
			                    : case_h_small;
		const wxString sectionName = section == 0
			                             ? "F"
			                             : std::to_string( section );
		const wxString radius = section == 11
			                        ? headers[ 2 ]
			                        : wxString::Format( "%.2f", TheoreticalRadius );

		dc->DrawLabel( sectionName, wxRect( x_start, y, case_w, size ), wxALIGN_CENTER );                                                                         // 1ere Colonne
		dc->DrawLabel( wxString::Format( "%.2f", Height ), wxRect( x_start + case_w, y, case_w, size ), wxALIGN_CENTER );                                         // 2e Colonne
		dc->DrawLabel( radius, wxRect( x_start + case_w * 2, y, case_w, size ), wxALIGN_CENTER );                                                                 // 3e Colonne
		dc->DrawLabel( wxString::Format( "± %.1f", hInst->ActiveProductData.RadiusTolerance ), wxRect( x_start + case_w * 3, y, case_w, size ), wxALIGN_CENTER ); // 4e Colonne
		dc->DrawLabel( sectionName, wxRect( x_start + case_w * 12, y, case_w, size ), wxALIGN_CENTER );                                                           // 13e Colonne

		for ( int j = 0; j < 8; j++ )
		{
			const eSideType side      = CalculationData::GetMirageSide( j );
			const PointMeasure& point = hInst->Calculation.PointData[ i ][ side ];

			if ( std::abs( point.RayDifference - UnassignedDoubleValue ) > Epsilon && !( ( side == AND_BD && section > 9 ) || ( side == AND_B && section > 8 ) || ( side == AND_BG && section > 9 ) ) )
			{
				if ( section != 11 )
				{
					SetToleranceColor( dc, point.RayDifference, hInst->ActiveProductData.RadiusTolerance );
					dc->DrawLabel( wxString::Format( "%.2f", point.RayDifference ), wxRect( x_start + case_w * ( side + 4 ), y, case_w, size ), wxALIGN_CENTER );
				}
				else
				{
					dc->SetTextForeground( BlackColor );
					dc->DrawLabel( wxString::Format( "%.2f", hInst->ActiveProductData.TheoreticalRadius[ point.Height ][ side ] ), wxRect( x_start + case_w * ( side + 4 ), y, case_w, case_h_small ), wxALIGN_CENTER );
					SetToleranceColor( dc, point.RayDifference, hInst->ActiveProductData.RadiusTolerance );
					dc->DrawLabel( wxString::Format( "%.2f", point.RayDifference ), wxRect( x_start + case_w * ( side + 4 ), y + case_h_small, case_w, case_h_small ), wxALIGN_CENTER );
				}
			}
			else
			{
				dc->GradientFillLinear( wxRect( x_start + case_w * ( side + 4 ), y, case_w, size ), GrayColor, GrayColor );
			}
		}

		dc->SetTextForeground( BlackColor );

		y += case_h_small;
	}
}

void PrintMirage::MiragePage_02()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 2 );

	constexpr int case_h = 32, case_w = 94, x_start = 43, y_start = 210;

	dc->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	DrawRectangle( dc, x_start, y_start, 1034, 40, "TABLEAU DES ECARTS GENERATRICES" );

	const std::vector < wxString > headers = { "Section", "Tol.", "H", "HD", "D", "BD", "B", "BG", "G", "HG", "Haut." };

	int x = x_start, y = y_start + 40;

	dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	DrawArray( dc, x, y, case_w, 11, case_h, 12, headers );
	DrawRectangle( dc, x + case_w * 10, y + case_h * 12, case_w, case_h );

	y += case_h;

	for ( int i = 12; i > 0; i-- )
	{
		CalculateHeight( hInst, i );
		CalculateTheoreticalRadius( hInst, i );

		const int section = 12 - i;

		dc->DrawLabel( wxString::Format( "%.2f", Height ), wxRect( x_start + case_w * 10, y, case_w, case_h ), wxALIGN_CENTER ); // 11e Colonne

		if ( i > 1 )
		{
			if ( section == 0 )
			{
				dc->DrawLabel( wxString::Format( "F à %i", section + 1 ), wxRect( x_start, y, case_w, case_h ), wxALIGN_CENTER );
			}
			else
			{
				dc->DrawLabel( wxString::Format( "%i à %i", section, section + 1 ), wxRect( x_start, y, case_w, case_h ), wxALIGN_CENTER );
			}

			const auto sectionType           = static_cast < eSectionType >( section );
			const double undulationTolerance = hInst->ActiveProductData.UndulationToleranceSection[ sectionType ];

			dc->DrawLabel( wxString::Format( "%.2f ", undulationTolerance ) + "%", wxRect( x_start + case_w, y, case_w, case_h ), wxALIGN_CENTER );

			for ( int j = 0; j < 8; j++ )
			{
				const eSideType side      = CalculationData::GetMirageSide( j );
				const PointMeasure& point = hInst->Calculation.PointData[ i - 1 ][ side ];

				if ( std::abs( point.Undulation - UnassignedDoubleValue ) > Epsilon && !( ( side == AND_BD && section > 8 ) || ( side == AND_B && section > 7 ) || ( side == AND_BG && section > 8 ) ) )
				{
					SetToleranceColor( dc, point.Undulation, undulationTolerance );
					dc->DrawLabel( wxString::Format( "%.2f ", point.Undulation ) + "%", wxRect( x_start + case_w * ( side + 2 ), y, case_w, case_h ), wxALIGN_CENTER );
				}
				else
				{
					dc->GradientFillLinear( wxRect( x_start + case_w * ( side + 2 ), y, case_w, case_h ), GrayColor, GrayColor );
				}
			}
		}

		dc->SetTextForeground( BlackColor );

		y += case_h;
	}
}

void PrintMirage::MiragePage_03()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 3 );
	Mirage_DrawGraphic( hInst, dc, "H", AND_H );
	Mirage_DrawGraphicStat( hInst, dc, AND_H );
}

void PrintMirage::MiragePage_04()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 4 );
	Mirage_DrawGraphic( hInst, dc, "HD", AND_HD );
	Mirage_DrawGraphicStat( hInst, dc, AND_HD );
}

void PrintMirage::MiragePage_05()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 5 );
	Mirage_DrawGraphic( hInst, dc, "D", AND_D );
	Mirage_DrawGraphicStat( hInst, dc, AND_D );
}

void PrintMirage::MiragePage_06()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 6 );
	Mirage_DrawGraphic( hInst, dc, "BD", AND_BD );
	Mirage_DrawGraphicStat( hInst, dc, AND_BD );
}

void PrintMirage::MiragePage_07()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 7 );
	Mirage_DrawGraphic( hInst, dc, "B", AND_B );
	Mirage_DrawGraphicStat( hInst, dc, AND_B );
}

void PrintMirage::MiragePage_08()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 8 );
	Mirage_DrawGraphic( hInst, dc, "BG", AND_BG );
	Mirage_DrawGraphicStat( hInst, dc, AND_BG );
}

void PrintMirage::MiragePage_09()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 9 );
	Mirage_DrawGraphic( hInst, dc, "G", AND_G );
	Mirage_DrawGraphicStat( hInst, dc, AND_G );
}

void PrintMirage::MiragePage_10()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawMainHeader( hInst, dc, 10 );
	Mirage_DrawGraphic( hInst, dc, "HG", AND_HG );
	Mirage_DrawGraphicStat( hInst, dc, AND_HG );
}

constexpr int X_START_VALUE = 0, X_END_VALUE = 2120, Y_START_VALUE = -2, Y_END_VALUE = 2;

void PrintMirage::Mirage_DrawGraphic( Program* hInst, wxDC* dc, const wxString& gen, const eSideType side )
{
	const wxPen pBigBlack( wxColour( 0, 0, 0 ), 3 );
	const wxPen pRed( wxColour( 224, 102, 102 ) );
	const wxPen pDotBlue( *wxBLUE, 1, wxPENSTYLE_DOT );

	std::map<eSectionType, int> xSectionPos;

	dc->SetPen( *wxBLACK_PEN );
	dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	DrawRectangle( dc, 120, 220, 880, 440 );
	DrawWhiteLabel( dc, 142, 205, 64, 30, L"Ecart en mm" );

	dc->SetPen(pBigBlack);

	constexpr int frame_x_start = 180, frame_x_length = 800, frame_y_start = 260, frame_y_length = 360, frame_y_tolerance_start = frame_y_start - 60, frame_y_tolerance_length = frame_y_length + 120;

	DrawRectangle( dc, frame_x_start, frame_y_start, frame_x_length, frame_y_length);

	dc->SetPen(BlackColor);

	for ( const auto& keyVal : hInst->ActiveProductData.GraphicData[ side ] )
	{
		eSectionType section = keyVal.first;
		const GraphicData graphicData = keyVal.second;

		dc->SetPen(BlackColor);
		const int x = frame_x_start + static_cast < int >( ( graphicData.Height - X_START_VALUE ) * frame_x_length / ( X_END_VALUE - X_START_VALUE ) );
		dc->DrawLine(x, frame_y_start, x, frame_y_start + frame_y_length);
		xSectionPos[ section ] = x;
		DrawWhiteLabel(dc, x - 6, frame_y_start + frame_y_length - 6, 12, 12, section == AND_F ? "F" : std::to_string( section ) );

		if ( graphicData.DeltaHeight != 0.0f )
		{
			dc->SetPen(pDotBlue);
			const int deltaX = frame_x_start + static_cast <int>((graphicData.DeltaHeight - X_START_VALUE) * frame_x_length / (X_END_VALUE - X_START_VALUE));

			const int y = frame_y_tolerance_start + static_cast <int>((graphicData.DeltaMin - graphicData.DeltaMin) * frame_y_tolerance_length / (graphicData.DeltaMax - graphicData.DeltaMin));

			dc->DrawLine(x, frame_y_start + frame_y_length / 2, FindXForY(frame_y_start, x, frame_y_start + frame_y_length / 2, deltaX, y), frame_y_start);
			dc->DrawLine(x, frame_y_start + frame_y_length / 2, FindXForY(frame_y_start, x, frame_y_start + frame_y_length / 2, deltaX, y), frame_y_start + frame_y_length);
		}
	}

	dc->SetPen(BlackColor);

	int y = 290;
	for (int i = 0; i < 11; i++)
	{
		dc->DrawLine(180, y, 980, y);
		if (i == 1 || i == 9)
		{
			dc->SetPen(pRed);
			dc->DrawLine(xSectionPos[AND_F], y, xSectionPos[AND_11], y);
			dc->SetPen(*wxBLACK_PEN);
		}
		y += 30;
	}

	y = 245;
	for ( double i = 1.50f; i > -2.00f; i -= 0.50f )
	{
		dc->DrawLabel( wxString::Format( wxT( "%.2f" ), i ), wxRect( 120, y, 60, 30 ), wxALIGN_CENTRE );
		y += 60;
	}

	///////////////////////////

	//// Cadre Principal
	//dc->DrawLine( 120, 220, 140, 220 );   // Haut 1
	//dc->DrawLine( 208, 220, 1000, 220 );  // Haut 2
	//dc->DrawLine( 120, 660, 1000, 660 );  // Bas
	//dc->DrawLine( 120, 220, 120, 660 );   // Gauche
	//dc->DrawLine( 1000, 220, 1000, 660 ); // Droit
	//
	//dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
	//dc->DrawLabel( L"Ecart en mm", wxRect( 142, 205, 64, 30 ), wxALIGN_CENTRE );
	//
	//// PM: 260 == Haut Y | 620 == Bas Y | 440 == Centre Y | 180 == Cote Gauche X | 1000 == Cote Droit X
	//// Ligne de tolerance, de gauche a droite
	//dc->SetPen( pDotBlue );
	//dc->DrawLine( 200, 440, 385, 260 ); // F
	//dc->DrawLine( 200, 440, 376, 610 ); // F // Leger offset pour ne pas cacher le chiffre 3
	//
	//dc->DrawLine( 230, 440, 416, 260 ); // 1
	//dc->DrawLine( 230, 440, 416, 620 ); // 1
	//
	//dc->DrawLine( 303, 440, 489, 260 ); // 2
	//dc->DrawLine( 303, 440, 489, 620 ); // 2
	//
	//dc->DrawLine( 376, 440, 562, 260 ); // 3
	//dc->DrawLine( 376, 440, 562, 620 ); // 3
	//
	//dc->DrawLine( 449, 440, 635, 260 ); // 4
	//dc->DrawLine( 449, 440, 635, 620 ); // 4
	//
	//dc->DrawLine( 522, 440, 708, 260 ); // 5
	//dc->DrawLine( 522, 440, 708, 620 ); // 5
	//
	//dc->DrawLine( 595, 440, 781, 260 ); // 6
	//dc->DrawLine( 595, 440, 781, 620 ); // 6
	//
	//dc->DrawLine( 668, 440, 854, 260 ); // 7
	//dc->DrawLine( 668, 440, 854, 620 ); // 7
	//
	//if ( gen != "B" )
	//{
	//	dc->DrawLine( 741, 440, 834, 260 ); // 8
	//	dc->DrawLine( 741, 440, 834, 620 ); // 8
	//
	//	dc->DrawLine( 814, 440, 907, 260 ); // 9
	//	dc->DrawLine( 814, 440, 907, 620 ); // 9
	//
	//	dc->DrawLine( 887, 440, 980, 260 ); // 10
	//	dc->DrawLine( 887, 440, 980, 620 ); // 10
	//}
	//
	//// Cadre Tableau
	//dc->SetPen( pBigBlack );
	//dc->DrawLine( 180, 260, 980, 260 ); // Haut
	//dc->DrawLine( 180, 260, 180, 620 ); // Gauche
	//dc->DrawLine( 980, 260, 980, 620 ); // Droit
	//
	//// Bas
	//dc->DrawLine( 180, 620, 190, 620 );
	//dc->DrawLabel( L"F", wxRect( 190, 610, 20, 20 ), wxALIGN_CENTRE );
	//dc->SetPen( *wxBLACK_PEN );
	//dc->DrawLine( 200, 260, 200, 610 );
	//dc->SetPen( pBigBlack );
	//dc->DrawLine( 210, 620, 220, 620 );
	//
	//int x = 220;
	//for ( int i = 1; i < 11; i++ )
	//{
	//	dc->DrawLabel( wxString::Format( wxT( "%d" ), i ), wxRect( x, 610, 20, 20 ), wxALIGN_CENTRE ); // Affiche les chiffres en bas
	//	x += 10;
	//	dc->SetPen( *wxBLACK_PEN );
	//	dc->DrawLine( x, 260, x, 610 ); // Ligne verticale au dessus des chiffres
	//	dc->SetPen( pBigBlack );
	//	x += 10;
	//	const int forward = i == 10
	//		                    ? x + 31
	//		                    : x + 53;
	//	dc->DrawLine( x, 620, forward, 620 ); // Ligne du bas entre chaque chiffres
	//	x = forward;
	//
	//	if ( i == 8 && gen == "B" )
	//		break;
	//}
	//
	//if ( gen != "B" )
	//{
	//	dc->DrawLabel( L"11", wxRect( 928, 610, 20, 20 ), wxALIGN_CENTRE );
	//	dc->SetPen( *wxBLACK_PEN );
	//	dc->DrawLine( 938, 260, 938, 610 );
	//	dc->SetPen( pBigBlack );
	//	dc->DrawLine( 950, 620, 980, 620 );
	//}
	//else
	//{
	//	dc->DrawLine( 800, 620, 980, 620 );
	//}
	//
	//dc->SetPen( *wxBLACK_PEN );
	//
	//// Valeur cote gauche
	//int y = 245;
	//for ( float i = 1.50f; i > -2.00f; i -= 0.50f )
	//{
	//	dc->DrawLabel( wxString::Format( wxT( "%.2f" ), i ), wxRect( 120, y, 60, 30 ), wxALIGN_CENTRE );
	//	y += 60;
	//}
	//
	//// Ligne Horizontale
	//y = 290;
	//for ( int i = 0; i < 11; i++ )
	//{
	//	dc->DrawLine( 180, y, 980, y );
	//	if ( i == 1 || i == 9 )
	//	{
	//		dc->SetPen( pRed );
	//		dc->DrawLine( 200, y, 938, y );
	//		dc->SetPen( *wxBLACK_PEN );
	//	}
	//	y += 30;
	//}
	//
	//dc->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	//dc->DrawLabel( "N° de Section", wxRect( 180, 620, 800, 40 ), wxALIGN_CENTRE );
	//
	//
	//dc->SetFont( wxFont( 16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, true ) );
	//dc->DrawLabel( L"Représentation graphique du profil: Génératrice " + gen, wxRect( 120, 120, 880, 100 ), wxALIGN_CENTRE );
}

constexpr int CircleRadius  = 2;
constexpr double ValueStart = 1.50f;
constexpr int PixelStart    = 260;
constexpr double PixelRange = 360.0f;
constexpr int PixelEnd      = PixelStart + static_cast < int >( PixelRange );
constexpr double ValueRange = 3.00f;

void PrintMirage::Mirage_DrawGraphicStat( Program* hInst, wxDC* dc, eSideType side )
{
	const wxBrush greenBrush( *wxGREEN, wxBRUSHSTYLE_SOLID );
	dc->SetBrush( greenBrush );

	int x_start = 200;
	int y_start = 0;
	int x_end   = x_start;
	int y_end   = 0;
	int diff    = 0;

	for ( int i = 12; i > 0; i-- )
	{
		y_start = PixelStart - static_cast < int >( ( hInst->Calculation.PointData[ i ][ side ].RayDifference - ValueStart ) * PixelRange / ValueRange );
		y_end   = PixelStart - static_cast < int >( ( hInst->Calculation.PointData[ i - 1 ][ side ].RayDifference - ValueStart ) * PixelRange / ValueRange );

		if ( i == 12 )
		{
			x_end += 30;
		}
		else if ( i == 2 )
		{
			x_end += 51;
		}
		else
		{
			x_end += 73;
		}

		if ( i != 1 )
		{
			if ( y_start < PixelStart ) // Limite Supérieure Début
			{
				x_start = FindXForY(PixelStart, x_start, y_start, x_end, y_end);
				y_start = PixelStart;
			}
			else if ( y_start > PixelEnd ) // Limite Inférieure Début
			{
				x_start = FindXForY(PixelEnd, x_start, y_start, x_end, y_end);
				y_start = PixelEnd;
			}
			else if ( y_end < PixelStart ) // Limite Supérieure Fin
			{
				x_end = FindXForY(PixelStart, x_start, y_start, x_end, y_end);
				y_end = PixelStart;
			}
			else if ( y_end > PixelEnd ) // Limite Inférieure Fin
			{
				x_end = FindXForY(PixelEnd, x_start, y_start, x_end, y_end);
				y_end = PixelEnd;
			}
			
			dc->DrawLine( x_start, y_start, x_end, y_end );
		}

		if ( y_start > PixelStart && y_start < PixelEnd )
		{
			dc->DrawCircle( x_start - CircleRadius / 2, y_start - CircleRadius / 2, CircleRadius );
		}

		x_start = x_end;

		if ( ( ( side == AND_BD || side == AND_BG ) && i == 4 ) || ( side == AND_B && i == 5 ) )
		{
			break;
		}
	}

	dc->DrawCircle( x_end - CircleRadius / 2, y_end - CircleRadius / 2, CircleRadius );
}

bool PrintMirage::OnPrintPage( const int page )
{
	if ( GetDC() )
	{
		switch ( page )
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

void PrintMirage::CalculateHeight( Program* hInst, const int i )
{
	// Hauteur Radome // Petit hack pour trouver les valeurs qui ne sont pas trouver (en particulier la 20e ligne)
	for ( int idx = 0; idx < 8; idx++ )
	{
		const PointMeasure& point = hInst->Calculation.PointData[ i ][ CalculationData::GetMirageSide( idx ) ];

		if ( std::abs( point.Height - UnassignedDoubleValue ) > Epsilon )
		{
			Height = static_cast < double >( point.Height ) - 32.3f;
			break;
		}
	}
}

void PrintMirage::CalculateTheoreticalRadius( Program* hInst, const int i )
{
	double sum = 0.0f;
	for ( int side = 0; side < 8; side++ )
	{
		PointMeasure point = hInst->Calculation.PointData[ i ][ CalculationData::GetMirageSide( side ) ];

		if ( std::abs( point.Height - UnassignedDoubleValue ) > Epsilon )
		{
			for ( int rad = 0; rad < 8; rad++ )
			{
				sum += hInst->ActiveProductData.TheoreticalRadius[ point.Height ][ CalculationData::GetRafaleSide( rad ) ];
			}

			TheoreticalRadius = sum / 8;
			break;
		}
	}
}

int PrintMirage::FindXForY( const int yTarget, const int xStart, const int yStart, const int xEnd, const int yEnd )
{
	// Calculer la pente
	const float m = static_cast< float >( xEnd - xStart ) / static_cast< float >( yEnd - yStart );

	// Calculer et retourner x pour yTarget
	return xStart + static_cast< int >( m * ( yTarget - yStart ) );
}
