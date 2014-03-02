using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ManagedPostProcessor
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ABCSpacing
    {
        public float A;
        public float B;
        public float C;
    }

    internal class CharacterGlyph
    {
        public char Character { get; private set; }
        public Bitmap Bitmap { get; private set; }
        public ABCSpacing ABCSpacing { get; private set; }
        public int YOffset { get; private set; }
        public int HorizontalOffset { get; set; }

        public CharacterGlyph(char character, Bitmap bitmap, ABCSpacing abcSpacing, int yOffset)
        {
            Character = character;
            Bitmap = bitmap;
            ABCSpacing = abcSpacing;
            YOffset = yOffset;
        }
    }
}
