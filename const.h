#pragma once

#include "pch.h"

inline int MwWindowSizeWidth  = 1260;
inline int MwWindowSizeHeight = 342;
inline wxSize MwWindowSize    = wxSize(MwWindowSizeWidth, MwWindowSizeHeight);

inline int ProgIdxMirage_C = 3980;
inline int ProgIdxMirage_R = 3981;
inline int ProgIdxRafale_C = 3986;
inline int ProgIdxRafale_R = 3985;

inline double UnassignedDoubleValue = -(1 << 10);
inline int UnassignedIntValue = -(1 << 10);

inline const std::string ExecutivePath = (std::string)wxGetCwd();
inline const std::string CsvPath = ExecutivePath + "\\csv\\*.csv";
inline const std::string ConfigPath = ExecutivePath + "\\resources\\config.json";

inline wxString JsonLineSeparator = "~";
inline wxString EndLineReplacer = "\n";
inline wxString SpaceReplacer = " ";

inline wxSize   LabelSize = wxSize( 160, 40 );
inline wxSize   ChoiceSizeLarge = wxSize( 550, 40 );
inline wxSize   ChoiceSizeSmall = wxSize(320, 40);
inline wxColour LabelColor = wxColour( 16, 125, 172 );
inline wxColour MainColor = wxColour( 62, 67, 73 );
inline wxFont   GlobalFont = wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial Narrow"));

enum eRadomeType
{
	Default,
	Mirage_C,
	Mirage_R,
	Rafale_C,
	Rafale_R
};

inline const std::map<std::string, int> RadomeIndexPosition =
{
	{ "H",  0 },
	{ "HD", 1 },
	{ "D",  2 },
	{ "BD", 3 },
	{ "B",  4 },
	{ "BG", 5 },
	{ "G",  6 },
	{ "HG", 7 }
};

class StringUtility
{
public:
	static std::wstring StringToWString(const std::string& str);
	static std::wstring KeepOnlyDigits(const std::wstring& str);
	static std::string WideStringToString(const std::wstring& wStr);
	static wxString ReplaceWxString(const wxString& wStr, const wxString& item, const wxString& replace);
};
