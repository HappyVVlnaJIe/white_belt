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
#include <iomanip>
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
		vector<int> date;
		string input;
		sstream >> input;
		////сделать посимвольный 
		//if (sscanf(input.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
		//	throw invalid_argument("Wrong date format: " + input);
		//}
		//if (input.size() != (to_string(year).size()+ to_string(month).size() + to_string(day).size() + 2 + std::count(input.begin(),input.end(),'+'))) {
		//	throw invalid_argument("Wrong date format: " + input);
		//}
		enum States {
			first_symbol,
			second_symbol,
			only_numbers
		};
		States current_state = first_symbol;
		string numb="";
		char c;
		int i = 0, k=0;
		while (i < input.size()) {
			switch (current_state)
			{
			case first_symbol:
				c = input[i];
				if (c != '-' && !isdigit(c) && c != '+') {
					throw invalid_argument("Wrong date format: " + input);
				}
				numb += c;
				i++;
				if (!isdigit(c)) {
					current_state = second_symbol;
				}
				else {
					current_state = only_numbers;
				}
				break;

			case second_symbol:
				c = input[i];
				if (!isdigit(c)) {
					throw invalid_argument("Wrong date format: " + input);
				}
				numb += c;
				i++;
				current_state = only_numbers;
				break;

			case only_numbers:
				c = input[i];
				if (!isdigit(c) && c != '-') {
					throw invalid_argument("Wrong date format: " + input);
				}
				if (isdigit(c)) {
					numb += c;
					i++;
				}
				if (c == '-') {
					k++;
					date.push_back(stoi(numb));
					numb = "";
					i++;
					current_state = first_symbol;
				}
				break;

			default:
				break;
			}
		}
		if (date.size() != 2 || k!=2 || numb.size()==0) {
			throw invalid_argument("Wrong date format: " + input);
		}
		date.push_back(stoi(numb));
		return Date(date[0],date[1],date[2]);
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
	static void write_events_with_date(Date date, set<string> events) {
		for (auto event : events) {
			cout << setfill('0');
			cout << setw(4) << date.Year() << '-' << setw(2) << date.Month() << '-' << setw(2) << date.Day()<<' ';
			cout << event << endl;
		}
	}
	static void write_events(set<string> events) {
		for (auto event : events) {
			cout << event << endl;
		}
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
		while (getline(cin, str)) {
			try
			{	
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
			Writer::write_events_with_date(temp.first, temp.second);
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