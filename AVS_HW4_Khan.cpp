#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <omp.h>

using namespace std;

static string result = "";
//Получение количества строк в файле
static int GetNumOfFileLines(string path) {
	ifstream fin;
	fin.open(path);
	string line;
	int length = 0;

	while (getline(fin, line))
		++length;
	return length;
}
//Получение даных с файла
static vector<string> GetDataFromFile(string path)
{
	ifstream fin;
	fin.open(path);
	vector<string> result;
	int numOfLines = GetNumOfFileLines(path);

	if (!fin.is_open())
	{
		throw invalid_argument("Error of opening the file!");
	}
	else
	{
		while (!fin.eof())
		{
			string str = "";
			getline(fin, str);
			if (str == "")
			{
				continue;
			}
			result.push_back(str);
		}
	}
	return result;
}
//Шифровать текст
static string EncodeString(string text) {
	string encoded_text = "";
	for (int i = 0; i < text.length(); i++)
	{
		encoded_text += to_string((int)text[i]);
	}
	return encoded_text;
}
//Расшифровать текст
static string DecryptString(string encoded_text) {
	string decrypted_text = "";
	for (int i = 0; i < encoded_text.length() - 1; i++)
	{
		if (encoded_text[i] == '1')
		{
			char arr[] = { encoded_text[i] , encoded_text[i + 1], encoded_text[i + 2] };
			char c = stoi(arr);
			decrypted_text += c;
			i += 2;
			continue;
		}
		char arr[] = { encoded_text[i] , encoded_text[i + 1] };
		try
		{
			char c = stoi(arr);
			decrypted_text += c;
			i++;
		}
		catch (const std::exception&)
		{
			cout << arr << endl;
		}

	}
	return decrypted_text;
}
//Расшифрование текста из файла
static void GetDecryptedData(vector<string> lines, int numberofThreads)
{
	omp_set_num_threads(numberofThreads);
#pragma omp parallel
	{
#pragma omp for 
		for (int i = 0; i < lines.size(); i++)
		{
#pragma omp critical
			{
				cout << "Thread(" << omp_get_thread_num() << ") Index of iteration:" << to_string(i) << endl;
				string decrypted_text = DecryptString(lines[i]);
				result += decrypted_text + "\n";
			}

		}
	}
}
//Получение результатов дешифрования
static void GetResults(vector<string> packs) {
	GetDecryptedData(packs, ceil(packs.size() / 3));
	cout << result << endl;
}
int main()
{
	try
	{
		string path = "input.txt";
		vector<string> packs = GetDataFromFile(path);

		GetResults(packs);
	}
	catch (const invalid_argument& e)
	{
		cout << "input.txt file is not found" << endl;
		return 1;
	}
	catch (const exception& e)
	{
		cout << "Incorrect encoded text" << endl;
		return 1;
	}
}

