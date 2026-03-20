#include "cli/argparse.h"
#include <vector>

using namespace sheep;

//-------------------------------------------------------
ArgParse::ArgParse(const std::vector<st_arg>& v_arguments, int argc, char *argv[])
    : m_args(v_arguments)
    , m_raw_arguments(argv + 1, argv + argc)
{
}

//-------------------------------------------------------
ArgParse::ArgParse(const std::vector<st_arg> &v_arguments, const std::vector<std::string> &v_given_arguments)
    : m_args(v_arguments)
    , m_raw_arguments(v_given_arguments.begin() + 1, v_given_arguments.end())
{
}

//-------------------------------------------------------
void ArgParse::add_argument(const st_arg &st_argument) { m_args.push_back(st_argument); }

//-------------------------------------------------------
void ArgParse::add_arguments(const std::vector<st_arg> &v_arguments)
{
    for (const st_arg& _argument : v_arguments)
    {
        add_argument(_argument);
    }
}

//-------------------------------------------------------
void ArgParse::clear_arguments()
{
    m_args.clear();
}

//-------------------------------------------------------
std::vector<st_arg> ArgParse::parse(void) const
{
    std::vector<st_arg> _v_result_args;

    for (auto _raw_args = m_raw_arguments.begin(); _raw_args != m_raw_arguments.end(); ++_raw_args)
    {
        for (st_arg _argument : m_args)
        {
            if (*_raw_args == _argument.s_long || *_raw_args == _argument.s_short)
            {
                if (_argument.b_value)
                {
                    if (const auto _next = std::next(_raw_args); _next != m_raw_arguments.end())
                    {
                        _argument.s_value = *_next;
                    }
                    else
                    {
                        _argument.b_value = true;
                        _argument.s_errormsg = "no value given";
                    }
                }
                _v_result_args.push_back(_argument);
            }
        }
    }

    return _v_result_args;
}