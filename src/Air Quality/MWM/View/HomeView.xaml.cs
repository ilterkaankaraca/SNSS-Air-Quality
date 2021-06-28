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
        string indoorTemperatureUrl, indoorHumidityUrl, indoorCo2Url, indoorTvocUrl, indoorPressureUrl, indoorPm25Url, indoorPm10Url;
        string outdoorTemperatureUrl, outdoorHumidityUrl, outdoorCo2Url, outdoorTvocUrl, outdoorPressureUrl, outdoorPm25Url, outdoorPm10Url;
        DispatcherTimer dispatcherTimer = new DispatcherTimer();

        public HomeView()
        {
            InitializeComponent();
           indoorTemperatureUrl = "http://esp32.local/indoorTemperature/";
           indoorHumidityUrl = "http://esp32.local/indoorHumidity/";
           indoorCo2Url = "http://esp32.local/indoorCo2/";
           indoorTvocUrl = "http://esp32.local/indoorTvoc/";
           indoorPressureUrl = "http://esp32.local/indoorPressure/";
           indoorPm25Url = "http://esp32.local/indoorPm25/";
           indoorPm10Url = "http://esp32.local/indoorPm10/";
            outdoorTemperatureUrl = "http://esp32.local/outdoorTemperature/";
            outdoorHumidityUrl = "http://esp32.local/outdoorHumidity/";
            outdoorCo2Url = "http://esp32.local/outdoorCo2/";
            outdoorTvocUrl = "http://esp32.local/outdoorTvoc/";
            outdoorPressureUrl = "http://esp32.local/outdoorPressure/";
            outdoorPm25Url = "http://esp32.local/outdoorPm25/";
            outdoorPm10Url = "http://esp32.local/outdoorPm10/";
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
            indoorMetrics.Temperature = Double.Parse(webClient.DownloadString(indoorTemperatureUrl).Replace('.', ','));
            indoorMetrics.Humidity = Double.Parse(webClient.DownloadString(indoorHumidityUrl).Replace('.', ','));
            indoorMetrics.Co2 = Double.Parse(webClient.DownloadString(indoorCo2Url).Replace('.', ','));
            indoorMetrics.Tvoc = Double.Parse(webClient.DownloadString(indoorTvocUrl).Replace('.', ','));
            indoorMetrics.Pressure = Double.Parse(webClient.DownloadString(indoorPressureUrl).Replace('.', ','));
            indoorMetrics.Pm25 = Double.Parse(webClient.DownloadString(indoorPm25Url).Replace('.', ','));
            indoorMetrics.Pm10 = Double.Parse(webClient.DownloadString(indoorPm10Url).Replace('.', ','));

            outdoorMetrics.Temperature = Double.Parse(webClient.DownloadString(outdoorTemperatureUrl).Replace('.', ','));
            outdoorMetrics.Humidity = Double.Parse(webClient.DownloadString(outdoorHumidityUrl).Replace('.', ','));
            outdoorMetrics.Co2 = Double.Parse(webClient.DownloadString(outdoorCo2Url).Replace('.', ','));
            outdoorMetrics.Tvoc = Double.Parse(webClient.DownloadString(outdoorTvocUrl).Replace('.', ','));
            outdoorMetrics.Pressure = Double.Parse(webClient.DownloadString(outdoorPressureUrl).Replace('.', ','));
            outdoorMetrics.Pm25 = Double.Parse(webClient.DownloadString(outdoorPm25Url).Replace('.', ','));
            outdoorMetrics.Pm10 = Double.Parse(webClient.DownloadString(outdoorPm10Url).Replace('.', ','));
        }
        private void updateComponents()
        {
            indoorTemperatureValue.Text = indoorMetrics.Temperature.ToString();
            indoorHumidityValue.Text = indoorMetrics.Humidity.ToString();
            indoorCo2Value.Text = indoorMetrics.Co2.ToString();
            indoorTvocValue.Text = indoorMetrics.Tvoc.ToString();
            indoorPressureValue.Text = indoorMetrics.Pressure.ToString();
            indoorPm25Value.Text = indoorMetrics.Pm25.ToString();
            indoorPm10Value.Text = indoorMetrics.Pm25.ToString();

            outdoorTemperatureValue.Text = outdoorMetrics.Temperature.ToString();
            outdoorHumidityValue.Text = outdoorMetrics.Humidity.ToString();
            outdoorCo2Value.Text = outdoorMetrics.Co2.ToString();
            outdoorTvocValue.Text = outdoorMetrics.Tvoc.ToString();
            outdoorPressureValue.Text = outdoorMetrics.Pressure.ToString();
            outdoorPm25Value.Text = outdoorMetrics.Pm25.ToString();
            outdoorPm10Value.Text = outdoorMetrics.Pm25.ToString();

        }
    }
}