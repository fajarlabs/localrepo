using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SoftwareParkir
{
    public partial class FormUser : Form
    {
        public FormUser()
        {
            InitializeComponent();
        }

        private void FormUser_Load(object sender, EventArgs e)
        {
            DataTable dt = new DataTable();
            DataColumn dc = new DataColumn("Username", typeof(String));
            dt.Columns.Add(dc);

            dc = new DataColumn("First Name", typeof(String));
            dt.Columns.Add(dc);

            dc = new DataColumn("Last Name", typeof(String));
            dt.Columns.Add(dc);

            dc = new DataColumn("Email", typeof(String));
            dt.Columns.Add(dc);

            dc = new DataColumn("Phone", typeof(String));
            dt.Columns.Add(dc);

            DataRow dr = dt.NewRow();
            dr[0] = "100020303045";
            dr[1] = "Fajar";
            dr[2] = "Rizki";
            dr[3] = "fajarrdp@gmail.com";
            dr[4] = "089663159652";
            dt.Rows.Add(dr);
            this.DGUser.DataSource = dt;

            DataGridViewButtonColumn btn = new DataGridViewButtonColumn();
            DGUser.Columns.Insert(5, btn);
            btn.HeaderText = "Action";
            btn.Text = "Edit";
            btn.FlatStyle = FlatStyle.Flat;
            btn.DefaultCellStyle.BackColor = Color.Red;
            btn.DefaultCellStyle.ForeColor = Color.White;
            btn.Name = "btn";

            btn.UseColumnTextForButtonValue = true;

            this.DGUser.AllowUserToAddRows = false;
        }

        private void DGUser_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.ColumnIndex == 3)
            {
                MessageBox.Show((e.RowIndex + 1) + "  Row  " + (e.ColumnIndex + 1) + "  Column button clicked ");
            }
        }
    }
}
