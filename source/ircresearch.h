/* 
 *  A class to calculate the Incidence Rate of a research;
 *
 *  Daniel Kofanov - kofanov.ds@phystech.edu
 *  2018 
 */

#ifndef IRC_RESEARCH_H_INCLUDDED
#define IRC_RESEARCH_H_INCLUDDED

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <cassert>




// Main class:
    class IR_Research
    {
        public:
            IR_Research();
            ~IR_Research();

            static char do_research();

        private:
        // Methods:
            void src_data_input();
            void src_data_out();
            void src_data_sort();
            static int comparator( const void* p1, const void* p2);

            void calculate();
            bool should_continue( size_t startIDX);

            void step_ans_out(size_t i) const;
            void final_ans_out() const;
    
        // Classes:
            class ResearchStepInfo;
            class TargetGroupInfo;




        /* Data */
        private:
            unsigned long brands_amount         = 0;        //
            unsigned long src_sum_interviews    = 0;        // calculated on input;
            unsigned long src_sum_IR            = 0;        //
        
            unsigned long       steps_amount    = 0;        // 
            TargetGroupInfo*    src_data        = nullptr;  // updated during the calculation();
            ResearchStepInfo*   research_data   = nullptr;  // (src_data is being changed as well)

    };










// Miscellaneous stuff
    class IR_Research::TargetGroupInfo		
    {
        friend class IR_Research;

        unsigned long target_interviews	= 0;    // also used to monitor the remaining
                                                // amount of interviews to collect on each step;
        unsigned char group_IR			= 0;
    };



    class IR_Research::ResearchStepInfo
    {
        friend class IR_Research;

        unsigned long   delta_entraces    = 0,
                        total_entraces    = 0,

                        delta_collected   = 0,
                        total_collected   = 0;


        unsigned char current_IR = 0;
    };





// legacy input parser and etc (need to rework prob):
    namespace Parser
    {
        const int max_len = 50;

        char            readAns();
        char            readAns2();
        int             readLine(char line[max_len]);
        void            clearLine(char line[max_len]);
        unsigned long   intInput();
    }



#endif
