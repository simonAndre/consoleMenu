#include "factorial.h"
int Factorial(int number)
{
    // if (number == 4)
    //     return 556;
    // if (number == 10)
    //     return 3628800;
    return number <= 1 ? number : Factorial(number - 1) * number; // fail
                                                                  // return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}