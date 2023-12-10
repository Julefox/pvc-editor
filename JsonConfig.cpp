
#include "JsonConfig.h"

void from_json(const nlohmann::json& j, ProductData& p)
{
	j.at("ProgramIdx").get_to(p.ProgramIdx);
	j.at("NameDescription").get_to(p.NameDescription);
	
	const std::string radomeTypeStr = j.at("RadomeType").get<std::string>();
	
	if (radomeTypeStr.find("Mirage_C") != std::string::npos)
	{
		p.RadomeType = Mirage_C;
	}
	else if (radomeTypeStr.find("Mirage_R") != std::string::npos)
	{
		p.RadomeType = Mirage_R;
	}
	else if (radomeTypeStr.find("Rafale_C") != std::string::npos)
	{
		p.RadomeType = Rafale_C;
	}
	else if (radomeTypeStr.find("Rafale_R") != std::string::npos)
	{
		p.RadomeType = Rafale_R;
	}
	else
	{
		p.RadomeType = Default;
	}
}
