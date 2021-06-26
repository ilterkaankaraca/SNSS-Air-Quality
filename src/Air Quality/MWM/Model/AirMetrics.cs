using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AirQuality.MWM.Model
{
    class AirMetrics
    {
        private double temperature, humidity, co2, tvoc, pressure, pm25, pm10;

        public double Temperature { get; set; }
        public double Humidity { get; set; }
        public double Co2 { get; set; }
        public double Tvoc { get; set; }
        public double Pressure { get; set; }
        public double Pm25 { get; set; }
        public double Pm10 { get; set; }

    }

}
