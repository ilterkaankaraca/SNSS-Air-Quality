using System;
using System.Windows.Forms;
using System.Net;

namespace Air_Quality
{
    public partial class OverviewForm : Form
    {
        
        private LoginForm form1;
        AirMetrics metrics;
        WebClient webClient;
        string temperatureUrl, humidityUrl, co2Url, airQualityUrl, pressureUrl, particleUrl;

        private void label1_Click(object sender, EventArgs e)
        {

        }

        public OverviewForm(LoginForm form1)
        {
            InitializeComponent();
            this.form1= form1;
            temperatureUrl = "http://" + form1.IpAddress + "/temperature/";
            humidityUrl = "http://" + form1.IpAddress + "/humidity/";
            co2Url = "http://" + form1.IpAddress + "/co2/";
            airQualityUrl = "http://" + form1.IpAddress + "/airQuality/";
            pressureUrl = "http://" + form1.IpAddress + "/pressure/";
            particleUrl = "http://" + form1.IpAddress + "/particle/";
            webClient = new WebClient();
            metrics = new AirMetrics();
            updateMetrics();
            updateComponents();
            
            timer.Enabled = true;
        }

        private void Form2_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        private void settingsButton_Click(object sender, EventArgs e)
        {
            SettingsForm formSettings = new SettingsForm(this);
            formSettings.Show();
            this.Hide();
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            updateMetrics();
            updateComponents();
        }

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
            indoorTemperatureLabel.Text=metrics.Temperature.ToString() ;
            indoorHumidityLabel.Text = metrics.Humidity.ToString();
            indoorCo2Label.Text = metrics.Co2.ToString();
            indoorAirqualityLevel.Text = metrics.AirQuality.ToString();
            indoorPressureLevel.Text = metrics.Pressure.ToString();
            indoorParticleLevel.Text = metrics.Particle.ToString();
        }
    }
}
