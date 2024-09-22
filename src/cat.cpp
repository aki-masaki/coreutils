#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cout << "Input file not specified! Exiting." << std::endl;

    return 1;
  }

  std::ifstream file(argv[1]);

  if (file.fail()) {
    std::cout << "Input file doesn't exist! Exiting." << std::endl;

    return 1;
  }

  std::stringstream stream;

  while (file >> stream.rdbuf())
    ;

  std::cout << stream.str();
}
