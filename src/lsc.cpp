#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "../lib/cmdparse.hpp"

#define RED_FG "\033[91m"
#define CYAN_FG "\033[96m"

#define DIM "\033[2m"

#define RESET_COLOR "\033[0m"

Args *init_args(int argc, char **argv) {
  Args *args = new Args(argc, argv);

  args->declare_option("h", "help", ArgType::Bool, false, "Show help and exit");
  args->declare_option("a", "all", ArgType::Bool, false, "Show entries starting with .");
  args->declare_option("A", "almost-all", ArgType::Bool, false, "Omit . and ..");
  args->declare_option("p", "plain", ArgType::Bool, false, "Only list the dir files in a plain format");
  args->declare_option("l", "", ArgType::Bool, false, "List dir in long format");

  args->read();

  return args;
}

int main(int argc, char **argv) {
  std::string dir_path;

  Args *args = init_args(argc, argv);

  if (argc == 1 || args->is_option_present("h")) {
    std::cout << "Usage: lsc [option] ... [dir]\n";
    std::cout << "List files in [dir] (defaults to .)\n\n";

    std::cout << args->generate_help();

    return 0;
  }

  if (argc - args->get_option_index_span() > 1) {
    dir_path = argv[args->get_option_index_span() + 1];

    if (!std::filesystem::exists(dir_path)) {
      std::cout << "Directory '" << dir_path << "' doesn't exist!" << std::endl;

      return 1;
    }
  } else {
    dir_path = std::filesystem::current_path();
  }

  std::vector<std::string> dirs = {};
  std::vector<std::string> files = {};

  if (args->is_option_present("a") && !args->is_option_present("A")) {
    dirs.push_back(".");
    dirs.push_back("..");
  }

  for (const auto & entry : std::filesystem::directory_iterator(dir_path)) {
    auto name = entry.path().filename().string();

    if (!args->is_option_present("a") && !args->is_option_present("A") && name[0] == '.')
      continue;

    if (entry.is_directory())
      dirs.push_back(name);
    else
      files.push_back(name);
  }

  for (const auto & dir : dirs) {
    if (args->is_option_present("p"))
      std::cout << dir << std::endl;
    else if (dir == ".")
      std::cout << RED_FG << "c  " << RESET_COLOR << dir << std::endl;
    else if (dir == "..")
      std::cout << RED_FG << "p  " << RESET_COLOR << dir << std::endl;
    else if (!std::filesystem::is_empty(dir_path + "/" + dir))
      std::cout << RED_FG << "d  " << RESET_COLOR << dir << std::endl;
    else
      std::cout << RED_FG << "de " << RESET_COLOR << dir << std::endl;
  }

  for (const auto & file : files) {
    std::string info = *new std::string("");

    if (args->is_option_present("l")) {
      info.append(" ");
      info.append(DIM);
      info.append(std::to_string(std::filesystem::file_size(file)));
      info.append("b");
      info.append(RESET_COLOR);
    }

    if (args->is_option_present("p"))
      std::cout << file << std::endl;
    else
      std::cout << CYAN_FG << "f  " << RESET_COLOR << file << info << std::endl;
  }
}
