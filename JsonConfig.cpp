
#include "JsonConfig.h"

void from_json(const nlohmann::json& j, ProductData& p)
{
	if (j.contains("ProgramIdx")) j.at("ProgramIdx").get_to(p.ProgramIdx);
	if (j.contains("NameDescription")) j.at("NameDescription").get_to(p.NameDescription);
	if (j.contains("ModificationIdx")) j.at("ModificationIdx").get_to(p.ModificationIdx);
	if (j.contains("OperationCode")) j.at("OperationCode").get_to(p.OperationCode);
	if (j.contains("TheoreticalHeight")) j.at("TheoreticalHeight").get_to(p.TheoreticalHeight);
	if (j.contains("HeightTolerance")) j.at("HeightTolerance").get_to(p.HeightTolerance);

	if (j.contains("TheoreticalRadius"))
	{
		const nlohmann::json& theoreticalDataJson = j.at("TheoreticalRadius");
		for (auto& [keyStr, values] : theoreticalDataJson.items())
		{
			int key = std::stoi(keyStr);
			std::vector<float> valueList;
			for (auto& val : values)
			{
				valueList.push_back(val.get<float>());
				std::cout << val << std::endl;
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
