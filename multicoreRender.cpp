#include <iostream>
#include <stdlib.h>
#include <fstream> //read config file
#include <stdlib.h>
#include <sstream>
#include <string>

using namespace std;

/*
frames
0-399 400-799 ... 2800-3199
*/

#define CORES 8 //how many cores to use

int startframe, endframe;

int main(int argc, char* argv[])
{
    int vidlength = stoi( argv[1] ); 

    for ( int i = 0; i < CORES; i++ )
    {
        startframe = ( vidlength / CORES ) * i;
        endframe   = startframe + (vidlength / CORES);
        string command = "./main " + to_string(startframe) + " " + to_string(endframe) + " " + to_string( vidlength );
        system( command.c_str() );
    }
 
}
