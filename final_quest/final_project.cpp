#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <sstream> 
#include <set>
#include <map>
#include <vector>
#include <array>
#include <functional>
using namespace std;

class Commands {
public:
	static bool is_command(string command) {
		return find(commands.begin(), commands.end(), command) != commands.end();
	}
private:
	static const array<string, 4> commands;
};

const array<string, 4> Commands::commands = { "Add", "Del", "Find", "Print" };

class Date {
public:
	Date(int year, int month, int day) {
		if (1>month || month>12) {
			throw invalid_argument("Month value is invalid: " + to_string(month));
		}
		if (1 > day || day > 31) {
			throw invalid_argument("Day value is invalid: " + to_string(day));
		}
		this->year = year;
		this->month = month;
		this->day = day;
	}
	int Year() const{
		return this->year;
	}
	int Month() const {
		return this->month;
	}
	int Day() const {
		return this->day;
	}
private:
	int day;
	int month;
	int year;

};

class Reader {
public:
	static string read_command(stringstream& sstream) {
		string command;
		sstream >> command;
		if (!Commands::is_command(command)) {
			throw invalid_argument("Unknown command: " + command);
		}
		return command;
	}

	static Date read_date(stringstream& sstream) {
		int year, month, day;
		string input;
		sstream >> input;
		if (sscanf(input.c_str(), "%d-%d-%d", &year, &month, &day) < 3) {
			throw invalid_argument("Wrong date format: " + input);
		}
		return Date(year, month, day);
	}

	static string read_event(stringstream& sstream) {
		string event;
		getline(sstream, event);
		if (event != "") {
			event.erase(0, 1);
		}
		return event;
	}
};

class Writer {
public:
	static void write_date(Date date) {
		printf("%04d-%02d-%02d ", date.Year(), date.Month(), date.Day());
	}
	static void write_events(set<string> events) {
		for (auto event : events) {
			cout << event << ' ';
		}
		cout << endl;
	}
};

class BD {
public:
	BD() {
		this->commands = {
			{"Add",&BD::Add},
			{"Del",&BD::Del},
			{"Find", &BD::Find},
			{"Print", &BD::Print}
		};
	}
	void Start() {
		string str, command;
		while (true) {
			try
			{	
				getline(cin, str);
				if (str == "") {
					continue;
				}
				stringstream sstream;
				sstream.str(str);
				command = Reader::read_command(sstream);
				this->commands[command](this, sstream);
			}
			catch (const std::exception& e)
			{
				cout<<e.what()<<endl;
			}
		}
	}

private:
	map <string, std::function <void(BD*, stringstream&)>> commands;
	map <Date, set<string>> bd;

	void Add(stringstream& sstream) {
		Date date = Reader::read_date(sstream);
		string event = Reader::read_event(sstream);
		this->bd[date].insert(event);
	}

	void Del(stringstream& sstream) {
		Date date = Reader::read_date(sstream);
		string event = Reader::read_event(sstream);
		if (event != "") {
			if (this->bd.find(date) != this->bd.end() && this->bd[date].find(event) != this->bd[date].end()) {
				this->bd[date].erase(event);
				cout << "Deleted successfully" << endl;
				return;
			}
			else {
				cout << "Event not found" << endl;
				return;
			}
		}
		else {
			int n = 0;
			if (this->bd.find(date) != this->bd.end()) {
				n = this->bd[date].size();
				this->bd.erase(date);
			}
			cout << "Deleted "+to_string(n)+" events" << endl;
		}
	}

	void Find(stringstream& sstream) {
		Date date = Reader::read_date(sstream);
		Writer::write_events(this->bd[date]);
	}

	void Print(stringstream& sstream) {
		for (auto temp : this->bd) {
			Writer::write_date(temp.first);
			Writer::write_events(temp.second);
		}
	}
};

bool operator<(const Date& first, const Date& second)  {
	if (first.Year() == second.Year()) {
		if (first.Month() == second.Month()) {
			return first.Day() < second.Day();
		}
		return first.Month() < second.Month();
	}
	return first.Year() < second.Year();
}

int main() {
	BD bd;
	bd.Start();
	return 0;
}