#include <string>
#include <unordered_map>

enum ArgType {
  Bool,
  String
};

struct option {
  std::string name;
  std::string value;
  ArgType type;
};

class Args {
private:
  std::unordered_map<std::string, option> *options;
  std::unordered_map<std::string, option> *real_options;
  char **command;
  int count;
  int argc;
  char **argv;
  int option_index_span;
public:
  Args(int, char **);
  ~Args();

  void read();

  std::unordered_map<std::string, option> *get_options();
  std::unordered_map<std::string, option> *get_real_options();
  int get_options_length();

  int get_option_index_span();

  void declare_option(std::string, ArgType);
};
