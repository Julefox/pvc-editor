﻿#include "main.h"
#include "CalculationData.h"

CalculationData::CalculationData(Program* hInst, const std::string& filePath)
{
    if (hInst->ActiveProductData.RadomeType == Rafale_C || hInst->ActiveProductData.RadomeType == Rafale_R)
    {
        this->RafaleCalculateData(hInst, filePath);
    }
    else if (hInst->ActiveProductData.RadomeType == Mirage_C || hInst->ActiveProductData.RadomeType == Mirage_R)
    {
        
    }
}

void CalculationData::RafaleCalculateData( Program* hInst, const std::string& filePath )
{
	wxTextFile textFile;

    this->MapInit();

	if (textFile.Open(filePath))
	{
        int lineIdx = 0;
        int i = 0;
        int j = -1;
        int angles0 = -1;
        int angles1 = -1;

        double radHeight = 0.0f;
        double radRay = 0.0f;
        double radAng = 0.0f;

        wxString line;

        while (!textFile.Eof())
        {
            lineIdx++;
            line = textFile.GetNextLine();

            wxArrayString array = wxSplit(line, ';');

            if (lineIdx == 1 && array.Count() >= 2)
            {
                double height;

                if (array[1].ToDouble(&height))
                {
                    this->RadomeLength = static_cast<float>(height);
                }
            }

            if (lineIdx > 3 && array.Count() >= 3)
            {
            	array[0].ToDouble( &radRay );
                array[1].ToDouble( &radHeight );
                array[2].ToDouble( &radAng );

                angles1 = angles0;
                angles0 = lround(radAng);


                i = 21 - lround(radHeight / 100.0f);

                if (angles0 == angles1)
                {
                    this->RayMeasure[i][j] = radRay;
                }
                else
                {
                    j++;
                    this->RayMeasure[i][j] = radRay;
                }

                this->Height[i][j] = lround(radHeight);

                std::cout << this->Height[i][j] << std::endl;
            }
        }
        textFile.Close();

        // Calcul de la différence de rayon
        for (int index1 = 0; index1 < 22; index1++)
        {
            for (int index2 = 0; index2 < 8; index2++)
            {
                if (this->RayMeasure[index1][index2] != UnassignedDoubleValue && hInst->ActiveProductData.TheoreticalRadius[index1][index2] != UnassignedDoubleValue)
                {
                    this->RayDifference[index1][index2] = this->RayMeasure[index1][index2] - hInst->ActiveProductData.TheoreticalRadius[index1][index2];
                }
            }
        }

        // Calcul de l'ondulation
        for (int index1 = 1; index1 < 21; index1++)
        {
            for (int index2 = 0; index2 < 8; index2++)
            {
                if (this->RayDifference[index1 - 1][index2] != UnassignedDoubleValue && this->RayDifference[index1][index2] != UnassignedDoubleValue && this->RayDifference[index1 + 1][index2] != UnassignedDoubleValue &&
                    this->Height[index1 - 1][index2] != UnassignedIntValue && this->Height[index1][index2] != UnassignedIntValue && this->Height[index1 + 1][index2] != UnassignedIntValue)
                {
                    const double deltaHeightForward = this->Height[index1 + 1][index2] - this->Height[index1][index2];
                    const double deltaHeightBackward = this->Height[index1][index2] - this->Height[index1 - 1][index2];
                    const double deltaRayonForward = this->RayDifference[index1 + 1][index2] - this->RayDifference[index1][index2];
                    const double deltaRayonBackward = this->RayDifference[index1][index2] - this->RayDifference[index1 - 1][index2];
                    const double heightSpan = this->Height[index1 + 1][index2] - this->Height[index1 - 1][index2];

                    this->Undulation[index1][index2] = -((deltaHeightForward * deltaRayonBackward) - (deltaHeightBackward * deltaRayonForward)) / (heightSpan * heightSpan);
                }
            }
        }

        // Parametrage des tolérances d'ondulation
        for (const auto& pair : hInst->ActiveProductData.UndulationTolerance)
        {
            const auto& key = pair.first;
            const auto& value = pair.second;
            const auto& it = RadomeIndexPosition.find(key);

            if (it != RadomeIndexPosition.end())
            {
                this->UndulationTolerance[it->second] = value;
            }
        }

        //for (int index1 = 1; index1 < 20; index1++)
        //{
        //    for (int index2 = 0; index2 < 8; index2++)
        //    {
        //        if (Undulation[index1 + 1][index2] != UnassignedDoubleValue)
        //        {
        //            double ondulationValue = Undulation[index1 + 1][index2];
        //            std::string color = "BLUE"; // Par défaut, la couleur est bleue.
        //
        //            if ((index2 > 2) && (index2 < 6))
        //            {
        //                if (fabs(ondulationValue) > 0.003)
        //                    color = "RED";
        //            }
        //            else
        //            {
        //                if (fabs(ondulationValue) > 0.005)
        //                    color = "RED";
        //            }
        //
        //            std::cout << "Ondulation [" << index1 + 1 << "][" << index2 << "] = "
        //                << ondulationValue * 10.0 << "% (" << color << ")" << std::endl;
        //        }
        //        else
        //        {
        //            std::cout << "Ondulation [" << index1 + 1 << "][" << index2 << "] = Non Mesurée" << std::endl;
        //        }
        //    }
        //}
	}
}

void CalculationData::MapInit()
{
	for ( int i = 0; i < 22; i++ )
	{
        for (int j = 0; j < 8; j++)
        {
            this->RayMeasure[i][j] = UnassignedDoubleValue;
            this->RayDifference[i][j] = UnassignedDoubleValue;
            this->Undulation[i][j] = UnassignedDoubleValue;
            this->Height[i][j] = UnassignedIntValue;
        }
	}

    for (int i = 0; i < 8; i++)
    {
        this->UndulationTolerance[i] = 0.0f;
    }
}
