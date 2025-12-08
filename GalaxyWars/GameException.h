#pragma once
#include <exception> // for the try catch shit
#include <string>

using namespace std;

// RUBRIC: EXCEPTION HANDLING - CUSTOM EXCEPTION CLASS
// inherits from standard exception class
class GameException :public exception {
private:
	string message;

public:
	// constructor: takes a custom error message
	GameException(const string& msg) : message(msg) {}

	// override 'what()' to return our custom message
	// virtual const char* what() is how cpp handles erroneous text
	virtual const char* what() const throw() {
		return this->message.c_str();
	}
};