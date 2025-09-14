#include<iostream>
#include<string>
#include<fstream>
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
//�����洢ָ������
int insstore[4];
//ͼ�������
string libbase[7];
//�û�����ӳ��
string namedate[26] = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
//������������ӳ��
string weeklist[7] = { "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" };
//ͼ������ݶ�ȡ
int dateread()
{
	ifstream fin;
	fin.open("data.txt", ios::in);
	if (!fin.is_open())
	{
		cout << "�ļ���ʧ��" << endl;
		return 0;
	}
	for (int i = 0; i < 7 && getline(fin, libbase[i]); i++) {};
	fin.close();
	return 1;
}
//���ݴ洢
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
//��ӡ��λ��״
void printdate(string *libbase, int day, int floor)
{
	int a = (floor - 1) * 16;
	for (int i = a; i < a + 16; i += 4)
	{
		cout << libbase[day - 1].substr(i, 4) << endl;
	}
}
//Clear���ܵ�ʵ��
void clear()
{
	string a = "00000000000000000000000000000000000000000000000000000000000000000000000000000000";
	for (int i = 0; i < 7; i++)
	{
		libbase[i] = a;
	}
}
//ԤԼ����ʵ��
void reserve(int day, int floor, int hang, int lie)
{
	if (user.m_seat[0] != 0)
	{
		cout << "ÿ��ֻ��ԤԼһ����λ" << endl;
		return;
	}
	if (libbase[day - 1][(floor - 1) * 16 + (hang - 1) * 4 + lie - 1] == '1')
	{
		cout << "��λ���Ѿ���ԤԼ" << endl;
		return;
	}
	libbase[day - 1][(floor - 1) * 16 + (hang - 1) * 4 + lie - 1] = '1';
	user.loadseat(day,floor,hang,lie);
}

//�ж��û��Ƿ����
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
//��¼���������ص�¼���
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
			cout << "�û���Ϣ��ȡʧ��" << endl;
			cout << "�������ݿ�������1" << endl;
			int f;
			cin >> f;
			if (f != 1) { return 3; }
			cout << "���ڴ����հ��û����ݿ�" << endl;
			ofstream fout;
			fout.open("userdata.txt",ios:: out);
			fout << "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
			fout.close();
			return 3;
		}
		getline(fin, tempuser);
		fin.close();
		num_user = a * 4;
		user.m_seat[0] = (int)tempuser[num_user] - 48;
		user.m_seat[1] = (int)tempuser[num_user + 1] - 48;
		user.m_seat[2] = (int)tempuser[num_user + 2] - 48;
		user.m_seat[3] = (int)tempuser[num_user + 3] - 48;
		return 1;
	}
	else { return 0; }
}
//�û���Ϣ����
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
//�鿴����ԤԼ����Ĺ���
void reservation()
{
	if (user.m_seat[0] == 0)
	{
		cout << "��ԤԼλ��" << endl;
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
//�ж�ָ���Ƿ�Ϊ�鿴���ݣ�����ȡָ��
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
//�ж�ָ���Ƿ�ΪԤԼ������ȡָ��
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
		insstore[0] = searchweek(ins.substr(8, a - 9));//����
		insstore[1] = b;//¥��
		insstore[2] = c;//��
		insstore[3] = d;//��
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
		cout << "�޷���ȡ����" << endl;
		cout << "�����հ����ݿ�������1" << endl;
		int j;
		cin >> j;
		if (j == 1)
		{
			clear();
			savedate();
			goto Start;
		}
		system("pause");
		return 0;
	}
	string ins1;
	while (1)
	{
	BEGIN:
		cin >> ins1;
		if (ins1 == "Quit") { break; }
		else if (ins1 == "Login")
		{
			string name;
			cout << "�û�����"; cin >> name;
			cin.ignore();
			switch (login(name))
			{
			case 0:
				cout << "�û���������" << endl; break;
			case 1:
				while (1)
				{
					//cout << "m_seatΪ" << user.m_seat[0] << user.m_seat[1] << user.m_seat[2] << user.m_seat[3] << endl;
					cout << "������ָ�";
					getline(cin, ins1);//���ʣ�Ϊʲô���ü�ignore
					if (ins1 == "Exit") { goto BEGIN; }
					else if (ins1 == "Quit") { goto END; }
					else if (is_see(ins1))
					{
						printdate(libbase, insstore[0], insstore[1]);
					}
					else if (is_reserve(ins1))
					{
						reserve(insstore[0], insstore[1], insstore[2], insstore[3]);
						
						cout << "m_seatΪ" << user.m_seat[0] << user.m_seat[1] << user.m_seat[2] << user.m_seat[3] << endl;
						savedate();
						saveuser();
					}
					else if (ins1 == "Reservation")
					{
					
						reservation();
					}
					else { cout << "��Чָ��" << endl; }
				}
				break;
			case 2:
				while (1)
				{
					cout << "������ָ�";
					getline(cin, ins1);
					if (ins1 == "Exit") { goto BEGIN; }
					else if (ins1 == "Quit") { goto END; }
					else if (ins1 == "Clear")
					{
						clear();
						savedate();
					}
					else { cout << "��������ȷָ��" << endl; }
				}
			case 3:
				cout << "�����µ�¼" << endl;
			}
			
		}
		else { cout << "��Чָ��" << endl; }
	}
	END:
	system("pause");
	return 0;
}
