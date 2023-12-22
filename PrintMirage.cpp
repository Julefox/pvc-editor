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

				if ( std::abs( point.Undulation - UnassignedDoubleValue ) > Epsilon && !( ( side == AND_BD && section > 8 ) || ( side == AND_B && section > 7 ) || ( side == AND_BG && section > 8 ) || ( hInst->ActiveProductData.RadomeType == Mirage_R && section > 7 ) ) )
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
	Mirage_DrawGraphic(hInst, dc, "H", AND_H);
	Mirage_DrawMainHeader( hInst, dc, 3 );
}

void PrintMirage::MiragePage_04()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawGraphic(hInst, dc, "HD", AND_HD);
	Mirage_DrawMainHeader( hInst, dc, 4 );
}

void PrintMirage::MiragePage_05()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawGraphic(hInst, dc, "D", AND_D);
	Mirage_DrawMainHeader( hInst, dc, 5 );
}

void PrintMirage::MiragePage_06()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawGraphic(hInst, dc, "BD", AND_BD);
	Mirage_DrawMainHeader( hInst, dc, 6 );
}

void PrintMirage::MiragePage_07()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawGraphic(hInst, dc, "B", AND_B);
	Mirage_DrawMainHeader( hInst, dc, 7 );
}

void PrintMirage::MiragePage_08()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawGraphic(hInst, dc, "BG", AND_BG);
	Mirage_DrawMainHeader( hInst, dc, 8 );
}

void PrintMirage::MiragePage_09()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawGraphic(hInst, dc, "G", AND_G);
	Mirage_DrawMainHeader( hInst, dc, 9 );
}

void PrintMirage::MiragePage_10()
{
	auto* hInst = dynamic_cast < Program* >( wxTheApp );
	wxDC* dc    = GetDC();
	SetDcScale( this, dc );
	Mirage_DrawGraphic(hInst, dc, "HG", AND_HG);
	Mirage_DrawMainHeader( hInst, dc, 10 );
}

constexpr int X_START_VALUE = 0, X_END_VALUE = 2120, Y_START_VALUE = -2, Y_END_VALUE = 2, CIRCLE_RADIUS = 2,
FRAME_X_START = 180, FRAME_X_LENGTH = 800, FRAME_Y_START = 260, FRAME_Y_LENGTH = 360,
FRAME_Y_TOLERANCE_START = FRAME_Y_START - 60, FRAME_Y_TOLERANCE_LENGTH = FRAME_Y_LENGTH + 120;

struct GraphPoint
{
	int X_LINE = 0;
	int Y_LINE_CENTER = FRAME_Y_START + FRAME_Y_LENGTH / 2;

	int X_TOLERANCE = 0;
	int Y_TOLERANCE_UPPER = 0;
	int Y_TOLERANCE_LOWER = 0;

	GraphicData GRAPHIC_DATA;

	int X_RAY_DIFF = 0;
	int Y_RAY_DIFF = 0;

	bool IS_OUT_OF_FRAME = false;
};

void PrintMirage::Mirage_DrawGraphic( Program* hInst, wxDC* dc, const wxString& gen, const eSideType side )
{
	const wxPen pBigBlack( wxColour( 0, 0, 0 ), 3 );
	const wxPen pRed( wxColour( 224, 102, 102 ) );
	const wxPen pDotBlue( *wxBLUE, 1, wxPENSTYLE_DOT );
	const wxBrush greenBrush(*wxGREEN, wxBRUSHSTYLE_SOLID);

	std::map<eSectionType, int> redLinePos;
	std::map<eSectionType, GraphPoint> sectionPos;

	for (const auto& keyVal : hInst->ActiveProductData.GraphicData[side])
	{
		eSectionType section = keyVal.first;
		const GraphicData graphicData = keyVal.second;
		const PointMeasure& point = hInst->Calculation.PointData[12 - section][side];

		const int x = FRAME_X_START + static_cast <int>((graphicData.Height - X_START_VALUE) * FRAME_X_LENGTH / (X_END_VALUE - X_START_VALUE));

		GraphPoint graphPoint;

		graphPoint.X_LINE = x;

		if (section == AND_F || section == AND_11)
		{
			redLinePos[section] = graphPoint.X_LINE;
		}

		if (std::abs(point.RayDifference - UnassignedDoubleValue) > Epsilon)
		{
			int xStart = graphPoint.X_LINE;
			int yStart = FRAME_Y_TOLERANCE_START + static_cast <int>((point.RayDifference - Y_START_VALUE) * FRAME_Y_TOLERANCE_LENGTH / (Y_END_VALUE - Y_START_VALUE));
			int xEnd = FRAME_X_START + static_cast <int>((graphicData.DeltaHeight - X_START_VALUE) * FRAME_X_LENGTH / (X_END_VALUE - X_START_VALUE));
			int yEnd = FRAME_Y_TOLERANCE_START + static_cast <int>((graphicData.DeltaMin - Y_START_VALUE) * FRAME_Y_TOLERANCE_LENGTH / (graphicData.DeltaMax - graphicData.DeltaMin));

			graphPoint.X_RAY_DIFF = xStart;
			graphPoint.Y_RAY_DIFF = FRAME_Y_START + FRAME_Y_LENGTH - yStart + FRAME_Y_START;

			graphPoint.X_TOLERANCE = xEnd;
			graphPoint.Y_TOLERANCE_UPPER = yEnd;
			graphPoint.Y_TOLERANCE_LOWER = yEnd + (FRAME_Y_START + FRAME_Y_LENGTH / 2 - yEnd) * 2;
			graphPoint.GRAPHIC_DATA = graphicData;

			sectionPos[section] = graphPoint;
		}
	}

	dc->SetPen(pDotBlue);

	for (int i = 0; i < 12; i++)
	{
		const auto section = static_cast<eSectionType>(i);

		if (sectionPos.find(section) != sectionPos.end())
		{
			const GraphPoint& graphPoint = sectionPos[section];
			if (graphPoint.GRAPHIC_DATA.DeltaHeight != 0.0f)
			{
				dc->DrawLine(graphPoint.X_LINE, graphPoint.Y_LINE_CENTER, graphPoint.X_TOLERANCE, graphPoint.Y_TOLERANCE_UPPER);
				dc->DrawLine(graphPoint.X_LINE, graphPoint.Y_LINE_CENTER, graphPoint.X_TOLERANCE, graphPoint.Y_TOLERANCE_LOWER);
			}
		}
	}

	dc->SetPen(*wxBLACK_PEN);

	for (int i = 0; i < 12; i++)
	{
		const auto section = static_cast<eSectionType>(i);

		if (sectionPos.find(section) != sectionPos.end())
		{
			const GraphPoint& graphPoint = sectionPos[section];
			dc->DrawLine(graphPoint.X_LINE, FRAME_Y_START, graphPoint.X_LINE, FRAME_Y_START + FRAME_Y_LENGTH);
		}
	}

	int lastX = 0, lastY = 0;

	for (int i = 1; i < 12; i++)
	{
		const auto section = static_cast<eSectionType>(i);
		const auto backSection = static_cast<eSectionType>(i - 1);

		const GraphPoint& graphPoint = sectionPos[section];
		const GraphPoint& backGraphPoint = sectionPos[backSection];

		if (sectionPos.find(section) != sectionPos.end() && sectionPos.find(backSection) != sectionPos.end() && graphPoint.Y_RAY_DIFF > 0 && !((side == AND_BD && section > 9) || (side == AND_B && section > 8) || (side == AND_BG && section > 9) || (hInst->ActiveProductData.RadomeType == Mirage_R && section > 8)))
		{
			dc->DrawLine(backGraphPoint.X_RAY_DIFF, backGraphPoint.Y_RAY_DIFF, graphPoint.X_RAY_DIFF, graphPoint.Y_RAY_DIFF);

			dc->SetBrush(greenBrush);
			dc->DrawCircle(backGraphPoint.X_RAY_DIFF - CIRCLE_RADIUS / 2, backGraphPoint.Y_RAY_DIFF - CIRCLE_RADIUS / 2, CIRCLE_RADIUS);

			lastX = graphPoint.X_RAY_DIFF - CIRCLE_RADIUS / 2;
			lastY = graphPoint.Y_RAY_DIFF - CIRCLE_RADIUS / 2;
		}
	}

	dc->SetBrush(greenBrush);
	dc->DrawCircle(lastX, lastY, CIRCLE_RADIUS);

	// BAD
	dc->SetBrush(*wxWHITE_BRUSH);
	dc->SetPen(*wxWHITE_PEN);
	dc->DrawRectangle(wxRect(0, 0, FRAME_X_START, 1200));
	dc->DrawRectangle(wxRect(0, 0, 1200, FRAME_Y_START));
	dc->DrawRectangle(wxRect(FRAME_X_START + FRAME_X_LENGTH, 0, 1200 - FRAME_X_START + FRAME_X_LENGTH, 1200));
	dc->DrawRectangle(wxRect(0, FRAME_Y_START + FRAME_Y_LENGTH, 1200, 1200 - FRAME_Y_START + FRAME_Y_LENGTH));

	dc->SetPen(*wxBLACK_PEN);
	DrawRectangle(dc, FRAME_X_START, FRAME_Y_START, FRAME_X_LENGTH, FRAME_Y_LENGTH);

	dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	
	DrawRectangle( dc, 120, 220, 880, 440 );
	DrawWhiteLabel( dc, 142, 205, 64, 30, L"Ecart en mm" );

	dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	dc->DrawLabel("N° de Section", wxRect(180, 620, 800, 40), wxALIGN_CENTRE);

	dc->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, true));
	dc->DrawLabel(L"Représentation graphique du profil: Génératrice " + gen, wxRect(120, 120, 880, 100), wxALIGN_CENTRE);

	dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	dc->SetPen(*wxBLACK_PEN);

	int y = 290;
	for (int i = 0; i < 12; i++)
	{
		dc->DrawLine(FRAME_X_START, y, FRAME_X_START + FRAME_X_LENGTH, y);
		if (i == 1 || i == 9)
		{
			dc->SetPen(pRed);
			dc->DrawLine(redLinePos[AND_F], y, redLinePos[AND_11], y);
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

	for (int i = 0; i < 12; i++)
	{
		const auto section = static_cast<eSectionType>(i);
	
		if (sectionPos.find(section) != sectionPos.end())
		{
			const GraphPoint& graphPoint = sectionPos[section];
	
			if (graphPoint.X_LINE == 0)
				break;
	
			DrawWhiteLabel(dc, graphPoint.X_LINE - 6, FRAME_Y_START + FRAME_Y_LENGTH - 6, 12, 12, section == AND_F ? "F" : std::to_string(section));
		}
	}
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
