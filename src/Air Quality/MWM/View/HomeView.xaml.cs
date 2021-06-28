using AirQuality.MWM.Model;
using System;
using System.Net;
using System.Windows.Controls;
using System.Windows.Threading;

namespace AirQuality.MWM.View
{
    /// <summary> 
    /// Interaction logic for HomeView.xaml 
    /// </summary> 

    public partial class HomeView : UserControl
    {
        // private LoginForm form1; 
        AirMetrics indoorMetrics, outdoorMetrics;
        WebClient webClient;
        string temperatureUrl, humidityUrl, co2Url, tvocUrl, pressureUrl, pm25Url, pm10Url;
        DispatcherTimer dispatcherTimer = new DispatcherTimer();

        public HomeView()
        {
            InitializeComponent();
            temperatureUrl = "http://esp32.local/temperature/";
            humidityUrl = "http://esp32.local/humidity/";
            co2Url = "http://esp32.local/co2/";
            tvocUrl = "http://esp32.local/tvoc/";
            pressureUrl = "http://esp32.local/pressure/";
            pm25Url = "http://esp32.local/pm25/";
            pm10Url = "http://esp32.local/pm10/";
            webClient = new WebClient();
            indoorMetrics = new AirMetrics();
            outdoorMetrics = new AirMetrics();
            updateMetrics();
            updateComponents();
            dispatcherTimer.Tick += dispatcherTimer_Tick;
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            dispatcherTimer.Start();
        }
        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            updateMetrics();
            updateComponents();
        }
        private void updateMetrics()
        {
            indoorMetrics.Temperature = Double.Parse(webClient.DownloadString(temperatureUrl).Replace('.', ','));
            indoorMetrics.Humidity = Double.Parse(webClient.DownloadString(humidityUrl).Replace('.', ','));
            indoorMetrics.Co2 = Double.Parse(webClient.DownloadString(co2Url).Replace('.', ','));
            indoorMetrics.Tvoc = Double.Parse(webClient.DownloadString(tvocUrl).Replace('.', ','));
            indoorMetrics.Pressure = Double.Parse(webClient.DownloadString(pressureUrl).Replace('.', ','));
            indoorMetrics.Pm25 = Double.Parse(webClient.DownloadString(pm25Url).Replace('.', ','));
            indoorMetrics.Pm10 = Double.Parse(webClient.DownloadString(pm10Url).Replace('.', ','));
        }
        private void updateComponents()
        {
            indoorTemperatureValue.Text = indoorMetrics.Temperature.ToString();
            indoorHumidityValue.Text = indoorMetrics.Humidity.ToString();
            co2IndoorValue.Text = indoorMetrics.Co2.ToString();
            tvocIndoorValue.Text = indoorMetrics.Tvoc.ToString();
            pressureIndoorValue.Text = indoorMetrics.Pressure.ToString();
            particleIndoorValue.Text = indoorMetrics.Pm25.ToString();
            //TODO: need two particle fields pm2.5 and pm10
        }
    }
}