#include <iostream>
#include <filesystem>
#include "../lib/cmdparse.hpp"

Args *init_args(int argc, char **argv) {
  Args *args = new Args(argc, argv);

  args->declare_option("h", "help", ArgType::Bool, false, "Show help and exit");
  args->declare_option("p", "", ArgType::Bool, false, "Don't throw error if [dir] already exists");

  args->read();

  return args;
}

int main(int argc, char **argv) {
  Args *args = init_args(argc, argv);

  if (args->get_error() != "") {
    std::cout << "mkdr: " << args->get_error() << "\n";

    std::cout << "Run mkdr -h for help\n";

    return 1;
  }

  if (argc == 1 || args->is_option_present("h")) {
    std::cout << "Usage: mkdr [option] ... [dirs]\n";
    std::cout << "Create [dirs]\n\n";

    std::cout << args->generate_help();

    return 0;
  }

  for (int i = args->get_option_index_span() + 1; i < argc; i++) {
    std::string dir = argv[i];

    if (argv[i][0] == '-')
      continue;

    if (!args->is_option_present("p") && std::filesystem::exists(dir)) {
      std::cout << dir << " exists!\n";

      continue;
    }

    std::filesystem::create_directories(dir);
  }
}
