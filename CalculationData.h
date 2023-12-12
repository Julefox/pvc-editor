#pragma once

#include "pch.h"

class Program;

class CalculationData
{
public:
	CalculationData() = default;
	explicit CalculationData(Program* hInst, const std::string& filePath);

	float RadomeLength;

	std::map<int, std::vector<double>> RayMeasure;
	std::map<int, std::vector<double>> RayDifference;
	std::map<int, std::vector<double>> Undulation;
	int Height[22][8];
private:
	void MapInit();
	void RafaleCalculateData(Program* hInst, const std::string& filePath);
};
