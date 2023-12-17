#pragma once

#include  "pch.h"
#include "PrintGlobal.h"

class PrintRafale final : PrintGlobal
{
public:
	static void DrawRafalePage( wxWindow* hInst );
private:
	void RafalePage_01();
	void RafalePage_02();
	void RafalePage_03();

	void CalculateHeight(Program* hInst, int i );
	void CalculateTheoreticalRadius(Program* hInst, int i);

	int UpperHeight = 0;
	int LowerHeight = 0;
	double TheoreticalRadius = 0.0;

	explicit PrintRafale( const wxChar* title = _T( "Impression PVC" ) ) : PrintGlobal( title )
	{
		ImpressNum = 3;
	}

	bool OnPrintPage( int page ) override;
};
