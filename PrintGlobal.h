#pragma once

#include "pch.h"
#include "main.h"

class PrintGlobal : public wxPrintout
{
public:
	explicit PrintGlobal(const wxChar* title = _T("Impression PVC")) : wxPrintout( title )
	{
		// Set 'ImpressNum' value in a class inherits ~Example: See 'PrintRafale.h'
	}

	bool HasPage( int page ) override;
	bool OnBeginDocument( int startPage, int endPage ) override;
	void GetPageInfo( int* minPage, int* maxPage, int* pPageFrom, int* pageTo ) override;

	int ImpressNum = 0;

	static void SetDcScale( wxPrintout* print, wxDC* dc );
	static void DrawArray(wxDC* dc, int x, int y, int w, int wIdx, int h, int hIdx, const std::vector<wxString>& headers = {});
	static void DrawRectangle(wxDC* dc, int x, int y, int w, int h, const wxString& title = "", bool absoluteCoords = false);
	static void SetToleranceColor(wxDC* dc, double value, double tolerance);
	void Rafale_DrawMainHeader( Program* hInst, wxDC* dc, int pageIdx );
	void Mirage_DrawMainHeader( Program* hInst, wxDC* dc, int pageIdx );
};
