#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

class word
{
public:
	word(string _kor) : kor(_kor) {};
	string getkor() { return kor; }
	string geteng() { return eng; }
	void addeng(string _eng)
	{
		eng = _eng;
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
private:
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


//단어장 클래스
class wordlist
{
public :
	void addword(word _word)
	{
		wlst.push_back(_word);
		size++;
	};
	void removeword(word _word)
	{
		auto it = find(wlst.begin(), wlst.end(), _word);
		if (it == wlst.end())
		{
			cout << "해당 단어가 단어장에 없습니다" << endl;
		}
		else
		{
			wlst.erase(remove(wlst.begin(), wlst.end(), _word), wlst.end());
			size--;
		}
	}
	

private :
	vector<word> wlst;
	unsigned size;
};


