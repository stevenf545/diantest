#include<iostream>
#include<string>
#include<fstream>
#include<limits>
using namespace std;
class User
{
public:
	string m_name;
	int m_seat[4];
	void loadseat(int a, int b, int c, int d)
	{
		m_seat[0] = a;
		m_seat[1] = b;
		m_seat[2] = c;
		m_seat[3] = d;
	}
};
User user;
//用来存储指令数据
int insstore[4];
//图书馆数据
string libbase[7];
//用户数据映射
string namedate[26] = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
//将日期与数字映射
string weeklist[7] = { "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" };
//图书馆数据读取
int dateread()
{
	ifstream fin;
	fin.open("data.txt", ios::in);
	if (!fin.is_open())
	{
		cout << "Failed to open file" << endl;
		return 0;
	}
	for (int i = 0; i < 7 && getline(fin, libbase[i]); i++) {};
	fin.close();
	return 1;
}
//数据存储
void saveuser();
void savedate()
{
	ofstream fout;
	fout.open("data.txt");
	for (int i = 0; i < 7; i++)
	{
		fout << libbase[i] << endl;
	}
	fout.close();
}
//打印座位现状
void printdate(string *libbase, int day, int floor)
{
	int a = (floor - 1) * 16;
	for (int i = a; i < a + 16; i += 4)
	{
		cout << libbase[day - 1].substr(i, 4) << endl;
	}
}
//Clear功能的实现
void clear()
{
	string a(80, '0');
	for (int i = 0; i < 7; i++)
	{
		libbase[i] = a;
	}
	ofstream fout;
	fout.open("userdata.txt", ios::out);
	string originuser(104, '0');
	fout << originuser << endl;
	fout.close();
}
//预约功能实现
void reserve(int day, int floor, int hang, int lie)
{
	if (user.m_seat[0] != 0)
	{
		cout << "Error: Each user can only reserve one seat" << endl;
		return;
	}
	if (libbase[day - 1][(floor - 1) * 16 + (hang - 1) * 4 + lie - 1] == '1')
	{
		cout << "Error: The seat is already reserved" << endl;
		return;
	}
	libbase[day - 1][(floor - 1) * 16 + (hang - 1) * 4 + lie - 1] = '1';
	user.loadseat(day,floor,hang,lie);
}

//判断用户是否存在
int search(string* a, string b, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (b ==a[i])
		{
			return i;
		}
	}
	return -1;
}
//登录函数，返回登录结果
string tempuser;
int num_user;
int login(string name)
{
	int a = search(namedate, name, 26);
	
	if (name =="Admin")
	{
		return 2;
	}
	else if (a != -1)
	{
		user.m_name = name;
		ifstream fin;
		fin.open("userdata.txt");
		if (!fin.is_open())
		{
			cout << "Creating a blank user database..." << endl;
			ofstream fout;
			fout.open("userdata.txt",ios:: out);
			string originuser(130, '0');
			fout << originuser << endl;
			fout.close();
			return 3;
		}
		getline(fin, tempuser);
		fin.close();
		num_user = a * 5;
		if (((int)tempuser[num_user + 4] - 48) == 1)
		{
			cout << "Username is already in use" << endl;
			return 3;
		}
		user.m_seat[0] = (int)tempuser[num_user] - 48;
		user.m_seat[1] = (int)tempuser[num_user + 1] - 48;
		user.m_seat[2] = (int)tempuser[num_user + 2] - 48;
		user.m_seat[3] = (int)tempuser[num_user + 3] - 48;
		ofstream fout;
		fout.open("userdata.txt");
		tempuser[num_user + 4] = '1';
		fout << tempuser;
		fout.close();
		saveuser();
		return 1;
	}
	else { return 0; }
}
//用户信息保存
void saveuser()
{
	tempuser[num_user] = (char)(user.m_seat[0] + 48);
	tempuser[num_user+1] = (char)(user.m_seat[1] + 48);
	tempuser[num_user+2] = (char)(user.m_seat[2] + 48);
	tempuser[num_user+3] = (char)(user.m_seat[3] + 48);
	ofstream fout;
	fout.open("userdata.txt");
	fout << tempuser;
}
//查看个人预约结果的功能
void reservation()
{
	if (user.m_seat[0] == 0)
	{
		cout << "No current reservation" << endl;
		return;
	}
	cout << weeklist[user.m_seat[0] - 1] << " floor " << user.m_seat[1] << " seat " << user.m_seat[2] << " " << user.m_seat[3] << endl;
}

int searchweek(string a)
{
	for (int i = 0; i < 7; i++)
	{
		if (a == weeklist[i])
		{
			return i+1;
			break;
		}
		
	}
	return 0;
}
//判断指令是否为查看数据，并提取指令
int is_see(string ins)
{
	int a = ins.find("Floor");
	int b;
	int c = (int)ins[ins.size() - 1] - 48;
	if (ins.size() >= 14 && ins.size()<=17 && a != -1 && c>= 1 && c <= 5 && searchweek(ins.substr(0,a-1)))
	{
		b = searchweek(ins.substr(0,a-1));
		insstore[0] = b;
		insstore[1] = c;
		return 1;
	}
	else { return 0; }
}
//判断指令是否为预约，并提取指令
bool hanglie(int a, int b)
{
	if (a >= 1 && a <= 4 && b >= 1 && b <= 4)
	{
		return true;
	}
	return false;
}
int is_reserve(string ins)
{
	int temp = ins.find("Seat");
	int a = ins.find("Floor");
	if (!(temp + 1) || !(a + 1))
	{
		return 0;
	}
	int b = (int)ins[temp - 2] - 48;
	int c = (int)ins[temp + 5] - 48;
	int d = (int)ins[temp + 7] - 48;
	if (ins.substr(0, 7) == "Reserve" && ins.size() >= 31 && ins.size() <= 34 && searchweek(ins.substr(8, a - 9)) && temp && b >= 1 && b <= 5 && hanglie(c, d))
	{
		insstore[0] = searchweek(ins.substr(8, a - 9));//日期
		insstore[1] = b;//楼层
		insstore[2] = c;//行
		insstore[3] = d;//列
		return 1;
	}
	return 0;
}

int main()
{
	Start:
	int is_open = dateread();
	if (!is_open)
	{
		cout << "Warning: Failed to open data file, using default data" << endl;
		clear();
		savedate();
		goto Start;
	}
	string ins1;
	while (1)
	{
	BEGIN:
		cin >> ins1;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (ins1 == "Quit") { break; }
		else if (ins1 == "Login")
		{
			string name;
			cout << "Username："; cin >> name;
			cin.ignore();
			switch (login(name))
			{
			case 0:
				cout << "Error: Username does not exist" << endl; break;
			case 1:
				while (1)
				{
					cout << "Enter command：";
					getline(cin, ins1);
					if (ins1 == "Exit") { goto BEGIN; }
					else if (ins1 == "Quit") { goto END; }
					else if (is_see(ins1))
					{
						printdate(libbase, insstore[0], insstore[1]);
					}
					else if (is_reserve(ins1))
					{
						reserve(insstore[0], insstore[1], insstore[2], insstore[3]);
						savedate();
					}
					else if (ins1 == "Reservation")
					{
					
						reservation();
					}
					else { cout << "Invalid command" << endl; }
				}
				break;
			case 2:
				while (1)
				{
					cout << "Please enter a command：";
					getline(cin, ins1);
					if (ins1 == "Exit") { goto BEGIN; }
					else if (ins1 == "Quit") { goto END; }
					else if (ins1 == "Clear")
					{
						clear();
						savedate();
						ofstream fout;
						fout.open("userdata.txt", ios::out);
						string originuser(130, '0');
						fout << originuser << endl;
						fout.close();
						cout << "All reservation data has been cleared" << endl;
					}
					else { cout << "Please enter a valid command" << endl; }
				}
			case 3:
				cout << "Please log in again" << endl;
			}
			
		}
		else { cout << "Invalid command" << endl; }
	}
	END:
	system("pause");
	return 0;
}
