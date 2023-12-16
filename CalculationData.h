#pragma once

#include "pch.h"

class Program;

struct PointMeasure
{
	double RayTolerance        = UnassignedDoubleValue;
	double RayMeasure          = UnassignedDoubleValue;
	double RayDifference       = UnassignedDoubleValue;
	double Undulation          = UnassignedDoubleValue;
	double UndulationTolerance = UnassignedDoubleValue;
	int    Height              = UnassignedIntValue;
};

class CalculationData
{
public:
	CalculationData() = default;
	explicit CalculationData(Program* hInst, const std::string& filePath);

	float RadomeHeight;

	// std::map< Index Hauteur, std::map< Coté, Point Mesurer >>
	std::map<int, std::map<eSideType, PointMeasure>> PointData;

	inline static eSideType GetRafaleSide(int i);
	inline static eSideType GetMirageSide(int i);

private:
	void RafaleCalculateData(Program* hInst, const std::string& filePath);
	void MirageCalculateData(Program* hInst, const std::string& filePath);
};
