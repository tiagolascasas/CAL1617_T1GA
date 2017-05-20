#include <cstdlib>
#include <climits>
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include "StringFunctions.h"
using namespace std;

string* randomStr(int size)
{
	string* s = new string(size, '\0');
	for (int i = 0; i < size; i++)
		s->at(i) = (char)(33 + rand() % 94);
	return s;
}

void measureTimeExact1()
{
	vector<long int> time;
	vector<int> size;

	for (long long i = 10000; i < INT_MAX; i *= 10)
	{
		string* s = randomStr(i);
		string* t = randomStr(100);
		long int start = GetTickCount();
		kmpStringMatching(*s, *t, true);
		long int end = GetTickCount();
		delete s;
		delete t;
		time.push_back(end - start);
		size.push_back(i);
	}
	cout << "Exact search: variable text size, fixed pattern size:\n[";
	for (int i = 0; i < time.size(); i++)
		cout << time.at(i) << ", ";
	cout << "],\n[";
	for (int i = 0; i < size.size(); i++)
		cout << size.at(i) << ", ";
	cout << "]\n";
}

void measureTimeExact2()
{
	vector<long int> time;
	vector<int> size;

	for (long long i = 10000; i < INT_MAX; i *= 10)
	{
		string* s = randomStr(1000);
		string* t = randomStr(i);
		long int start = GetTickCount();
		kmpStringMatching(*s, *t, true);
		long int end = GetTickCount();
		delete s;
		delete t;
		time.push_back(end - start);
		size.push_back(i);
	}
	cout << "Exact search: variable pattern size, fixed text size:\n[";
	for (int i = 0; i < time.size(); i++)
		cout << time.at(i) << ", ";
	cout << "],\n[";
	for (int i = 0; i < size.size(); i++)
		cout << size.at(i) << ", ";
	cout << "]\n";
}

void measureTimeApprox1()
{
	vector<long int> time;
	vector<int> size;

	for (long long i = 100; i < INT_MAX; i *= 10)
	{
		string* s = randomStr(i);
		string* t = randomStr(i);
		long int start = GetTickCount();
		levenshtein_distance(*s, *t, true);
		long int end = GetTickCount();
		delete s;
		delete t;
		time.push_back(end - start);
		size.push_back(i);
	}
	cout << "Approx search:\n[";
	for (int i = 0; i < time.size(); i++)
		cout << time.at(i) << ", ";
	cout << "],\n[";
	for (int i = 0; i < size.size(); i++)
		cout << size.at(i) << ", ";
	cout << "]\n";
}

int main()
{
	srand(time(NULL));
//	measureTimeExact1();
	measureTimeExact2();
//	measureTimeApprox1();
}
