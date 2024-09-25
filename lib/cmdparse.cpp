#include "./cmdparse.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

Args::Args(int argc, char **argv) {
  this->count = argc - 1;
  this->command = argv;
  this->options = new std::unordered_map<std::string, option>();
  this->real_options = new std::unordered_map<std::string, option>();
  this->argc = argc;
  this->argv = argv;
  this->option_index_span = 0;
}

void Args::read() {
  for (int i = 1; i < this->argc; i++) {
    if (this->argv[i][0] == '-') {
      auto found_option = this->options->find(this->argv[i]);
      option *option;

      if (found_option == NULL)
        continue;

      option = &found_option->second;

      if (option->type == ArgType::String && i != this->argc - 1 &&
          this->argv[i + 1][0] != '-') {
        option->value.assign(argv[i + 1]);

        this->option_index_span += 2;
      } else {
        option->value.assign("true");
        this->option_index_span++;
      }

      this->real_options->insert_or_assign(option->name, *option);
    }
  }
}

void Args::declare_option(std::string name, ArgType type) {
  option option = {.name = name, .value = (char *)malloc(0), .type = type};

  this->options->insert_or_assign(name, option);
}

std::unordered_map<std::string, option> *Args::get_options() {
  return this->options;
}

std::unordered_map<std::string, option> *Args::get_real_options() {
  return this->real_options;
}

int Args::get_options_length() { return this->options->size(); }

int Args::get_option_index_span() { return this->option_index_span; }
