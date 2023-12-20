#pragma once

#include "pch.h"
#include "PrintGlobal.h"

class PrintMirage final : PrintGlobal
{
public:
	static void DrawMiragePage( wxWindow* hInst );

private:
	void MiragePage_01();
	void MiragePage_02();
	void MiragePage_03();
	void MiragePage_04();
	void MiragePage_05();
	void MiragePage_06();
	void MiragePage_07();
	void MiragePage_08();
	void MiragePage_09();
	void MiragePage_10();

	void CalculateHeight( Program* hInst, int i );
	void CalculateTheoreticalRadius( Program* hInst, int i );

	double Height            = 0.0f;
	double TheoreticalRadius = 0.0f;

	static void Mirage_DrawGraphic( Program* hInst, wxDC* dc, const wxString& gen, eSideType side );
	static void Mirage_DrawGraphicStat( Program* hInst, wxDC* dc, eSideType side );

	static int FindXForY( int yTarget, int xStart, int yStart, int xEnd, int yEnd );

	static void AdjustLinePoints(int& xStart, int& yStart, int& xEnd, int& yEnd,
		int xMin, int yMin, int xMax, int yMax);

	explicit PrintMirage( const wxChar* title = _T( "Impression PVC" ) ) : PrintGlobal( title )
	{
		ImpressNum = 10;
	}

	bool OnPrintPage( int page ) override;
};
