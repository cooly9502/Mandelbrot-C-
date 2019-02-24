#include <iostream>
#include "FreeImage.h"
#include "math.h"
#include <fstream> // reading config file
#include <string>
#include <stdlib.h>
#include <sstream>
#include <math.h>

//they're backwards, just deal with it im too lazy to fix it
#define WIDTH 1080
#define HEIGHT 1920
#define BPP 24
#define MAXiter 255
#define BPP 24 //for image type stuff idk
#define ZOOMSPEED .045/4 //remove 4 for 30 fps
int VIDLENGTH;

using namespace std;

int main(int argc, char* argv[]){
    int STARTFRAME   =   stoi(argv[1]); //string to int
    int ENDFRAME     =   stoi(argv[2]);
    int VIDLENGTH    =   ENDFRAME - STARTFRAME;
    
    //for config file
    long double SCALE, xedit, yedit;
    string line;
    ifstream file ("config.txt");
    if ( file.is_open() ) 
    {
        getline ( file, line ); SCALE      =     atof(line.c_str());
        getline ( file, line ); xedit      =     atof(line.c_str());
        getline ( file, line ); yedit      =     atof(line.c_str());
        file.close();
    }

    //calculate starting scale
    for ( int i = 0; i < ENDFRAME-STARTFRAME; i++){
        SCALE -= SCALE * ZOOMSPEED;
    }

    //constants 
    const double colorValRatio   =     ( MAXiter / 255) * 255;
    const double calcXvar        =     (WIDTH / 2 ) * SCALE;
    const double calcYvar        =     (WIDTH / 2 ) * SCALE;

    string fileOutputName; 
    int percentage_complete;
    //repeat over and over for each frame of video and output as number
    for (int f = STARTFRAME; f < ENDFRAME; f++)
    {
        FreeImage_Initialise();
        FIBITMAP * bitmap = FreeImage_Allocate(HEIGHT, WIDTH, BPP);
        RGBQUAD color;

        int i;
        long double calcx, calcy, z, a, b, zx, zy, colorVal, originalX, originalY;
        bool notEscaped;
        for (int y = 0; y < WIDTH; y++)
        {
            for (int x = 0; x < HEIGHT; x++)
            {
                //convert the x, y to fit the scale
                calcx       =   -calcXvar + (x * SCALE) + xedit;
                calcy       =   -calcYvar + (y * SCALE) + yedit;
                originalX   =   calcx;
                originalY   =   calcy;
                notEscaped  =   true;
                i = 0;
                z = 0;
                while ( z < 4 && i < MAXiter )
                {
                    a   =  ( calcx * calcx - calcy * calcy );
                    b   =  ( 2 * calcx * calcy)             ;
                    zx  =  a + originalX                    ;
                    zy  =  b + originalY                    ;
                    z   =  zx * zx + zy * zy                ;
                    calcx = zx; calcy = zy;
                    i++;
                }
                if ( i < MAXiter )
                {
                    colorVal         =      log2( i ) * colorValRatio;
                    color.rgbRed     =      150;
                    color.rgbGreen   =      0; 
                    color.rgbBlue    =      colorVal ;
                }else 
                {
                    color.rgbRed    =  255;
                    color.rgbGreen  =  255 ; 
                    color.rgbBlue   =  255 ;
                }
                FreeImage_SetPixelColor(bitmap, x, y, &color);
            }
        }
        //output IMAGE!!!
        percentage_complete = (float(f) / float( ENDFRAME - STARTFRAME)) * 100 ;
        cout << "   Render Progress: " << percentage_complete << "%" <<
                "   Current Frame: " << f <<
                "   Current Depth (scale): " << SCALE << 
                "\r" << flush;

        fileOutputName = ("frames/frame_" + to_string(f) +".png").c_str();
        if (FreeImage_Save(FIF_PNG, bitmap, fileOutputName.c_str(), 0)){}
        FreeImage_DeInitialise();
        SCALE -= SCALE * ZOOMSPEED;
    }
    return 0;
}