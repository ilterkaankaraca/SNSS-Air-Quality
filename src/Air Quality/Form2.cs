using System;
using System.Windows.Forms;
using System.Net;

namespace Air_Quality
{
    public partial class Form2 : Form
    {
        
        private ConnectionForm form1;
        AirMetrics metrics;
        WebClient webClient;
        string temperatureUrl;
        int count = 0;
        public Form2(ConnectionForm form1)
        {
            this.form1= form1;
            temperatureUrl = "http://" + form1.IpAddress + "/temperature/";
            InitializeComponent();
            webClient = new WebClient();
            metrics = new AirMetrics();
            metrics.Temperature = Double.Parse(webClient.DownloadString(temperatureUrl));
            timer.Enabled = true;
           
        }

        private void Form2_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        private void settingsButton_Click(object sender, EventArgs e)
        {
            FormSettings formSettings = new FormSettings(this);
            formSettings.Show();
            this.Hide();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            metrics.Temperature = Double.Parse(webClient.DownloadString(temperatureUrl));
            indoorTemperatureLabel.Text = metrics.Temperature.ToString();
        }
    }
}
