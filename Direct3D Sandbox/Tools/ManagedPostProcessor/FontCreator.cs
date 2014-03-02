using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ManagedPostProcessor
{
    public static class FontCreator
    {
        static Brush ForegroundBrush = new SolidBrush(Color.White);
        static StringFormat NoFontFallbackFormat = new StringFormat(StringFormatFlags.NoFontFallback);
        const int BitmapSize = 1024;

        public static void CreateFont(string fontName, float fontSize, string outputDirectory)
        {
            fontSize *= 96.0f / 72.0f;  // Converts points to pixels

            using (var font = new Font(fontName, fontSize, FontStyle.Regular, GraphicsUnit.Pixel))
            {
                Console.WriteLine("Loading font {0}. Loaded font name: {1}.", fontName, font.FontFamily.Name);

                using (var bitmap = new Bitmap(BitmapSize, BitmapSize, PixelFormat.Format32bppArgb))
                {
                    using (var graphics = Graphics.FromImage(bitmap))
                    {
                        graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;
                        graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
                        graphics.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;
                        
                        var characters = ImportFont(font, bitmap, graphics);
                        var lineSpace = font.GetHeight();

                        int fontBitmapWidth, fontBitmapHeight;
                        var fontBitmap = PackCharacters(characters, out fontBitmapWidth, out fontBitmapHeight);                        
                        SaveFont(characters, fontBitmapWidth, fontBitmapHeight, fontBitmap, Path.Combine(outputDirectory, fontName + ".font"));
                    }
                }
            }
        }

        private static CharacterGlyph[] ImportFont(Font font, Bitmap stagingBitmap, Graphics stagingGraphics)
        {
            var characters = new CharacterGlyph[128];

            for (int i = 0; i < 128; i++)
            {
                characters[i] = ImportCharacter((char)i, font, stagingBitmap, stagingGraphics);
            }

            return characters;
        }

        private static CharacterGlyph ImportCharacter(char character, Font font, Bitmap stagingBitmap, Graphics stagingGraphics)
        {
            var str = character.ToString();
            var characterSize = stagingGraphics.MeasureString(str, font, Point.Empty, NoFontFallbackFormat);
            var abcSpacing = CalculateABCSpacing(character, font, stagingGraphics);
            var yOffset = (int)Math.Ceiling(characterSize.Height / 2.0f);
            
            int width = (int)Math.Ceiling(characterSize.Width + abcSpacing.A + abcSpacing.B);
            int height = 2 * (int)Math.Ceiling(characterSize.Height);
            Debug.Assert(width <= BitmapSize && height < BitmapSize);

            stagingGraphics.Clear(Color.Black);
            stagingGraphics.DrawString(str, font, ForegroundBrush, abcSpacing.A, yOffset, NoFontFallbackFormat);
            stagingGraphics.Flush();

            var characterBitmap = stagingBitmap.Clone(new Rectangle(0, 0, width, height), PixelFormat.Format32bppArgb);
            ConvertBitmapToMonochrome(characterBitmap);

            return new CharacterGlyph(character, characterBitmap, abcSpacing, yOffset);
        }

        private static unsafe void ConvertBitmapToMonochrome(Bitmap bitmap)
        {
            uint white = unchecked((uint)-1) - 255;
            var bitmapData = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);

            for (int i = 0; i < bitmapData.Height; i++)
            {
                var line = (byte*)bitmapData.Scan0.ToPointer() + bitmapData.Stride * i;
                for (int j = 0; j < bitmapData.Width; j++)
                {
                    var colorPtr = (uint*)line + j;
                    var color = *colorPtr;

                    uint luminosity = (uint)Math.Ceiling(0.2126 * (color >> 24) + 0.7152 * ((color >> 16) % 255) + 0.0722 * ((color >> 8) % 255));
                    *colorPtr = white + luminosity;
                }
            }

            bitmap.UnlockBits(bitmapData);
        }

        // Calculate ABC spacing
        // http://msdn.microsoft.com/en-us/library/windows/desktop/dd162454(v=vs.85).aspx
        private static ABCSpacing CalculateABCSpacing(char character, Font font, Graphics stagingGraphics)
        {
            var hdc = stagingGraphics.GetHdc();

            try
            {
                var hFont = font.ToHfont();

                try
                {
                    var oldFont = GDI.SelectObject(hdc, hFont);

                    try
                    {
                        var abcSpacing = new ABCSpacing[1];
                        var result = GDI.GetCharABCWidthsFloat(hdc, character, character, abcSpacing);

                        if (!result)
                        {
                            throw new Exception("GetCharABCWidthsFloat failed!");
                        }

                        return abcSpacing[0];
                    }
                    finally
                    {
                        GDI.SelectObject(hdc, oldFont);
                    }
                }
                finally
                {
                    GDI.DeleteObject(hFont);
                }
            }
            finally
            {
                stagingGraphics.ReleaseHdc(hdc);
            }
        }

        static byte[] PackCharacters(CharacterGlyph[] characters, out int bitmapWidth, out int bitmapHeight)
        {
            var offset = 0;

            bitmapHeight = characters.Max(x => x.Bitmap.Height);
            bitmapWidth = characters.Sum(x => x.Bitmap.Width);

            var bitmap = new byte[bitmapHeight * bitmapWidth];
        
            for (int i = 0; i < characters.Length; i++)
            {
                var sourceBitmap = characters[i].Bitmap;
                var sourceBitmapData = sourceBitmap.LockBits(new Rectangle(0, 0, sourceBitmap.Width, sourceBitmap.Height), 
                        ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);

                for (int u = 0; u < sourceBitmap.Height; u++)
                {
                    unsafe
                    {
                        var sourceLine = (byte*)sourceBitmapData.Scan0.ToPointer() + sourceBitmapData.Stride * u;

                        for (int v = 0; v < sourceBitmap.Width; v++)
                        {
                            bitmap[u * bitmapHeight + v + offset] = *sourceLine;
                            sourceLine += 4;
                        }
                    }
                }

                characters[i].Bitmap.UnlockBits(sourceBitmapData);

                characters[i].HorizontalOffset = offset;
                offset += characters[i].Bitmap.Width;
            }

            return bitmap;
        }

        // Binary format:
        // 4 bytes - bitmap width
        // 4 bytes - bitmap height
        //      * - bitmap data
        // 4 bytes - number of characters
        //      1 byte - character
        //      12 bytes - ABC spacing
        //      4 bytes - yOffset
        //      4 bytes - horizontalOffset
        private static void SaveFont(CharacterGlyph[] characters, int bitmapWidth, int bitmapHeight, byte[] bitmapData, string outputPath)
        {
            using (var fileStream = new FileStream(outputPath, FileMode.Create))
            {
                using (var writer = new BinaryWriter(fileStream))
                {
                    writer.Write(bitmapWidth);
                    writer.Write(bitmapHeight);
                    writer.Write(bitmapData);

                    writer.Write(characters.Length);

                    foreach (var character in characters)
                    {
                        writer.Write((byte)character.Character);
                        writer.Write(character.ABCSpacing.A);
                        writer.Write(character.ABCSpacing.B);
                        writer.Write(character.ABCSpacing.C);
                        writer.Write(character.YOffset);
                        writer.Write(character.HorizontalOffset);
                    }
                }
            }
        }

        static class GDI
        {
            [DllImport("gdi32.dll")]
            public static extern IntPtr SelectObject(IntPtr hdc, IntPtr hObject);

            [DllImport("gdi32.dll")]
            public static extern bool DeleteObject(IntPtr hObject);

            [DllImport("gdi32.dll", CharSet = CharSet.Unicode)]
            public static extern bool GetCharABCWidthsFloat(IntPtr hdc, uint firstChar, uint lastChar, [Out] ABCSpacing[] abcSpacingPtr);
        }
    }
}