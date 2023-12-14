#include "main.h"
#include "CalculationData.h"

CalculationData::CalculationData(Program* hInst, const std::string& filePath)
{
    if (hInst->ActiveProductData.RadomeType == Rafale_C || hInst->ActiveProductData.RadomeType == Rafale_R)
    {
        this->RafaleCalculateData(hInst, filePath);
    }
    else if (hInst->ActiveProductData.RadomeType == Mirage_C || hInst->ActiveProductData.RadomeType == Mirage_R)
    {
        this->MirageCalculateData(hInst, filePath);
    }
}

void CalculationData::RafaleCalculateData(Program* hInst, const std::string& filePath)
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
                    this->RadomeHeight = static_cast<float>(height);
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
            }
        }
        textFile.Close();

        // Calcul de la différence de rayon
        for (int k = 0; k < 22; k++)
        {
            for (int l = 0; l < 8; l++)
            {
                if (std::abs(this->RayMeasure[k][l] - UnassignedDoubleValue) > Epsilon && std::abs(hInst->ActiveProductData.TheoreticalRadius[k][l] - UnassignedDoubleValue) > Epsilon)
                {
                    this->RayDifference[k][l] = this->RayMeasure[k][l] - hInst->ActiveProductData.TheoreticalRadius[k][l];
                }
            }
        }

        // Calcul de l'ondulation
        for (int k = 1; k < 21; k++)
        {
            for (int l = 0; l < 8; l++)
            {
                if (std::abs(this->RayDifference[k - 1][l] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->RayDifference[k][l] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->RayDifference[k + 1][l] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->Height[k - 1][l] - UnassignedIntValue) > Epsilon &&
                    std::abs(this->Height[k][l] - UnassignedIntValue) > Epsilon &&
                    std::abs(this->Height[k + 1][l] - UnassignedIntValue) > Epsilon)
                {
                    const double deltaHeightForward = this->Height[k + 1][l] - this->Height[k][l];
                    const double deltaHeightBackward = this->Height[k][l] - this->Height[k - 1][l];
                    const double deltaRayonForward = this->RayDifference[k + 1][l] - this->RayDifference[k][l];
                    const double deltaRayonBackward = this->RayDifference[k][l] - this->RayDifference[k - 1][l];
                    const double heightSpan = this->Height[k + 1][l] - this->Height[k - 1][l];

                    this->Undulation[k][l] = -((deltaHeightForward * deltaRayonBackward) - (deltaHeightBackward * deltaRayonForward)) / (heightSpan * heightSpan);
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
	}
}

void CalculationData::MirageCalculateData(Program* hInst, const std::string& filePath)
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

            if (lineIdx > 2 && array.Count() >= 3)
            {
                array[0].ToDouble(&radRay);
                array[1].ToDouble(&radHeight);
                array[2].ToDouble(&radAng);

                radHeight = std::abs(radHeight); // -100.04 => 100.04

                angles1 = angles0;
                angles0 = lround(radAng);

                // Méthode pour toujours trouver entre 1 et 12
                i = (MIRAGE_MAX_HEIGHT - lround(radHeight) + MIRAGE_HEIGHT_STEP / 2) / MIRAGE_HEIGHT_STEP;
                i = std::max(1, std::min(i + 1, 12));

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

                //std::cout << this->Height[i][j] << " " << i << " " << j << " " << radRay << " " << radHeight << " " << lround(radHeight / 100.0f) << std::endl;
            }
        }
        textFile.Close();

        // Calcul de la différence de rayon
        for (int k = 0; k < 14; k++)
        {
            for (int l = 0; l < 8; l++)
            {
                if (std::abs(this->RayMeasure[k][l] - UnassignedDoubleValue) > Epsilon && std::abs(hInst->ActiveProductData.TheoreticalRadius[k][l] - UnassignedDoubleValue) > Epsilon)
                {
                    this->RayDifference[k][l] = this->RayMeasure[k][l] - hInst->ActiveProductData.TheoreticalRadius[k][l];

                    std::cout << this->RayDifference[k][l] << " = " << this->RayMeasure[k][l] << " - " << hInst->ActiveProductData.TheoreticalRadius[k][l] << std::endl;
                }
            }
        }

        // Calcul de l'ondulation
        for (int k = 1; k < 13; k++)
        {
            for (int l = 0; l < 8; l++)
            {
                if (std::abs(this->RayDifference[k - 1][l] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->RayDifference[k][l] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->RayDifference[k + 1][l] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->Height[k - 1][l] - UnassignedIntValue) > Epsilon &&
                    std::abs(this->Height[k][l] - UnassignedIntValue) > Epsilon &&
                    std::abs(this->Height[k + 1][l] - UnassignedIntValue) > Epsilon)
                {
                    const double deltaHeightForward = this->Height[k + 1][l] - this->Height[k][l];
                    const double deltaHeightBackward = this->Height[k][l] - this->Height[k - 1][l];
                    const double deltaRayonForward = this->RayDifference[k + 1][l] - this->RayDifference[k][l];
                    const double deltaRayonBackward = this->RayDifference[k][l] - this->RayDifference[k - 1][l];
                    const double heightSpan = this->Height[k + 1][l] - this->Height[k - 1][l];

                    this->Undulation[k][l] = -((deltaHeightForward * deltaRayonBackward) - (deltaHeightBackward * deltaRayonForward)) / (heightSpan * heightSpan);
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
