
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
		for (const auto& keyval : theoreticalDataJson.items())
		{
			const int i = 21 - lround(std::stoi(keyval.key()) / 100.0f);

			if (i < 0 || i > 20)
			{
				continue;
			}

			int columnIndex = 0;
			for (const auto& val : keyval.value())
			{
				p.TheoreticalRadius[i][columnIndex++] = val.get<double>();
			}
		}
	}

	//for (int row = 20; row >= 0; --row)
	//{
	//	for (int col = 7; col >= 0; --col)
	//	{
	//		std::cout << p.TheoreticalRadius[row][col] << " " << row << " " << col << std::endl;
	//	}
	//}

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
