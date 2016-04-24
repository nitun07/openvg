// first OpenVG program
// Anthony Starks (ajstarks@gmail.com)
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"

int main(int argc, char *argv[]) {
	int width, height, i, j, k, l, pos;
	char s[3], buf[3*64+4];
        Fontinfo myserif;

                // Need to set the multi-byte type to UTF-8.
                // If this isn't set to whatever you use then only
                // valid ASCII characters can be in the strings that
                // Text prints (that's char values < 128). If you
                // attempt to use a string that isn't valid then the
                // Text functions will silently do nothing!
        setlocale(LC_CTYPE, "");    // Set locale to system default
        init(&width, &height);				   // Graphics initialization

	Start(width, height);				   // Start the picture
	Background(0, 0, 0);				   // Black background
	Fill(44, 77, 232, 1);				   // Big blue marble
	Circle(width / 2, 0, width);			   // The "world"
	Fill(255, 255, 255, 1);				   // White text

                // 1st argument on cmd-line is name of font to use, if
                // none is given then DejaVuSerif will be used.
        myserif = LoadTTF(argc > 1 ? argv[1] : "DejaVuSerif");
        if (!myserif) {
                fputs("Failed to load font.", stderr);
                finish();
                exit(1);
        }
        
                // Print name, style, and number of glyphs in the font.
        snprintf(buf, 64, "Hello, font %s, style %s, with %d glyphs", myserif->Name, myserif->Style, myserif->Count);
        Text(10, 50, buf, myserif, 26);
        End();
	fgets(s, 2, stdin);
                // Step through 30 pages of charcodes, 40 lines per
                // page and 64 characters per line. That's 76800 codes
                // (including the 2048 characters near the end that are
                // invalid UTF-8 codes). I doubt many fonts have
                // glyphs higher than that - but they will be available.
        VGfloat lineh = TextHeight(myserif, 15);
        for (l=0; l<30; l++) {
                WindowClear();
                sprintf(buf, "Page %d", l);
                TextMid(960, 5, buf, myserif, 16);

                for (j=0; j<40; j++) {
                        pos=0;
                        for ( k=0; k < 64; k++ ) {
                                i = (j * 64) + k + (l*64*40)+1;
                                if ((i >= 0xd800) && (i <= 0xdfff))
                                        i = 0; // Invalid UTF-8 codes
                                if (i < 0x80) {
                                        buf[pos++] = (char)i;
                                } else if (i < 0x800) {
                                        buf[pos++] = 0xC0 | (i >> 6);
                                        buf[pos++] = 0x80 | (i & 0x3F);
                                } else if (i < 0x10000) {
                                        buf[pos++] = 0xE0 | (i >> 12);
                                        buf[pos++] = 0x80 | ((i >> 6) & 0x3F);
                                        buf[pos++] = 0x80 | (i & 0x3F);
                                }
                        }
                        buf[pos] = 0;
                        Text(10, 20+(j*lineh), buf, myserif, 15);
                }
                End();
                sleep(1);
        }
                // unloadfont now recognises fontsystem fonts and will
                // correctly call UnloadTTF() for you.
//        UnloadTTF(myserif);
        unloadfont(myserif);
        finish();					   // Graphics cleanup
	exit(0);
}