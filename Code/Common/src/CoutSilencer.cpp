#include "CoutSilencer.h"

void CoutSilencer::SilenceCout()
{
    std::cout.setstate(std::ios_base::failbit); // Set the failbit, so std::cout ignores any output
}

void CoutSilencer::RestoreCout()
{
    std::cout.clear(); // Clear the failbit, so std::cout functions normally again
}
