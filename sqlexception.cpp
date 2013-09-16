#include "sqlexception.h"

#include <iostream>

std::ostream &operator<<(std::ostream &o, const SQLException s)
{
    return o << s.fullError();
}
