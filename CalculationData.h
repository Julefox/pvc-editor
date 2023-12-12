#pragma once

#include "pch.h"

class Program;

class CalculationData
{
public:
	CalculationData() = default;
	explicit CalculationData(Program* hInst, const std::string& filePath);

	float RadomeLength;

	double RayMeasure[22][8];
	double RayDifference[22][8];
	double Undulation[22][8];
	int Height[22][8];
	std::map<int, double> UndulationTolerance;
private:
	void MapInit();
	void RafaleCalculateData(Program* hInst, const std::string& filePath);
};
