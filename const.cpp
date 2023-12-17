#include "const.h"

std::wstring StringUtility::StringToWString( const std::string& str )
{
	if ( str.empty() )
		return {};

	const int count = MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, nullptr, 0 );
	std::wstring wStr( count, 0 );
	MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, &wStr[ 0 ], count );

	return wStr;
}

std::wstring StringUtility::KeepOnlyDigits( const std::wstring& str )
{
	std::wstring result;
	for ( wchar_t ch : str )
	{
		if ( ch >= L'0' && ch <= L'9' )
		{
			result += ch;
		}
	}
	return result;
}

std::string StringUtility::WideStringToString( const std::wstring& wStr )
{
	if ( wStr.empty() )
		return {};

	const int size_needed = WideCharToMultiByte( CP_UTF8, 0, &wStr[ 0 ], static_cast < int >( wStr.size() ), nullptr, 0, nullptr, nullptr );
	std::string strTo( size_needed, 0 );
	WideCharToMultiByte( CP_UTF8, 0, &wStr[ 0 ], static_cast < int >( wStr.size() ), &strTo[ 0 ], size_needed, nullptr, nullptr );
	return strTo;
}

wxString StringUtility::ReplaceWxString( const wxString& wStr, const wxString& item, const wxString& replace )
{
	wxString newStr = wStr;
	newStr.Replace( item, replace );
	return newStr;
}

double MathUtility::RoundValue( const double value, const int precision )
{
	const double factor = std::pow( 10.0, precision );
	return std::floor( value * factor ) / factor;
}
