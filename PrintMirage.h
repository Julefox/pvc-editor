#pragma once

#include "pch.h"
#include "PrintGlobal.h"

class PrintMirage final : PrintGlobal
{
public:
	static void DrawMiragePage(wxWindow* hInst);
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

	void Mirage_DrawGraphic(Program* hInst, wxDC* dc, const wxString& gen);

	explicit PrintMirage(const wxChar* title = _T("Impression PVC")) : PrintGlobal( title )
	{
		ImpressNum = 10;
	}

	bool OnPrintPage(int page) override;
};
