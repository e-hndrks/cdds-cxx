/*
*                         Vortex Lite
*
*   This software and documentation are Copyright 2006 to 2015 PrismTech
*   Limited and its licensees. All rights reserved. See file:
*
*                     $LITE_HOME/LICENSE
*
*   for full copyright notice and license terms.
*
*/


/**
 * @file
 */

#ifndef CYCLONEDDS_CORE_REPORT_UTILS_HPP_
#define CYCLONEDDS_CORE_REPORT_UTILS_HPP_

#include <string>
#include <sstream>
#include <org/eclipse/cyclonedds/core/config.hpp>
#include <dds/core/Exception.hpp>
#include <dds/dds.h>
#include <ios>
#include <stdarg.h>

//#define _OS_WARNING 2
//#define _OS_ERROR 4

#define ISOCPP_ERROR                       org::eclipse::cyclonedds::core::utils::error_code
#define ISOCPP_UNSUPPORTED_ERROR           org::eclipse::cyclonedds::core::utils::unsupported_error_code
#define ISOCPP_INVALID_ARGUMENT_ERROR      org::eclipse::cyclonedds::core::utils::invalid_argument_code
#define ISOCPP_PRECONDITION_NOT_MET_ERROR  org::eclipse::cyclonedds::core::utils::precondition_not_met_error_code
#define ISOCPP_OUT_OF_RESOURCES_ERROR      org::eclipse::cyclonedds::core::utils::out_of_resources_error_code
#define ISOCPP_NOT_ENABLED_ERROR           org::eclipse::cyclonedds::core::utils::not_enabled_error_code
#define ISOCPP_IMMUTABLE_POLICY_ERROR      org::eclipse::cyclonedds::core::utils::immutable_policy_error_code
#define ISOCPP_INCONSISTENT_POLICY_ERROR   org::eclipse::cyclonedds::core::utils::inconsistent_policy_error_code
#define ISOCPP_ALREADY_CLOSED_ERROR        org::eclipse::cyclonedds::core::utils::already_closed_error_code
#define ISOCPP_TIMEOUT_ERROR               org::eclipse::cyclonedds::core::utils::timeout_error_code
#define ISOCPP_NO_DATA_ERROR               org::eclipse::cyclonedds::core::utils::no_data_error_code
#define ISOCPP_ILLEGAL_OPERATION_ERROR     org::eclipse::cyclonedds::core::utils::illegal_operation_error_code
#define ISOCPP_NULL_REFERENCE_ERROR        org::eclipse::cyclonedds::core::utils::null_reference_error_code

/* \brief OS_FUNCTION provides undecorated function name of current function
 *
 * Behavior of OS_FUNCTION outside a function is undefined. Note that
 * implementations differ across compilers and compiler versions. It might be
 * implemented as either a string literal or a constant variable.
 */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#   define OS_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#   define OS_FUNCTION __func__
#elif defined(__GNUC__)
#   define OS_FUNCTION __FUNCTION__
#elif defined(__clang__)
#   define OS_FUNCTION __FUNCTION__
#elif defined(__ghs__)
#   define OS_FUNCTION __FUNCTION__
#elif (defined(__SUNPRO_C) || defined(__SUNPRO_CC))
/* Solaris Studio had support for __func__ before it supported __FUNCTION__.
   Compiler flag -features=extensions is required on older versions. */
#   define OS_FUNCTION __func__
#elif defined(__FUNCTION__)
/* Visual Studio */
#   define OS_FUNCTION __FUNCTION__
#elif defined(__vxworks)
/* At least versions 2.9.6 and 3.3.4 of the GNU C Preprocessor only define
   __GNUC__ if the entire GNU C compiler is in use. VxWorks 5.5 targets invoke
   the preprocessor separately resulting in __GNUC__ not being defined. */
#   define OS_FUNCTION __FUNCTION__
#else
#   warning "OS_FUNCTION is not supported"
#endif

/* \brief OS_PRETTY_FUNCTION provides function signature of current function
 *
 * See comments on OS_FUNCTION for details.
 */
#if defined(__GNUC__)
#   define OS_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(__clang__)
#   define OS_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(__ghs__)
#   define OS_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif (defined(__SUNPRO_C) && __SUNPRO_C >= 0x5100)
/* Solaris Studio supports __PRETTY_FUNCTION__ in C since version 12.1 */
#   define OS_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x5120)
/* Solaris Studio supports __PRETTY_FUNCTION__ in C++ since version 12.3 */
#   define OS_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
/* Visual Studio */
#   define OS_PRETTY_FUNCTION __FUNCSIG__
#elif defined(__vxworks)
/* See comments on __vxworks macro above. */
#   define OS_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
/* Do not warn user about OS_PRETTY_FUNCTION falling back to OS_FUNCTION.
#   warning "OS_PRETTY_FUNCTION is not supported, using OS_FUNCTION"
*/
#   define OS_PRETTY_FUNCTION OS_FUNCTION
#endif

#define ISOCPP_DDSC_RESULT_CHECK_AND_THROW(code, ...)      \
    org::eclipse::cyclonedds::core::utils::check_ddsc_result_and_throw_exception(    \
        (dds_return_t)(code),                         \
        __FILE__,                                     \
        __LINE__,                                     \
        OS_PRETTY_FUNCTION,                           \
        __VA_ARGS__)                                  \

#define ISOCPP_THROW_EXCEPTION(code, ...)             \
    org::eclipse::cyclonedds::core::utils::throw_exception(    \
        (code),                                       \
        __FILE__,                                     \
        __LINE__,                                     \
        OS_PRETTY_FUNCTION,                           \
        __VA_ARGS__)                                  \

#define ISOCPP_BOOL_CHECK_AND_THROW(test, code, ...)  \
    if (!(test)) {                                      \
        ISOCPP_THROW_EXCEPTION(code, __VA_ARGS__);    \
    }



namespace org
{
namespace eclipse
{
namespace cyclonedds
{
namespace core
{
namespace utils
{

const int32_t error_code                       = 1;
const int32_t unsupported_error_code           = 2;
const int32_t invalid_argument_code            = 3;
const int32_t precondition_not_met_error_code  = 4;
const int32_t out_of_resources_error_code      = 5;
const int32_t not_enabled_error_code           = 6;
const int32_t immutable_policy_error_code      = 7;
const int32_t inconsistent_policy_error_code   = 8;
const int32_t already_closed_error_code        = 9;
const int32_t timeout_error_code               = 10;
const int32_t no_data_error_code               = 11;
const int32_t illegal_operation_error_code     = 12;
const int32_t null_reference_error_code        = 13;



OSPL_ISOCPP_IMPL_API void
report(
    int32_t code,
    int32_t reportType,
    const char *file,
    int32_t line,
    const char *signature,
    const char *format,
    ...);

OSPL_ISOCPP_IMPL_API void
throw_exception(
    int32_t code,
    const char *file,
    int32_t line,
    const char *signature,
    const char *format,
    ...);

OSPL_ISOCPP_IMPL_API void
check_ddsc_result_and_throw_exception(
    dds_return_t code,
    const char *file,
    int32_t line,
    const char *signature,
    const char *format,
    ...);

}
}
}
}
}

#endif /* CYCLONEDDS_CORE_REPORT_UTILS_HPP_ */
