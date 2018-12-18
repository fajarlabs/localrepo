using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SQLite;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SoftwareParkir
{
    public partial class FormLogin : Form
    {
        public FormLogin()
        {
            InitializeComponent();
            this.cb_level.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.TimerDateTime.Start();
            SQLiteConnection.CreateFile("DB.sqlite");
            System.Console.WriteLine("Create Database");
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        private void TimerDateTime_Tick(object sender, EventArgs e)
        {
            this.LTimer.Text = DateTime.Now.ToString("dd/MM/yyyy hh:mm:ss");
        }

        private void BTLogin_Click(object sender, EventArgs e)
        {
            FormMain fm = new FormMain();
            this.Hide();
            fm.ShowDialog();
            this.Close();
        }

        private void cb_level_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
