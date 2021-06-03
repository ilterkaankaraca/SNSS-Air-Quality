using System;
using System.Collections.Generic;
using System.Text;

namespace Air_Quality
{


    class AirMetrics
    {
        private int temperature, humidity, co2, airQuality,pressure,particle;

        public int Temperature { get; set; }
        public int Humidity { get; set; }
        public int Co2 { get; set; }
        public int AirQuality { get; set; }
        public int Pressure { get; set; }
        public int Particle { get; set; }

    }
}
