using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Air_Quality
{
    public partial class Form1 : Form
    {
        string ipAddress;
        public Form1()
        {
            //TODO: Read the latest IP from the file.
            InitializeComponent();
        }

        private void connectButton_Click(object sender, EventArgs e)
        {   
            //TODO: Save the latest IP in a file.
            ipAddress = ipTextbox.Text;
            Form2 form2 = new Form2(this);
            form2.Show();
            this.Hide();
        }
    }
}
