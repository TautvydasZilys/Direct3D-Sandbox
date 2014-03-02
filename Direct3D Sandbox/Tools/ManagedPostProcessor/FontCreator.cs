using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ManagedPostProcessor
{
    public static class FontCreator
    {
        public static void CreateFont(string fontName, float fontSize, string outputDirectory)
        {
            MessageBox.Show("Got args.\r\nfontName: " + fontName + "\r\nfontSize: " + fontSize.ToString() + "\r\noutputDirectory: " + outputDirectory, "Hi from C#!");
        }
    }
}
