#include "openai.hpp"

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstring>

using Json = nlohmann::json;

int main() {
    // OpenAI API 키
    std::string apiKey = "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";

    // 대화 시작
    std::string userInput;
    while (true) {
        std::cout << "나: ";
        std::getline(std::cin, userInput);

        // 사용자 입력을 OpenAI에 보내고 응답 받기
        //openai::CompletionResponse response = chat.complete({userInput});

        Json request;
        request["model"] = "gpt-3.5-turbo";
        request["messages"][0]["role"] = "user";
        request["messages"][0]["content"] = userInput;
        request["temperature"] = 0;

        auto chat = openai::chat().create(request);
        

        std::cout << "ChatGPT: " << chat["choices"][0]["message"]["content"] << std::endl;

        // 대화 종료 조건 설정 (예: "종료" 입력 시 종료)
        if (userInput == "종료") {
            break;
        }
    }

    return 0;
}