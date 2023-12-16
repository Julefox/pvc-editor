﻿
#include "JsonConfig.h"

void from_json(const nlohmann::json& j, ProductData& p)
{
	if (j.contains("ProgramIdx")) j.at("ProgramIdx").get_to(p.ProgramIdx);
	if (j.contains("ProductNames")) j.at("ProductNames").get_to(p.ProductNames);
	if (j.contains("WorkCards")) j.at("WorkCards").get_to(p.WorkCards);
	if (j.contains("OperationCodes")) j.at("OperationCodes").get_to(p.OperationCodes);
	if (j.contains("TheoreticalHeight")) j.at("TheoreticalHeight").get_to(p.TheoreticalHeight);
	if (j.contains("HeightTolerance")) j.at("HeightTolerance").get_to(p.HeightTolerance);
	if (j.contains("RadiusTolerance")) j.at("RadiusTolerance").get_to(p.RadiusTolerance);

	if (j.contains("TheoreticalRadius"))
	{
		const nlohmann::json& json = j.at("TheoreticalRadius");
		for (const auto& keyVal : json.items())
		{
			int key = std::stoi(keyVal.key());
			std::map<eSideType, double> innerMap;
			for (const auto& innerKeyVal : keyVal.value().items())
			{
				eSideType innerKey = JsonConfig::ConvertEnumSideType(innerKeyVal.key());
				const double innerValue = innerKeyVal.value().get<double>();
				innerMap[innerKey] = innerValue;
			}
			p.TheoreticalRadius[key] = innerMap;


		}
	}

	if (j.contains("UndulationTolerance"))
	{
		const nlohmann::json& json = j.at("UndulationTolerance");
		for (const auto& keyVal : json.items())
		{
			eSideType key = JsonConfig::ConvertEnumSideType(keyVal.key());
			const double value = keyVal.value().get<double>();
			p.UndulationTolerance[key] = value;
		}
	}

	if (!j.contains("RadomeType"))
		return;
	
	const std::string radomeTypeStr = j.at("RadomeType").get<std::string>();

	p.RadomeType = JsonConfig::ConvertEnumRadomeType(radomeTypeStr);
}

eRadomeType JsonConfig::ConvertEnumRadomeType(const std::string& str)
{
	if (str.find("Mirage_C") != std::string::npos)
	{
		return Mirage_C;
	}

	if (str.find("Mirage_R") != std::string::npos)
	{
		return Mirage_R;
	}

	if (str.find("Rafale_C") != std::string::npos)
	{
		return Rafale_C;
	}

	if (str.find("Rafale_R") != std::string::npos)
	{
		return Rafale_R;
	}

	return Default;
}

eSideType JsonConfig::ConvertEnumSideType(const std::string& str)
{
	if (str.find("RBE_HG") != std::string::npos)
	{
		return RBE_HG;
	}

	if (str.find("RBE_HD") != std::string::npos)
	{
		return RBE_HD;
	}

	if (str.find("RBE_H") != std::string::npos)
	{
		return RBE_H;
	}

	if (str.find("RBE_BD") != std::string::npos)
	{
		return RBE_BD;
	}

	if (str.find("RBE_BG") != std::string::npos)
	{
		return RBE_BG;
	}

	if (str.find("RBE_B") != std::string::npos)
	{
		return RBE_B;
	}

	if (str.find("RBE_D") != std::string::npos)
	{
		return RBE_D;
	}

	if (str.find("RBE_G") != std::string::npos)
	{
		return RBE_G;
	}

	if (str.find("AND_HD") != std::string::npos)
	{
		return AND_HD;
	}

	if (str.find("AND_HG") != std::string::npos)
	{
		return AND_HG;
	}

	if (str.find("AND_H") != std::string::npos)
	{
		return AND_H;
	}

	if (str.find("AND_BD") != std::string::npos)
	{
		return AND_BD;
	}

	if (str.find("AND_BG") != std::string::npos)
	{
		return AND_BG;
	}

	if (str.find("AND_B") != std::string::npos)
	{
		return AND_B;
	}

	if (str.find("AND_D") != std::string::npos)
	{
		return AND_D;
	}

	if (str.find("AND_G") != std::string::npos)
	{
		return AND_G;
	}

	return NOT_FOUND;
}
