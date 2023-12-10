
#include "pch.h"
#include "PrintUtility.h"
#include "PrintRafale.h"

void PrintUtility::PrintFile()
{
	if (RadomeType == Rafale_C || radomeType == Rafale_R)
	{
		std::cout << radomeType;
		PrintRafale::DrawRafalePage();
	}
	else if (radomeType == Mirage_C || radomeType == Mirage_R)
	{
		
	}
}
