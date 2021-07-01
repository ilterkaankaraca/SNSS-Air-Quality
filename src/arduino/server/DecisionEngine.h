int airQualityCategories[6];


float calculateIndoorHumidity(float outdoorTemperature, float outdoorHumidity, float indoorTemperature)
{
  //This function is to calculate the outdoor humidity if the outdoor air comes inside.
  float maxWaterVapor[71] = {0.9, 0.99, 1.08, 1.180, 1.29, 1.405, 1.53, 1.67, 1.82, 1.98, 2.15,
                             2.34, 2.55, 2.77, 3.005, 3.26, 3.53, 3.82, 4.14, 4.475, 4.84, 5.205, 5.590, 5.985, 6.395, 6.825,
                             7.28, 7.76, 8.27, 8.82, 9.4, 10, 10.65, 11.35, 12.1, 12.85, 13.65, 14.5, 15.4, 16.3, 17.3, 18.35,
                             19.4, 20.55, 21.8, 23.05, 24.35, 25.75, 27.2, 28.7, 30.35, 32.05, 33.85, 35.7, 37.65, 39.6, 41.7,
                             43.9, 46.2, 48.6, 51.15, 53.8, 56.7, 59.6, 62.5, 65.4, 68.5, 71.8, 75.3, 79, 83};
  float maxAbsHumidityOutdoor = maxWaterVapor[(int)(roundf(outdoorTemperature) + 20)];
  float absHumidityOutdoor = (outdoorHumidity / 100) * maxAbsHumidityOutdoor;
  float maxAbsHumidityIndoor = maxWaterVapor[(int)(roundf(indoorTemperature) + 20)];
  float absHumidityIndoor = (absHumidityOutdoor / maxAbsHumidityIndoor) * 100;
  return absHumidityIndoor;
}

int getHighestElement()
{
  int max = airQualityCategories[0];
  for (int i = 1; i <= 5; i++)
  {
    if (airQualityCategories[i] > max)
      max = airQualityCategories[i];
  }
  return max;
}

int getNumberOfHighestElement(int max)
{
  int numberOfMax = 0;
  for (int i = 0; i <= 5; i++)
  {
    if (airQualityCategories[i] == max)
      numberOfMax++;
  }
  return numberOfMax;
}

bool decision()
{

  bool ventilate = false;
  int maxValueAqIndoor = 1;
  int numberOfHighestElementAqIndoor = 0;
  int maxValueAqOutdoor = 1;
  int numberOfHighestElementAqOutdoor = 0;

  for (int i = 0; i <= 5; ++i)
  {
    airQualityCategories[i] = 1;
  }
  categorizeAirQualityMetrics(indoorTemperature, indoorHumidity, indoorTvoc, indoorCo2, indoorPm25, indoorPm10);
  maxValueAqIndoor = getHighestElement();
  numberOfHighestElementAqIndoor = getNumberOfHighestElement(maxValueAqIndoor);

  float extrapolatedHumidityIndoor = calculateIndoorHumidity(outdoorTemperature, outdoorHumidity, indoorTemperature);
  for (int i = 0; i <= 5; ++i)
  {
    airQualityCategories[i] = 1;
  }
  categorizeAirQualityMetrics(outdoorTemperature, extrapolatedHumidityIndoor, outdoorTvoc, outdoorCo2, outdoorPm25, outdoorPm10);
  maxValueAqOutdoor = getHighestElement();
  numberOfHighestElementAqOutdoor = getNumberOfHighestElement(maxValueAqOutdoor);

  if (maxValueAqIndoor > maxValueAqOutdoor)
  {
    ventilate = true;
  }
  else if (maxValueAqIndoor == maxValueAqOutdoor)
  {
    if (numberOfHighestElementAqIndoor > numberOfHighestElementAqOutdoor)
    {
      ventilate = true;
    }
  }

  return ventilate;
}

void categorizeAirQualityMetrics(float temperature, float humidity, float tvoc, float co2, float pm2_5, float pm10)
{
  if (temperature <= 26)
  {
    airQualityCategories[0] = 1;
  }
  else if (temperature <= 39)
  {
    airQualityCategories[0] = 2;
  }
  else if (temperature <= 46)
  {
    airQualityCategories[0] = 3;
  }
  else if (temperature > 46)
  {
    airQualityCategories[0] = 4;
  }

  if (humidity >= 30 || humidity <= 60)
  {
    airQualityCategories[1] = 1;
  }
  else if (humidity < 30 || humidity > 60)
  {
    airQualityCategories[1] = 2;
  }

  if (tvoc <= 1)
  {
    airQualityCategories[2] = 1;
  }
  else if (tvoc <= 10)
  {
    airQualityCategories[2] = 2;
  }
  else if (tvoc <= 25)
  {
    airQualityCategories[2] = 3;
  }
  else if (tvoc > 25)
  {
    airQualityCategories[2] = 4;
  }

  if (co2 <= 1000)
  {
    airQualityCategories[3] = 1;
  }
  else if (co2 <= 2000)
  {
    airQualityCategories[3] = 2;
  }
  else if (co2 <= 5000)
  {
    airQualityCategories[3] = 3;
  }
  else if (co2 > 5000)
  {
    airQualityCategories[3] = 4;
  }

  if (pm2_5 <= 10)
  {
    airQualityCategories[4] = 1;
  }
  else if (pm2_5 <= 25)
  {
    airQualityCategories[4] = 2;
  }
  else if (pm2_5 <= 50)
  {
    airQualityCategories[4] = 3;
  }
  else if (pm2_5 > 50)
  {
    airQualityCategories[4] = 4;
  }

  if (pm10 <= 20)
  {
    airQualityCategories[5] = 1;
  }
  else if (pm10 <= 50)
  {
    airQualityCategories[5] = 2;
  }
  else if (pm10 <= 100)
  {
    airQualityCategories[5] = 3;
  }
  else if (pm10 > 100)
  {
    airQualityCategories[5] = 4;
  }
}