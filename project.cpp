#include "openai.hpp"
#include "hdr_project.hpp"

using Json = nlohmann::json;

int main()
{
    openai::start();

    string userInput;
    vector<Json> chathistory;
    vector<wordlist> w_lists;

    cout << "안녕하세요? 단어장에 오신 것을 환영합니다." << endl;
    cout << "어떤 작업을 하고 싶으신가요? 숫자로 입력해 주세요." << endl << endl;
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
        cout << "어떤 작업을 하고 싶으신가요? 숫자로 입력해 주세요." << endl;
        cout << "나 : ";
        getline(cin, userInput);
    }

    while (1)
    {
        if (userInput == "exit")
        {
            cout << endl << "종료합니다. 수고 많으셨습니다" << endl;
            return 0;
        }
        else if (userInput == "help")
        {
            cout << "1. 번역" << endl;
            cout << "-입력해주신 문장을 원하는 언어로 번역해 드립니다." << endl
                 << endl;
            cout << "2. 단어 검색" << endl;
            cout << "-궁금한 단어의 영/한, 한/영 번역을 도와드립니다." << endl
                 << endl;
        }
        else if (userInput == "2") // 단어 검색
        {
            cout << endl
                 << "단어 검색에 오신 것을 환영합니다." << endl;
            cout << "1. 한국어 -> 영어 2. 영어 -> 한국어 중 원하는 기능을 숫자로 입력해 주세요." << endl;
            cout << "나 : ";
            getline(cin, userInput);

            if (userInput == "1") // 한국어 -> 영어 단어
            {
                cout << endl
                     << "검색을 원하는 한국어 단어를 입력해 주세요." << endl;
                cout << "예시) 반도체" << endl;
                cout << "나 : ";
                getline(cin, userInput);

                word newword = word(userInput);
                // newword.displaykor();

                Json request;
                request["model"] = "gpt-3.5-turbo-1106";
                request["messages"][0]["role"] = "system";
                request["messages"][0]["content"] = "you are a translator model which translates a korean word input into english, and give only the translated word without any other words";
                request["messages"][1]["role"] = "user";
                request["messages"][1]["content"] = "translate" + newword.getkor() + " into english and give only a translated word, without anything else";
                request["temperature"] = 0;

                auto chat = openai::chat().create(request);

                // 번역한 단어 출력
                string _eng = chat["choices"][0]["message"]["content"].dump();
                _eng = _eng.substr(1, _eng.size() - 2);
                cout << endl
                     << newword.getkor() << "의 영어 번역 결과는 다음과 같습니다 : " << _eng << endl;
                newword.addeng(_eng);
                // newword.displayeng();

                // 예문 생성
                // Json request;
                request["model"] = "gpt-3.5-turbo-1106";
                request["messages"][0]["role"] = "system";
                request["messages"][0]["content"] = "you are a language model that makes an example sentence from the given word and prints the example sentence";
                request["messages"][1]["role"] = "user";
                request["messages"][1]["content"] = "make an example sentence that includes" + newword.geteng() + "and give only the example sentence, without anything else";
                request["temperature"] = 0;

                chat = openai::chat().create(request);

                string examsent = chat["choices"][0]["message"]["content"].dump();
                examsent = examsent.substr(1, examsent.size() - 2);
                cout << "예문 : " << examsent << endl;

                // 단어장에 추가
                cout << endl
                     << "해당 단어를 단어장에 추가하시겠습니까? [Y/N]" << endl;
                cout << "나 : ";
                getline(cin, userInput);
                if (userInput == "Y" || userInput == "y")
                {
                    string w_name;
                    if (w_lists.empty())
                    {
                        cout << endl
                             << "현재 생성된 단어장이 없습니다. 새롭게 생성할 단어장의 이름을 입력해 주세요." << endl;
                        cout << "나 : ";
                        getline(cin, w_name);
                        wordlist newwlist;
                        newwlist.setname(w_name);
                        time_t curtime = time(NULL);
                        struct tm *t = localtime(&curtime);
                        wordentry newentry(newword, t);
                        newwlist.addentry(newentry);
                        w_lists.push_back(newwlist);
                    }
                    else
                    {
                        cout << endl
                             << "넣을 단어장을 선택해 단어장의 이름을 입력해 주세요. 다음은 현재 단어장 목록입니다." << endl;
                        for (auto it = w_lists.begin(); it != w_lists.end(); it++)
                        {
                            int i = 1;
                            cout << i << ". " << (*it).getname() << " ";
                            i++;
                        }
                        cout << endl;
                        cout << "나 : ";
                        getline(cin, w_name);

                        for (auto it = w_lists.begin(); it != w_lists.end(); it++)
                        {
                            if ((*it).getname() == w_name)
                            {
                                time_t curtime = time(NULL);
                                struct tm *t = localtime(&curtime);
                                wordentry newentry(newword, t);
                                (*it).addentry(newentry);
                                break;
                            }
                        }
                    }
                }
            }
            else if (userInput == "2") // 영어 -> 한국어 단어
            {
                cout << endl
                     << "검색을 원하는 영어 단어를 입력해 주세요." << endl;
                cout << "예시) semiconductor" << endl;
                cout << "나 : ";
                getline(cin, userInput);

                Json request;
                request["model"] = "gpt-3.5-turbo-1106";
                request["messages"][0]["role"] = "system";
                request["messages"][0]["content"] = "you are a translator model which translates an english word input into korean, and give only the translated word without any other words";
                request["messages"][1]["role"] = "user";
                request["messages"][1]["content"] = "translate the word " + userInput + " into korean and give only the translated word, without pronounciation";
                request["temperature"] = 0;

                auto chat = openai::chat().create(request);
                string _kor = chat["choices"][0]["message"]["content"].dump();
                _kor = _kor.substr(1, _kor.size() - 2);
                cout << endl
                     << userInput << "의 한국어 번역 결과는 다음과 같습니다 : " << _kor << endl;

                word newword = word(_kor);
                newword.addeng(userInput);

                // 단어장에 추가
                cout << endl
                     << "해당 단어를 단어장에 추가하시겠습니까? [Y/N]" << endl;
                cout << "나 : ";
                getline(cin, userInput);
                if (userInput == "Y" || userInput == "y")
                {
                    string w_name;
                    if (w_lists.empty())
                    {
                        cout << "현재 생성된 단어장이 없습니다. 새롭게 생성할 단어장의 이름을 입력해 주세요." << endl;
                        cout << "나 : ";
                        getline(cin, w_name);
                        wordlist newwlist;
                        newwlist.setname(w_name);
                        time_t curtime = time(NULL);
                        struct tm *t = localtime(&curtime);
                        wordentry newentry(newword, t);
                        newwlist.addentry(newentry);
                        w_lists.push_back(newwlist);
                    }
                    else
                    {
                        cout << "넣을 단어장을 선택해 단어장의 이름을 입력해 주세요. 다음은 현재 단어장 목록입니다." << endl;
                        cout << "새로운 이름을 입력할 시 새로운 단어장을 생성합니다." << endl;
                        int i = 1;
                        for (auto it = w_lists.begin(); it != w_lists.end(); it++)
                        {
                            cout << i << ". " << (*it).getname() << " ";
                            i++;
                        }
                        cout << endl;
                        cout << "나 : ";
                        getline(cin, w_name);

                        int flag = 0;
                        for (auto it = w_lists.begin(); it != w_lists.end(); it++)
                        {
                            if ((*it).getname() == w_name)
                            {
                                time_t curtime = time(NULL);
                                struct tm *t = localtime(&curtime);
                                wordentry newentry(newword, t);
                                (*it).addentry(newentry);
                                flag = 1;
                                break;
                            }
                        }
                        if (flag == 0)
                        {
                            wordlist newwlist;
                            newwlist.setname(w_name);
                            time_t curtime = time(NULL);
                            struct tm *t = localtime(&curtime);
                            wordentry newentry(newword, t);
                            newwlist.addentry(newentry);
                            w_lists.push_back(newwlist);
                        }
                    }
                }
            }
        }
        else if (userInput == "1") // 번역
        {
            cout << endl
                 << "번역에 오신 것을 환영합니다." << endl;
            cout << "1. 한국어 -> 다른 언어 2. 다른 언어 -> 한국어 중 원하는 기능을 숫자로 입력해 주세요." << endl;
            cout << "나 : ";
            getline(cin, userInput);

            if (userInput == "1")
            {
                cout << endl
                     << "번역을 원하시는 문장을 입력해주세요." << endl;
                cout << "나: ";
                getline(cin, userInput);
                Sentence origin = Sentence(userInput);
                cout << endl
                     << "번역을 원하시는 언어를 입력해주세요." << endl;
                cout << "나: ";
                getline(cin, userInput);
                string lang = userInput;

                Json request;
                request["model"] = "gpt-3.5-turbo-1106";
                request["messages"][0]["role"] = "system";
                request["messages"][0]["content"] = "You are a translator model which translates a korean sentence input into given language";
                request["messages"][1]["role"] = "user";
                request["messages"][1]["content"] = "translate" + origin.getoriginalText() + "into" + lang + "without its pronounciation";
                request["temperature"] = 0;

                auto chat = openai::chat().create(request);

                string translatedText = chat["choices"][0]["message"]["content"].dump();
                translatedText = translatedText.substr(1, translatedText.size() - 2);
                cout << endl
                     << "문장 번역 결과는 다음과 같습니다: " << translatedText << endl;
                origin.add_engtrans(translatedText);
            }
            else if (userInput == "2")
            {
                cout << endl
                     << "번역을 원하시는 언어를 입력해주세요." << endl;
                cout << "나: ";
                getline(cin, userInput);
                string lang = userInput;
                cout << endl
                     << "번역을 원하시는 문장을 입력해주세요." << endl;
                cout << "나: ";
                getline(cin, userInput);
                Sentence origin = Sentence(userInput);

                Json request;
                request["model"] = "gpt-3.5-turbo-1106";
                request["messages"][0]["role"] = "system";
                request["messages"][0]["content"] = "You are a translator model which translates a sentence input contained of given language into korean";
                request["messages"][1]["role"] = "user";
                request["messages"][1]["content"] = "translate" + origin.getoriginalText() + "into korean";
                request["temperature"] = 0;

                auto chat = openai::chat().create(request);

                string translatedText = chat["choices"][0]["message"]["content"].dump();
                translatedText = translatedText.substr(1, translatedText.size() - 2);
                cout << endl
                     << "문장 번역 결과는 다음과 같습니다: " << translatedText << endl;
                origin.add_kortrans(translatedText);
            }
        }
        int i = 1;
        for (auto it = w_lists.begin(); it != w_lists.end(); it++)
        {
            cout << "단어장 " << i << endl;
            (*it).display();
            cout << endl;
            i++;
        }
        cout << endl
             << "어떤 작업을 하고 싶으신가요? 숫자로 입력해 주세요. 도움이 필요하시면 help를, 종료하시려면 exit를 입력해 주세요." << endl;
        cout << "나 : ";
        getline(cin, userInput);
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