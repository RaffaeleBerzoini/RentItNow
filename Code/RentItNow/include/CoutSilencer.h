#include <iostream>

// Just a simple class to silence std::cout
// to make the demo less spammy
class CoutSilencer
{

public:
    void SilenceCout()
    {
        std::cout.setstate(std::ios_base::failbit); // Set the failbit, so std::cout ignores any output
    }

    void RestoreCout()
    {
        std::cout.clear(); // Clear the failbit, so std::cout functions normally again
    }
};