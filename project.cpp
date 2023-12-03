#include "openai.hpp"
#include "hdr_project.hpp"

using Json = nlohmann::json;

int main()
{
	openai::start();
	srand((unsigned int)time(NULL));

	string userInput;
	vector<wordlist> w_lists;

	cout << "------------------------------------------------------------------------------------------" << endl;
	cout << "                    안녕하세요? 언어학습 프로그램에 오신 것을 환영합니다.                     " << endl;
	cout << "------------------------------------------------------------------------------------------" << endl;
	cout << endl
		 << "                       어떤 작업을 하고 싶으신가요? 숫자로 입력해 주세요.                      " << endl
		 << "                          1. 만능 번역기 2. 단어 검색 3. 단어장 확인                          " << endl
		 << "                                   4. 퀴즈 5. 단어추측게임                                   " << endl
		 << "                  도움이 필요하시면 help를, 종료하시려면 exit을 입력해 주세요.                  " << endl;
	cout << endl;
	cout << "사용자: ";

	string wordexample_kor_fruit[10] = {"사과", "바나나", "배", "수박", "귤", "딸기", "복숭아", "체리", "포도", "오렌지"};
	string wordexample_eng_fruit[10] = {"apple", "banana", "pear", "watermelon", "tangerine", "strawberry", "peach", "cherry", "grape", "orange"};

	word newword(wordexample_kor_fruit[0], wordexample_eng_fruit[0]);

	wordlist newwlist;
	newwlist.setname("fruit");

	for (int i = 0; i < 10; i++)
	{
		word newword(wordexample_kor_fruit[i], wordexample_eng_fruit[i]);
		time_t curtime = time(NULL);
		struct tm *t = localtime(&curtime);
		wordentry newentry(newword, t);
		newwlist.addentry(newentry, 0);
	}

	w_lists.push_back(newwlist);

	wordlist newlist;
	newlist.setname("오답노트");
	w_lists.push_back(newlist);

	getline(cin, userInput);

	while (1)
	{
		if (userInput == "exit")
		{
			cout << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << "                             프로그램이 종료됩니다. 감사합니다.                             " << endl
				 << "------------------------------------------------------------------------------------------" << endl;
			return 0;
		}
		else if (userInput == "help")
		{
			cout << endl;
			cout << "------------------------------------------------------------------------------------------" << endl;
			cout << "                                      1번 기능: 번역기                                     " << endl;
			cout << "                       입력해주신 문장을 원하는 언어로 번역해 드립니다.                       " << endl
				 << endl;
			cout << "                                      2번 기능: 단어 검색                               " << endl;
			cout << "                       궁금한 단어의 영/한, 한/영 번역을 도와드립니다.                 " << endl
				 << endl;
			cout << "                                      3번 기능: 단어장 확인                           " << endl;
			cout << "                          생성되어 있는 단어장을 확인할 수 있습니다.                   " << endl
				 << endl;
			cout << "                                      4번 기능: 퀴즈                                 " << endl;
			cout << "                         어휘 퀴즈와 이미지 퀴즈를 진행할 수 있습니다.                  " << endl
				 << endl;
			cout << "                                      5번 기능: 단어 추측 게임                        " << endl;
			cout << "                단어 간의 유사도를 통해 정답을 맞추는 게임을 진행할 수 있습니다.         " << endl;
			cout << "------------------------------------------------------------------------------------------" << endl;
			cout << endl;
		}
		else if (userInput == "1") // 번역
		{
			cout << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << "                                번역에 오신 것을 환영합니다.                                " << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << endl;
			cout << "1. 한국어->다른 언어 번역 2. 다른 언어->한국어 번역 중 원하는 기능을 숫자로 입력해 주세요." << endl
				 << endl;
			cout << "사용자: ";
			getline(cin, userInput);

			if (userInput == "1")
			{
				cout << endl
					 << "번역을 원하시는 문장을 입력해주세요.(한국어)" << endl;
				cout << "사용자: ";
				getline(cin, userInput);
				Sentence origin = Sentence(userInput);
				cout << endl
					 << "번역을 원하시는 언어를 입력해주세요." << endl;
				cout << "사용자: ";
				getline(cin, userInput);

				Json request;
				request["model"] = "gpt-3.5-turbo-1106";
				request["messages"][0]["role"] = "system";
				request["messages"][0]["content"] = "You are a translator model which translates a korean sentence input into given language, and give only the result without anything else";
				request["messages"][1]["role"] = "user";
				request["messages"][1]["content"] = "translate" + origin.getoriginalText() + "into" + userInput + "language without its pronounciation";
				request["temperature"] = 0;

				auto chat = openai::chat().create(request);

				string translatedText = chat["choices"][0]["message"]["content"].dump();
				translatedText = translatedText.substr(1, translatedText.size() - 2);
				cout << endl
					 << "한국어에서 " << userInput << "(으)로의 번역입니다." << endl
					 << "번역 결과는 다음과 같습니다: " << translatedText << endl;
				origin.add_engtrans(translatedText);
			}
			else if (userInput == "2")
			{
				cout << endl
					 << "번역을 원하시는 언어를 입력해주세요." << endl;
				cout << "사용자: ";
				getline(cin, userInput);
				string lang = userInput;
				cout << endl
					 << "번역을 원하시는 문장을 입력해주세요(" << userInput << ")." << endl;
				cout << "사용자: ";
				getline(cin, userInput);
				Sentence origin = Sentence(userInput);

				Json request;
				request["model"] = "gpt-3.5-turbo-1106";
				request["messages"][0]["role"] = "system";
				request["messages"][0]["content"] = "You are a translator model which translates a sentence input of given language into korean, and give only the result without anything else";
				request["messages"][1]["role"] = "user";
				request["messages"][1]["content"] = "translate" + origin.getoriginalText() + "into korean";
				request["temperature"] = 0;

				auto chat = openai::chat().create(request);

				string translatedText = chat["choices"][0]["message"]["content"].dump();
				translatedText = translatedText.substr(1, translatedText.size() - 2);
				cout << endl
					 << lang << "에서 한국어로의 번역입니다." << endl
					 << "번역 결과는 다음과 같습니다: " << translatedText << endl;
				origin.add_kortrans(translatedText);
			}
		}
		else if (userInput == "2") // 단어 검색
		{
			cout << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << "                              단어 검색에 오신 것을 환영합니다.                             " << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << endl;
			cout << "1. 한국어 -> 영어 2. 영어 -> 한국어 중 원하는 기능을 숫자로 입력해 주세요." << endl
				 << endl;
			cout << "사용자: ";
			getline(cin, userInput);

			if (userInput == "1") // 한국어 -> 영어 단어
			{
				cout << endl
					 << "검색을 원하는 한국어 단어를 입력해 주세요." << endl;
				cout << "예시) 반도체" << endl;
				cout << "사용자: ";
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
				cout << "예문: " << examsent << endl;

				// 단어장에 추가
				cout << endl
					 << "해당 단어를 단어장에 추가하시겠습니까? [Y/N]" << endl;
				cout << "사용자: ";
				getline(cin, userInput);
				if (userInput == "Y" || userInput == "y")
				{
					string w_name;
					if (w_lists.empty())
					{
						cout << "현재 생성된 단어장이 없습니다. 새롭게 생성할 단어장의 이름을 입력해 주세요." << endl;
						cout << "사용자: ";
						getline(cin, w_name);
						wordlist newwlist;
						newwlist.setname(w_name);
						time_t curtime = time(NULL);
						struct tm *t = localtime(&curtime);
						wordentry newentry(newword, t);
						newwlist.addentry(newentry, 1);
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
						cout << "사용자: ";
						getline(cin, w_name);

						int flag = 0;
						for (auto it = w_lists.begin(); it != w_lists.end(); it++)
						{
							if ((*it).getname() == w_name)
							{
								time_t curtime = time(NULL);
								struct tm *t = localtime(&curtime);
								wordentry newentry(newword, t);
								(*it).addentry(newentry, 1);
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
							newwlist.addentry(newentry, 1);
							w_lists.push_back(newwlist);
						}
					}
				}
			}
			else if (userInput == "2") // 영어 -> 한국어 단어
			{
				cout << endl
					 << "검색을 원하는 영어 단어를 입력해 주세요." << endl;
				cout << "예시) semiconductor" << endl;
				cout << "사용자: ";
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
				cout << "사용자: ";
				getline(cin, userInput);
				if (userInput == "Y" || userInput == "y")
				{
					string w_name;
					if (w_lists.empty())
					{
						cout << "현재 생성된 단어장이 없습니다. 새롭게 생성할 단어장의 이름을 입력해 주세요." << endl;
						cout << "사용자: ";
						getline(cin, w_name);
						wordlist newwlist;
						newwlist.setname(w_name);
						time_t curtime = time(NULL);
						struct tm *t = localtime(&curtime);
						wordentry newentry(newword, t);
						newwlist.addentry(newentry, 1);
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
						cout << "사용자: ";
						getline(cin, w_name);

						int flag = 0;
						for (auto it = w_lists.begin(); it != w_lists.end(); it++)
						{
							if ((*it).getname() == w_name)
							{
								time_t curtime = time(NULL);
								struct tm *t = localtime(&curtime);
								wordentry newentry(newword, t);
								(*it).addentry(newentry, 1);
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
							newwlist.addentry(newentry, 1);
							w_lists.push_back(newwlist);
						}
					}
				}
			}
		}
		else if (userInput == "3") // 단어장 확인
		{
			cout << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << "                              단어 검색에 오신 것을 환영합니다.                             " << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << endl;

			if (w_lists.empty()) // 단어장이 없는 경우
			{
				cout << "현재 생성된 단어장이 없습니다. 단어 검색을 통해 단어장을 생성해 주세요." << endl;
			}
			else // 단어장이 존재하는 경우
			{
				cout << "확인할 단어장의 이름을 입력해 주세요. 다음은 현재 단어장 목록입니다" << endl;
				int i = 1;
				for (auto it = w_lists.begin(); it != w_lists.end(); it++)
				{
					cout << i << ". " << (*it).getname() << " ";
					i++;
				}
				cout << endl
					 << endl;
				cout << "사용자 : ";
				string w_name;
				getline(cin, w_name);

				wordlist curlist;
				// 존재하는 단어장 이름인지 확인
				int flag = 0;
				for (auto it = w_lists.begin(); it != w_lists.end(); it++)
				{
					if ((*it).getname() == w_name)
					{
						flag = 1;
						curlist = (*it);
						break;
					}
				}
				if (flag == 0) // 잘못된 입력
				{
					do
					{
						cout << endl
							 << "해당 이름(" << w_name << ")의 단어장이 존재하지 않습니다.다시 입력해 주세요." << endl;
						cout << "사용자 : ";
						getline(cin, w_name);
						for (auto it = w_lists.begin(); it != w_lists.end(); it++)
						{
							if ((*it).getname() == w_name)
							{
								flag = 1;
								curlist = (*it);
								break;
							}
						}
					} while (flag == 0);
				}
				curlist.display();
			}
		}
		else if (userInput == "4") // 퀴즈
		{
			string userNum;
			string userWord;
			string userCount;
			cout << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << "                                퀴즈에 오신 것을 환영합니다.                                " << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << endl;
			cout << "1. 어휘 퀴즈 2. 이미지 퀴즈 중 원하는 퀴즈를 숫자로 입력해 주세요." << endl;
			cout << "사용자: ";
			getline(cin, userNum);

			if (userNum == "1")
			{
				int grade = 0;
				cout << endl
					 << "어휘 퀴즈을 시작하겠습니다! 단어의 뜻을 보고 해당하는 영어 단어를 적어주세요." << endl
					 << "틀릴 경우 단어가 자동적으로 오답노트 단어장에 들어가게 됩니다." << endl;
				cout << "문제가 출제될 단어장을 골라 이름을 입력해 주세요." << endl
					 << endl;
				cout << "다음은 현재 단어장 목록입니다." << endl;
				int i = 1;
				for (auto it = w_lists.begin(); it != w_lists.end(); it++)
				{
					cout << i << ". " << (*it).getname() << "     ";
					i++;
				}
				cout << endl;
				cout << "사용자: ";
				getline(cin, userWord);
				wordlist newlist;

				// 존재하는 단어장 이름인지 확인
				int flag = 0;
				for (auto it = w_lists.begin(); it != w_lists.end(); it++)
				{
					if ((*it).getname() == userWord)
					{
						flag = 1;
						newlist = (*it);
						if (newlist.getsize() == 0)
						{
							flag = 2;
						}
						break;
					}
				}
				if (flag == 0 || flag == 2) // 잘못된 입력 or 빈 단어장
				{
					do
					{
						if (flag == 0)
						{
							cout << endl
								 << "해당 이름의 단어장이 존재하지 않습니다. 다시 입력해 주세요." << endl;
							cout << "사용자: ";
							getline(cin, userWord);
							for (auto it = w_lists.begin(); it != w_lists.end(); it++)
							{
								if ((*it).getname() == userWord)
								{
									flag = 1;
									newlist = (*it);
									if (newlist.getsize() == 0)
									{
										flag = 2;
									}
									break;
								}
							}
						}
						else if (flag == 2)
						{
							cout << endl
								 << "해당 단어장은 비어 있습니다. 다시 입력해 주세요." << endl;
							cout << "사용자: ";
							getline(cin, userWord);
							flag = 0;
							for (auto it = w_lists.begin(); it != w_lists.end(); it++)
							{
								if ((*it).getname() == userWord)
								{
									flag = 1;
									newlist = (*it);
									if (newlist.getsize() == 0)
									{
										flag = 2;
									}
									break;
								}
							}
						}
					} while (flag == 0 || flag == 2);
				}

				cout << endl
					 << "1부터 " << newlist.getsize() << "중 원하시는 문제 수를 입력해주세요" << endl;
				cout << "사용자: ";
				getline(cin, userCount);
				cout << endl;
				int *checker;
				checker = (int *)calloc(newlist.getsize(), sizeof(int));

				for (int i = 1; i <= stoi(userCount); i++)
				{
					cout << "어휘퀴즈 " << i << "번 문제" << endl;

					int num = rand() % newlist.getsize();
					while (checker[num] == 1)
					{
						num = rand() % newlist.getsize();
					}
					checker[num] = 1;
					cout << i << "번째 단어의 뜻: " << newlist.getentry(num).getkor() << endl;
					cout << "사용자: ";
					string userAnswer;
					getline(cin, userAnswer);
					if (userAnswer == newlist.getentry(num).geteng())
					{
						cout << "정답입니다!" << endl;
						grade++;
					}
					else
					{
						cout << "오답입니다!" << endl;
						cout << "정답은 " << newlist.getentry(num).geteng() << "입니다." << endl;
						for (auto it = w_lists.begin(); it != w_lists.end(); it++)
						{
							if ((*it).getname() == "오답노트")
							{
								time_t curtime = time(NULL);
								struct tm *t = localtime(&curtime);
								wordentry newentry(newlist.getentry(num), t);
								(*it).addentry(newentry, 0);
								break;
							}
						}
					}
					cout << "현재 점수는 " << grade << "점입니다." << endl;
					cout << endl;
				}

				cout << "******************************************" << endl;
				// cout << "최종점수는 " << grade  << "점입니다." << endl;
				cout << "총 " << userCount << "문제 중 " << grade << "문제를 맞히셨습니다. " << endl;
			}
			else if (userNum == "2")
			{
				cout << "이미지 퀴즈을 시작하겠습니다. 문제가 출제될 단어장을 골라 이름을 입력해 주세요." << endl
					 << endl;
				cout << "다음은 현재 단어장 목록입니다" << endl;
				int i = 1;
				for (auto it = w_lists.begin(); it != w_lists.end(); it++)
				{
					cout << i << ". " << (*it).getname() << "     ";
					i++;
				}
				cout << endl;
				cout << "사용자: ";
				getline(cin, userWord);
				wordlist newlist;

				// 존재하는 단어장 이름인지 확인
				int flag = 0;
				for (auto it = w_lists.begin(); it != w_lists.end(); it++)
				{
					if ((*it).getname() == userWord)
					{
						flag = 1;
						newlist = (*it);
						if (newlist.getsize() == 0)
						{
							flag = 2;
						}
						break;
					}
				}
				if (flag == 0 || flag == 2) // 잘못된 입력 or 빈 단어장
				{
					do
					{
						if (flag == 0)
						{
							cout << endl
								 << "해당 이름의 단어장이 존재하지 않습니다. 다시 입력해 주세요." << endl;
							cout << "사용자: ";
							getline(cin, userWord);
							for (auto it = w_lists.begin(); it != w_lists.end(); it++)
							{
								if ((*it).getname() == userWord)
								{
									flag = 1;
									newlist = (*it);
									if (newlist.getsize() == 0)
									{
										flag = 2;
									}
									break;
								}
							}
						}
						else if (flag == 2)
						{
							cout << endl
								 << "해당 단어장은 비어 있습니다. 다시 입력해 주세요." << endl;
							cout << "사용자: ";
							getline(cin, userWord);
							flag = 0;
							for (auto it = w_lists.begin(); it != w_lists.end(); it++)
							{
								if ((*it).getname() == userWord)
								{
									flag = 1;
									newlist = (*it);
									if (newlist.getsize() == 0)
									{
										flag = 2;
									}
									break;
								}
							}
						}
					} while (flag == 0 || flag == 2);
				}

				int num = rand() % newlist.getsize();

				Json request;
				request["model"] = "dall-e-3";
				request["prompt"] = newlist.getentry(num).geteng();

				auto image = openai::image().create(request);
				cout << "문제 url: " << image["data"][0]["url"] << endl;
				cout << "사용자: ";
				string userAnswer;
				getline(cin, userAnswer);
				if (userAnswer == newlist.getentry(num).geteng())
				{
					cout << "정답입니다!" << endl;
				}
				else
				{
					cout << "오답입니다!" << endl;
					cout << "정답은 " << newlist.getentry(num).geteng() << "입니다." << endl;
				}
			}
		}
		else if (userInput == "5") // 단어 추측 게임
		{
			cout << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << "                           단어 추측 게임에 오신 것을 환영합니다.                            " << endl
				 << "------------------------------------------------------------------------------------------" << endl
				 << endl;
			cout << "단어 추측 게임은 사용자가 단어를 하나씩 입력하며, 숨겨진 단어가 무엇인지 찾는 게임입니다." << endl;
			cout << "오답을 입력할때마다 입력한 단어와 정답 단어간의 유사도가 출력되며, " << endl
				 << "이를 바탕으로 정답 단어를 맞혀주시면 됩니다. " << endl;
			cout << "단어는 단어장에 있는 단어들 중 무작위로 출제됩니다. " << endl
				 << endl;
			cout << "게임을 시작하겠습니다. 문제가 출제될 단어장을 골라 이름을 입력해 주세요." << endl
				 << endl;
			cout << "다음은 현재 단어장 목록입니다" << endl;
			int i = 1;
			for (auto it = w_lists.begin(); it != w_lists.end(); it++)
			{
				cout << i << ". " << (*it).getname() << "     ";
				i++;
			}
			cout << endl;
			cout << "사용자: ";
			string w_name;
			getline(cin, w_name);
			wordlist curlist;

			// 존재하는 단어장 이름인지 확인
			int flag = 0;
			for (auto it = w_lists.begin(); it != w_lists.end(); it++)
			{
				if ((*it).getname() == w_name)
				{
					flag = 1;
					curlist = (*it);
					if (curlist.getsize() == 0)
					{
						flag = 2;
					}
					break;
				}
			}
			if (flag == 0 || flag == 2) // 잘못된 입력 or 빈 단어장
			{
				do
				{
					if (flag == 0)
					{
						cout << endl
							 << "해당 이름의 단어장이 존재하지 않습니다. 다시 입력해 주세요." << endl;
						cout << "사용자: ";
						getline(cin, w_name);
						for (auto it = w_lists.begin(); it != w_lists.end(); it++)
						{
							if ((*it).getname() == w_name)
							{
								flag = 1;
								curlist = (*it);
								if (curlist.getsize() == 0)
								{
									flag = 2;
								}
								break;
							}
						}
					}
					else if (flag == 2)
					{
						cout << endl
							 << "해당 단어장은 비어 있습니다. 다시 입력해 주세요." << endl;
						cout << "사용자: ";
						getline(cin, w_name);
						flag = 0;
						for (auto it = w_lists.begin(); it != w_lists.end(); it++)
						{
							if ((*it).getname() == w_name)
							{
								flag = 1;
								curlist = (*it);
								if (curlist.getsize() == 0)
								{
									flag = 2;
								}
								break;
							}
						}
					}
				} while (flag == 0 || flag == 2);
			}

			int num = rand() % curlist.getsize();
			wordentry curentry = curlist.getentry(num);

			string ans = curentry.geteng();

			vector<double> ans_embed;
			auto res = openai::embedding().create({{"model", "text-embedding-ada-002"},
												   {"input", ans}});

			for (const auto &val : res["data"][0]["embedding"])
			{
				ans_embed.push_back(val.get<double>());
			}
			cout << endl
				 << "정답 단어가 설정되었습니다. 게임을 시작하겠습니다. 단어를 입력해 주세요." << endl
				 << endl;
			int count = 0;
			string wordinp;
			while (1)
			{
				cout << "사용자: ";
				getline(cin, wordinp);
				count++;
				if (wordinp == ans) // 정답
				{
					cout << "정답입니다! 축하드립니다." << endl;
					cout << count << "회만에 정답을 맞추셨습니다." << endl;
					break;
				}
				else // 오답
				{
					vector<double> input_embed;
					auto tmp = openai::embedding().create({{"model", "text-embedding-ada-002"},
														   {"input", wordinp}});

					for (const auto &val : tmp["data"][0]["embedding"])
					{
						input_embed.push_back(val.get<double>());
					}
					// 유사도 계산
					double similarity = cossim(input_embed, ans_embed);
					
					// 퍼센트 출력을 위해
					similarity *= 100;
					//scale result
					similarity = (similarity - 70) * (100/25);
					cout << "오답입니다. 입력하신 단어와 정답의 유사도는 ";
					cout << fixed;
					cout.precision(2);
					cout << similarity;
					cout << "% 입니다. " << endl
						 << endl;
				}
			}
		}
		else
		{
			cout << "잘못된 입력입니다. 다시 입력해 주세요." << endl;
		}

		cout << endl
			 << "                       어떤 작업을 하고 싶으신가요? 숫자로 입력해 주세요.                       " << endl
			 << "                           1. 만능 번역기 2. 단어 검색 3. 단어장 확인                          " << endl
			 << "                                    4. 퀴즈 5. 단어추측게임                                   " << endl
			 << "                   도움이 필요하시면 help를, 종료하시려면 exit을 입력해 주세요.                  " << endl;
		cout << endl;
		cout << "사용자: ";
		getline(cin, userInput);
	}

	return 0;
}
