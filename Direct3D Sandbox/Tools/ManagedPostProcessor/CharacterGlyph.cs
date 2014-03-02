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
        public uint XOffset { get; set; }
        public uint YOffset { get; set; }
        public uint CharacterWidth { get; private set; }
        public uint CharacterHeight { get; private set; }

        public CharacterGlyph(char character, Bitmap bitmap, uint characterWidth, uint characterHeight)
        {
            Character = character;
            Bitmap = bitmap;
            CharacterWidth = characterWidth;
            CharacterHeight = characterHeight;
        }
    }
}
