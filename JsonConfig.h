#pragma once

#include "pch.h"

struct ProductData
{
public:
	std::string ProgramIdx;
	eRadomeType RadomeType = Default;
	std::vector < std::string > ProductNames;
	std::vector < std::string > WorkCards;
	std::vector < std::string > OperationCodes;
	std::vector < std::string > CoilMounting;

	float TheoreticalHeight = 0.0f;
	float HeightTolerance   = 0.0f;
	float RadiusTolerance   = 0.0f;

	std::map < int, std::map < eSideType, double > > TheoreticalRadius;
	std::map < eSideType, double > UndulationTolerance;
	std::map < eSectionType, double > UndulationToleranceSection;
};

void from_json( const nlohmann::json& j, ProductData& p );

class JsonConfig
{
public:
	std::vector < ProductData > Products;
	std::vector < std::string > Operators;
	std::vector < std::string > Posts;
	std::string MirageAnnexHeader;
	std::string RafaleAnnexHeader;
	std::string RafaleFrameLeftContent;
	std::string RafaleFrameRightContent;

	static JsonConfig LoadJsonConfig( const std::string& filename )
	{
		std::ifstream file( filename );
		if ( !file.is_open() )
		{
			throw std::runtime_error( "Impossible d'ouvrir le fichier" );
		}

		nlohmann::json j;
		file >> j;

		return { j };
	}

	JsonConfig() = default;

	JsonConfig( const nlohmann::json& j )
	{
		if ( j.contains( "Products" ) )
			j.at( "Products" ).get_to( Products );
		if ( j.contains( "Operators" ) )
			j.at( "Operators" ).get_to( Operators );
		if ( j.contains( "Posts" ) )
			j.at( "Posts" ).get_to( Posts );
		if ( j.contains( "MirageAnnexHeader" ) )
			j.at( "MirageAnnexHeader" ).get_to( MirageAnnexHeader );
		if ( j.contains( "RafaleAnnexHeader" ) )
			j.at( "RafaleAnnexHeader" ).get_to( RafaleAnnexHeader );
		if ( j.contains( "RafaleFrameLeftContent" ) )
			j.at( "RafaleFrameLeftContent" ).get_to( RafaleFrameLeftContent );
		if ( j.contains( "RafaleFrameRightContent" ) )
			j.at( "RafaleFrameRightContent" ).get_to( RafaleFrameRightContent );
	}

	static eRadomeType ConvertEnumRadomeType( const std::string& str );

	static eSideType ConvertEnumSideType( const std::string& str );

	static eSectionType ConvertEnumSectionType( const std::string& str );
};
