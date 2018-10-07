#pragma once
#include <stdexcept>
#include <string>

class unimpl_ex: public std::runtime_error {

	public:
		using std::runtime_error::runtime_error;

};


class bug_ex: public std::runtime_error {

	public:
		using std::runtime_error::runtime_error;

};


#define THROW_UNIMPL_EX(_MSG_) do { throw unimpl_ex("UNIMPLEMENTED: " + std::string(__FILE__) + " at line: " + std::to_string(__LINE__) + " \"" + _MSG_ + "\" "); } while (0)
#define THROW_BUG_EX(_MSG_)    do { throw bug_ex("BUG: " + std::string(__FILE__) + " at line: " + std::to_string(__LINE__) + " \"" + _MSG_ + "\" "); } while (0)