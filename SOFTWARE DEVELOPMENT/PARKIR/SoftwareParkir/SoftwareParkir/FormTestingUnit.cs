using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using NAudio;
using NAudio.Wave;
using System.Media;

namespace SoftwareParkir
{
    public partial class FormTestingUnit : Form
    {
        delegate void SetTextCallback(string text);
        private String Sr;
        private IWavePlayer waveOutDevice;

        // state untuk MP3
        private static Boolean stateMp3 = false;

        // state STEP Pintu Masuk
        private String statePintuMasuk = "STEP1";
        private Boolean btnStatePintuMasuk = false;
        private Boolean openLockDoor = false;

        // variable conversi
        private Boolean loop1 = false;
        private Boolean tombol1 = false;
        private Boolean loop2 = false;

        public FormTestingUnit()
        {
            InitializeComponent();
            initComPortToCBox();
            disableButton();
            this.btnPintuMasuk.BackColor = Color.DarkGray;
            this.btnPintuMasuk.Text = "Flow Masuk [Off]";
        }

        private void initComPortToCBox()
        {
            List<String> tList = new List<String>();

            comboBoxCom.Items.Clear();

            foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
            {
                tList.Add(s);
            }

            tList.Sort();
            comboBoxCom.Items.AddRange(tList.ToArray());
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if(this.btnConnect.Text.Equals("Connect"))
            {
                var stats = this.openComPort();
                if (stats)
                {
                    this.btnConnect.Text = "Disconnect";
                    this.enableButton();
                }
            } else
            {
                this.btnConnect.Text = "Connect";
                this.closeComPort();
                this.disableButton();
            }
            
        }

        private Boolean openComPort()
        {
            if (!this.serialPort1.IsOpen)
            {
                try
                {
                    this.serialPort1.PortName = "COM3";
                    this.serialPort1.Parity = System.IO.Ports.Parity.None;
                    this.serialPort1.DataBits = 8;
                    this.serialPort1.StopBits = System.IO.Ports.StopBits.One;
                    this.serialPort1.RtsEnable = false;
                    this.serialPort1.DtrEnable = false;
                    this.serialPort1.BaudRate = 9600;
                    this.serialPort1.Handshake = System.IO.Ports.Handshake.None;
                    this.serialPort1.WriteTimeout = System.IO.Ports.SerialPort.InfiniteTimeout;
                    this.serialPort1.Open();
                    this.timer1.Start();
                    this.enableButton();
                    return true;
                }catch(Exception e) {
                    MessageBox.Show("Connection failed. Please check usb plug or rs232 cable.", "Error Dialog");
                }
            }
            return false;
        }

        private void closeComPort()
        {
            if (this.serialPort1.IsOpen)
            {
                this.serialPort1.Close();
            }
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            this.Sr += this.serialPort1.ReadExisting().ToString();
            Console.WriteLine(this.Sr);
        }

        private void commandSerial(String Cs)
        {
            if (this.serialPort1.IsOpen)
            {
                this.serialPort1.Write(Cs);
            } else
            {
                MessageBox.Show("Port is closed!","Error Information");
            }
        }

        private void btnPing_Click(object sender, EventArgs e)
        {
            this.commandSerial("*PING#");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            closeComPort();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(this.Sr))
            {
                this.textBox1.AppendText(this.Sr);
                /*if (this.Sr.ToLower().Contains('*') && this.Sr.ToLower().Contains('#'))
                {
                    this.textBox1.AppendText(this.Sr + Environment.NewLine);
                    String rs = convertSerial(this.Sr);
                    this.textBox1.AppendText(rs + Environment.NewLine);
                    ledInput(this.Sr);
                    // implementasi untuk flow pintu masuk
                    flow_pintu_masuk();
                    this.Sr = "";
                }
                */
            }
        }

        private String convertSerial(String dataSerial)
        {

            // IN1 sebagai loop1
            if(dataSerial.Equals("*IN1ON#"))
            {
                this.loop1 = true;
            }
            if (dataSerial.Equals("*IN1OFF#"))
            {
                this.loop1 = false;
            }

            // IN1 sebagai tombol
            if (dataSerial.Equals("*IN2ON#"))
            {
                this.tombol1 = true;
            }
            if (dataSerial.Equals("*IN2OFF#"))
            {
                this.tombol1 = false;
            }

            // IN1 sebagai loop2
            if (dataSerial.Equals("*IN3ON#"))
            {
                this.loop2 = true;
            }
            if (dataSerial.Equals("*IN3OFF#"))
            {
                this.loop2 = false;
            }

            if((this.loop1 == true) && (this.tombol1 == false) && (this.loop2 == false))
            {
                return "A";
            }
            if (this.loop1 == false && this.tombol1 == true && this.loop2 == false)
            {
                return "C";
            }
            if (this.loop1 == false && this.tombol1 == false && this.loop2 == true)
            {
                return "E";
            }
            if (this.loop1 == true && this.tombol1 == true && this.loop2 == false)
            {
                return "B";
            }

            return "";
        }

        private void reset_led_flow()
        {
            this.led_flow_1.Image = Properties.Resources.red_circle;
            this.led_flow_2.Image = Properties.Resources.red_circle;
            this.led_flow_3.Image = Properties.Resources.red_circle;
            this.led_flow_4.Image = Properties.Resources.red_circle;
        }
        private void flow_pintu_masuk()
        {
            // ==============================================================================================
            // FLOW UNTUK PINTU MASUK
            // ==============================================================================================
            if (this.btnStatePintuMasuk)
            {
                switch (this.statePintuMasuk)
                {
                    case "STEP1":
                        if (this.Sr.Equals("*IN1ON#"))
                        {
                            this.reset_led_flow();
                            this.led_flow_1.Image = Properties.Resources.green_circle;
                            play_mp3("D:\\github\\kumpulan-skema\\SOFTWARE DEVELOPMENT\\PARKIR\\SoftwareParkir\\SoftwareParkir\\voice\\welcome.mp3");
                            this.statePintuMasuk = "STEP2";
                        }
                        break;
                    case "STEP2":
                        if (this.Sr.Equals("*IN2ON#"))
                        {
                            this.reset_led_flow();
                            this.led_flow_2.Image = Properties.Resources.green_circle;
                            // Cetak tiket parkir
                            play_mp3("D:\\github\\kumpulan-skema\\SOFTWARE DEVELOPMENT\\PARKIR\\SoftwareParkir\\SoftwareParkir\\voice\\print_ticket.mp3");
                            this.statePintuMasuk = "STEP3";
                        }
                        break;
                    case "STEP3":
                        if (this.Sr.Equals("*IN3ON#"))
                        {
                            this.reset_led_flow();
                            this.led_flow_3.Image = Properties.Resources.green_circle;
                            // Menunggu status OFF agar palang turun kembali
                            play_mp3("D:\\github\\kumpulan-skema\\SOFTWARE DEVELOPMENT\\PARKIR\\SoftwareParkir\\SoftwareParkir\\voice\\thanks.mp3");
                        }
                        if (this.Sr.Equals("*IN3OFF#"))
                        {
                            this.reset_led_flow();
                            this.led_flow_4.Image = Properties.Resources.green_circle;
                            play_mp3("D:\\github\\kumpulan-skema\\SOFTWARE DEVELOPMENT\\PARKIR\\SoftwareParkir\\SoftwareParkir\\voice\\close_door.mp3");
                            this.statePintuMasuk = "STEP1";
                        }
                        break;
                }
            }
        }

        private void play_mp3(String path)
        {
            Task.Run(() =>
            {
                if (stateMp3 != true)
                {
                    stateMp3 = true;
                    try
                    {
                        waveOutDevice = new WaveOut();
                        AudioFileReader audioFileReader = new AudioFileReader(path);
                        waveOutDevice.Init(audioFileReader);
                        waveOutDevice.Play();
                        while (waveOutDevice.PlaybackState == PlaybackState.Playing)
                        {
                            Thread.Sleep(500);
                        }
                        stateMp3 = false;
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);
                    }
                }
            });
        }

        private void ledInput(String str)
        {
            if(str.Equals("*IN1ON#"))
            {
                this.pictureBox1.Image = Properties.Resources.green_circle;
            } else
            {
                this.pictureBox1.Image = Properties.Resources.red_circle;
            }

            if (str.Equals("*IN2ON#"))
            {
                this.pictureBox2.Image = Properties.Resources.green_circle;
            }
            else
            {
                this.pictureBox2.Image = Properties.Resources.red_circle;
            }

            if (str.Equals("*IN3ON#"))
            {
                this.pictureBox3.Image = Properties.Resources.green_circle;
            }
            else
            {
                this.pictureBox3.Image = Properties.Resources.red_circle;
            }

            if (str.Equals("*IN4ON#"))
            {
                this.pictureBox4.Image = Properties.Resources.green_circle;
            }
            else
            {
                this.pictureBox4.Image = Properties.Resources.red_circle;
            }
        }

        private void btnStat_Click(object sender, EventArgs e)
        {
            this.commandSerial("*STAT#");
        }

        private void btnOpen1_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OPEN1#");
        }

        private void btnClose1_Click(object sender, EventArgs e)
        {
            this.commandSerial("*CLOSE1#");
        }

        private void btnStop1_Click(object sender, EventArgs e)
        {
            this.commandSerial("*STOP1#");
        }

        private void btnOpen2_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OPEN2#");
        }

        private void btnClose2_Click(object sender, EventArgs e)
        {
            this.commandSerial("*CLOSE2#");
        }

        private void btnStop2_Click(object sender, EventArgs e)
        {
            this.commandSerial("*STOP2#");
        }

        private void btnOpen3_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OPEN3#");
        }

        private void btnClose3_Click(object sender, EventArgs e)
        {
            this.commandSerial("*CLOSE3#");
        }

        private void btnStop3_Click(object sender, EventArgs e)
        {
            this.commandSerial("*STOP3#");
        }

        private void btnOut1On_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT1ON#");
        }

        private void btnOut2On_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT2ON#");
        }

        private void btnOut3On_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT3ON#");
        }

        private void btnOut4On_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT4ON#");
        }

        private void btnOut5On_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT5ON#");
        }

        private void btnOut1Off_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT1OFF#");
        }

        private void btnOut2Off_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT2OFF#");
        }

        private void btnOut3Off_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT3OFF#");
        }

        private void btnOut4Off_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT4OFF#");
        }

        private void btnOut5Off_Click(object sender, EventArgs e)
        {
            this.commandSerial("*OUT5OFF#");
        }

        private void FormTestingUnit_Load(object sender, EventArgs e)
        {
            this.pictureBox1.Image = Properties.Resources.red_circle;
            this.pictureBox2.Image = Properties.Resources.red_circle;
            this.pictureBox3.Image = Properties.Resources.red_circle;
            this.pictureBox4.Image = Properties.Resources.red_circle;

            this.led_flow_1.Image = Properties.Resources.red_circle;
            this.led_flow_2.Image = Properties.Resources.red_circle;
            this.led_flow_3.Image = Properties.Resources.red_circle;
            this.led_flow_4.Image = Properties.Resources.red_circle;
        }

        private void enableButton()
        {
            this.btnOpen1.Enabled = true;
            this.btnClose1.Enabled = true;
            this.btnStop1.Enabled = true;

            this.btnOpen2.Enabled = true;
            this.btnClose2.Enabled = true;
            this.btnStop2.Enabled = true;

            this.btnOpen3.Enabled = true;
            this.btnClose3.Enabled = true;
            this.btnStop3.Enabled = true;

            this.btnOut1On.Enabled = true;
            this.btnOut2On.Enabled = true;
            this.btnOut3On.Enabled = true;
            this.btnOut4On.Enabled = true;
            this.btnOut5On.Enabled = true;

            this.btnOut1Off.Enabled = true;
            this.btnOut2Off.Enabled = true;
            this.btnOut3Off.Enabled = true;
            this.btnOut4Off.Enabled = true;
            this.btnOut5Off.Enabled = true;

            this.btnPing.Enabled = true;
            this.btnStat.Enabled = true;
        }

        private void disableButton()
        {
            this.btnOpen1.Enabled = false;
            this.btnClose1.Enabled = false;
            this.btnStop1.Enabled = false;

            this.btnOpen2.Enabled = false;
            this.btnClose2.Enabled = false;
            this.btnStop2.Enabled = false;

            this.btnOpen3.Enabled = false;
            this.btnClose3.Enabled = false;
            this.btnStop3.Enabled = false;

            this.btnOut1On.Enabled = false;
            this.btnOut2On.Enabled = false;
            this.btnOut3On.Enabled = false;
            this.btnOut4On.Enabled = false;
            this.btnOut5On.Enabled = false;

            this.btnOut1Off.Enabled = false;
            this.btnOut2Off.Enabled = false;
            this.btnOut3Off.Enabled = false;
            this.btnOut4Off.Enabled = false;
            this.btnOut5Off.Enabled = false;

            this.btnPing.Enabled = false;
            this.btnStat.Enabled = false;
        }

        private void btnPintuMasuk_Click(object sender, EventArgs e)
        {
            if(!this.btnStatePintuMasuk)
            {
                this.btnPintuMasuk.BackColor = Color.Red;
                this.btnStatePintuMasuk = true;
                this.btnPintuMasuk.Text = "Flow Masuk [On]";
            } else
            {
                this.btnPintuMasuk.BackColor = Color.DarkGray;
                this.btnStatePintuMasuk = false;
                this.btnPintuMasuk.Text = "Flow Masuk [Off]";
            }
        }

        private void btnPintuKeluar_Click(object sender, EventArgs e)
        {
            if(this.tb_output_command.Text.Equals("Q"))
            {
                this.commandSerial("*OUT1ON#");
                this.commandSerial("*OUT2ON#");
                this.commandSerial("*OUT3ON#");
            }

            if (this.tb_output_command.Text.Equals("P"))
            {
                this.commandSerial("*OUT1OFF#");
                this.commandSerial("*OUT2OFF#");
                this.commandSerial("*OUT3OFF#");
            }
        }

        private void label10_Click(object sender, EventArgs e)
        {

        }
    }
}
