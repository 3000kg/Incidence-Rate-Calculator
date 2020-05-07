/* 
 *  A class to calculate the Incidence Rate of a research;
 *
 *  Daniel Kofanov - kofanov.ds@phystech.edu
 *  2018 
 */

#include "ircresearch.h"

#include <windows.h>
#include <conio.h>
#include <iostream>
#include <exception>
#include <cstdio>
#include <cassert>




char IR_Research::do_research()
{
    IR_Research research;


    research.src_data_out();

    if(research.src_sum_IR <= 100)
        research.calculate();

    else
        throw(std::invalid_argument("TOTAL IR OF THE TARGET GROUPS"));



    return Parser::readAns();
}








// ctor/dtor
///////////////////////////////////////////////////////////////////////////////
//
    IR_Research::IR_Research()
    {
        std::cout << "Choose number of Target Groups:\n";
        brands_amount   = Parser::intInput();
        src_data        = new TargetGroupInfo[brands_amount];
        research_data   = new ResearchStepInfo[brands_amount];

        src_data_input();
        src_data_sort();

        system("cls");

        // std::printf("_created\n");

        return;
    }

    IR_Research::~IR_Research()
    {
        delete [] src_data;
        delete [] research_data;
    
        src_data        = nullptr;
        research_data   = nullptr;

        //std::printf("_deleted\n");
        return;
    }
//
///////////////////////////////////////////////////////////////////////////////



// I/O:
///////////////////////////////////////////////////////////////////////////////
//
    void IR_Research::src_data_input()
    {
        std::cout << "\nInput values:\n";

        if (src_data == nullptr)
            throw( std::invalid_argument("src_data"));


        for (size_t i = 0; i < brands_amount; i++)
        {
            std::printf("|=================================================\n||FOR GROUP #%d:\n", i + 1);
            std::printf("||\tTarget number of interviews: ");

            src_data[i].target_interviews   =   Parser::intInput();
            src_sum_interviews              +=  src_data[i].target_interviews;

            std::printf("||\t  IR of Target Group (in %%): ");
            src_data[i].group_IR   =   Parser::intInput();
            src_sum_IR              +=  src_data[i].group_IR;
        }

        std::printf("|=================================================\n\n");

        return;
    }


    void IR_Research::src_data_out()
    {
        std::printf("\n Target | Number of | Incidence\n group  | interviews| rate, %%\n--------+-----------+-----------\n");
        for(size_t i = 0; i < brands_amount; i++)
        {
            std::printf("%6.lu  |", i + 1);
            std::printf("%9.lu  |", src_data[i].target_interviews);
            std::printf("%9.lu\n", src_data[i].group_IR);
            std::printf("--------+-----------+-----------\n");
        }

        //printf("%lu) %ld\t(%lu%%)\n", i, , );
        std::printf("\n");

        return;
    }







    void IR_Research::step_ans_out(size_t i) const
    {
        std::printf("/ Research part %lu:\n", i + 1);
        std::printf("|-----Picking:   %lu (IR = %lu%%)\n", research_data[i].delta_collected, research_data[i].current_IR);
        std::printf("|-----Collected: %lu (%lu + %lu)\n", research_data[i].total_collected, research_data[i].total_collected - research_data[i].delta_collected, research_data[i].delta_collected);
        std::printf("\\-----Entraces:  %lu (%lu + %lu)\n\n", research_data[i].total_entraces, research_data[i].total_entraces - research_data[i].delta_entraces, research_data[i].delta_entraces);

        return;
    }



    void IR_Research::final_ans_out() const
    {
        std::printf("Final IR is %lu%%\n", research_data[steps_amount - 1].current_IR);
        std::printf("===============================================\n\n");

        return;
    }
//
///////////////////////////////////////////////////////////////////////////////





// Sort:
///////////////////////////////////////////////////////////////////////////////
//
    void IR_Research::src_data_sort()
    {
        if(src_data == nullptr)
            throw(std::invalid_argument("src_data"));

        std::qsort(src_data, brands_amount, sizeof(TargetGroupInfo), comparator);

        return;
    }

    int IR_Research::comparator(const void* p1, const void* p2)
    {
        return static_cast<const TargetGroupInfo*>(p2)->group_IR - static_cast<const TargetGroupInfo*>(p1)->group_IR;
    }
//
///////////////////////////////////////////////////////////////////////////////



// Calculation:
///////////////////////////////////////////////////////////////////////////////
//
    void IR_Research::calculate()
    {
        if ((src_data == nullptr) or (research_data == nullptr))
            throw( std::invalid_argument("src_data or research_data"));

        size_t idxSRC = 0, idxRES = 0;


        while (should_continue(idxSRC))			            // check if we need to continue research;
        {
            while(src_data[idxSRC].target_interviews == 0)     // skip non-endian zeroes;
                idxSRC++;							


            research_data[idxRES].delta_entraces = (unsigned long)(((double)src_data[idxSRC].target_interviews) / ((double)src_data[idxSRC].group_IR) * 100.);

            unsigned long tempNumReq = 0;
            //resData[idxRES].deltaCollected = ;
            for(size_t idx0 = idxSRC + 1; idx0 < brands_amount; idx0++)
            {
                tempNumReq = (unsigned long)(((double)research_data[idxRES].delta_entraces) * ((double)src_data[idx0].group_IR) / 100.);
                if(tempNumReq > src_data[idx0].target_interviews)
                {
                    tempNumReq = src_data[idx0].target_interviews;
                    src_data[idx0].target_interviews = 0;
                }

                else
                    src_data[idx0].target_interviews -= tempNumReq;

                research_data[idxRES].delta_collected += tempNumReq;
            }

            research_data[idxRES].total_entraces    += research_data[idxRES].delta_entraces;
            research_data[idxRES].delta_collected   += src_data[idxSRC].target_interviews;
            research_data[idxRES].total_collected   += research_data[idxRES].delta_collected;
            research_data[idxRES].current_IR         = research_data[idxRES].total_collected * 100 / research_data[idxRES].total_entraces;
                //static_cast<unsigned long>(((double)research_data[idxRES].total_collected) / ((double)resData[idxRES].entraces) * 100.);


            step_ans_out( idxRES);

            if (idxRES < brands_amount - 1)
            {
                research_data[idxRES + 1].total_collected   = research_data[idxRES].total_collected;  // preparation for the following iteration;
                research_data[idxRES + 1].total_entraces    = research_data[idxRES].total_entraces;
            }

            idxRES++;
            idxSRC++;
        }

        steps_amount = idxRES;

        return;
    }






    bool IR_Research::should_continue( size_t startIDX)
    {
        for (startIDX; startIDX < brands_amount; startIDX++)
            if(src_data[startIDX].target_interviews > 0)
                return true;

        return false;
    }
//
///////////////////////////////////////////////////////////////////////////////











// Parser:
///////////////////////////////////////////////////////////////////////////////
//
    char Parser::readAns()
    {
        std::printf("Do you want to restart the program? [Y/N]\n");
    

        while (1)
        {
        
            char c = _getche();
            if (c != '\0')
            {
                if ((c == 'Y') || (c == 'N') || (c == 'y') || (c == 'n'))
                    return c;

                if ((c == 'h'))
                    return Parser::readAns2();

                else
                    std::printf("\nAnswer %c is incorrect!\n", c);
                std::printf("Please, enter the answer (Y/N):\n");
            }

        }
    }




    unsigned long Parser::intInput()
    {
        char line[Parser::max_len] = {};
        unsigned long retAns = 0, term = 0, factor = 1;
        char c = '0';
        int i = 0;
        while (1)
        {
            Parser::readLine(line);
            for (i = 0; ; i++)
            {
                c = line[i];
                if ((c >= '0') && (c <= '9'))
                {
                    term = (c - '0');
                    retAns = retAns * 10 + term;
                }
                else if (c == '\0')
                    return retAns;
                else if (c == '\n')
                {
                    if (retAns == 0)
                    {
                        std::printf("Character '\\n' is incorrect! Please, re-enter the number:\n");
                        retAns = 0;
                        factor = 1;
                        term = 0;
                        break;
                    }
                    else
                        return retAns;
                }
                else
                {
                    std::printf("Character %c is incorrect! Please, re-enter the number:\n", c);
                    retAns = 0;
                    factor = 1;
                    term = 0;
                    break;
                }
            }
        }
    }




    int Parser::readLine(char line[Parser::max_len])
    {
        clearLine(line);
        int i = 0;
        char c = '0';
        c = std::getchar();
        while (c != '\n')
        {
            if(i >= Parser::max_len)
                throw(std::length_error("INPUT LINE SIZE"));

            line[i] = c;
            c = std::getchar();
            i++;
        }
        line[i] = c;
    
        return i;
    }







    char Parser::readAns2()
    {
        system("cls");
        Sleep(2000);
        putchar('h');
        Sleep(80);
        putchar('v');
        Sleep(80);

        putchar('h');
        Sleep(80);

        putchar('?');
        Sleep(80);

        Sleep(1500);
        system("cls");

        std::printf("Do you want to restart the program? [Y/N]\n");
        //	char ans[MAX_LEN] = {};

        while(1)
        {
            //	readLine(ans);
            char c = _getche();
            if(c != '\0')
            {
                if((c == 'Y') || (c == 'N') || (c == 'y') || (c == 'n'))
                    return c;


                else
                    std::printf("\nAnswer %c is incorrect!\n", c);
                std::printf("Please, enter the answer (Y/N):\n");
            }

        }
        return 0;
    }




    void Parser::clearLine(char line[Parser::max_len])
    {
        int i = 0;
        while(line[i] != '\0')
        {
            line[i] = '\0';
            i++;
        }
    }
//
///////////////////////////////////////////////////////////////////////////////
