#define REGISTER(cmd)                                                          \
  callbacks[#cmd] = std::bind(&ConsoleUI::cmd, this, std::placeholders::_1);

#define LOG(words) std::cout << __func__ << " >> " << words
#define LOG_MAIN(words) std::cout << "Console >> " << words

// nullable read
#define N_READ(var)                                                            \
  std::string var;                                                             \
  LOG(#var ": ");                                                              \
  std::getline(std::cin, var);

#define READ(var)                                                              \
  N_READ(var)                                                                  \
  if (var.size() == 0) {                                                       \
    LOG("invalid input !");                                                    \
    return;                                                                    \
  }
