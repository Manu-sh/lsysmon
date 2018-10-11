#include "utils_line.hpp"
#include <algorithm>

using namespace utils::Line;

std::istream_iterator<Line> ifstream_l::begin()     { return utils::Line::begin(*this); }
const std::istream_iterator<Line> ifstream_l::end() { return utils::Line::end(*this);   }
void ifstream_l::each_line(std::function<void(const Line &)> f) { std::for_each(this->begin(), this->end(), f);  }
void ifstream_l::default_exceptions() { this->exceptions(std::ifstream::failbit|std::ifstream::badbit); }