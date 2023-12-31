#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iomanip>

using namespace std;

class word
{
public:
	word(){};
	word(string _kor) : kor(_kor){};
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
		cout << kor;
	};
	void displayeng()
	{
		cout << eng;
	};
	void display()
	{
		cout << "한글 : " << kor << endl;
		cout << "영어 : " << eng << endl;
	}
	bool operator==(const word &w1)
	{
		return ((*this).kor == w1.kor);
	}

protected:
	string kor;
	string eng;
};

class Sentence
{
public:
	Sentence(const string &original) : originalText(original){};
	string getoriginalText() const
	{
		return originalText;
	}
	string gettranslatedText() const
	{
		return translatedText;
	}
	void add_trans(const string &trans)
	{
		translatedText = trans;
	}

private:
	string originalText;
	string translatedText;
};

// 단어장 entry
class wordentry : public word
{
public:
	wordentry(word _word, struct tm *_add_time)
	{
		kor = _word.getkor();
		eng = _word.geteng();
		add_time = _add_time;
	};
	struct tm *gettime() { return add_time; }
	void display()
	{
		cout << "한글 : ";
		displaykor();
		cout << endl;
		cout << "영어 : ";
		displayeng();
		cout << endl;
		cout << "저장 날짜 : ";
		cout << add_time->tm_mon + 1 << "/" << add_time->tm_mday << endl;
	}
private:
	struct tm *add_time;
};

// 단어장 클래스
class wordlist
{
public:
	wordlist()
	{ size = 0;};
	void setname(string _name)
	{ name = _name;}
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
		if (a)
		{
			cout << endl << "단어장 " << name << "에 단어 " << _entry.geteng() << "를 추가했습니다. " << endl;
		}
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
		cout << endl
			 << "단어장 " << name << "의 단어 목록" << endl;
		cout << "--------------------" << endl;
		for (auto it = wlst.begin(); it != wlst.end(); it++)
		{
			(*it).display();
			cout << "--------------------" << endl;
		}
	}
	wordentry getentry(int num)
	{return wlst.at(num);}
private:
	string name;
	vector<wordentry> wlst;
	unsigned size;
};

double cossim(const vector<double> &vec1, const vector<double> &vec2)
{
	double dotproduct = 0.0;
	double norm1 = 0.0, norm2 = 0.0;

	for (size_t i = 0; i < vec1.size(); i++)
	{
		dotproduct += vec1[i] * vec2[i];
		norm1 += vec1[i] * vec1[i];
		norm2 += vec2[i] * vec2[i];
	}

	if (norm1 == 0.0 || norm2 == 0.0) //floating point exception 방지
	{
		return 0.0;
	}

	return (dotproduct / (sqrt(norm1) * sqrt(norm2)));
}
