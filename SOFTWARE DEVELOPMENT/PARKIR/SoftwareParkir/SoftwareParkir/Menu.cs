using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SoftwareParkir
{
    class Menu
    {
        public int id { set; get; }
        public String name { set; get;  }

        public Menu(int id, String name)
        {
            this.id = id;
            this.name = name;
        }
    }
}
