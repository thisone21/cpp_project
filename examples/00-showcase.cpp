#include "openai.hpp"

#include <iostream>

int main() {
	std::cout << "Welcome to Language Learning App!" << std::endl;
	std::cout << "Select an option:" << std::endl;
	std::cout << "1. Translate text" << std::endl;
	std::cout << "2. Check grammar" << std::endl;
	std::cout << "3. Practice pronunciation" << std::endl;

	int choice;
	std::cin >> choice;

	switch (choice) {
		case 1:
			// Code for text translation using a translation API
			std::cout << "Enter text to translate: ";
			// Implement translation logic here
			break;
		case 2:
			// Code for grammar checking using a grammar checking API
			std::cout << "Enter text to check grammar: ";
			// Implement grammar checking logic here
			break;
		case 3:
			// Code for pronunciation practice using a speech-to-text API
			std::cout << "Speak the phrase: ";
			// Implement speech-to-text and pronunciation practice logic here
			break;
		default:
			std::cout << "Invalid choice. Exiting program." << std::endl;
			break;
	}

	openai::start(); // Will use the api key provided by `OPENAI_API_KEY` environment variable
	
	auto completion = openai::completion().create(R"(
	  {
	  "model": "text-davinci-003",
	  "prompt": "Say this is a test",
	  "max_tokens": 7,
	  "temperature": 0
	  }
	  )"_json); // Using user-defined (raw) string literals
	  std::cout << "Response is:\n" << completion.dump(2) << '\n'; 

	  return 0;
	  }

