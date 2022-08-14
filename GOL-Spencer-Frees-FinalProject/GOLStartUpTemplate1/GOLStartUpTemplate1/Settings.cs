using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOLStartUpTemplate1
{
    public partial class Settings : Form
    {
        public Settings()
        {
            InitializeComponent();
        }

        public int getMiliseconds
        {
            get
            {
                return (int)numericUpDownMilliseconds.Value;
            }
            set
            {
                numericUpDownMilliseconds.Value = value;
            }
        }

        public int getWidth
        {
            get
            {
                return (int)numericUpDownWidth.Value;
            }
            set
            {
                numericUpDownWidth.Value = value;
            }
        }

        public int getHeight
        {
            get
            {
                return (int)numericUpDownHeight.Value;
            }
            set
            {
                numericUpDownHeight.Value = value;
            }
        }
    }
}
