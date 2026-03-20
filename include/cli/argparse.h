#pragma once
#include <string>
#include <vector>

namespace sheep {

/*
* @brief struct which configures an argument
*/
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
 * @brief easily parse optional and mandatory arguments from the commandline 
 */
class ArgParse {
public:
  /*
   * @brief Constructor
   * 
   * @param v_arguments configured known arguments
   * @param argc number of arguments received
   * @param argv array holding received arguments
   */
  ArgParse(const std::vector<st_arg>& v_arguments, int argc, char *argv[]);

  /*
  * @brief Constructor
  *
  * @param v_arguments configured known arguments
  * @param v_given_arguments received arguments
  */
  ArgParse(const std::vector<st_arg>& v_arguments, const std::vector<std::string> &v_given_arguments);
  
  /*
   * @brief Destructor
   */
  ~ArgParse() = default;

  /*
   * @brief adds an argument
   * 
   * @param st_argument known argument configuration struct
   */
  void add_argument(const st_arg &st_argument);

  /*
   * @brief adds a list of arguments
   * 
   * @param v_arguments configured known arguments
   */
  void add_arguments(const std::vector<st_arg> &v_arguments);

  /*
   * @brief clears all set arguments
   */
  void clear_arguments();

  /*
   * @brief parses and returns a list of all parsed arguments
   * 
   * @return list of parsed arguments
   */
  std::vector<st_arg> parse(void) const;

private:
  std::vector<st_arg> m_args; // configured known arguments
  std::vector<std::string> m_raw_arguments; // all received arguments
};
} // namespace sheep