#include "openai.hpp"
#include "hdr_project.hpp"

using Json = nlohmann::json;

int main()
{
    openai::start();

    string userInput;
    vector<Json> chathistory;
    wordlist w_list;

    /*Json initrequest;
    initrequest["model"] = "gpt-3.5-turbo";
    initrequest["messages"][0]["role"] = "user";
    initrequest["messages"][0]["content"] = "Say greetings to user";
    initrequest["temperature"] = 0;

    auto chat = openai::chat().create(initrequest);
    std::cout << "ChatGPT: " << chat["choices"][0]["message"]["content"] << std::endl;*/
    cout << "안녕하세요? 단어장에 오신 것을 환영합니다." << endl;
    cout << "어떤 작업을 하고 싶으신가요? 숫자로 입력해 주세요." << endl;
    cout << "1. 만능 번역기 2. 단어 검색" << endl;
    cout << "각 기능들에 대한 설명이 필요하시다면, help를 입력해 주세요." << endl;
    cout << "나 : ";
    getline(cin, userInput);

    if (userInput == "help")
    {
        // cout << endl;
        // cout << "*************************도움말*************************" << endl;
        cout << "1. 번역" << endl;
        cout << "-입력해주신 문장을 원하는 언어로 번역해 드립니다." << endl
             << endl;
        cout << "2. 단어 검색" << endl;
        cout << "-궁금한 단어의 영/한, 한/영 번역을 도와드립니다." << endl
             << endl;
        /*cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;*/
        cout << "어떤 작업을 하고 싶으신가요? 숫자로 입력해 주세요." << endl;
        cout << "나 : ";
        getline(cin, userInput);
    }

    if (userInput == "exit")
    {
        cout << "종료합니다." << endl;
        return 0;
    }
    else if (userInput == "2") // 단어 검색
    {
        cout << "단어 검색에 오신 것을 환영합니다." << endl;
        cout << "1. 한국어 -> 다른 언어 2. 영어 -> 한국어 중 원하는 기능을 숫자로 입력해 주세요." << endl;
        cout << "나 : ";
        getline(cin, userInput);

        if (userInput == "1") //한국어 -> 영어
        {
            cout << "검색을 원하는 한국어 단어를 입력해 주세요." << endl;
            cout << "예시) 반도체" << endl;
            cout << "나 : ";
            getline(cin, userInput);

            word newword = word(userInput);
            //newword.displaykor();

            Json request;
            request["model"] = "gpt-3.5-turbo-1106";
            request["messages"][0]["role"] = "system";
            request["messages"][0]["content"] = "you are a translator model which translates a korean word input into english, and give only the translated word without any other words";
            request["messages"][0]["role"] = "user";
            request["messages"][0]["content"] = "translate" + newword.getkor() + " into english and give only a translated word, without anything else";
            request["temperature"] = 0;

            auto chat = openai::chat().create(request);

            //번역한 단어 출력
            string _eng = chat["choices"][0]["message"]["content"].dump();
            _eng = _eng.substr(1, _eng.size() - 2);
            cout << newword.getkor() << "의 영어 번역 결과는 다음과 같습니다 : " << _eng << endl;
            newword.addeng(_eng);
            //newword.displayeng();

            //예문 생성
            //Json request;
            request["model"] = "gpt-3.5-turbo-1106";
            request["messages"][0]["role"] = "system";
            request["messages"][0]["content"] = "you are a language model that makes an example sentence from the given word and prints the example sentence";
            request["messages"][0]["role"] = "user";
            request["messages"][0]["content"] = "make an example sentence that includes" + newword.geteng()  + "and give only the example sentence, without anything else";
            request["temperature"] = 0;

            chat = openai::chat().create(request);

            string examsent = chat["choices"][0]["message"]["content"].dump();
            examsent = examsent.substr(1, examsent.size() - 2);
            cout << "예문 : " << examsent << endl;

            //단어장에 추가
            cout << "해당 단어를 단어장에 추가하시겠습니까? [Y/N]" << endl;
            cout << "나 : ";
            getline(cin, userInput);
            if (userInput == "Y" || userInput == "y")
            {
                w_list.addword(newword);
            }

        }
        else if (userInput == "2") //영어 -> 한국어
        {            
            cout << "검색을 원하는 영어 단어를 입력해 주세요." << endl;
            cout << "예시) semiconductor" << endl;
            cout << "나 : ";
            getline(cin, userInput);

            Json request;
            request["model"] = "gpt-3.5-turbo-1106";
            request["messages"][0]["role"] = "system";
            request["messages"][0]["content"] = "you are a translator model which translates an english word input into korean, and give only the translated word without any other words";
            request["messages"][0]["role"] = "user";
            request["messages"][0]["content"] = "translate" + userInput + " into korean"+ "and give only a translated word, without anything else";
            request["temperature"] = 0;

            auto chat = openai::chat().create(request);
            string _kor = chat["choices"][0]["message"]["content"].dump();
            _kor = _kor.substr(1, _kor.size() - 2);
            cout << userInput << "의 한국어 번역 결과는 다음과 같습니다 : " << _kor << endl;
        }
    }
    else if (userInput == "1") // 번역
    {
        cout << "번역에 오신 것을 환영합니다." << endl;
        cout << "1. 한국어 -> 다른 언어 2. 다른 언어 -> 한국어 중 원하는 기능을 숫자로 입력해 주세요." << endl;
		cout << "나 : ";
		getline(cin, userInput);

		if(userInput == "1")
		{
			cout << "번역을 원하시는 문장을 입력해주세요." << endl;
			cout << "나: ";
			getline(cin, userInput);
			Sentence origin = Sentence(userInput);
			cout << "번역을 원하시는 언어를 입력해주세요." << endl;
			cout << "나: ";
			getline(cin, userInput);
			string lang = userInput;

			Json request;
			request["model"]="gpt-3.5-turbo-1106";
			request["messages"][0]["role"] = "system";
			request["messages"][0]["content"] = "You are a translator model which translates a korean sentence input into given language";
			request["messages"][0]["role"] = "user";
			request["messages"][0]["content"] = "translate" + origin.getoriginalText() + "into" + lang;
			request["temperature"] = 0;

			auto chat = openai::chat().create(request);

			cout << "문장 번역 결과는 다음과 같습니다: " << chat["choices"][0]["message"]["content"] << endl;
			string translatedText = chat["choices"][0]["message"]["content"].dump();
			origin.add_engtrans(translatedText);
		}
		else if(userInput == "2")
		{
			cout << "번역을 원하시는 언어를 입력해주세요." << endl;
			cout << "나: ";
			getline(cin, userInput);
			string lang = userInput;
			cout << "번역을 원하시는 문장을 입력해주세요." << endl;
			cout << "나: ";
			getline(cin, userInput);
			Sentence origin = Sentence(userInput);

			Json request;
			request["model"]="gpt-3.5-turbo-1106";
			request["messages"][0]["role"] = "system";
			request["messages"][0]["content"] = "You are a translator model which translates a sentence input contained of given language into korean";
			request["messages"][0]["role"] = "user";
			request["messages"][0]["content"] = "translate" + origin.getoriginalText() + "into korean";
			request["temperature"] = 0;

			auto chat = openai::chat().create(request);

			cout << "문장 번역 결과는 다음과 같습니다: " << chat["choices"][0]["message"]["content"] << endl;
			string translatedText = chat["choices"][0]["message"]["content"].dump();
			origin.add_kortrans(translatedText);
		}
    }

    // 대화 내용 유지 예시
    /*while (true)
    {
        cout << "나 : ";
        getline(std::cin, userInput);

        //exit인 경우 종료
        if (userInput == "exit")
        {
            break;
        }

        chathistory.push_back({{"role", "user"}, {"content", userInput}});
        Json request;
        request["model"] = "gpt-3.5-turbo-1106";
        request["messages"] = chathistory;
        request["temperature"] = 0;

        auto chat = openai::chat().create(request);

        std::cout << "ChatGPT: " << chat["choices"][0]["message"]["content"] << std::endl;
        chathistory.push_back({{"role", "assistant"}, {"content", chat["choices"][0]["message"]["content"]}});
    }*/

    return 0;
}