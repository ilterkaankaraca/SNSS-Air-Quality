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
        AirInformation airInfo;
        WebClient webClient;
        string jsonUrl;
        DispatcherTimer dispatcherTimer = new DispatcherTimer();
        string ipAddress;
        string cityName;
        bool pressedEnter;
        public MainWindow()
        {
            InitializeComponent();
            webClient = new WebClient();
            airInfo = new AirInformation();
            dispatcherTimer.Tick += dispatcherTimer_Tick;
            dispatcherTimer.Interval = new TimeSpan(0, 0, 5);
            button.IsEnabled = false;
            button.Foreground = Brushes.Gray;
            loginGrid.Visibility = Visibility.Visible;
            homeGrid1.Visibility = Visibility.Hidden;
            homeGrid2.Visibility = Visibility.Hidden;
            citySearchTextBox.Visibility = Visibility.Hidden;
            notificationLabel.Visibility = Visibility.Hidden;
            cityName = citySearchTextBox.Text;
            pressedEnter = false;
        }
        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Application.Current.MainWindow.DragMove();
        }
        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            getAirInformationData();
            updateComponents();
        }
        private void searchTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            string cityName = citySearchTextBox.Text;
            if (e.Key == Key.Enter)
            {
                string answer = webClient.DownloadString("http://" + ipAddress + "/city/?name=" + cityName.Replace(" ", "+"));
                airInfo.City = cityName;
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

                    getAirInformationData();
                    updateComponents();
                    dispatcherTimer.Start();
                    loginGrid.Visibility = Visibility.Hidden;
                    homeGrid1.Visibility = Visibility.Visible;
                    homeGrid2.Visibility = Visibility.Visible;
                    citySearchTextBox.Visibility = Visibility.Visible;
                    notificationLabel.Visibility = Visibility.Visible;
                }
            }
            else
            {
                ipTextBox.Foreground = Brushes.Red;
                ipTextBox.Text = "Please type an IP address";
            }
        }
        //to get air metrics and notification from server
        private void getAirInformationData()
        {
            string json;
            try
            {
                json = webClient.DownloadString(jsonUrl);
                airInfo = JsonConvert.DeserializeObject<AirInformation>(json);
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

        private void citySearchTextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            pressedEnter = false;
            cityName = citySearchTextBox.Text;
            citySearchTextBox.Text = String.Empty;
        }

        private void updateComponents()
        {
            indoorTemperatureValue.Text = airInfo.IndoorTemperature.ToString().Substring(0, 2);
            indoorHumidityValue.Text = airInfo.IndoorHumidity.ToString().Substring(0, 2);
            indoorCo2Value.Text = airInfo.IndoorCo2.ToString();
            indoorTvocValue.Text = airInfo.IndoorTvoc.ToString();
            indoorPressureValue.Text = airInfo.IndoorPressure.ToString();
            indoorPm25Value.Text = airInfo.IndoorPm25.ToString();
            indoorPm10Value.Text = airInfo.IndoorPm25.ToString();

            outdoorTemperatureValue.Text = airInfo.OutdoorTemperature.ToString().Substring(0, 2);
            outdoorHumidityValue.Text = airInfo.OutdoorHumidity.ToString().Substring(0, 2);
            outdoorCo2Value.Text = airInfo.OutdoorCo2.ToString();
            outdoorTvocValue.Text = airInfo.OutdoorTvoc.ToString();
            outdoorPressureValue.Text = airInfo.OutdoorPressure.ToString();
            outdoorPm25Value.Text = airInfo.OutdoorPm25.ToString();
            outdoorPm10Value.Text = airInfo.OutdoorPm10.ToString();
            if (citySearchTextBox.IsFocused == false)
            {
                citySearchTextBox.Text = airInfo.City;
            }
            if (airInfo.Notification == "1")
                notificationLabel.Content = "Ventilation is required";
            else
                notificationLabel.Content = "Ventilation is not required";
        }
        private void citySearchTextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (!pressedEnter)
                citySearchTextBox.Text = cityName;
        }      
    }
}


