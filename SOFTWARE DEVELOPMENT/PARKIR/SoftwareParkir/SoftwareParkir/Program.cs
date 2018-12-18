using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace SoftwareParkir
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// If Error "Add Reference" for resolving in https://developercommunity.visualstudio.com/content/problem/260196/add-a-reference-raise-error-error-hresult-e-fail-h.html
        /// </summary>
        [STAThread]
        static void Main()
        {/*
            new Thread(() =>
            {
                Thread.CurrentThread.IsBackground = true;
                while (true)
                {
                    Console.WriteLine("Hello, world");
                    Thread.Sleep(1000);
                }
            }).Start();
            */

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new FormLogin());
            //Application.Run(new FormTestingUnit());
        }
    }
}
