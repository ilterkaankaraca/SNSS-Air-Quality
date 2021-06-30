using System;
using System.Net;
using System.Net.Sockets;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace AirQuality.MWM.View
{
    /// <summary>
    /// Interaction logic for LoginView.xaml
    /// </summary>
    public partial class LoginView : UserControl
    {
        private string ipAddress;

        public LoginView()
        {
            InitializeComponent();
        }

        private void connectButton_click(object sender, RoutedEventArgs e)
        {
            //TODO: Save the latest IP in a file.
            if (String.IsNullOrWhiteSpace(ipTextBox.Text) == false)
            {
                string answer;
                try
                {
                    ipAddress = ipTextBox.Text;
                    using (WebClient wc = new WebClient())
                    {
                        
                        //check if it is ip or hostname
                        if (int.TryParse(ipAddress[0].ToString(), out _))
                            answer = wc.DownloadString("http://" + ipAddress + "/connect");
                        else
                            answer = wc.DownloadString("http://" + ipAddress + ".local/connect");
                    }
                    titleLabel.Text = answer;
                }
                catch (ArgumentNullException A)
                {
                    Console.WriteLine("ArgumentNullException: {0}", A);
                }
                catch (SocketException A)
                {
                    Console.WriteLine("SocketException: {0}", A);
                }
            }
            else
            {
                //errorLabel.Foreground = Brushes.Red;
                //errorLabel.Content = "Please type an IP address";
            }
        }
        
        private void ipTextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            ipTextBox.Text = null;
        }
    }
}
