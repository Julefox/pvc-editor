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

	explicit PrintRafale( const wxChar* title = _T( "Impression PVC" ) ) : PrintGlobal( title )
	{
		ImpressNum = 3;
	}

	bool OnPrintPage( int page ) override;
};
