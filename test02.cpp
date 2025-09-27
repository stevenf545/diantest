#include<iostream>
#include<string>
#include<fstream>
using namespace std;
int main()
{
	//对文件进行读取和分析
	ifstream fin;
	fin.open("data.txt");
	string buf;
	string arr1[128];
	string arr2[128];
	int a;
	int b = 0;
	while (getline(fin,buf))
	{
		a = buf.find(":");
		if (a != -1)
		{
			arr1[b] = buf.substr(0, a);
			arr2[b] = buf.substr(a + 1, buf.size() - a - 1);
			b++;
		}
	}
	fin.close();

	//用户交互
	string c;
	int d;
	while (1)
	{
		cin >> c;
		if (c == "Quit") { break; }
		for (int i = 0; i < b; i++)
		{
			if (arr1[i] == c)
			{
				d = i;
				break;
			}
			else { d = -1; }

		}
		if (d == -1)
		{
			cout << "Error" << endl;
		}
		else { cout << arr2[d] << endl; }

	}
	return 0;
}