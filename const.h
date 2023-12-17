#pragma once

#include "pch.h"

inline int MwWindowSizeWidth  = 1260;
inline int MwWindowSizeHeight = 342;
inline wxSize MwWindowSize    = wxSize(MwWindowSizeWidth, MwWindowSizeHeight);

inline int ProgIdxMirage_C = 3980;
inline int ProgIdxMirage_R = 3981;
inline int ProgIdxRafale_C = 3986;
inline int ProgIdxRafale_R = 3985;

inline constexpr double Epsilon = 1e-6;
inline constexpr double UnassignedDoubleValue = 1 << 10;
inline constexpr int UnassignedIntValue = 1 << 10;

inline constexpr int MIRAGE_MAX_HEIGHT = 2110;
inline constexpr int MIRAGE_HEIGHT_STEP = 200;

inline const std::string ExecutivePath = (std::string)wxGetCwd();
inline const std::string CsvPath = ExecutivePath + "\\csv\\*.csv";
inline const std::string ConfigPath = ExecutivePath + "\\resources\\config.json";

inline const std::string Copyright = "Ce document est la propriété de l'AIA-CP, il ne peut être reproduit ou communiqué sans autorisation écrite.";

inline wxString JsonLineSeparator = "~";
inline wxString EndLineReplacer = "\n";
inline wxString SpaceReplacer = " ";

inline wxSize   LabelSize = wxSize( 160, 40 );
inline wxSize   ChoiceSizeLarge = wxSize( 550, 40 );
inline wxSize   ChoiceSizeSmall = wxSize(320, 40);
inline wxColour LabelColor = wxColour( 16, 125, 172, 255);
inline wxColour MainColor = wxColour( 62, 67, 73, 255);
inline wxColour RedColor = wxColour(224, 102, 102, 255);
inline wxColour BlueColor = wxColour(0, 64, 127, 255);
inline wxColour GrayColor = wxColour(89, 89, 89, 255);
inline wxColour BlackColor = wxColour(0, 0, 0, 255);
inline wxFont   GlobalFont = wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial Narrow"));
inline wxBrush  GreenBrush = wxBrush(wxColour(106, 168, 79, 255), wxBRUSHSTYLE_SOLID);

enum eRadomeType
{
	Default,
	Mirage_C,
	Mirage_R,
	Rafale_C,
	Rafale_R
};

enum eSideType
{
	// Rafale
	RBE_H  = 0,
	RBE_HD = 1,
	RBE_D  = 2,
	RBE_BD = 3,
	RBE_B  = 4,
	RBE_BG = 5,
	RBE_G  = 6,
	RBE_HG = 7,

	// Mirage 2000
	AND_H  = 0,
	AND_HD = 1,
	AND_D  = 2,
	AND_BD = 3,
	AND_B  = 4,
	AND_BG = 5,
	AND_G  = 6,
	AND_HG = 7,

	NO_SIDE = 0
};

enum eSectionType
{
	// Mirage 2000
	AND_F_1   = 0,
	AND_1_2   = 1,
	AND_2_3   = 2,
	AND_3_4   = 3,
	AND_4_5   = 4,
	AND_5_6   = 5,
	AND_6_7   = 6,
	AND_7_8   = 7,
	AND_8_9   = 8,
	AND_9_10  = 9,
	AND_10_11 = 10,

	NO_SECTION = 0
};

class StringUtility
{
public:
	static std::wstring StringToWString(const std::string& str);
	static std::wstring KeepOnlyDigits(const std::wstring& str);
	static std::string WideStringToString(const std::wstring& wStr);
	static wxString ReplaceWxString(const wxString& wStr, const wxString& item, const wxString& replace);
};

class MathUtility
{
public:
	static double RoundValue(double value, int precision);
};
