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

            if (lineIdx > 2 && array.Count() >= 3)
            {
            	array[0].ToDouble( &radRay );
                array[1].ToDouble( &radHeight );
                array[2].ToDouble( &radAng );

                angles1 = angles0;
                angles0 = lround(radAng);

                i = 21 - lround(radHeight / 100.0f);

                PointMeasure point;
                point.RayMeasure = radRay;
                point.Height = lround(radHeight);

                if (angles0 != angles1)
                {
                    j++;
                }

                this->PointData[i][GetRafaleSide(j)] = point;
            }
        }
        textFile.Close();

        // Calcul de la différence de rayon
        for (int k = 0; k < 22; k++)
        {
            for (int l = 0; l < 8; l++)
            {
                const eSideType side = GetRafaleSide(l);
                PointMeasure& point = this->PointData[k][side];

                if (std::abs(point.RayMeasure - UnassignedDoubleValue) > Epsilon && std::abs(hInst->ActiveProductData.TheoreticalRadius[k][side] - UnassignedDoubleValue) > Epsilon)
                {
                    point.RayDifference = point.RayMeasure - hInst->ActiveProductData.TheoreticalRadius[point.Height][side];
                    //std::cout << point.RayDifference << " " << point.RayMeasure << " " << point.Height << " " << side << " " << hInst->ActiveProductData.TheoreticalRadius[point.Height][side] << "\n";
                }
            }
        }

        // Calcul de l'ondulation
        for (int k = 1; k < 21; k++)
        {
            for (int l = 0; l < 8; l++)
            {
                const eSideType side = GetRafaleSide(l);
                const PointMeasure& backPoint = this->PointData[k - 1][side];
                PointMeasure& point = this->PointData[k][side];
                const PointMeasure& forwardPoint = this->PointData[k + 1][side];

                if (std::abs(backPoint.RayDifference - UnassignedDoubleValue) > Epsilon &&
                    std::abs(point.RayDifference - UnassignedDoubleValue) > Epsilon &&
                    std::abs(forwardPoint.RayDifference - UnassignedDoubleValue) > Epsilon &&
                    std::abs(backPoint.Height - UnassignedIntValue) > Epsilon &&
                    std::abs(point.Height - UnassignedIntValue) > Epsilon &&
                    std::abs(forwardPoint.Height - UnassignedIntValue) > Epsilon)
                {
                    const double deltaHeightForward = forwardPoint.Height - point.Height;
                    const double deltaHeightBackward = point.Height - backPoint.Height;
                    const double deltaRayonForward = forwardPoint.RayDifference - point.RayDifference;
                    const double deltaRayonBackward = point.RayDifference - backPoint.RayDifference;
                    const double heightSpan = forwardPoint.Height -backPoint.Height;

                    point.Undulation = -((deltaHeightForward * deltaRayonBackward - deltaHeightBackward * deltaRayonForward) / (heightSpan * heightSpan)) * 100;
                    point.UndulationTolerance = hInst->ActiveProductData.UndulationTolerance[side];
                    //std::cout << std::fixed << std::setprecision(2) << point.Undulation << " " << point.Height << " " << side << " " << point.UndulationTolerance << "\n";
                }
            }
        }
	}
}

void CalculationData::MirageCalculateData(Program* hInst, const std::string& filePath)
{
    wxTextFile textFile;

    /*
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

            if (lineIdx > 1 && array.Count() >= 3)
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

                int j_temp = IntOffset( j );

                if (angles0 == angles1)
                {
                    this->RayMeasure[i][j_temp] = radRay;
                }
                else
                {
                    j++;
                    j_temp++;
                    this->RayMeasure[i][j_temp] = radRay;
                }

                this->Height[i][j_temp] = lround(radHeight);

                //std::cout << this->Height[i][j_temp] << " " << i << " " << j_temp << " " << radRay << " " << radHeight << std::endl;
            }
        }
        textFile.Close();

        // Calcul de la différence de rayon
        for (int k = 1; k < 13; k++)
        {
            for (int l = 0; l < 8; l++)
            {
                if (std::abs(this->RayMeasure[k][IntOffset(l)] - UnassignedDoubleValue) > Epsilon && std::abs(hInst->ActiveProductData.TheoreticalRadius[k][IntOffset(l)] - UnassignedDoubleValue) > Epsilon)
                {
                    this->RayDifference[k][IntOffset(l)] = this->RayMeasure[k][IntOffset(l)] - hInst->ActiveProductData.TheoreticalRadius[k][IntOffset(l)];

                    //std::cout << this->RayDifference[k][l] << " = " << this->RayMeasure[k][l] << " - " << hInst->ActiveProductData.TheoreticalRadius[k][l] << std::endl;
                }
            }
        }

        // Calcul de l'ondulation
        for (int k = 1; k < 13; k++)
        {
            for (int l = 0; l < 8; l++)
            {
                if (std::abs(this->RayDifference[k - 1][IntOffset(l)] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->RayDifference[k][IntOffset(l)] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->RayDifference[k + 1][IntOffset(l)] - UnassignedDoubleValue) > Epsilon &&
                    std::abs(this->Height[k - 1][IntOffset(l)] - UnassignedIntValue) > Epsilon &&
                    std::abs(this->Height[k][IntOffset(l)] - UnassignedIntValue) > Epsilon &&
                    std::abs(this->Height[k + 1][IntOffset(l)] - UnassignedIntValue) > Epsilon)
                {
                    const double deltaHeightForward = this->Height[k + 1][IntOffset(l)] - this->Height[k][IntOffset(l)];
                    const double deltaHeightBackward = this->Height[k][IntOffset(l)] - this->Height[k - 1][IntOffset(l)];
                    const double deltaRayonForward = this->RayDifference[k + 1][IntOffset(l)] - this->RayDifference[k][IntOffset(l)];
                    const double deltaRayonBackward = this->RayDifference[k][IntOffset(l)] - this->RayDifference[k - 1][IntOffset(l)];
                    const double heightSpan = this->Height[k + 1][IntOffset(l)] - this->Height[k - 1][IntOffset(l)];

                    this->Undulation[k][IntOffset(l)] = -((deltaHeightForward * deltaRayonBackward) - (deltaHeightBackward * deltaRayonForward)) / (heightSpan * heightSpan);
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
    }*/
}

eSideType CalculationData::GetRafaleSide(const int i)
{
    switch (i)
    {
    case 0:  return RBE_H;
    case 1:  return RBE_HD;
    case 2:  return RBE_D;
    case 3:  return RBE_BD;
    case 4:  return RBE_B;
    case 5:  return RBE_BG;
    case 6:  return RBE_G;
    case 7:  return RBE_HG;
    default: return NOT_FOUND;
    }
}

eSideType CalculationData::GetMirageSide(const int i)
{
    switch (i)
    {
    case 0:  return AND_H;
    case 7:  return AND_HD;
    case 6:  return AND_D;
    case 5:  return AND_BD;
    case 4:  return AND_B;
    case 3:  return AND_BG;
    case 2:  return AND_G;
    case 1:  return AND_HG;
    default: return NOT_FOUND;
    }
}

int CalculationData::IntOffset( const int i )
{
    switch (i)
    {
    case 1:
        return 7;
    case 2:
        return 6;
    case 3:
        return 5;
    case 5:
        return 3;
    case 6:
        return 2;
    case 7:
        return 1;
    default:
        return i;
    }
}

