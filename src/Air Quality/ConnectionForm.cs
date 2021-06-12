using System;
using System.Drawing;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;

namespace Air_Quality
{
    public partial class ConnectionForm : Form
    {
        private string ipAddress;//   The servers hostname or IP address need //need to find a way to get it  
        private int PORT = 80;
        private string responseData;
        private TcpClient tcpClient;
        private NetworkStream stream;
        private Byte[] data;
        private int streamRead=-1;
        
        public ConnectionForm()
        {
            //TODO: Read the latest IP from the file.
            InitializeComponent();
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            //TODO: Save the latest IP in a file.
            Console.WriteLine(12);
            if (String.IsNullOrWhiteSpace(ipTextbox.Text)==false)
            {
                string answer;
                try
                {
                    ipAddress = ipTextbox.Text;
                    using (WebClient wc = new WebClient())
                    {
                        answer = wc.DownloadString("http://"+ ipAddress + "/connect");
                    }
                    statusLabel.Text = answer;
                }
                catch (ArgumentNullException A)
                {
                    Console.WriteLine("ArgumentNullException: {0}", A);
                }
                catch (SocketException A)
                {
                    Console.WriteLine("SocketException: {0}", A);
                }
            this.Hide();
            Form2 form2 = new Form2(this);
            form2.Show();
            }
            else 
            {
                statusLabel.ForeColor = Color.Red;
                statusLabel.Text = "Please type an IP address";
            }
        }
        public string IpAddress { get => ipAddress; set => ipAddress = value; }
    }
    
}
