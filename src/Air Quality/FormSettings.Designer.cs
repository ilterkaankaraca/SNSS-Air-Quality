
namespace Air_Quality
{
    partial class FormSettings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.searchButton = new System.Windows.Forms.Button();
            this.saveButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.temperatureCheckBox = new System.Windows.Forms.CheckBox();
            this.pressureCheckBox = new System.Windows.Forms.CheckBox();
            this.particleCheckBox = new System.Windows.Forms.CheckBox();
            this.airQualityCheckBox = new System.Windows.Forms.CheckBox();
            this.humidityCheckBox = new System.Windows.Forms.CheckBox();
            this.co2CheckBox = new System.Windows.Forms.CheckBox();
            this.cityTextBox = new System.Windows.Forms.TextBox();
            this.particleTextBox = new System.Windows.Forms.TextBox();
            this.humidityTextBox = new System.Windows.Forms.TextBox();
            this.airQualityTextBox = new System.Windows.Forms.TextBox();
            this.pressureTextBox = new System.Windows.Forms.TextBox();
            this.co2TextBox = new System.Windows.Forms.TextBox();
            this.temperatureTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // searchButton
            // 
            this.searchButton.Location = new System.Drawing.Point(242, 264);
            this.searchButton.Name = "searchButton";
            this.searchButton.Size = new System.Drawing.Size(75, 23);
            this.searchButton.TabIndex = 0;
            this.searchButton.Text = "Search";
            this.searchButton.UseVisualStyleBackColor = true;
            this.searchButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // saveButton
            // 
            this.saveButton.Location = new System.Drawing.Point(161, 217);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(75, 23);
            this.saveButton.TabIndex = 1;
            this.saveButton.Text = "Save";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(242, 217);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 2;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // temperatureCheckBox
            // 
            this.temperatureCheckBox.AutoSize = true;
            this.temperatureCheckBox.Location = new System.Drawing.Point(53, 71);
            this.temperatureCheckBox.Name = "temperatureCheckBox";
            this.temperatureCheckBox.Size = new System.Drawing.Size(92, 19);
            this.temperatureCheckBox.TabIndex = 3;
            this.temperatureCheckBox.Text = "Temperature";
            this.temperatureCheckBox.UseVisualStyleBackColor = true;
            this.temperatureCheckBox.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // pressureCheckBox
            // 
            this.pressureCheckBox.AutoSize = true;
            this.pressureCheckBox.Location = new System.Drawing.Point(47, 180);
            this.pressureCheckBox.Name = "pressureCheckBox";
            this.pressureCheckBox.Size = new System.Drawing.Size(70, 19);
            this.pressureCheckBox.TabIndex = 4;
            this.pressureCheckBox.Text = "Pressure";
            this.pressureCheckBox.UseVisualStyleBackColor = true;
            // 
            // particleCheckBox
            // 
            this.particleCheckBox.AutoSize = true;
            this.particleCheckBox.Location = new System.Drawing.Point(183, 179);
            this.particleCheckBox.Name = "particleCheckBox";
            this.particleCheckBox.Size = new System.Drawing.Size(65, 19);
            this.particleCheckBox.TabIndex = 5;
            this.particleCheckBox.Text = "Particle";
            this.particleCheckBox.UseVisualStyleBackColor = true;
            // 
            // airQualityCheckBox
            // 
            this.airQualityCheckBox.AutoSize = true;
            this.airQualityCheckBox.Location = new System.Drawing.Point(183, 125);
            this.airQualityCheckBox.Name = "airQualityCheckBox";
            this.airQualityCheckBox.Size = new System.Drawing.Size(82, 19);
            this.airQualityCheckBox.TabIndex = 6;
            this.airQualityCheckBox.Text = "Air Quality";
            this.airQualityCheckBox.UseVisualStyleBackColor = true;
            // 
            // humidityCheckBox
            // 
            this.humidityCheckBox.AutoSize = true;
            this.humidityCheckBox.Location = new System.Drawing.Point(183, 71);
            this.humidityCheckBox.Name = "humidityCheckBox";
            this.humidityCheckBox.Size = new System.Drawing.Size(76, 19);
            this.humidityCheckBox.TabIndex = 7;
            this.humidityCheckBox.Text = "Humidity";
            this.humidityCheckBox.UseVisualStyleBackColor = true;
            // 
            // co2CheckBox
            // 
            this.co2CheckBox.AutoSize = true;
            this.co2CheckBox.Location = new System.Drawing.Point(53, 125);
            this.co2CheckBox.Name = "co2CheckBox";
            this.co2CheckBox.Size = new System.Drawing.Size(49, 19);
            this.co2CheckBox.TabIndex = 8;
            this.co2CheckBox.Text = "CO2";
            this.co2CheckBox.UseVisualStyleBackColor = true;
            // 
            // cityTextBox
            // 
            this.cityTextBox.ForeColor = System.Drawing.SystemColors.ActiveBorder;
            this.cityTextBox.Location = new System.Drawing.Point(14, 265);
            this.cityTextBox.Name = "cityTextBox";
            this.cityTextBox.Size = new System.Drawing.Size(222, 23);
            this.cityTextBox.TabIndex = 9;
            this.cityTextBox.Text = "Search city";
            // 
            // particleTextBox
            // 
            this.particleTextBox.Location = new System.Drawing.Point(183, 150);
            this.particleTextBox.Name = "particleTextBox";
            this.particleTextBox.Size = new System.Drawing.Size(100, 23);
            this.particleTextBox.TabIndex = 10;
            // 
            // humidityTextBox
            // 
            this.humidityTextBox.Location = new System.Drawing.Point(183, 42);
            this.humidityTextBox.Name = "humidityTextBox";
            this.humidityTextBox.Size = new System.Drawing.Size(100, 23);
            this.humidityTextBox.TabIndex = 11;
            // 
            // airQualityTextBox
            // 
            this.airQualityTextBox.Location = new System.Drawing.Point(183, 96);
            this.airQualityTextBox.Name = "airQualityTextBox";
            this.airQualityTextBox.Size = new System.Drawing.Size(100, 23);
            this.airQualityTextBox.TabIndex = 12;
            // 
            // pressureTextBox
            // 
            this.pressureTextBox.Location = new System.Drawing.Point(47, 151);
            this.pressureTextBox.Name = "pressureTextBox";
            this.pressureTextBox.Size = new System.Drawing.Size(100, 23);
            this.pressureTextBox.TabIndex = 13;
            // 
            // co2TextBox
            // 
            this.co2TextBox.Location = new System.Drawing.Point(47, 96);
            this.co2TextBox.Name = "co2TextBox";
            this.co2TextBox.Size = new System.Drawing.Size(100, 23);
            this.co2TextBox.TabIndex = 15;
            // 
            // temperatureTextBox
            // 
            this.temperatureTextBox.Location = new System.Drawing.Point(47, 42);
            this.temperatureTextBox.Name = "temperatureTextBox";
            this.temperatureTextBox.Size = new System.Drawing.Size(100, 23);
            this.temperatureTextBox.TabIndex = 16;
            // 
            // FormSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(327, 294);
            this.Controls.Add(this.temperatureTextBox);
            this.Controls.Add(this.co2TextBox);
            this.Controls.Add(this.pressureTextBox);
            this.Controls.Add(this.airQualityTextBox);
            this.Controls.Add(this.humidityTextBox);
            this.Controls.Add(this.particleTextBox);
            this.Controls.Add(this.cityTextBox);
            this.Controls.Add(this.co2CheckBox);
            this.Controls.Add(this.humidityCheckBox);
            this.Controls.Add(this.airQualityCheckBox);
            this.Controls.Add(this.particleCheckBox);
            this.Controls.Add(this.pressureCheckBox);
            this.Controls.Add(this.temperatureCheckBox);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.saveButton);
            this.Controls.Add(this.searchButton);
            this.Name = "FormSettings";
            this.Text = "Settings";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormSettings_FormClosing);
            this.Load += new System.EventHandler(this.FormSettings_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button searchButton;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.CheckBox temperatureCheckBox;
        private System.Windows.Forms.CheckBox pressureCheckBox;
        private System.Windows.Forms.CheckBox particleCheckBox;
        private System.Windows.Forms.CheckBox airQualityCheckBox;
        private System.Windows.Forms.CheckBox humidityCheckBox;
        private System.Windows.Forms.CheckBox co2CheckBox;
        private System.Windows.Forms.TextBox cityTextBox;
        private System.Windows.Forms.TextBox particleTextBox;
        private System.Windows.Forms.TextBox humidityTextBox;
        private System.Windows.Forms.TextBox airQualityTextBox;
        private System.Windows.Forms.TextBox pressureTextBox;
        private System.Windows.Forms.TextBox co2TextBox;
        private System.Windows.Forms.TextBox temperatureTextBox;
    }
}