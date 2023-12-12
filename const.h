#pragma once

#include "pch.h"

inline int MwWindowSizeWidth  = 800;
inline int MwWindowSizeHeight = 412;
inline int MwGridSizerGap     = 20;

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

inline wxSize   LabelSize = wxSize( 200, 40 );
inline wxSize   ChoiceSize = wxSize( 550, 40 );
inline wxColour LabelColor = wxColour( 16, 125, 172 );
inline wxFont   GlobalFont = wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial Narrow"));

enum eRadomeType
{
	Default,
	Mirage_C,
	Mirage_R,
	Rafale_C,
	Rafale_R
};

class StringUtility
{
public:
	static std::wstring StringToWString(const std::string& str);
	static std::string WideStringToString(const std::wstring& wStr);
	static wxString ReplaceWxString(const wxString& wStr, const wxString& item, const wxString& replace);
};
