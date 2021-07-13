#pragma once
#include <iostream>
#include <string>
#include "Builder/MinimalSubsequentialTransducerBuilder.h"
#include <fstream>
#include <sstream> 
#include <ctime>
#include <algorithm>
using namespace std;

char separator = ' ';

void tokenize(std::string& str, const char delim,
	std::vector<std::string> &out)
{

	for (int i = 0; i < str.size(); i++)
	{

		if (str[i] == delim)
		{
			str[i] = '\0';
			string word1(str.c_str());
			string word2(&str[i + 1]);
			out.push_back(word1);
			out.push_back(word2);
		}
	}
}

SubsequentialTransducer buildByFile(const string& fileName)
{
	cout << "Building MST from sorted file!" << endl;
	ifstream file(fileName);

	if (!file.is_open())
	{
		cout << "ERROR: No such file!" << endl;
	}
	vector<pair<string, string>> result;

	size_t maxLen = 0;
	string str;
	cout << "Reading file...!" << endl;
	while (getline(file, str))
	{
		std::vector<std::string> out;
		tokenize(str, separator, out);
		result.push_back(make_pair(out[0], out[1]));
		maxLen = max(maxLen, out[0].size());

	}
	cout << "Reading file done!" << endl;
	file.close();

	cout << "Building transducer..." << endl;
	SubsequentialTransducer transuser = MinTrans(result, maxLen + 1);
	return std::move(transuser);

}

void insertFromFile(const string& fileName, SubsequentialTransducer& tr)
{
	cout << "Inserting words from file." << endl;
	ifstream file(fileName);

	if (!file.is_open())
	{
		cout << "ERROR: No such file!" << endl;
	}
	vector<pair<string, string>> result;

	string str;
	cout << "Reading file...!" << endl;
	while (getline(file, str))
	{
		std::vector<std::string> out;
		tokenize(str, separator, out);
		result.push_back(make_pair(out[0], out[1]));

	}
	//reverse(result.begin(), result.end());

	cout << "Reading file done!" << endl;
	file.close();
	auto begin = clock();
	double totalTime = 0;
	cout << "Inserting words..." << endl;
	for (int i = 0; i < result.size(); i++)
	{
		if (i > 0 && i % 10000 == 0)
		{

			auto end = clock();
			auto elapsed = double(end - begin);
			cout << "Inserted words: " << i << endl;
			cout << "Time:         " << (elapsed / 1000.0) << " seconds" << endl << endl;
			totalTime += elapsed / 1000.0;
			//cout << "Hash collisions: " <<  transduser.collisions << endl;
			begin = clock();
		}
		insertWord(result[i].first, atoi(result[i].second.c_str()), tr);
	}
	cout << "Inserting words.done!" << endl;
	cout << "Total time: " << totalTime << endl;

}

void removeFromFile(const string& fileName, SubsequentialTransducer& tr)
{
	cout << "Removing words from file." << endl;
	ifstream file(fileName);

	if (!file.is_open())
	{
		cout << "ERROR: No such file!" << endl;
	}
	vector<string> result;

	string str;
	cout << "Reading file...!" << endl;
	while (getline(file, str))
	{
		std::vector<std::string> out;
		tokenize(str, separator, out);
		result.push_back(out[0]);
	}

	cout << "Reading file done!" << endl;
	file.close();
	auto begin = clock();
	double totalTime = 0;
	cout << "Removing words..." << endl;
	for (int i = 0; i < result.size(); i++)
	{
		if (i > 0 && i % 10000 == 0)
		{

			auto end = clock();
			auto elapsed = double(end - begin);
			cout << "Removed words: " << i << endl;
			cout << "Time:         " << (elapsed / 1000.0) << " seconds" << endl << endl;
			totalTime += elapsed / 1000.0;
			//cout << "Hash collisions: " <<  transduser.collisions << endl;
			begin = clock();
		}

		removeWord(result[i], tr);
	}
	auto end = clock();
	auto elapsed = double(end - begin);
	totalTime += elapsed / 1000.0;

	cout << "Removing words done!" << endl;
	cout << "Total time: " << totalTime << endl;

}