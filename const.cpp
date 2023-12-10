
#include "const.h"

std::wstring StringUtility::StringToWString(const std::string& str)
{
    if (str.empty()) return {};

    const int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wStr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wStr[0], count);

    return wStr;
}

std::string StringUtility::WideStringToString( const std::wstring& wStr )
{
    if (wStr.empty()) return {};

    const int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wStr[0], (int)wStr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wStr[0], (int)wStr.size(), &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}

wxString StringUtility::ReplaceWxString(const wxString& wStr, const wxString& item, const wxString& replace)
{
    wxString newStr = wStr;
    newStr.Replace(item, replace);
    return newStr;
}
