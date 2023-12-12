
#include "JsonConfig.h"

void from_json(const nlohmann::json& j, ProductData& p)
{
	if (j.contains("ProgramIdx")) j.at("ProgramIdx").get_to(p.ProgramIdx);
	if (j.contains("ProductNames")) j.at("ProductNames").get_to(p.ProductNames);
	if (j.contains("WorkCards")) j.at("WorkCards").get_to(p.WorkCards);
	if (j.contains("OperationCodes")) j.at("OperationCodes").get_to(p.OperationCodes);
	if (j.contains("TheoreticalHeight")) j.at("TheoreticalHeight").get_to(p.TheoreticalHeight);
	if (j.contains("HeightTolerance")) j.at("HeightTolerance").get_to(p.HeightTolerance);

	if (j.contains("TheoreticalRadius"))
	{
		const nlohmann::json& theoreticalDataJson = j.at("TheoreticalRadius");
		for (auto& [keyStr, values] : theoreticalDataJson.items())
		{
			int key = std::stoi(keyStr);
			std::vector<double> valueList;
			for (auto& val : values)
			{
				valueList.push_back(val.get<double>());
			}
			p.TheoreticalRadius[key] = valueList;
		}
	}

	if (!j.contains("RadomeType"))
		return;
	
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
