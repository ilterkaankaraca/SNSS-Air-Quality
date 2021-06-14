using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Air_Quality
{
    public partial class SettingsForm : Form
    {
        OverviewForm form2;
        public SettingsForm(OverviewForm form2)
        {
            //TODO: get the values somewhere
            this.form2 = form2;
            InitializeComponent();
        }

        private void FormSettings_FormClosing(object sender, FormClosingEventArgs e)
        {
            //TODO: save the values somewhere
            this.Hide();
            form2.Show();
            this.Dispose();
        }

        private void FormSettings_Load(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            //TODO: assign values
            this.Hide();
            form2.Show();
            this.Dispose();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            this.Hide();
            form2.Show();
            this.Dispose();
        }
    }
}
