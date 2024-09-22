#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#define RED_FG "\033[91m"
#define CYAN_FG "\033[96m"

#define RESET_COLOR "\033[0m"

int main(int argc, char **argv) {
  std::string dir_path;

  if (argc > 1) {
    dir_path = argv[1];

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
