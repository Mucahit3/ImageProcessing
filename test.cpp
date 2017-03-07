#include <iostream>
#include <fstream>
#include <cstring> //for memset
#include <ctime> //for time
#include <iomanip> //for setw

#include "bmp.h"

using namespace std;

#define DEBUG 1
#define PRINTSIZE 6

#if DEBUG
#define dout cout
#else
#define dout 0 && cout
#endif

#define DRAW_TESTS 0
#define INTEGRAL_TEST 1
#define HAAR_TEST 1

int main(int argc, char *argv[])
{
    int height, width;
    long size;
    ifstream file;

    const char* filePath = "images/result.bmp";

    file.open("images/backup/test.bmp", ios::in | ios::binary);

    BYTE* buffer = loadBMP(&height, &width, &size, file);

    if(buffer == NULL){
        cout << "Error: buffer null, returned in LoadBMP()!" << endl;
        return 1;
    }
    
    BYTE* ramIntensity = convertBMPToIntensity(buffer, width, height);

    if(ramIntensity == NULL){
        cout << "Error: ramIntensity null, returned in convertBMPToIntensity()!" << endl;
        return 1;
    }

#if INTEGRAL_TEST
    int start_c = clock();
    DWORD* integralImage = getIntegralImage(ramIntensity, width, height);
    int stop_c = clock();

    int i, j;
#if DEBUG
    for(i = 0; i < PRINTSIZE; i++){
        for(j = 0; j < PRINTSIZE; j++){
            dout << setw (5) << (int)*(ramIntensity + i * width + j) << " ";
        }
        dout << endl;
    }
    dout << endl << endl;

    for(i = 0; i < PRINTSIZE; i++){
        for(j = 0; j < PRINTSIZE; j++){
            dout << setw (5) << *(integralImage + i * (width + 1) + j) << " ";
        }
        dout << endl;
    }
#endif /* DEBUG */
    //dout << "Total = " << getTotal(integralImage, 0, 0, 1, 1, width, height) << endl;

    dout << "Calculate integralImage time: " << (stop_c - start_c) / double(CLOCKS_PER_SEC) << endl << endl;
#if HAAR_TEST
    for(i = 1; i < 6; i++){
        start_c = clock();
        int* result = haarCascade(i, 1, integralImage, width, height);
        stop_c = clock();

        dout << i << " - Calculate haarCascade time: " << (stop_c    - start_c) / double(CLOCKS_PER_SEC) << endl << endl;        
    }
#endif /* HAAR_TEST */
    delete[] integralImage;
#endif /* INTEGRAL_TEST */

#if DRAW_TESTS    
    int centerX = 300;
    int centerY = 290;

    drawPlus(centerX, centerY, width, ramIntensity);

    /* Test drawCircle */
    if(!drawCircle(centerX, centerY, 110, ramIntensity, width, height)){
        dout << "Error: drawCircle (big) failed!" << endl;
    }else{
        dout << "drawCircle (big) succed!" << endl;
    }

    /* Test drawCircle */
    if(!drawCircle(centerX, centerY, 50, ramIntensity, width, height)){
        dout << "Error: drawCircle (small) failed!" << endl;
    }else{
        dout << "drawCircle (small) succed!" << endl;
    }

    /* Test drawRect */
    if(!drawRect(centerX, centerY, 110, 110, ramIntensity, width, height)){
        dout << "Error: drawRect failed!" << endl;
    }else{
        dout << "drawRect succed!" << endl;
    }

    /* Test drawElips */
    if(!drawElips(centerX, centerY, 110, 50, ramIntensity, width, height)){
        dout << "Error: drawElips failed!" << endl;
    }else{
        dout << "drawElips succed" << endl;
    }

#if DEBUG > 1
    const char* filePathDebug = "images/intensity.bmp";
    saveBMP(filePathDebug, height, width, ramIntensity);
#endif /* DEBUG > 1 */

    saveBMP(filePath, height, width, convertIntensityToBMP(ramIntensity, width, height, &size));
#endif /* DRAW_TESTS */


    delete[] buffer;
    delete[] ramIntensity;

    return 0;
}