#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<map>
#include<fstream> 
#include <ctime>
#define FLOOR 5
#define HANG 4
#define LIE 4
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
	}
	return 0;
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
	bool maintenanceMode; 
	Admin() : maintenanceMode(false)
	{
		readMaintenanceMode(); 
		readMaintenanceMode(); 
	}
public:
	void readuserfeedback()  
	{
		ifstream fin("userfeedback.txt");
		userfeedback.clear(); 
		if (!fin.is_open())
		{
			ofstream fout("userfeedback.txt");
			fout.close();
			return;
		}
		string temp;
		while (getline(fin, temp))
		{
			if (!temp.empty()) {
				userfeedback.push_back(temp);
			}
		}
		fin.close();
	}
	void clearall(Library& library, User& user);
	void readmainnotice()  
	{
		ifstream fin("mainnotice.txt");
		if (fin.is_open())
		{
			getline(fin, mainnotice);
			fin.close();
		}
	}
	void readusernotice()  // 实现这个函数  
	{
		ifstream fin("usernotice.txt");
		if (fin.is_open())
		{
			getline(fin, usernotice);
			fin.close();
		}
	}
	void savemainnotice()  // 实现这个函数
	{
		ofstream fout("mainnotice.txt");
		if (fout.is_open())
		{
			fout << mainnotice;
			fout.close();
		}
	}
	void saveusernotice()  // 实现这个函数
	{
		ofstream fout("usernotice.txt");
		if (fout.is_open())
		{
			fout << usernotice;
			fout.close();
		}
	}
	void publishmainnotice()  // 实现这个函数
	{
		cout << "Enter main notice: ";
		getline(cin, mainnotice);
		savemainnotice();
		cout << "Main notice published successfully!" << endl;
	}
	void publishusernotice()  // 实现这个函数
	{
		cout << "Enter user notice: ";
		getline(cin, usernotice);
		saveusernotice();
		cout << "User notice published successfully!" << endl;
	}
	void checkuserfeedback()  // 实现这个函数
	{
		readuserfeedback(); // 读取最新反馈
		if (userfeedback.empty())
		{
			cout << "No feedback yet." << endl;
			return;
		}

		cout << "=== User Feedback ===" << endl;
		for (size_t i = 0; i < userfeedback.size(); ++i)
		{
			cout << i + 1 << ". " << userfeedback[i] << endl;
		}
		cout << "=====================" << endl;
	}
	void saveuserfeedback()
	{
		ofstream fout("userfeedback.txt");
		if (fout.is_open())
		{
			for (const auto& feedback : userfeedback)
			{
				fout << feedback << endl;
			}
			fout.close();
		}
	}


	//ai辅助区
	void reserveForUser(User& user, Library& library, const string& username, int day, int floor, int hang, int lie);
	void cancelSeat(User& user, Library& library, int day, int floor, int hang, int lie);
	void setSeatAsUsing(Library& library, int day, int floor);
	void publishNoteToUser(const string& username, const string& note);
	void checkFeedback();
	void saveUserFeedback();
	void enterEditMode(Library& library,User&user);
	bool isValidLayout(const string& layout) {
		vector<string> rows;
		istringstream iss(layout);
		string row;

		while (iss >> row) {
			if (row.length() != LIE) return false;
			for (char c : row) {
				if (c != '0' && c != '1' && c != '2' && '*') return false;
			}
			rows.push_back(row);
		}

		return rows.size() == HANG;
	}

	void removeReservationsForFloor(int floorNum, User& user);
	void replyToFeedback(int index, const string& reply)
	{
		readuserfeedback();
		if (index < 1 || index > static_cast<int>(userfeedback.size()))
		{
			cout << "Invalid feedback index!" << endl;
			return;
		}

		// 从反馈中提取用户名
		string feedbackStr = userfeedback[index - 1];
		string username = "unknown";

		// 查找反馈中的用户名（格式：[timestamp] username: feedback）
		size_t bracketEnd = feedbackStr.find(']');
		if (bracketEnd != string::npos && bracketEnd + 2 < feedbackStr.length()) {
			size_t colonPos = feedbackStr.find(':', bracketEnd + 2);
			if (colonPos != string::npos) {
				username = feedbackStr.substr(bracketEnd + 2, colonPos - bracketEnd - 2);
			}
		}

		// 将回复保存到文件，包含用户名
		ofstream fout("feedback_replies.txt", ios::app);
		if (fout.is_open())
		{
			fout << "Reply to " << username << " (feedback " << index << "): " << reply << endl;
			fout.close();
		}
		cout << "Reply sent successfully to " << username << "!" << endl;
	}
	void viewUserList(User& user);

	// 重置用户密码
	void resetUserPassword(User& user, const string& username, const string& newPassword);

	// 查看seatlog文件信息
	void viewSeatLog();
	void readMaintenanceMode()
	{
		ifstream fin("maintenance_mode.txt");
		if (fin.is_open())
		{
			string mode;
			fin >> mode;
			maintenanceMode = (mode == "true");
			fin.close();
		}
		else
		{
			maintenanceMode = false;
			saveMaintenanceMode(); // 创建默认文件
		}
	}

	void saveMaintenanceMode()
	{
		ofstream fout("maintenance_mode.txt");
		if (fout.is_open())
		{
			fout << (maintenanceMode ? "true" : "false");
			fout.close();
		}
	}

	void enableMaintenanceMode()
	{
		maintenanceMode = true;
		saveMaintenanceMode();
		cout << "Maintenance mode enabled. Users cannot login now." << endl;
	}

	void disableMaintenanceMode()
	{
		maintenanceMode = false;
		saveMaintenanceMode();
		cout << "Maintenance mode disabled. Users can login now." << endl;
	}

	bool isMaintenanceMode() const
	{
		return maintenanceMode;
	}
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
		readdata();
		readseatlog();
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
	void feedback(Admin& admin)
	{
		cout << "Please input your feedback: ";
		string temp;
		getline(cin, temp);

		if (temp.empty()) {
			cout << "Feedback cannot be empty!" << endl;
			return;
		}

		// 使用安全的ctime_s函数
		time_t now = time(0);
		char timeStr[26];
		ctime_s(timeStr, sizeof(timeStr), &now);

		string timestamp(timeStr);
		timestamp.pop_back(); // 移除换行符

		string formattedFeedback = "[" + timestamp + "] " + m_name + ": " + temp;

		admin.userfeedback.push_back(formattedFeedback);
		admin.saveuserfeedback();
		cout << "Thank you for your feedback!" << endl;
	}

	// 新增：检查反馈回复
	void checkFeedbackReplies()
	{
		ifstream fin("feedback_replies.txt");
		if (!fin.is_open())
		{
			cout << "No replies yet." << endl;
			return;
		}

		string line;
		bool found = false;
		int replyCount = 0;

		cout << "=== Feedback Replies for " << m_name << " ===" << endl;

		while (getline(fin, line))
		{
			string pattern = "Reply to " + m_name + " ";
			if (line.find(pattern) == 0) {
				replyCount++;
				string replyContent = line.substr(pattern.length());
				cout << "Reply #" << replyCount << ": " << replyContent << endl;
				found = true;
			}
		}
		fin.close();

		if (!found) {
			cout << "No replies for you yet." << endl;
		}
		else {
			cout << "Total replies: " << replyCount << endl;
		}
		cout << "===============================" << endl;
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

	//ai辅助区
	void is_cancel(Library& library);
	void checkNotes();
};
class Mainmenu 
{
public:
	void showmainins()
	{
		cout << "--------------------" << endl;
		cout << "Login" << endl;
		cout << "Register" << endl;
		cout << "Help -- show help" << endl;
		cout << "Quit -- exit the application" << endl;
		cout << "Exit -- log off" << endl;
		cout << "--------------------" << endl;
	}
	void showuserins()
	{
		cout << "---------------------" << endl;
		cout << "Check [day] Floor [num]" << endl;
		cout << "Reserve [day] Floor [num] Seat [hang] [lie]" << endl;
		cout << "Cancel [day] Floor [num] Seat [hang] [lie]" << endl;
		cout << "Reservation -- check my reservation" << endl;
		cout << "Check Notes -- check admin messages" << endl;
		cout << "Feedback -- submit feedback" << endl;
		cout << "Check Replies -- check admin replies" << endl;
		cout << "Reset password" << endl;
		cout << "Quit" << endl;
		cout << "Exit" << endl;
		cout << "Help" << endl;
		cout << "---------------------" << endl;
	}
	void showadminins()
	{
		cout << "-----------------------" << endl;
		cout << "View User List -- view all users and passwords" << endl;
		cout << "View Seat Log -- view all reservation records" << endl;
		cout << "Reset User Password [username] ([newpassword]) -- reset user password" << endl;
		cout << "Reservefor [username] [day] [floor] Seat [hang] [lie]" << endl;
		cout << "Cancel [day] Floor [num] Seat [hang] [lie]" << endl;
		cout << "Send note to [username] [message]" << endl;
		cout << "Check Feedback -- view user feedback" << endl;
		cout << "Reply to [index] [message] -- reply to feedback" << endl;
		cout << "Publish Main Notice -- publish main notice" << endl;
		cout << "Publish User Notice -- publish user notice" << endl;
		cout << "Set [day] Floor [num] as using" << endl;
		cout << "Seat Layout Compilation Mode" << endl;
		cout << "Clear -- clear all reservation" << endl;
		cout << "Maintenance On/Off/Status" << endl;
		cout << "Exit" << endl;
		cout << "Quit" << endl;
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
//ai区
void Admin::reserveForUser(User& user, Library& library, const string& username, int day, int floor, int hang, int lie)
{
	library.readdata();
	user.readdata();
	user.readseatlog();

	// 检查座位是否可用
	for (auto it = user.other_seat.begin(); it != user.other_seat.end(); ++it)
	{
		if (it->day == day && it->floor == floor && it->hang == hang && it->lie == lie)
		{
			cout << "Sorry, this seat has been reserved" << endl;
			return;
		}
	}

	// 创建预约记录
	userSeatData temp = { username, day, floor, hang, lie };
	user.other_seat.push_back(temp);

	// 更新图书馆数据
	string tempfloor;
	string temphang;
	istringstream isstr(library.libseatdata[day][floor - 1]);
	for (int i = 1; isstr >> temphang; i++)
	{
		if (i == hang) { temphang[lie - 1] = '1'; }
		if (i != 1) { temphang.insert(0, 1, ' '); }
		tempfloor += temphang;
	}
	library.libseatdata[day][floor - 1] = tempfloor;

	library.savedata();
	user.savaseatlog();
	cout << "Reserved successfully for " << username << endl;
}
void Admin::cancelSeat(User& user, Library& library, int day, int floor, int hang, int lie)
{
	library.readdata();
	user.readdata();
	user.readseatlog();

	bool found = false;
	// 在用户预约记录中查找并删除
	for (auto it = user.m_seat.begin(); it != user.m_seat.end(); ++it)
	{
		if (it->day == day && it->floor == floor && it->hang == hang && it->lie == lie)
		{
			user.m_seat.erase(it);
			found = true;
			break;
		}
	}

	// 在其他用户预约记录中查找并删除
	for (auto it = user.other_seat.begin(); it != user.other_seat.end(); ++it)
	{
		if (it->day == day && it->floor == floor && it->hang == hang && it->lie == lie)
		{
			user.other_seat.erase(it);
			found = true;
			break;
		}
	}

	if (found)
	{
		// 更新图书馆数据
		string tempfloor;
		string temphang;
		istringstream isstr(library.libseatdata[day][floor - 1]);
		for (int i = 1; isstr >> temphang; i++)
		{
			if (i == hang) { temphang[lie - 1] = '0'; }
			if (i != 1) { temphang.insert(0, 1, ' '); }
			tempfloor += temphang;
		}
		library.libseatdata[day][floor - 1] = tempfloor;

		library.savedata();
		user.savaseatlog();
		cout << "Canceled successfully" << endl;
	}
	else
	{
		cout << "Seat not found" << endl;
	}
}
void Admin::setSeatAsUsing(Library& library, int day, int floor)
{
	library.readdata();

	// 添加参数验证
	if (day < 0 || day >= 7) {
		cout << "Invalid day: must be between 0 and 6" << endl;
		return;
	}
	if (floor <= 0 || floor > FLOOR) {
		cout << "Invalid floor: must be between 1 and " << FLOOR << endl;
		return;
	}

	// 检查楼层数据是否存在
	if (library.libseatdata[day].size() < (size_t)floor) {
		cout << "Floor data not found for day " << day << " floor " << floor << endl;
		return;
	}

	// 获取当前楼层的布局
	string floorLayout = library.libseatdata[day][floor - 1];
	string newLayout;
	istringstream iss(floorLayout);
	string row;

	// 将每个座位的状态改为'2'（使用中）
	bool first = true;
	while (iss >> row) {
		if (!first) {
			newLayout += " ";
		}
		first = false;

		// 将这一行所有座位改为'2'
		string newRow(row.length(), '2');
		newLayout += newRow;
	}

	// 更新图书馆数据
	library.libseatdata[day][floor - 1] = newLayout;
	library.savedata();

	cout << "Entire floor " << floor << " on " << weeklist[day] << " has been set as using" << endl;

	// 同时需要更新座位日志，移除该楼层所有的预约
	User tempUser; // 创建一个临时用户对象来操作座位日志
	tempUser.readseatlog();

	vector<userSeatData> updatedOtherSeat;
	for (const auto& seat : tempUser.other_seat) {
		if (!(seat.day == day && seat.floor == floor)) {
			updatedOtherSeat.push_back(seat);
		}
	}

	tempUser.other_seat = updatedOtherSeat;
	tempUser.savaseatlog();
}
void Admin::publishNoteToUser(const string& username, const string& note)
{
	ofstream fout("usernotes.txt", ios::app);
	if (fout.is_open())
	{
		fout << username << ": " << note << endl;
		fout.close();
		cout << "Note published to " << username << endl;
	}
}
void Admin::checkFeedback()
{
	readuserfeedback();
	if (userfeedback.empty())
	{
		cout << "No feedback yet" << endl;
		return;
	}

	cout << "User Feedback:" << endl;
	for (size_t i = 0; i < userfeedback.size(); ++i)
	{
		cout << i + 1 << ". " << userfeedback[i] << endl;
	}
}
void Admin::saveUserFeedback()
{
	ofstream fout("userfeedback.txt");
	if (fout.is_open())
	{
		for (const auto& feedback : userfeedback)
		{
			fout << feedback << endl;
		}
		fout.close();
	}
}
void Admin::enterEditMode(Library& library, User& user)
{
	library.readdata();

	cout << "=== Seat Layout Compilation Mode ===" << endl;
	cout << "Available commands:" << endl;
	cout << "Reset Floor [num] - Reset layout for a floor (all days)" << endl;
	cout << "Check Floor [num] - Display layout for a floor (all days)" << endl;
	cout << "exit - Exit edit mode" << endl;
	cout << "=====================================" << endl;

	string command;
	while (true) {
		cout << "EditMode> ";
		getline(cin, command);

		if (command == "exit") {
			cout << "Exiting edit mode..." << endl;
			break;
		}

		vector<string> tokens;
		istringstream iss(command);
		string token;
		while (iss >> token) tokens.push_back(token);

		if (tokens.size() >= 3 && tokens[0] == "Reset" && tokens[1] == "Floor") {
			int floorNum;
			try {
				floorNum = stoi(tokens[2]);
			}
			catch (...) {
				cout << "Invalid floor number" << endl;
				continue;
			}

			if (floorNum <= 0 || floorNum > FLOOR) {
				cout << "Floor number must be between 1 and " << FLOOR << endl;
				continue;
			}

			cout << "Enter new layout for floor " << floorNum << " (" << HANG << " rows, " << LIE << " seats per row):" << endl;
			cout << "Format: ";
			for (int i = 0; i < HANG; i++) {
				cout << string(LIE, '0');
				if (i < HANG - 1) cout << " ";
			}
			cout << endl << "> ";

			string newLayout;
			getline(cin, newLayout);

			if (isValidLayout(newLayout)) {
				for (int day = 0; day < 7; day++) {
					library.libseatdata[day][floorNum - 1] = newLayout;
				}
				library.savedata();

				removeReservationsForFloor(floorNum, user);

				cout << "Layout updated successfully for all days" << endl;
			}
			else {
				cout << "Invalid layout format. Please use " << HANG << " groups of " << LIE << " digits (0/1/2) or *" << endl;
			}

		}
		else if (tokens.size() >= 3 && tokens[0] == "Check" && tokens[1] == "Floor") {
			int floorNum;
			try {
				floorNum = stoi(tokens[2]);
			}
			catch (...) {
				cout << "Invalid floor number" << endl;
				continue;
			}

			if (floorNum <= 0 || floorNum > FLOOR) {
				cout << "Floor number must be between 1 and " << FLOOR << endl;
				continue;
			}

			cout << "Layout for Floor " << floorNum << " (all days):" << endl;
			for (int day = 0; day < 7; day++) {
				cout << weeklist[day] << ": " << library.libseatdata[day][floorNum - 1] << endl;
			}

		}
		else {
			cout << "Unknown command. Available: Reset Floor [num], Check Floor [num], exit" << endl;
		}
	}
}
void Admin::removeReservationsForFloor(int floorNum, User& user)
{
	// 读取用户座位数据
	user.readseatlog();

	cout << "Removing reservations for floor " << floorNum << "..." << endl;

	// 删除当前用户的预约记录
	vector<userSeatData> updatedSeat;
	int removedCount = 0;

	for (auto it = user.m_seat.begin(); it != user.m_seat.end(); ++it) {
		if (it->floor == floorNum) {
			cout << "Removed reservation for " << it->name << " on "
				<< weeklist[it->day] << " at floor " << it->floor
				<< " seat " << it->hang << "-" << it->lie << endl;
			removedCount++;
		}
		else {
			updatedSeat.push_back(*it);
		}
	}
	user.m_seat = updatedSeat;

	// 删除其他用户的预约记录
	vector<userSeatData> updatedOtherSeat;
	for (auto it = user.other_seat.begin(); it != user.other_seat.end(); ++it) {
		if (it->floor == floorNum) {
			cout << "Removed reservation for " << it->name << " on "
				<< weeklist[it->day] << " at floor " << it->floor
				<< " seat " << it->hang << "-" << it->lie << endl;
			removedCount++;
		}
		else {
			updatedOtherSeat.push_back(*it);
		}
	}
	user.other_seat = updatedOtherSeat;

	// 保存更新后的座位日志
	user.savaseatlog();

	cout << "Total reservations removed: " << removedCount << endl;
}
void Admin::viewUserList(User& user) {
	user.readdata(); // 确保读取最新数据
	cout << "=== User List ===" << endl;
	cout << "Username\tPassword" << endl;
	cout << "-------------------" << endl;
	for (const auto& userPair : user.userdata) {
		cout << userPair.first << "\t\t" << userPair.second << endl;
	}
	cout << "===================" << endl;
}

// 重置用户密码
void Admin::resetUserPassword(User& user, const string& username, const string& newPassword) {
	user.readdata(); // 读取最新用户数据
	auto it = user.userdata.find(username);
	if (it != user.userdata.end()) {
		user.userdata[username] = newPassword;
		user.saveuserdata();
		cout << "Password for user '" << username << "' has been reset successfully!" << endl;
	}
	else {
		cout << "User '" << username << "' not found!" << endl;
	}
}

// 查看seatlog文件信息
void Admin::viewSeatLog() {
	ifstream fin("seatlog.txt");
	if (!fin.is_open()) {
		cout << "No seat reservation data found." << endl;
		return;
	}

	cout << "=== Seat Reservation Log ===" << endl;
	cout << "Format: Username Day Floor Row Column" << endl;
	cout << "----------------------------" << endl;

	string line;
	while (getline(fin, line)) {
		if (!line.empty()) {
			cout << line << endl;
		}
	}
	fin.close();
	cout << "============================" << endl;
}
//User的函数类外定义
//ai区
void User::is_cancel(Library& library)
{
	library.readdata();
	readseatlog();

	vector<userSeatData> toRemove;

	for (const auto& seat : m_seat)
	{
		if (seat.day >= 0 && seat.day < 7 && seat.floor > 0 && seat.floor <= FLOOR &&
			seat.hang > 0 && seat.hang <= HANG && seat.lie > 0 && seat.lie <= LIE)
		{
			string floorStr = library.libseatdata[seat.day][seat.floor - 1];
			vector<string> seats;
			istringstream iss(floorStr);
			string seatStr;

			int currentHang = 1;
			while (iss >> seatStr)
			{
				if (currentHang == seat.hang)
				{
					if (seatStr[seat.lie - 1] == '0' || seatStr[seat.lie - 1] == '2' || seatStr[seat.lie - 1] == '*')
					{
						cout << "Your seat of " << weeklist[seat.day] << " floor "
							<< seat.floor << " seat " << seat.hang << " " << seat.lie
							<< " has been canceled" << endl;
						toRemove.push_back(seat);
					}
					break;
				}
				currentHang++;
			}
		}
	}

	// 移除被取消的座位
	for (const auto& seatToRemove : toRemove)
	{
		for (auto it = m_seat.begin(); it != m_seat.end(); )
		{
			if (it->day == seatToRemove.day && it->floor == seatToRemove.floor &&
				it->hang == seatToRemove.hang && it->lie == seatToRemove.lie)
			{
				it = m_seat.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	if (!toRemove.empty())
	{
		savaseatlog();
	}
}
void User::checkNotes()
{
	ifstream fin("usernotes.txt");
	if (!fin.is_open()) {
		cout << "No notes from admin." << endl;
		return;
	}

	string line;
	bool found = false;
	cout << "=== Notes from Admin ===" << endl;
	while (getline(fin, line)) {
		// 查找以当前用户名开头的行
		if (line.find(m_name + ": ") == 0) {
			// 输出消息（去掉用户名和冒号）
			string note = line.substr(m_name.length() + 2);
			cout << note << endl;
			found = true;
		}
	}
	fin.close();

	if (!found) {
		cout << "No notes for you." << endl;
	}
	cout << "=======================" << endl;
}


void mainload()
{
	cout << "==========================================" << endl << "         Welcome to the library           " <<endl << "===========================================" << endl;
	Mainmenu mainmenu;
	Library library;
	User user;
	Admin admin;
	string tempname, temppass;
	admin.readuserfeedback(); 
	vector<string> insdata;
	string tempins;
	mainmenu.showmainins();
	string ins;
	user.readdata();
	user.readseatlog();
	library.readdata();
	admin.readmainnotice();
	if (!admin.mainnotice.empty()) {
		cout << "=== Main Notice ===" << endl;
		cout << admin.mainnotice << endl;
		cout << "===================" << endl;
	}
	while (1) {
	BEGIN:
		admin.readMaintenanceMode();
		if (admin.isMaintenanceMode()) {
			cout << "=== SYSTEM MAINTENANCE MODE ===" << endl;
			cout << "User login is currently disabled." << endl;
			cout << "===============================" << endl;
		}
		getline(cin, ins);
		if (ins == "Quit") { user.saveuserdata(); user.savaseatlog(); library.savedata();  admin.saveUserFeedback(); return; }
		else if (ins == "Help") { mainmenu.showmainins(); }
		else if (ins == "Register")
		{
			if (admin.isMaintenanceMode()) {
				cout << "Registration is disabled during maintenance mode." << endl;
				continue;
			}
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
			if (admin.isMaintenanceMode() && tempname != "Admin") {
				cout << "System is under maintenance. Please try again later." << endl;
				cin.ignore(); // 清除输入缓冲区
				continue;
			}
			switch (user.login(tempname, temppass))
			{
			case 0:break;
			case 1:
				user.is_cancel(library);
				admin.readusernotice();
				if (!admin.usernotice.empty()) {
					cout << "=== User Notice ===" << endl;
					cout << admin.usernotice << endl;
					cout << "===================" << endl;
				}
				mainmenu.showuserins();
				while (1) {
					if (cin.peek() == '/n') { cin.ignore(); }
					getline(cin, ins);
					istringstream isstr(ins); insdata.clear();
					while (isstr >> tempins) { insdata.push_back(tempins); }
					if (insdata.size() == 0) {}
					else if (ins == "Quit") { user.saveuserdata(); user.savaseatlog(); library.savedata();  admin.saveUserFeedback(); return; }
					else if (ins == "Exit") { user.saveuserdata(); user.savaseatlog(); library.savedata(); goto BEGIN; }
					else if (ins == "Reservation") { user.readseatlog(); user.reservation(); }
					else if (ins == "Feedback") { user.feedback(admin); }
					else if (ins == "Feedback") { user.feedback(admin); }
					else if (ins == "Check Replies") {user.checkFeedbackReplies();}
					else if (ins == "Check Notes") {user.checkNotes();}
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
					else if (ins == "Quit") { user.saveuserdata(); user.savaseatlog(); library.savedata();  admin.saveUserFeedback(); return; }
					else if (ins == "Exit") { user.saveuserdata(); user.savaseatlog(); library.savedata(); goto BEGIN; }
					else if (ins == "Help") { mainmenu.showadminins(); }
					else if (ins == "Clear") { admin.clearall(library, user); }
					else if (ins == "View User List") {
						admin.viewUserList(user);
					}
					else if (insdata.size() == 4 && insdata[0] == "Reset" && insdata[1] == "User" && insdata[2] == "Password") {
						admin.resetUserPassword(user, insdata[3], "default123"); // 重置为默认密码
					}
					else if (insdata.size() == 5 && insdata[0] == "Reset" && insdata[1] == "User" && insdata[2] == "Password") {
						admin.resetUserPassword(user, insdata[3], insdata[4]); // 重置为指定密码
					}
					else if (ins == "View Seat Log") {
						admin.viewSeatLog();
					}
					else if (ins == "Check Feedback") {
						admin.checkuserfeedback();
					}
					else if (ins == "Publish Main Notice") {
						admin.publishmainnotice();
					}
					else if (ins == "Publish User Notice") {
						admin.publishusernotice();
					}
					else if (ins == "Maintenance On") {
						admin.enableMaintenanceMode();
					}
					else if (ins == "Maintenance Off") {
						admin.disableMaintenanceMode();
					}
					else if (ins == "Maintenance Status") {
						cout << "Maintenance mode is " << (admin.isMaintenanceMode() ? "ON" : "OFF") << endl;
					}
					else if (insdata.size() >= 3 && insdata[0] == "Reply" && insdata[1] == "to") {
						// 格式：Reply to [index] [message]
						try {
							int index = stoi(insdata[2]);
							string reply;
							for (size_t i = 3; i < insdata.size(); ++i) {
								reply += insdata[i] + " ";
							}
							admin.replyToFeedback(index, reply);
						}
						catch (...) {
							cout << "Invalid feedback index!" << endl;
						}
					}
					else if (insdata.size() == 8 && insdata[0] == "Reservefor")
					{
						if (is_week(insdata[2]) && insdata[3] == "Floor" && insdata[5] == "Seat")
						{
							admin.reserveForUser(user, library, insdata[1], is_week(insdata[2]) - 1,
								stoi(insdata[4]), stoi(insdata[6]), stoi(insdata[7]));
						}
						else { cout << "Invalid instruction" << endl; }
					}
					else if (insdata.size() == 7 && insdata[0] == "Cancel")
					{
						if (insdata[2] == "Floor" && insdata[4] == "Seat" && is_week(insdata[1]))
						{
							admin.cancelSeat(user, library, is_week(insdata[1]) - 1,
								stoi(insdata[3]), stoi(insdata[5]), stoi(insdata[6]));
						}
					}
					else if (insdata.size() == 6 && insdata[0] == "Set" )
					{
						if (is_week(insdata[1]) && insdata[2] == "Floor" && insdata[4] == "as" && insdata[5] == "using")
						{
							admin.setSeatAsUsing(library, is_week(insdata[1]) - 1,stoi(insdata[3]));
						}
					}
					else if (insdata.size() >= 4 && insdata[0] == "Send" )
					{
						if (insdata[1] == "note" && insdata[2] == "to")
						{
							string username = insdata[3];
							string note;
							for (size_t i = 4; i < insdata.size(); ++i)
							{
								note += insdata[i] + " ";
							}
							admin.publishNoteToUser(username, note);
						}
					}
					else if (ins == "Check Feedback")
					{
						admin.checkFeedback();
					}
					else if (ins == "Seat Layout Compilation Mode")
					{
						admin.enterEditMode(library,user);
					}
					else { cout << "invalid instruction" << endl; }
				}
			}
		}
		else { cout << "Invalid instruction!" << endl; }
	}
}
int main()
{
	mainload();
	return 0;
}
