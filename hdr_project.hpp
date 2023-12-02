#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

class word
{
public:
	word(){};
	word(string _kor) : kor(_kor) {};
	word(string _strkr, string _streng)
	{
		kor = _strkr;
		eng = _streng;
	};
	string getkor() { return kor; }
	string geteng() { return eng; }
	void addeng(string _eng)
	{
		eng = _eng;
	}
	void addkor(string _kor)
	{
		kor = _kor;
	}
	void displaykor()
	{
		cout << kor << endl;
	};
	void displayeng()
	{
		cout << eng << endl;
	};
	bool operator == (const word &w1)
	{
		return ((*this).kor == w1.kor);
	}
protected:
	string kor;
	string eng;
};

//유의어 클래스
class synonym : public word
{
public:
	synonym(string _kor) : word(_kor) {};

private:
	vector<string> synonyms;
};

//반의어 클래스
class antonym : public word
{
public:
	antonym(string _kor) : word(_kor) {};

private:
	vector<string> antonyms;
};

class Sentence
{
public:
	Sentence(const string& original): originalText(original){};
	string getoriginalText() const {
		return originalText;
	}
	string gettranslatedText() const {
		return translatedText;
	}
	void add_kortrans(const string& trans){
		translatedText = trans;
	}
	void add_engtrans(const string& trans){
		translatedText = trans;
	}
private:
	string originalText;
	string translatedText;
};

//단어장 entry
class wordentry : public word
{
public:
	wordentry(word _word, struct tm* _add_time){ 
		kor=_word.getkor();
		eng=_word.geteng();
		add_time = _add_time;
		};
	struct tm* gettime(){return add_time;}
private:
	struct tm* add_time;
};

//단어장 클래스
class wordlist
{
public :
	wordlist(){};
	void setname(string _name)
	{
		name = _name;
	}
	string getname() { return name; }
	int getsize() { return size; }
	void addentry(wordentry _entry, int a)
	{
		auto it = find(wlst.begin(), wlst.end(), _entry);
		if (it != wlst.end())
		{
			cout << "해당 단어가 이미 단어장에 있습니다." << endl;
			return;
		}

		wlst.push_back(_entry);
		size++;
		if(a)
			cout << "단어장에 추가 : " << _entry.getkor() << endl;
	};
	void removeentry(wordentry _entry)
	{
		auto it = find(wlst.begin(), wlst.end(), _entry);
		if (it == wlst.end())
		{
			cout << "해당 단어가 단어장에 없습니다" << endl;
		}
		else
		{
			wlst.erase(remove(wlst.begin(), wlst.end(), _entry), wlst.end());
			size--;
		}
	}
	void display()
	{
		cout << endl << "단어장 " << name << "의 단어 목록" << endl;
		for (auto it = wlst.begin(); it != wlst.end(); it++)
		{
			cout << "한글 : " << (*it).getkor() << " 영어 : " << (*it).geteng() << " 저장 날짜 : " << (*it).gettime()->tm_mon + 1 << "/" <<(*it).gettime()->tm_mday << endl;
		}
	}
	wordentry getentry(int num)
	{
		return wlst.at(num);
	}
private :
	string name;
	vector<wordentry> wlst;
	unsigned size;
};


