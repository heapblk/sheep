//
// Created by heapblk on 18.03.26.
//
#include <gtest/gtest.h>
#include <cli/argparse.h>
#include <string>
#include <vector>

using namespace sheep;

TEST(ArgParse, parse)
{
    std::vector<st_arg> v_argument_structs;
    std::vector<std::string> v_long_arguments =
        {
        "programname",
        "--longflagname",
        "--slfn",
        "--notsupported"
        };

    st_arg st_argumentone;
    st_argumentone.s_long = "--longflagname";
    st_argumentone.s_short = "--lfn";

    st_arg st_argumenttwo;
    st_argumenttwo.s_long = "--secondlongflagname";
    st_argumenttwo.s_short = "--slfn";

    v_argument_structs.push_back(st_argumentone);
    v_argument_structs.push_back(st_argumenttwo);

    ArgParse o_argparse(v_argument_structs, v_long_arguments);

    std::vector<st_arg> v_args =  o_argparse.parse();

    bool success = true;
    if (v_args.size() == 2)
    {
        for (st_arg arg : v_args)
        {
            if (arg.s_long != "--longflagname" && arg.s_long != "--secondlongflagname")
            {
                success &= false;
            }
        }
    }
    else
    {
        success &= false;
    }

    ASSERT_TRUE(success);
};

TEST(ArgParse, parse_fail)
{
    std::vector<st_arg> v_argument_structs;
    std::vector<std::string> v_long_arguments =
    {
        "programname",
        "--wrongflagname",
        "--wrongsecondflagname",
        "--notsupported"
        };

    st_arg st_argumentone;
    st_argumentone.s_long = "--longflagname";
    st_argumentone.s_short = "--lfn";

    st_arg st_argumenttwo;
    st_argumenttwo.s_long = "--secondlongflagname";
    st_argumenttwo.s_short = "--slfn";

    v_argument_structs.push_back(st_argumentone);
    v_argument_structs.push_back(st_argumenttwo);

    ArgParse o_argparse(v_argument_structs, v_long_arguments);

    std::vector<st_arg> v_args =  o_argparse.parse();

    if (v_args.size() == 0)
    {
        ASSERT_TRUE(true);
    }
};

TEST(ArgParse, parse_value)
{
    std::vector<st_arg> v_argument_structs;
    std::vector<std::string> v_long_arguments =
    {
        "programname",
        "--flagwithvalue",
        "superduperawesomevalue",
        };

    st_arg st_argument;
    st_argument.s_long = "--flagwithvalue";
    st_argument.s_short = "--fwv";
    st_argument.b_value = true;

    v_argument_structs.push_back(st_argument);

    ArgParse o_argparse(v_argument_structs, v_long_arguments);

    std::vector<st_arg> v_args =  o_argparse.parse();

    bool success = true;
    if (v_args.size() == 1)
    {
        if (v_args.at(0).s_value != "superduperawesomevalue")
            success &= false;
    }
    ASSERT_TRUE(success);
};
