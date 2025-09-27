
#include<iostream>
#include<string>
using namespace std;

int main()
{
	string a;
	while (1)
	{
		cout << "ÇëÊäÈë£º";
		cin >> a;
		if (a == "Dian")
		{
			cout << "2002" << endl;
		}
		else if (a == "Quit")
		{
			break;
		}
		else
		{
			cout << "Error" << endl;
		}
	}
}
