#ifndef ARGPARSE_H
#define ARGPARSE_H
#include <string>
#include <vector>

namespace sheep
{
/*
 * argparse can be used to easily parse optional and mandatory arguments from the commandline
 */
class ArgParse
{
  public:
    /*
     * Constructor
     */
    ArgParse(int argc, char *argv[]);

    /*
     * Destructor
     */
    ~ArgParse() = default;

  private:
    std::vector<std::string> m_args;
};
} // namespace sheep
#endif // ARGPARSE_H