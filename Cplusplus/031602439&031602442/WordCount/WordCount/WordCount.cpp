#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <queue>

using namespace std;

char s[5000000];
int charreallynum = 0;
int charnum = 0;
int linenum = 0;
int wordnum = 0;
map <string, int> w_c; //存放单词和单词出现次数
map <string, int>::iterator w_c_iter;
vector<pair<string, int> > w_c2; //将map放入vector中排序
vector<pair<string, int> >::iterator w_c2_iter;

map <string, int> cz_c; //存放词组和词组出现次数
map <string, int>::iterator cz_c_iter;
vector<pair<string, int> > cz_c2; //将map放入vector中排序
vector<pair<string, int> >::iterator cz_c2_iter;

queue<string> cz_q1; //用于存放词组中的单词
queue<string> cz_q2; //用于输出满足条件词组的单词
string pop[10000];

bool Comp(const pair<string, int> &a, const pair<string, int> &b)
{
	if (a.second == b.second)
		return a.first < b.first;
	else
		return a.second > b.second;
}
int readtxt(char *a)  //读取文件并判断字符数
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, a, "r");
	while (fgetc(fp) != EOF)
	{
		charnum++;
	}
	charreallynum = charnum;
	rewind(fp); //将指针放回文本开头
	fread(s, charnum, 1, fp);
	return 1;
}
int readtxt2(char *a)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, a, "r");
	rewind(fp); //将指针放回文本开头
	fread(s, charreallynum, 1, fp);
	return 1;
}
void divide_n(int w)
{
	size_t length;
	string wordstring;
	char wordchar[999];
	int w2 = 1;
	int i, j, k = 0;
	int pos = 0;
	const char *sep = "./;'[] \\<>?:\"{}|`~!@#$%^&*()_+-=\n"; //需要分割的字符
	char *p;
	char *buf;
	p = strtok_s(s, sep, &buf);
	while (p)
	{
		wordstring = p;
		strcpy_s(wordchar, wordstring.c_str());
		if (strcmp(wordchar, "Title") == 0)
		{
			charnum -= 11;
			linenum++;
			w2 = w;
		} // 出现Title 权重为w；
		else if (strcmp(wordchar, "Abstract") == 0)
		{
			charnum -= 10;
			linenum++;
			w2 = 1;
		}// 出现Abstract 权重为1；
		else
		{
			length = wordstring.length();
			for (i = 0; i <= length; i++)
			{
				if (wordchar[i] >= 'A' && wordchar[i] <= 'Z')
				{
					wordchar[i] = wordchar[i] + 32;
				}
			}
			wordstring = wordchar;
			if (wordstring.length() >= 4)
			{
				for (j = 0; j <= 3; j++)//判断该单词是否符合前四位为字母
				{
					if (wordchar[j] >= 'a' && wordchar[j] <= 'z')
						pos = 1;
					else
					{
						pos = 0;
						break;
					}
				}
			}
			if (pos == 1)
			{
				wordnum++;
				if (w_c.count(wordstring) == 0)
				{
					w_c.insert(make_pair(wordstring, w2));
				}
				else
				{
					w_c[wordstring] += w2;
				}
				pos = 0;
			}
		}
		p = strtok_s(NULL, sep, &buf);
	}
	for (w_c_iter = w_c.begin(); w_c_iter != w_c.end(); w_c_iter++)
	{
		w_c2.push_back(make_pair(w_c_iter->first, w_c_iter->second));
	}
	sort(w_c2.begin(), w_c2.end(), Comp);
}
void writetxt_n(char *b, int n)
{
	char charnum_s[10], wordnum_s[10], linenum_s[10];
	char num_s[10];
	string res;
	char res_c[200000];
	_itoa_s(charnum + 2, charnum_s, 10);
	_itoa_s(wordnum, wordnum_s, 10);
	_itoa_s(linenum, linenum_s, 10);
	res = res + "characters: " + charnum_s + "\n";
	res = res + "words: " + wordnum_s + "\n";
	res = res + "lines: " + linenum_s + "\n";
	if (n == 0)
	{
		n = 10;
	}
	if (w_c2.size() >= n)
	{
		for (w_c2_iter = w_c2.begin(); w_c2_iter != w_c2.begin() + n; w_c2_iter++)
		{
			_itoa_s(w_c2_iter->second, num_s, 10);
			res = res + "<" + w_c2_iter->first + ">: " + num_s + "\n";
		}
	}
	else
	{
		for (w_c2_iter = w_c2.begin(); w_c2_iter != w_c2.end(); w_c2_iter++)
		{
			_itoa_s(w_c2_iter->second, num_s, 10);
			res = res + "<" + w_c2_iter->first + ">: " + num_s + "\n";
		}
	}
	strcpy_s(res_c, res.c_str());
	FILE *fp1;
	errno_t err;
	err = fopen_s(&fp1, b, "w");
	fwrite(res_c, res.length(), 1, fp1);
}
void divide_m(int w, int m)
{
	size_t length;
	int cznum = 0;
	string cz;
	string wordstring;
	char wordchar[999];
	int w2 = 1;
	int i, j, k = 0;
	int pos = 0;
	const char *sep = "./;'[] \\<>?:\"{}|`~!@#$%^&*()_+-=\n"; //需要分割的字符
	char *p = NULL;
	char *buf;
	p = strtok_s(s, sep, &buf);
	while (p)
	{
		wordstring = p;
		strcpy_s(wordchar, wordstring.c_str());
		if (strcmp(wordchar, "Title") == 0)
		{
			w2 = w;
			while (cz_q1.empty() == 0)
			{
				cz_q1.pop();
			}
			while (cz_q2.empty() == 0)
			{
				cz_q2.pop();
			}
		} // 出现Title 权重为w；
		else if (strcmp(wordchar, "Abstract") == 0)
		{
			w2 = 1;
			while (cz_q1.empty() == 0)
			{
				cz_q1.pop();
			}
			while (cz_q2.empty() == 0)
			{
				cz_q2.pop();
			}
		}// 出现Abstract 权重为1；
		else
		{
			length = wordstring.length();
			for (i = 0; i <= length; i++)
			{
				if (wordchar[i] >= 'A' && wordchar[i] <= 'Z')
				{

					wordchar[i] = wordchar[i] + 32;
				}
			}
			wordstring = wordchar;
			if (wordstring.length() >= 4) //合法pos=1  不合法pos=0
			{
				for (j = 0; j <= 3; j++)//判断该单词是否符合前四位为字母
				{
					if (wordchar[j] >= 'a' && wordchar[j] <= 'z')
					{
						pos = 1;
					}
					else
					{
						pos = 0;
						break;
					}
				}
			}
			else
			{
				pos = 0;
			}

			if (pos == 1)
			{
				if (cz_q2.size() == 0)
				{
					cz = "";
				}
				cz_q1.push(wordstring); //将合法单词入队q1
				cz_q2.push(wordstring); //将合法单词入队q2
				if (cz_q2.size() == m)
				{
					cz_q1.pop();	//若满足条件称为词组 则q1的首个单词出队
					for (i = 1; i <= m; i++)   //q2的所有单词存入cz中 用于输出并清空q2
					{
						if (i == m)
						{
							cz = cz + cz_q2.front();
							cz_q2.pop();
						}
						else
						{
							cz = cz + cz_q2.front() + " ";
							cz_q2.pop();
						}
					}
					if (cz_c.count(cz) == 0) //查询map中是否有该词组 无则将 词组，频率 引入  有则将原有 词组的频率累加
					{
						cz_c.insert(make_pair(cz, w2));
						cz = "";
					}
					else
					{
						cz_c[cz] += w2;
						cz = "";
					}
					for (j = 1; j <= cz_q1.size(); j++)  //将q1中剩余单词存入pop[]中 同步存入q2
					{
						pop[j] = cz_q1.front();
						cz_q1.pop();
						cz_q1.push(pop[j]);
						cz_q2.push(pop[j]);
					}
				}
			}
			else if (pos == 0) //当遇到非法单词 将两个队列清空
			{
				while (cz_q1.empty() == 0)
				{
					cz_q1.pop();
				}
				while (cz_q2.empty() == 0)
				{
					cz_q2.pop();
				}
			}
		}
		p = strtok_s(NULL, sep, &buf);
	}
	for (cz_c_iter = cz_c.begin(); cz_c_iter != cz_c.end(); cz_c_iter++)
	{
		cz_c2.push_back(make_pair(cz_c_iter->first, cz_c_iter->second));
	}
	sort(cz_c2.begin(), cz_c2.end(), Comp);
}
void writetxt_m(char *b)
{
	char charnum_s[10], wordnum_s[10], linenum_s[10];
	char num_s[10];
	string res;
	char res_c[200000];
	_itoa_s(charnum + 2, charnum_s, 10);
	_itoa_s(wordnum, wordnum_s, 10);
	_itoa_s(linenum, linenum_s, 10);
	res = res + "characters: " + charnum_s + "\n";
	res = res + "words: " + wordnum_s + "\n";
	res = res + "lines: " + linenum_s + "\n";
	if (cz_c2.size() >= 10)
	{
		for (cz_c2_iter = cz_c2.begin(); cz_c2_iter != cz_c2.begin() + 10; cz_c2_iter++)
		{
			_itoa_s(cz_c2_iter->second, num_s, 10);
			res = res + "<" + cz_c2_iter->first + ">: " + num_s + "\n";
		}
	}
	else
	{
		for (cz_c2_iter = cz_c2.begin(); cz_c2_iter != cz_c2.end(); cz_c2_iter++)
		{
			_itoa_s(cz_c2_iter->second, num_s, 10);
			res = res + "<" + cz_c2_iter->first + ">: " + num_s + "\n";
		}
	}
	strcpy_s(res_c, res.c_str());
	FILE *fp1;
	errno_t err;
	err = fopen_s(&fp1, b, "w");
	fwrite(res_c, res.length(), 1, fp1);
}
int main(int args, char* argv[])
{
	char* a = NULL;
	char* b = NULL;
	char* c = NULL;
	char* d = NULL;
	char* e = NULL;
	string cstr, dstr, estr;
	int i;
	int w = 0, m = 0, n = 0;
	for (i = 0; i < args; i++)
	{
		if (strcmp(argv[i], "-i") == 0)  //检测-i指令
		{
			a = argv[i + 1];
		}
		if (strcmp(argv[i], "-o") == 0) //检测-o指令
		{
			b = argv[i + 1];
		}
		if (strcmp(argv[i], "-w") == 0) //检测-w指令
		{
			c = argv[i + 1];
			cstr = c;
			w = atoi(cstr.c_str());
		}
		if (strcmp(argv[i], "-n") == 0)
		{
			d = argv[i + 1];
			dstr = d;
			n = atoi(dstr.c_str());
		}
		if (strcmp(argv[i], "-m") == 0)
		{
			e = argv[i + 1];
			estr = e;
			m = atoi(estr.c_str());
		}
	}
	readtxt(a);//读取文件
	divide_n(w);//分割并排序
	if (m == 0)
	{
		writetxt_n(b, n);//写进文件
	}
	else
	{
		readtxt2(a);
		divide_m(w, m);
		writetxt_m(b);
	}
}


