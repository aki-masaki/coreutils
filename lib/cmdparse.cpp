#include "./cmdparse.hpp"
#include <cstdlib>
#include <cstring>

Args::Args(int argc, char **argv) {
  this->count = argc - 1;
  this->command = argv;
  this->options = new std::unordered_map<std::string, option>();
  this->real_options = new std::unordered_map<std::string, option>();
  this->argc = argc;
  this->argv = argv;
  this->option_index_span = 0;
  this->error = *new std::string("");
}

void Args::read() {
  option *found_option;
  std::string arg;

  for (int i = 1; i < this->argc; i++) {
    // --long
    if (this->argv[i][0] == '-' && this->argv[i][1] == '-') {
      arg = *new std::string(this->argv[i]);

      for (auto option : *this->options)
        if (option.second.long_name == arg.substr(2))
          found_option = &option.second;

      if (found_option == NULL) {
        this->error.assign("invalid option: " + arg);

        return;
      }

      if (found_option->type == ArgType::String && i != this->argc - 1 &&
          this->argv[i + 1][0] != '-') {
        found_option->value.assign(argv[i + 1]);

        this->option_index_span += 2;
      } else {
        found_option->value.assign("true");
        this->option_index_span++;
      }

      this->real_options->insert_or_assign(found_option->short_name,
                                           *found_option);
    }
    // -s, which means multiple options can be nested in the same -
    // e.g., -lah
    else if (this->argv[i][0] == '-') {
      for (char c : *new std::string(this->argv[i])) {
        std::string *char_string = new std::string(1, c);

        if (c == '-') {
          this->option_index_span++;

          continue;
        }

        auto found_option = this->options->find(*char_string);

        option *option;

        if (found_option == NULL) {
          this->error.assign("invalid option: " + *char_string);

          return;
        }

        option = &found_option->second;

        if (sizeof(this->argv[i]) == 2 && option->type == ArgType::String &&
            i != this->argc - 1 && this->argv[i + 1][0] != '-')
          option->value.assign(argv[i + 1]);
        else
          option->value.assign("true");

        this->real_options->insert_or_assign(*char_string, *option);
      }
    }
  }
}

void Args::declare_option(std::string short_name, std::string long_name,
                          ArgType type, bool required,
                          std::string description) {
  option option = {.short_name = short_name,
                   .long_name = long_name,
                   .value = (char *)malloc(0),
                   .description = description,
                   .type = type,
                   .required = required};

  this->options->insert_or_assign(short_name, option);
}

std::unordered_map<std::string, option> *Args::get_options() {
  return this->options;
}

std::unordered_map<std::string, option> *Args::get_real_options() {
  return this->real_options;
}

int Args::get_options_length() { return this->options->size(); }

int Args::get_option_index_span() { return this->option_index_span; }

bool Args::is_option_present(std::string name) {
  auto found_option = this->real_options->find(name);

  return found_option != this->real_options->end();
}

std::string Args::get_option_value(std::string name) {
  auto found_option = this->real_options->find(name);

  if (found_option == this->real_options->end())
    return *new std::string("");

  return found_option->second.value;
}

std::string Args::generate_help() {
  std::string help = *new std::string();

  for (auto option : *this->options) {
    help.append("-");
    help.append(option.second.short_name);

    if (option.second.long_name != "") {
      help.append(", --");
      help.append(option.second.long_name);
    }

    help.append("\n    ");
    help.append(option.second.description);

    help.append("\n\n");
  }

  // Remove the last newline added after the last option
  help.pop_back();

  return help;
}

std::string Args::get_error() {
  return this->error;
}
