////////////////////////////////
///// ExceptionThrower.hpp /////
////////////////////////////////

#pragma once
#include "../Libraries.hpp"

#ifndef NDEBUG
#define EXCEPTION_THROWER(__REASON__)\
    throw std::runtime_error(std::string().append("File : ").append(__FILE__).append("\nFunction : ").append(__PRETTY_FUNCTION__).append("\nDate : ").append(__DATE__)\
        .append("\nTime : ").append(__TIME__).append("\nLine : ").append(std::to_string(__LINE__)).append("\nReason : ").append(__REASON__));

#else
#define EXCEPTION_THROWER(message)

#endif // !NDEBUG
