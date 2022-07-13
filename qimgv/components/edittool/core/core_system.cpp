#include "core_system.h"

EditCore * core_system::instance()
{
    static EditCore __core;
    return &__core;
}

//TextLogger *core_system::getTextLogger()
//{
//    static TextLogger __textLogger;
//    return &__textLogger;
//}
