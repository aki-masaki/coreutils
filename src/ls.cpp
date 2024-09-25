#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "../lib/cmdparse.hpp"

#define RED_FG "\033[91m"
#define CYAN_FG "\033[96m"

#define RESET_COLOR "\033[0m"

int main(int argc, char **argv) {
  std::string dir_path;

  Args *args = new Args(argc, argv);

  args->declare_option((char *) "-h", ArgType::Bool);
  args->declare_option((char *) "-d", ArgType::String);

  args->read();

  for (auto it : *args->get_real_options())
    std::cout << it.first << " " << it.second.value << std::endl;

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

  for (const auto & entry : std::filesystem::directory_iterator(dir_path)) {
    auto name = entry.path().filename().string();

    if (name[0] == '.')
      continue;

    if (entry.is_directory())
      dirs.push_back(name);
    else
      files.push_back(name);
  }

  for (const auto & dir : dirs) {
    std::cout << RED_FG << "\u{f024b} " << RESET_COLOR << dir << std::endl;
  }

  for (const auto & file : files) {
    std::cout << CYAN_FG << "\u{f0214} " << RESET_COLOR << file << std::endl;
  }
}
