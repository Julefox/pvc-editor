#pragma once

#include "pch.h"

struct ProductData
{
public:
	std::string ProgramIdx;
	std::string NameDescription;
	eRadomeType RadomeType = Default;
	std::string ModificationIdx;
	std::string OperationCode;
	std::map<int, std::vector<double>> TheoreticalRadius;
	float TheoreticalHeight;
	float HeightTolerance;
};

void from_json(const nlohmann::json& j, ProductData& p);

class JsonConfig
{
public:
	std::vector< ProductData > Products;
	std::vector< std::string > Operators;
	std::vector< std::string > WorkCards;
	std::vector< std::string > Posts;

	static JsonConfig LoadJsonConfig(const std::string& filename)
	{
		std::ifstream file(filename);
		if (!file.is_open())
		{
			throw std::runtime_error("Impossible d'ouvrir le fichier");
		}

		nlohmann::json j;
		file >> j;

		return { j };
	}

	JsonConfig() = default;

	JsonConfig(const nlohmann::json& j)
	{
		if (j.contains("Products")) j.at("Products").get_to(Products);
		if (j.contains("Operators")) j.at("Operators").get_to(Operators);
		if (j.contains("WorkCards")) j.at("WorkCards").get_to(WorkCards);
		if (j.contains("Posts")) j.at("Posts").get_to(Posts);
	}
};
