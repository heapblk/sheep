#include "cli/argparse.h"
#include <vector>

using namespace sheep;

//-------------------------------------------------------
ArgParse::ArgParse(const std::vector<st_arg>& v_arguments, int argc, char *argv[])
    : m_args(v_arguments)
    , m_raw_arguments(argv + sizeof(int), argv + argc)
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
    for (const st_arg& argument : v_arguments)
    {
        add_argument(argument);
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
    std::vector<st_arg> v_result_args;

    for (auto raw_args = m_raw_arguments.begin(); raw_args != m_raw_arguments.end(); ++raw_args)
    {
        for (st_arg argument : m_args)
        {
            if (*raw_args == argument.s_long || *raw_args == argument.s_short)
            {
                if (argument.b_value)
                {
                    if (const auto next = std::next(raw_args); next != m_raw_arguments.end())
                    {
                        argument.s_value = *next;
                    }
                    else
                    {
                        argument.b_value = true;
                        argument.s_errormsg = "no value given";
                    }
                }
                v_result_args.push_back(argument);
            }
        }
    }

    return v_result_args;
}