using AirQuality.MWM.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace AirQuality.MWM.View
{
    /// <summary> 
    /// Interaction logic for HomeView.xaml 
    /// </summary> 

    public partial class HomeView : UserControl
    {
        // private LoginForm form1; 
        AirMetrics metrics;
        WebClient webClient;
        string temperatureUrl, humidityUrl, co2Url, airQualityUrl, pressureUrl, particleUrl;



        public HomeView()
        {
            InitializeComponent();
            temperatureUrl = "http://esp32.local/temperature/";
            humidityUrl = "http://esp32.local/humidity/";
            co2Url = "http://esp32.local/co2/";
            airQualityUrl = "http://esp32.local/airQuality/";
            pressureUrl = "http://esp32.local/pressure/";
            particleUrl = "http://esp32.local/particle/";
            webClient = new WebClient();
            metrics = new AirMetrics();
            updateMetrics();
            updateComponents();
        }


        //public OverviewForm(LoginForm form1) 

        //{ 

        //    InitializeComponent(); 

        //    this.form1 = form1; 



        //    webClient = new WebClient(); 

        //    metrics = new AirMetrics(); 

        //    updateMetrics(); 

        //    updateComponents(); 



        //    timer.Enabled = true; 

        //} 



        //private void Form2_FormClosing(object sender, FormClosingEventArgs e) 

        //{ 

        //    Application.Exit(); 

        //} 



        //private void settingsButton_Click(object sender, EventArgs e) 

        //{ 

        //    SettingsForm formSettings = new SettingsForm(this); 

        //    formSettings.Show(); 

        //    this.Hide(); 

        //} 



        //private void timer_Tick(object sender, EventArgs e) 

        //{ 

        //    updateMetrics(); 

        //    updateComponents(); 

        //} 



        private void updateMetrics()

        {

            metrics.Temperature = Double.Parse(webClient.DownloadString(temperatureUrl).Replace('.', ','));

            metrics.Humidity = Double.Parse(webClient.DownloadString(humidityUrl).Replace('.', ','));

            metrics.Co2 = Double.Parse(webClient.DownloadString(co2Url).Replace('.', ','));

            metrics.AirQuality = Double.Parse(webClient.DownloadString(airQualityUrl).Replace('.', ','));

            metrics.Pressure = Double.Parse(webClient.DownloadString(pressureUrl).Replace('.', ','));

            metrics.Particle = Double.Parse(webClient.DownloadString(particleUrl).Replace('.', ','));

        }



        private void updateComponents()

        {



            indoorTemperatureValue.Text = metrics.Temperature.ToString();

            indoorHumidityValue.Text = metrics.Temperature.ToString();

            co2IndoorValue.Text = metrics.Temperature.ToString();

            tvocIndoorValue.Text = metrics.Temperature.ToString();

            pressureIndoorValue.Text = metrics.Humidity.ToString();

            particleIndoorValue.Text = metrics.AirQuality.ToString();



        }





    }

}