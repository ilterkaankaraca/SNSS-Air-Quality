using AirQuality.MWM.Model;
using System;
using System.Net;
using System.Net.Sockets;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;

namespace AirQuality
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        AirMetrics indoorMetrics, outdoorMetrics;
        WebClient webClient;
        string indoorTemperatureUrl, indoorHumidityUrl, indoorCo2Url, indoorTvocUrl, indoorPressureUrl, indoorPm25Url, indoorPm10Url;
        string outdoorTemperatureUrl, outdoorHumidityUrl, outdoorCo2Url, outdoorTvocUrl, outdoorPressureUrl, outdoorPm25Url, outdoorPm10Url;

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            App.Current.MainWindow.DragMove();
        }
 

        DispatcherTimer dispatcherTimer = new DispatcherTimer();
        WebClient wc;
        string ipAddress;

        public MainWindow()
        {
            InitializeComponent();
            wc = new WebClient();
            webClient = new WebClient();
            indoorMetrics = new AirMetrics();
            outdoorMetrics = new AirMetrics();
            dispatcherTimer.Tick += dispatcherTimer_Tick;
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            button.IsEnabled = false;
            button.Foreground = Brushes.Gray;
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            updateMetrics();
            updateComponents();
        }


        private void searchTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            string cityName = searchTextBox.Text;
            searchTextBox.Text = String.Empty;
            if (e.Key == Key.Enter)
            {
                string answer = wc.DownloadString("http://+ ipAddress +/city/?name=" + cityName);
            }
            else
            {
                searchTextBox.Text = cityName;
            }

        }

        private void ipTextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            ipTextBox.Text = null;
            button.IsEnabled = true;
            button.Foreground = Brushes.Black;
        }
        private void closeButton_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();

        }
        private void connectButton_Click(object sender, RoutedEventArgs e)
        {

            if (String.IsNullOrWhiteSpace(ipTextBox.Text) == false)
            {
                string answer = "";
                try
                {
                    ipAddress = ipTextBox.Text;
                    using (WebClient wc = new WebClient())
                    {

                        //check if it is ip or hostname
                        if (int.TryParse(ipAddress[0].ToString(), out _))
                        {

                            answer = wc.DownloadString("http://" + ipAddress + "/connect");
                        }
                        else
                        {
                            ipAddress = ipAddress + ".local";
                            answer = wc.DownloadString("http://" + ipAddress + "/connect");

                        }
                    }
                    ipTextBox.Foreground = Brushes.Red;
                }
                catch (ArgumentNullException A)
                {
                    Console.WriteLine("ArgumentNullException: {0}", A);
                }
                catch (SocketException A)
                {
                    Console.WriteLine("SocketException: {0}", A);
                }
                catch (WebException A)
                {
                    ipTextBox.Text = "connection error";
                }
                if (answer == "connected")
                {
                    indoorTemperatureUrl = "http://" + ipAddress + "/indoorTemperature/";
                    indoorHumidityUrl = "http://" + ipAddress + "/indoorHumidity/";
                    indoorCo2Url = "http://" + ipAddress + "/indoorCo2/";
                    indoorTvocUrl = "http://" + ipAddress + "/indoorTvoc/";
                    indoorPressureUrl = "http://" + ipAddress + "/indoorPressure/";
                    indoorPm25Url = "http://" + ipAddress + "/indoorPm25/";
                    indoorPm10Url = "http://" + ipAddress + "/indoorPm10/";
                    outdoorTemperatureUrl = "http://" + ipAddress + "/outdoorTemperature/";
                    outdoorHumidityUrl = "http://" + ipAddress + "/outdoorHumidity/";
                    outdoorCo2Url = "http://" + ipAddress + "/outdoorCo2/";
                    outdoorTvocUrl = "http://" + ipAddress + "/outdoorTvoc/";
                    outdoorPressureUrl = "http://" + ipAddress + "/outdoorPressure/";
                    outdoorPm25Url = "http://" + ipAddress + "/outdoorPm25/";
                    outdoorPm10Url = "http://" + ipAddress + "/outdoorPm10/";
                    updateMetrics();
                    updateComponents();
                    dispatcherTimer.Start();
                    
                }
                loginGrid.Visibility = Visibility.Hidden;
                homeGrid1.Visibility = Visibility.Visible;
                homeGrid2.Visibility = Visibility.Visible;
                searchTextBox.Visibility = Visibility.Visible;
            }
            else
            {
                ipTextBox.Foreground = Brushes.Red;
                ipTextBox.Text = "Please type an IP address";
            }
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
            outdoorPm10Value.Text = outdoorMetrics.Pm10.ToString();
        }
    }
}

