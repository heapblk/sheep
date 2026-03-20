#ifndef ARGPARSE_H
#define ARGPARSE_H
#include <string>
#include <vector>

namespace sheep {

struct st_arg {
  std::string s_long; // long argument name
  std::string s_short; // short argument name
  std::string s_description; // description of argument
  bool b_value = false; // is a value expected
  bool b_required = false; // both the argument and the value are required
  bool b_error = false; // true if an argument has thrown an error
  std::string s_value; // returned with the read value
  std::string s_errormsg; // errormsg if something went wrong
};

/*
 * argparse can be used to easily parse optional and mandatory arguments from
 * the commandline its modeled after pythons argparse
 */
class ArgParse {
public:
  /*
   * Constructor
   */
  ArgParse(const std::vector<st_arg>& v_arguments, int argc, char *argv[]);

  //TODO: remove after testing (lol)
  ArgParse(const std::vector<st_arg>& v_arguments, const std::vector<std::string> &v_given_arguments);

  /*
   * Destructor
   */
  ~ArgParse() = default;

  /*
   * adds an argument
   */
  void add_argument(const st_arg &st_argument);

  /*
   * adds a list of arguments
   */
  void add_arguments(const std::vector<st_arg> &v_arguments);

  /*
   * clears all set arguments
   */
  void clear_arguments();

  /*
   * returns a list of all parsed arguments
   */
  std::vector<st_arg> parse(void) const;

private:
  std::vector<st_arg> m_args;
  std::vector<std::string> m_raw_arguments;
};
} // namespace sheep
#endif // ARGPARSE_H
