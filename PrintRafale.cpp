
#include "PrintRafale.h"

#include "main.h"

void PrintRafale::DrawRafalePage( wxWindow* hInst )
{
    wxPrintDialogData g_printDialogData;
    wxPrintData g_printData;
    g_printData.SetOrientation( wxLANDSCAPE );
    g_printDialogData.SetPrintData( g_printData );

    auto* preview = new wxPrintPreview( new PrintRafale, new PrintRafale, &g_printDialogData );

    preview->SetZoom( 100 );

    if ( !preview->Ok() )
    {
        delete preview;
        wxMessageBox(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"), wxT("Previewing"), wxOK);
        return;
    }

    auto* frame = new wxPreviewFrame( preview, hInst, wxT("Impression"), wxPoint(-1, -1), wxSize(1280, 880), wxDEFAULT_FRAME_STYLE | wxFRAME_SHAPED);
    frame->Centre( wxBOTH );
    frame->Initialize();
    frame->Show( true );
}

void PrintRafale::RafalePage_01()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale( this, dc );
    Rafale_DrawMainPv(hInst, dc, 1);
}

void PrintRafale::RafalePage_02()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Rafale_DrawMainPv(hInst, dc, 2);
}

void PrintRafale::RafalePage_03()
{
    auto* hInst = dynamic_cast<Program*>(wxTheApp);
    wxDC* dc = GetDC();
    SetDcScale(this, dc);
    Rafale_DrawMainPv(hInst, dc, 3);
}

bool PrintRafale::OnPrintPage(const int page)
{
    if (GetDC())
    {
        switch (page)
        {
        case 1:
            this->RafalePage_01();
            break;
        case 2:
            this->RafalePage_02();
            break;
        case 3:
            this->RafalePage_03();
            break;
        default:
            break;
        }

        this->MapScreenSizeToPage();

        return true;
    }
    return false;
}
