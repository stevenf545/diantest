#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<map>
#include<fstream> 
#define FLOOR 6
#define HANG 5
#define LIE 5
using namespace std;

class Library;
class User;
class Admin;
class Mainmenu;

struct userSeatData { string name = ""; int day= 0; int floor=0; int hang=0; int lie=0; };
string weeklist[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

int is_week(string str)
{
	for (int i = 0; i < 7; i++)
	{
		if (weeklist[i] == str) { return (i + 1); }
		else { return 0; }
	}
}

class Library
{
public:
	vector<string> libseatdata[7];
public:
	void initialdata()//yes
	{
		ofstream fout("libseatdata.txt");
		string str(LIE, '0');
		for (int i = 0; i < 7; i++)
		{
			fout << "library" << endl;
			for (int j = 0; j < FLOOR; j++)
			{
				for (int k = 0; k < HANG - 1; k++) { fout << str << " "; }
				fout << str << endl;
			}
		}
		fout.close();
	}
	void readdata()//yes
	{
		ifstream fin("libseatdata.txt"); static int libreadtimes = 0;
		if (!fin.is_open() && libreadtimes == 0) { cout << "Data read failed, default data will be used" << endl; initialdata(); libreadtimes++; readdata(); libreadtimes = 0; }
		string temp; getline(fin, temp);
		for (int i = 0; i < 7; i++)
		{
			libseatdata[i].clear();
		}
		for (int i = 0; i < 7; i++)
		{
			while (getline(fin, temp) && temp != "library")
			{
				libseatdata[i].push_back(temp);
			}
		}
		fin.close();
	}
	void savedata()//yes
	{
		ofstream fout("libseatdata.txt");
		for (int i = 0; i < 7; i++)
		{
			fout << "library" << endl;
			for (auto it = libseatdata[i].begin(); it != libseatdata[i].end(); ++it)
			{
				fout << *it << endl;
			}
		}
		fout.close();
	}
}; 
class Admin
{
public:
	string mainnotice;
	string usernotice;
	vector<string> userfeedback;
public:
	void readuserfeedback()
	{
		ifstream fin("userfeedback.txt");
		if (!fin.is_open())
		{
			ofstream fout("userfeedback.txt");
			fout.close();
			return;
		}
		string temp;
		while (getline(fin, temp))
		{
			userfeedback.push_back(temp);
		}
		fin.close();
	}
	void clearall(Library& library, User& user);
	void readmainnotice()
	{ }
	void readusernotice()
	{ }
	void savemainnotice()
	{ }
	void saveusernotice()
	{ }
	void publishmainnotice(){}
	void publishusernotice(){}
	void checkuserfeedback(){}
};
class User
{
public:
	map<string, string> userdata;
	string m_name;
	string m_pass;
	vector<userSeatData> m_seat;
	vector<userSeatData> other_seat;
public:
	void initialmap()//yes
	{
		for(int i = 'A';i < 91;i++)
		{
			string a, b;
			a += static_cast<char>(i); b += static_cast<char>(i + 32);
			userdata.insert({ a,b });
		}
	}
	void readdata()//yes
	{
		ifstream fin("userdata.txt");
		if (!fin.is_open()) { cout << "Reading data failed, default data will be used" << endl; initialmap(); saveuserdata(); return; }
		string tempstring; string tempname; string temppassword;
		userdata.clear();
		while (getline(fin, tempstring))
		{
			istringstream isstr(tempstring);
			isstr >> tempname; isstr >> temppassword;
			userdata.insert({ tempname, temppassword });
		};
		fin.close();
	}
	void saveuserdata()//yes
	{
		ofstream fout("userdata.txt");
		for (auto it = userdata.begin(); it != userdata.end(); ++it)
		{
			fout << it->first << " " << it->second << endl;
		}
		fout.close();
	}
	void readseatlog()//yes
	{
		ifstream fin("seatlog.txt");
		if (!fin.is_open()) { cout << "User data read failed, default data will be used" << endl; ofstream fout("seatlog.txt"); fout.close(); return; }
		string tempseatdata;
		m_seat.clear(); other_seat.clear();
		while (getline(fin, tempseatdata))
		{
			istringstream isstr(tempseatdata);
			string name; isstr >> name;
			if (name == m_name)
			{
				userSeatData tempdata;
				tempdata.name = name;
				isstr >> tempdata.day >> tempdata.floor >> tempdata.hang >> tempdata.lie;
				m_seat.push_back(tempdata);
			}
			else 
			{
				userSeatData tempdata; 
				tempdata.name = name; isstr >> tempdata.day >> tempdata.floor >> tempdata.hang >> tempdata.lie;
				other_seat.push_back(tempdata);
			}
		}
		fin.close();
	}
	void savaseatlog()//yes
	{
		ofstream fout("seatlog.txt");
		for (auto it = m_seat.begin(); it != m_seat.end(); ++it)
		{
			fout << it->name << " " << it->day << " " << it->floor << " " << it->hang << " " << it->lie << endl;
		}
		for (auto it = other_seat.begin(); it != other_seat.end(); ++it)
		{
			fout << it->name << " " << it->day << " " << it->floor << " " << it->hang << " " <<it->lie << endl;
		}
		fout.close();
	}
	int login(string name, string password)//yes
	{
		if (name == "Admin")
		{
			if (password == "admin123")
			{
				cout << "Login successfully" << endl;
				return 2;
			}
			cout << "Incorrect password" << endl; return 0;
		}
		auto it = userdata.find(name);
		if (it != userdata.end())
		{
			if (password == it->second)
			{
				m_name = name;
				m_pass = password;
				readseatlog();
				cout << "Login suceessfully" << endl; return 1;
			}
			cout << "Incorrect password" << endl; return 0;
		}
		cout << "Uername does not exit" << endl; return 0;
	}
	void regist(string name,string password)//yes
	{ 
		auto it = userdata.find(name);
		if (it != userdata.end())
		{
			cout << "The username already exists" << endl; return;
		}
		readdata();
		userdata.insert({ name, password });
		cout << "Register Suceessfully!" << endl;
		saveuserdata();
	}
	void reservation()//yes
	{
		if (m_seat.empty())
		{
			cout << "You haven't reserved a seat yet" << endl; return;
		}
		for (auto it = m_seat.begin(); it != m_seat.end(); ++it)
		{
			cout << weeklist[it->day] << " floor " << it->floor << " Seat " << it->hang << " " << it->lie << endl;
		}
	}
	//不读取和保存
	void reserve(int day,int floor ,int hang,int lie,Library &lib)//yes
	{ 
		for (auto it = m_seat.begin(); it != m_seat.end(); ++it)
		{
			if (it->day == day) { cout << "Sorry, you already have a reservation on that day" << endl; return; }
		}
		for (auto it = other_seat.begin(); it != other_seat.end(); ++it)
		{
			if (it->day == day && it->floor == floor && it->hang == hang && it->lie == lie)
			{
				cout << "Sorry,this seat has been reseved" << endl; return;
			}
		}
		userSeatData temp = { m_name,day,floor,hang,lie };
		m_seat.push_back(temp);
		string tempfloor; string temphang;
		istringstream isstr(lib.libseatdata[day][floor - 1]);
		for (int i = 1; isstr >> temphang; i++)
		{
			if (i == hang) { temphang[lie - 1] = '1'; }
			if (i != 1) { temphang.insert(0, 1, ' '); }
			tempfloor += temphang;
		}
		lib.libseatdata[day][floor - 1] = tempfloor;
	}
	void cancel(int day,int floor,int hang,int lie,Library &lib)//yes
	{ 
		bool is_find = 0;
		for (auto it = m_seat.begin(); it != m_seat.end();++it)
		{
			if (it->day == day && it->floor == floor && it->hang == hang && it->lie == lie)
			{
				m_seat.erase(it); is_find = 1; break;
			}
		}
		if (is_find)
		{
			string tempfloor; string temphang;
			istringstream isstr(lib.libseatdata[day][floor -1 ]);
			for (int i = 1; isstr >> temphang; i++)
			{
				if (i == hang) { temphang[lie - 1] = '0'; }
				if (i != 1) { temphang.insert(0, 1, ' '); }
				tempfloor += temphang;
			}
			lib.libseatdata[day][floor - 1] = tempfloor;
		}
		else { cout << R"(You haven't reserved this seat)" << endl; }
	}
	void feedback(Admin &admin)
	{
		cout << "please input your feedback:" << endl;
		string temp;
		getline(cin, temp);
		admin.userfeedback.push_back(temp);
	}
	void changepass()//yes
	{ 
		string input_pass;
		cout << "please input original password: "; cin >> input_pass; cin.ignore();
		if (input_pass == m_pass)//验证，防止忘记退出时他人修改密码
		{
			cout << "please input new password:"; string newpass1; cin >> newpass1; cin.ignore();
			cout << "please input password again:"; string newpass2; cin >> newpass2; cin.ignore();
			if (newpass1 == newpass2)
			{
				readdata();
				userdata[m_name] = newpass1;
				saveuserdata();
			}
			else { cout << "Please ensure that the passwords are consistent" << endl; }
		}
		else { cout << "Your password is error" << endl; }
	}
	void checkseat(int day, int floor,Library &library)
	{
		string temp;
		istringstream isstr(library.libseatdata[day][floor - 1]);
		while (isstr >> temp)
		{
			cout << temp << endl;
		}
	}
};
class Mainmenu 
{
public:
	void showmainins()
	{
		cout << "--------------------" << endl;
		cout << "Help -- show help" << endl;
		cout << "Quit -- exit the application" << endl;
		cout << "Exit -- log off" << endl;
		cout << "Login" << endl;
		cout << "Register" << endl;
		//cout << "Feedback" << endl;
		cout << "--------------------" << endl;
	}
	void showuserins()
	{
		cout << "---------------------" << endl;
		cout << "Reserve [day] Floor [num] Seat [hang] [lie]" << endl;
		cout << "Check [day] Floor [num]" << endl;
		cout << "Cancel [day] Floor [num] Seat [hang] [lie]" << endl;
		cout << "Reset password" << endl;
		cout << "Reservation -- check my reservation" << endl;
		cout << "Quit" << endl;
		cout << "Exit" << endl;
		cout << "Help" << endl;
		//cout << "Feedback" << endl;
		cout << "---------------------" << endl;
	}
	void showadminins()
	{
		cout << "-----------------------" << endl;
		//cout << "Reservefor [username] [day] [floor] Seat [hang] [lie]" << endl;
		//cout << "Cancel [day] [floor] Seat [hang] [lie]" << endl;
		//cout << "Check Feedback" << endl;
		//cout << "Publish note to [username]" << endl;
		cout << "Clear -- clear all reservation" << endl;
		cout << "Exit" << endl;
		cout << "Quit" << endl;
		cout << "Exit" << endl;
		cout << "Help" << endl;
		cout << "-----------------------" << endl;
	}
};

//Admin的类外函数
void Admin::clearall(Library& library, User& user)
{
	library.initialdata();
	library.readdata();
	user.m_seat.clear();
	user.other_seat.clear();
	user.savaseatlog();
	cout << "All reservation have been cleared" << endl;
}

void mainload()
{
	cout << "==========================================" << endl << "         Welcome to the library           " <<endl << "===========================================" << endl;
	Mainmenu mainmenu;
	Library library;
	User user;
	Admin admin;
	string tempname, temppass;
	vector<string> insdata;
	string tempins;
	mainmenu.showmainins();
	string ins;
	user.readdata();
	user.readseatlog();
	library.readdata();
	while (1) {
	BEGIN:
		getline(cin, ins);
		if (ins == "Quit") { return; }
		else if (ins == "Help") { mainmenu.showmainins(); }
		else if (ins == "Register")
		{
			cout << "name: "; getline(cin, tempname);
			cout << "password: "; getline(cin, temppass);
			if (tempname.find(" ") != -1 || temppass.find(" ") != -1)
			{
				cout << "Account and password cannot contain spaces" << endl;
			}
			else { user.regist(tempname, temppass); }
		}
		else if (ins == "Login")
		{
			cout << "name: "; cin >> tempname;
			cout << "password: "; cin >> temppass;
			switch (user.login(tempname, temppass))
			{
			case 0:break;
			case 1:mainmenu.showuserins();
				while (1) {
					if (cin.peek() == '/n') { cin.ignore(); }
					getline(cin, ins);
					istringstream isstr(ins); insdata.clear();
					while (isstr >> tempins) { insdata.push_back(tempins); }
					if (insdata.size() == 0) {}
					else if (ins == "Quit") { user.saveuserdata(); user.savaseatlog(); library.savedata(); return; }
					else if (ins == "Exit") { user.saveuserdata(); user.savaseatlog(); library.savedata(); goto BEGIN; }
					else if (ins == "Reservation") { user.readseatlog(); user.reservation(); }
					else if (ins == "Feedback") { user.feedback(admin); }
					else if (insdata[0] == "Check" && insdata.size() == 4 )
					{
						if (insdata[2] == "Floor" && stoi(insdata[3]) > 0 && stoi(insdata[3]) < FLOOR + 1 && is_week(insdata[1])) 
						{
							library.readdata();
							user.checkseat(is_week(insdata[1]) - 1, stoi(insdata[3]), library);
						}
						else { cout << "invalid instruction" << endl; }
					}
					else if (insdata[0] == "Reserve" && insdata.size() == 7)
					{
						if (insdata[2] == "Floor" && insdata[4] == "Seat"&& is_week(insdata[1]) &&
							stoi(insdata[3]) > 0 && stoi(insdata[3]) < FLOOR + 1 && stoi(insdata[5]) > 0 && stoi(insdata[5]) < HANG + 1 && stoi(insdata[6]) > 0 && stoi(insdata[6]) < LIE + 1)
						{
							library.readdata(); user.readdata(); user.readseatlog();
							user.reserve(is_week(insdata[1]) - 1, stoi(insdata[3]), stoi(insdata[5]), stoi(insdata[6]), library);
							library.savedata(); user.saveuserdata(); user.savaseatlog();
						}
						else { cout << "invalid instruction" << endl; }
					}
					else if (insdata[0] == "Cancel" && insdata.size() == 7 )
					{
						if (insdata[2] == "Floor" && insdata[4] == "Seat"&& is_week(insdata[1]) &&
							stoi(insdata[3]) > 0 && stoi(insdata[3]) < FLOOR + 1 && stoi(insdata[5]) > 0 && stoi(insdata[5]) < HANG + 1 && stoi(insdata[6]) > 0 && stoi(insdata[6]) < LIE + 1)
						{
							library.readdata(); user.readdata(); user.readseatlog();
							user.cancel(is_week(insdata[1]) - 1, stoi(insdata[3]), stoi(insdata[5]), stoi(insdata[6]), library);
							library.savedata(); user.saveuserdata(); user.savaseatlog();
						}
						else { cout << "invalid instruction" << endl; }
					}
					else if (insdata.size() == 2 && insdata[0] == "Reset")
					{
						if (insdata[1] == "password")
						{
							user.changepass();
						}
						else { cout << "invalid instruction" << endl; }
					}
					else { cout << "invalid instruction" << endl; }
				}
			case 2:
				mainmenu.showadminins();
				while(1)
				{
					if (cin.peek() == '/n') { cin.ignore(); }
					getline(cin, ins);
					istringstream isstr(ins); insdata.clear();
					while (isstr >> tempins) { insdata.push_back(tempins); }
					if (insdata.size() == 0) {}
					else if (ins == "Quit") { user.saveuserdata(); user.savaseatlog(); library.savedata(); return; }
					else if (ins == "Exit") { user.saveuserdata(); user.savaseatlog(); library.savedata(); goto BEGIN; }
					else if (ins == "Help") { mainmenu.showadminins(); }
					else if (ins == "Clear") { admin.clearall(library, user); }
					else { cout << "invalid instruction" << endl; }
				}
			}
		}
	}
}
int main()
{
	mainload();
	return 0;
}
