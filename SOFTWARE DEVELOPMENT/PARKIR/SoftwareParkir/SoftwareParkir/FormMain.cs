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
    public partial class FormMain : Form
    {
        const int LEADING_SPACE = 12;
        const int CLOSE_SPACE = 15;
        const int CLOSE_AREA = 15;

        public FormMain()
        {
            InitializeComponent();
            tabControlPage.DrawMode = TabDrawMode.OwnerDrawFixed;
            tabControlPage.DrawItem += new DrawItemEventHandler(tabControlPage_DrawItem);
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            loadSideMenu();
            this.timer1.Start();
        }

        void switchPage(Form _form, String _tabName)
        {
            // Create new tab
            TabPage tp = new TabPage(_tabName);

            bool tab_is_exist = false;
            foreach (TabPage tab in tabControlPage.TabPages)
            {
                if(tab.Text.Equals(_tabName))
                {
                    tab_is_exist = true;
                    tp = tab;
                    break;
                }
            }

            // Check if tab is exist
            if (tab_is_exist)
            {
                tabControlPage.SelectedTab = tp;
            } else
            {
                tabControlPage.TabPages.Add(tp);
                tabControlPage.SelectedTab = tp;
                // Create component to add in panel
                Panel panel = new Panel();
                panel.Dock = DockStyle.Fill;
                tp.Controls.Add(panel);

                _form.FormBorderStyle = FormBorderStyle.None;
                _form.TopLevel = false;
                _form.AutoScroll = true;
                // Add form to panel
                panel.Controls.Add(_form);
                // show form
                _form.Show();
            }
        }

        void b_Click(object sender, EventArgs e,int id,String extraData)
        {
            var b = sender as Button;
            if (b != null)
            {
                // switch page
                switch(id)
                {
                    case 1:
                        switchPage(new FormTransaction(), extraData);
                        break;
                    case 6:
                        switchPage(new FormTestingUnit(),extraData);
                        break;
                    case 3:
                        switchPage(new FormUser(), extraData);
                        break;
                    case 5:
                        switchPage(new FormSetting(), extraData);
                        break;
                    default:
                        MessageBox.Show("Page not found!");
                        break;
                }

                // width tab control
                tabControlPage.SizeMode = TabSizeMode.Fixed;
                tabControlPage.ItemSize = new Size(100, 25);

                // x button
                // get the inital length
                int tabLength = tabControlPage.ItemSize.Width;

                // measure the text in each tab and make adjustment to the size
                for (int i = 0; i < this.tabControlPage.TabPages.Count; i++)
                {
                    TabPage currentPage = tabControlPage.TabPages[i];

                    int currentTabLength = TextRenderer.MeasureText(currentPage.Text, tabControlPage.Font).Width;
                    // adjust the length for what text is written
                    currentTabLength += LEADING_SPACE + CLOSE_SPACE + CLOSE_AREA;

                    if (currentTabLength > tabLength)
                    {
                        tabLength = currentTabLength;
                    }
                }

                // create the new size
                Size newTabSize = new Size(tabLength, tabControlPage.ItemSize.Height);
                tabControlPage.ItemSize = newTabSize;

            }
        }

        private void tabControlPage_DrawItem(object sender, DrawItemEventArgs e)
        {
            
            if (e.Index == tabControlPage.SelectedIndex)
            {

                Graphics g = e.Graphics;
                // This is the rectangle to draw "over" the tabpage title
                RectangleF headerRect = new RectangleF(e.Bounds.X, e.Bounds.Y + 2, e.Bounds.Width, e.Bounds.Height - 2);
                // This is the default colour to use for the non-selected tabs
                SolidBrush sb = new SolidBrush(Color.AntiqueWhite);
                g.FillRectangle(sb, e.Bounds);

                e.Graphics.DrawString(tabControlPage.TabPages[e.Index].Text,
                    new Font("Arial",9, FontStyle.Bold),
                    Brushes.DarkRed,
                    new PointF(e.Bounds.X + 3, e.Bounds.Y + 6));
            }
            else
            {
                e.Graphics.DrawString(tabControlPage.TabPages[e.Index].Text,
                    tabControlPage.Font,
                    Brushes.Black,
                    new PointF(e.Bounds.X + 3, e.Bounds.Y + 6));
            }
            
            //This code will render a "x" mark at the end of the Tab caption. 
            e.Graphics.DrawString("x", new Font("Arial", 9, FontStyle.Bold), Brushes.DarkRed, e.Bounds.Right - CLOSE_AREA, e.Bounds.Top + 6);
            e.DrawFocusRectangle();


        }

        private void loadSideMenu()
        {
            var listMenu = new List<Menu>();
            listMenu.Add(new Menu(1, "Transaction"));
            listMenu.Add(new Menu(2, "Dashboard"));
            listMenu.Add(new Menu(3, "User"));
            listMenu.Add(new Menu(4, "Report"));
            listMenu.Add(new Menu(5, "Setting"));
            listMenu.Add(new Menu(6, "Test Device"));
            listMenu.Add(new Menu(7, "Quit"));

            var rowCount = listMenu.Count;
            this.TBLayoutPanelMenu.RowCount = rowCount;
            this.TBLayoutPanelMenu.RowStyles.Clear();
            this.TBLayoutPanelMenu.Height = rowCount * 50;

            for (int i = 0; i < rowCount; i++)
            {
                this.TBLayoutPanelMenu.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100));
            }

            for (int i = 0; i < rowCount; i++)
            {
                Menu item = listMenu.ElementAt(i);
                var b = new Button();
                var id = item.id;
                b.Text = "» " + item.name;
                b.Name = string.Format("b_{0}", i + 1);

                // pass parameter via event onclick in button
                b.Click += (sender, EventArgs) => { b_Click(sender, EventArgs,id, item.name); };
                b.Dock = DockStyle.Fill;
                b.Font = new Font(b.Font.Name, 12, FontStyle.Bold);
                b.Margin = new Padding(0);
                b.TextAlign = ContentAlignment.MiddleLeft;
                b.BackColor = Color.DarkBlue;
                b.ForeColor = Color.White;
                this.TBLayoutPanelMenu.Controls.Add(b);
            }
        }

        private void tabControlPage_MouseDown(object sender, MouseEventArgs e)
        {
            //Looping through the controls.
            for (int i = 0; i < this.tabControlPage.TabPages.Count; i++)
            {
                Rectangle r = tabControlPage.GetTabRect(i);
                //Getting the position of the "x" mark.
                Rectangle closeButton = new Rectangle(r.Right - 12, r.Top + 8, 9, 7);
                if (closeButton.Contains(e.Location))
                {
                    if (MessageBox.Show("Would you like to Close this Tab?", "Confirm", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                    {
                        this.tabControlPage.TabPages.RemoveAt(i);
                        break;
                    }
                }
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            this.labelMainTimer.Text = DateTime.Now.ToString("dd/MM/yyyy hh:mm:ss");
        }
    }
}
