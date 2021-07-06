namespace AirQuality.MWM.Model
{
    class AirInformation
    {
        //All data we get from server will be stored in instance of this class
        private int outdoorTemperature, indoorTemperature, indoorHumidity, outdoorHumidity;
        private double indoorCo2, indoorTvoc, indoorPressure, indoorPm25, indoorPm10;
        private double outdoorCo2, outdoorTvoc, outdoorPressure, outdoorPm25, outdoorPm10;
        private string notification, city;

        public double IndoorTemperature { get => indoorTemperature; set => indoorTemperature = (int)value; }
        public double IndoorHumidity { get => indoorHumidity; set => indoorHumidity = (int)value; }
        public double IndoorCo2 { get => indoorCo2; set => indoorCo2 = value; }
        public double IndoorTvoc { get => indoorTvoc; set => indoorTvoc = value; }
        public double IndoorPressure { get => indoorPressure; set => indoorPressure = value; }
        public double IndoorPm25 { get => indoorPm25; set => indoorPm25 = value; }
        public double IndoorPm10 { get => indoorPm10; set => indoorPm10 = value; }
        public double OutdoorTemperature { get => outdoorTemperature; set => outdoorTemperature = (int)value; }
        public double OutdoorHumidity { get => outdoorHumidity; set => outdoorHumidity = (int) value; }
        public double OutdoorCo2 { get => outdoorCo2; set => outdoorCo2 = value; }
        public double OutdoorTvoc { get => outdoorTvoc; set => outdoorTvoc = value; }
        public double OutdoorPressure { get => outdoorPressure; set => outdoorPressure = value; }
        public double OutdoorPm25 { get => outdoorPm25; set => outdoorPm25 = value; }
        public double OutdoorPm10 { get => outdoorPm10; set => outdoorPm10 = value; }
        public string Notification { get => notification; set => notification = value; }
        public string City { get => city; set => city = value; }
    }
}
