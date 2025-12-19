#pragma once

#include <stdexcept>
#include <string>

namespace lib::exception {

class InvalidCoordinates : public std::runtime_error {
public:
  explicit InvalidCoordinates(const std::string &msg)
      : std::runtime_error(msg) {}
};
} // namespace lib::exception