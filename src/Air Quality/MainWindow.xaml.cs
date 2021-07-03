using AirQuality.MWM.Model;
using System;
using System.Net;
using System.Net.Sockets;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;
using Newtonsoft.Json;

namespace AirQuality
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        AirMetrics metrics;
        WebClient webClient;
        string jsonUrl;
        DispatcherTimer dispatcherTimer = new DispatcherTimer();
        string ipAddress;

        public MainWindow()
        {
            InitializeComponent();
            webClient = new WebClient();
            metrics = new AirMetrics();
            dispatcherTimer.Tick += dispatcherTimer_Tick;
            dispatcherTimer.Interval = new TimeSpan(0, 0, 5);
            button.IsEnabled = false;
            button.Foreground = Brushes.Gray;
            loginGrid.Visibility = Visibility.Visible;
            homeGrid1.Visibility = Visibility.Hidden;
            homeGrid2.Visibility = Visibility.Hidden;
            searchTextBox.Visibility = Visibility.Hidden;
        }
        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Application.Current.MainWindow.DragMove();
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
                string answer = webClient.DownloadString("http://" + ipAddress + "/city/?name=" + cityName);
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
                    using (webClient)
                    {

                        //check if it is ip or hostname
                        if (int.TryParse(ipAddress[0].ToString(), out _))
                        {

                            answer = webClient.DownloadString("http://" + ipAddress + "/connect");
                        }
                        else
                        {
                            ipAddress = ipAddress + ".local";
                            answer = webClient.DownloadString("http://" + ipAddress + "/connect");

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
                catch (WebException)
                {
                    ipTextBox.Text = "connection error";
                }
                if (answer == "connected")
                {
                    //Send city
                    jsonUrl = "http://" + ipAddress + "/json";
                    updateMetrics();
                    updateComponents();
                    dispatcherTimer.Start();
                    loginGrid.Visibility = Visibility.Hidden;
                    homeGrid1.Visibility = Visibility.Visible;
                    homeGrid2.Visibility = Visibility.Visible;
                    searchTextBox.Visibility = Visibility.Visible;
                }
            }
            else
            {
                ipTextBox.Foreground = Brushes.Red;
                ipTextBox.Text = "Please type an IP address";
            }
        }
        private void updateMetrics()
        {
            string json;
            try
            {       
                json = webClient.DownloadString(jsonUrl);
                metrics = JsonConvert.DeserializeObject<AirMetrics>(json);
            }
            catch (ArgumentNullException A)
            {
                Console.WriteLine("ArgumentNullException: {0}", A);
            }
            catch (SocketException A)
            {
                Console.WriteLine("SocketException: {0}", A);
            }
            catch (WebException)
            {
                ipTextBox.Text = "connection error";
            }
        }
        private void updateComponents()
        {
            indoorTemperatureValue.Text = metrics.IndoorTemperature.ToString().Substring(0, 2);
            indoorHumidityValue.Text = metrics.IndoorHumidity.ToString().Substring(0, 2);
            indoorCo2Value.Text = metrics.IndoorCo2.ToString();
            indoorTvocValue.Text = metrics.IndoorTvoc.ToString();
            indoorPressureValue.Text = metrics.IndoorPressure.ToString();
            indoorPm25Value.Text = metrics.IndoorPm25.ToString();
            indoorPm10Value.Text = metrics.IndoorPm25.ToString();

            outdoorTemperatureValue.Text = metrics.OutdoorTemperature.ToString().Substring(0, 2);
            outdoorHumidityValue.Text = metrics.OutdoorHumidity.ToString().Substring(0, 2);
            outdoorCo2Value.Text = metrics.OutdoorCo2.ToString();
            outdoorTvocValue.Text = metrics.OutdoorTvoc.ToString();
            outdoorPressureValue.Text = metrics.OutdoorPressure.ToString();
            outdoorPm25Value.Text = metrics.OutdoorPm25.ToString();
            outdoorPm10Value.Text = metrics.OutdoorPm10.ToString();
        }
        private bool isConnected()
        {
            try
            {
                webClient.DownloadString("http://" + ipAddress + "/connect");
            }
            catch (ArgumentNullException A)
            {
                return false;
            }
            catch (SocketException A)
            {
                return false;
            }
            catch (WebException)
            {
                return false;
            }
            return true;
        }
    }
}


