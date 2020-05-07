/*
 *  A Windows console application to calculate
 *  the Incidence Rate of a research;
 *
 *  Daniel Kofanov - kofanov.ds@phystech.edu
 *  2018
*/



#include "ircresearch.h"

#include <cstdio>
#include <conio.h>



int main()
{
    while(1)
    {
        std::printf("Final IR Calculator\n");
        try
        {
            char ans = IR_Research::do_research();


            if((ans == 'N') || (ans == 'n'))
                break;

        }
        
        catch(std::exception exc)   // handle invalid_argument, length_error, bad_alloc;
        {
            std::cout << "Exception: " << exc.what() << "\nPress any key to restart the program:\n";
            _getche();
        }
        system("cls");

    }

    std::printf("_END_OF_THE_PROGRAM\n");
    return 0;
}


