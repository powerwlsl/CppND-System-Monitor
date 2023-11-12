#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
//string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
string TwoSigFig(long time){
    string twoSigFig;
    if(time<10){twoSigFig+="0";}
    twoSigFig+=std::to_string(time);
    return twoSigFig;
}

string Format::ElapsedTime(long int seconds) { 
    string HHMMSS;
    long int S = seconds%60;
    long int M = (seconds-S)/60;
    long int H = (M-(M%60))/60;
    M-= H*60;
    string SS = TwoSigFig(S);
    string MM = TwoSigFig(M);
    string HH = TwoSigFig(H);
    HHMMSS += HH + ":" + MM + ":" + SS;
    return HHMMSS;
    }