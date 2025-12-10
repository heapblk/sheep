#include "cli/argparse.h"
#include <vector>

using namespace sheep;

//-------------------------------------------------------
ArgParse::ArgParse(int argc, char *argv[])
    :m_args(argv, argv + argc)
{
}