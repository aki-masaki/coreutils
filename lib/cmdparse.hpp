#include <string>
#include <unordered_map>

enum ArgType {
  Bool,
  String
};

struct option {
  std::string short_name;
  std::string long_name;
  std::string value;
  std::string description;
  ArgType type;
  bool required;
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

  void declare_option(std::string, std::string, ArgType, bool, std::string);

  bool is_option_present(std::string);
  std::string get_option_value(std::string);

  std::string generate_help();
};
