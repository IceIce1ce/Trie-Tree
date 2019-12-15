#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<sstream>
using namespace std;
#define ALPHABET 26
#define char_int(c) ((int)c - (int)'a')
#define int_to_char(c) ((char)c + (char)'a') 

struct Trie
{
	Trie* child[ALPHABET];
	bool isEndOfWord;
};

struct Dictionary
{
	string wordList;	
};

Trie* createNode()
{
	Trie* newNode = new Trie();
	newNode->isEndOfWord = false;
	for(int i = 0; i < ALPHABET; i++) newNode->child[i] = NULL;
	return newNode;
}

void insert(Trie* root, char* key)
{
	Trie* pChild = root;
	for(int i = 0; i < strlen(key); i++)
	{
		int index = char_int(key[i]);
		if(pChild->child[index] == nullptr) pChild->child[index] = createNode();
		pChild = pChild->child[index];
	}
	pChild->isEndOfWord = true;
}

void search(Trie* root, bool table[], string key)
{
	ofstream fout;
	fout.open("output.txt",ios::app);
	if (!fout.is_open()) return;
	if(root->isEndOfWord == true) fout << key << endl;
	for(int i = 0; i < ALPHABET; i++)
	{
		if(table[i] == true && root->child[i] != NULL)
		{
			char c = int_to_char(i);
			search(root->child[i], table, key + c);
		}
	}
	fout.close();
}

void print(vector<char> a, Trie* root, int n, string output)
{
	ofstream fout;
	fout.open(output);
	if (!fout.is_open()) return;
	bool table[ALPHABET];
	for(int i = 0; i < n; i++) table[char_int(a[i])] = true;
	Trie* pChild = root;
	string key = "";
	string s;
	for(int i = 0; i < ALPHABET; i++)
	{
		if(table[i] == true && pChild->child[i])
		{
			key += (char)int_to_char(i);
			search(pChild->child[i], table, key);
			key = "";
		}
	}
	fout.close();
}

istream& operator >> (istream& is, Dictionary& dict)
{
	return is >> dict.wordList;
}

void sortDictionaryFile() //if dic.txt not sort, this function will sort them
{
	ifstream fin;
	fin.open("Dic.txt");
	if(!fin.is_open())
	{
		cerr << "File can't open";
		exit(0);
	}
	vector<Dictionary> dictionary;
	Dictionary dic;
	while(fin >> dic) dictionary.push_back(dic);
	sort(dictionary.begin(), dictionary.end(), [](const Dictionary& d1, const Dictionary& d2) { return d1.wordList < d2.wordList; });
	ofstream fout;
	fout.open("Dic_sorted.txt");
	if(!fout.is_open())
	{
		cerr << "File not found";
		exit(0);
	}
	for(auto i = 0; i < dictionary.size(); i++) fout << dictionary[i].wordList << endl;
	fin.close();
	fout.close();
}

void File_Process(string input, string output)
{
	ifstream fin;
	ofstream fout;
	fin.open(input);
	fout.open(output);
	if (!fin.is_open() || !fout.is_open()) return;
	vector<char> a;
	char alpha;
	fin >> alpha;
	while (!fin.eof())	
	{
		if (alpha != ' ') a.push_back(alpha);
		fin >> alpha;
	}
	ifstream fin1;
	fin1.open("Dic.txt");
	if (!fin1.is_open()) return;
	vector<string> dict;
	string data;
	while (!fin1.eof())
	{
		fin1 >> data;
		dict.push_back(data);
	}
	vector<char*> b;
	for (int i = 0; i < dict.size(); i++) b.push_back(_strdup(dict[i].c_str()));
	Trie* root = createNode();
	int n = dict.size();
	for (int i = 0; i < n; i++) insert(root, b[i]);
	int N = a.size();
	print(a, root, N, "output.txt");
	fin1.close();
	fin.close();
	fout.close();
}

string reverseNumber(unsigned num) //reverse number keep leading zero
{
	stringstream writer;
	for(; num > 0; num /= 10) writer << num % 10;
	return writer.str();
}

void countWordFound() //count all word found in output.txt 
{
	ifstream fin;
	ofstream fout;
	fin.open("output.txt");
	fout.open("output.txt", ios::app);
	if(!fin.is_open() || !fout.is_open()) return;
	string word;
	int count = 0;
	while(fin >> word) count++;
	fout << reverseNumber(count);
	fin.close();
	fout.close();
}

void reverseFile() //reverse all word in output.txt
{
	const char* file_name = "output.txt";
	string str;
	ifstream fin(file_name);
	char c;
	while(fin.get(c)) str += c;
	reverse(str.begin(), str.end());
	ofstream fout(file_name);
	fout << str << endl;
	fin.close();
	fout.close();
}

void reverseFile1() //reverse all word in output.txt except first line
{
	const char* file_name = "output.txt";
	string str;
	ifstream fin(file_name);
	char c;
	while(fin.get(c)) str += c;
	reverse(str.begin() + 1, str.end());
	ofstream fout(file_name);
	fout << str;
	fin.close();
	fout.close();
}

void reverseFile2() //reverse all word in output.txt except first and two line
{
	const char* file_name = "output.txt";
	string str;
	ifstream fin(file_name);
	char c;
	while(fin.get(c)) str += c;
	reverse(str.begin() + 2, str.end());
	ofstream fout(file_name);
	fout << str;
	fin.close();
	fout.close();
}

int main()
{
	File_Process("input.txt", "output.txt");
	countWordFound(); //count word
	reverseFile(); //reverse all word in file
	reverseFile1(); //reverse all word in file expect first line
	reverseFile1(); //reverse last line to first line expect other all word
	reverseFile2(); //reverse all word back to original expect first line
	return 0;
}