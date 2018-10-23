#pragma once

#include <exception>
#include <string>

namespace ecosnail {
namespace config {

class ReadError : public std::exception {
public:
    ReadError(const char *message)
		: std::exception()
		, _message(message)
	{ }
    //~ReadError() noexcept {}

	const char * what() const noexcept override
	{
		return _message.c_str();
	}

private:
	std::string _message;
};

}} // namespace ecosnail::config
