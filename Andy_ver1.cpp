#include <iostream>
#include <fstream> 
#include<set>
#include<map>
#include<string>
#include<vector>
#define print(x) cout<<(x);
using namespace std;

class Mail{
public:
	int id, length;
	unsigned long long date;
	vector<string> content;
	string from, to, subject;
	/*
	bool operator< (const Mail& a) {
		if (this->date == a.date) {
			return this->id < a.id;
		}
		else {
			return this->date < a.date;
		}
	}
	bool operator> (const Mail& a) {
		if (this->date == a.date) {
			return this->id > a.id;
		}
		else {
			return this->date > a.date;
		}
	}*/
	Mail(int a = 0) :id(a) {
		length = 0;
		date = 0;
	}
};
unsigned long long convert_date(const char* a) {
	unsigned long long t = 0;
	int i = 0;
	while (a[i] == ' ')i++;
	t += (a[i++] - '0') * 10000;//day
	if (isdigit(a[i]))t *= 10, t += (a[i++] - '0') * 10000;//day
	while (a[i] == ' ')i++;
	// cout << t<<endl;
	unsigned long long month = 0;
	if (a[i] == 'J') {
		if (a[i + 1] == 'a')month = 1;
		else if (a[i + 2] == 'n')month = 6;
		else month = 7;
	}
	else if (a[i] == 'F')month = 2;
	else if (a[i] == 'M') {
		if (a[i + 2] == 'r')month = 3;
		else month = 5;
	}
	else if (a[i] == 'A') {
		if (a[i + 1] == 'p')month = 4;
		else month = 8;
	}
	else if (a[i] == 'S')month = 9;
	else if (a[i] == 'O')month = 10;
	else if (a[i] == 'N')month = 11;
	else if (a[i] == 'D')month = 12;
	else cout << "month error";
	t += month * 1000000;
	// cout << t<<endl;
	while (isalpha(a[i]) || a[i] == ' ')i++;
	unsigned long long year;
	//cout << "?";
	sscanf(a + i, "%llu", &year);
	t += year * 100000000;
	//cout << t<<endl ;
	while (isdigit(a[i]) || a[i] == ' ')i++;
	i += 3; // skip "at"
	t += (a[i++] - '0') * 1000;
	t += (a[i++] - '0') * 100;
	i++; //:
	t += (a[i++] - '0') * 10;
	t += (a[i++] - '0');
	// cout << t<<endl;
	return t;
}
bool add(Mail& M) {
	char path[32];
	cin >> path;
	fstream f;
	f.open(path, ios::in);
	if (!f.is_open()){
		return 0;//read file fail
	}
	char trash[64];

	f >> trash;//from
	f >> M.from;

	f >> trash;//date
	f.getline(trash, 64);
	M.date = convert_date(trash);

	f >> trash;//ID
	f >> M.id;

	f >> trash;
	f.ignore();
	getline(f, M.subject);

	f >> trash;//To
	f >> M.to;

	M.length = 0;
	string temp;
	while (getline(f, temp)) {
		int i = 0, j = 0;
		while (j < temp.length()) {
			while (isalpha(temp[j]) || isdigit(temp[j]))j++;
			if (i != j) {
				//this is segment of content
				string inserting_element = string(temp.begin() + i, temp.begin() + j);
				// cout << "inserted " << inserting_element << endl;
				M.content.push_back(inserting_element);
				M.length += inserting_element.length();
			}
			j++;
			i = j;
		}
	}
	f.close();
	//print("GOOD\n");
	return 1;
}


//set<Mail*, date_compare> mails;
struct length_compare {
	bool operator() (const Mail* a, const Mail* b) const {
		if ((a->length) == (b->length)) {
			return a->id < b->id;
		}
		return (a->length) > (b->length);
	}
};
struct date_compare {
	bool operator() (const Mail& a, const Mail& b) const {
		if (a.date == b.date) {
			return a.id < b.id;
		}
		return a.date < b.date;
	}
};
int main() {
	map<string, set<int>> from;
	map<string, set<int>> to;
	map<int, Mail*> mails;
	set<Mail*, length_compare> lengthset;
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	string input;
	while (cin >> input) {
		if (input == "add") {
			Mail* new_mail = new Mail;
			if (add(*new_mail) && (mails.find(new_mail->id) == mails.end())) {
				lengthset.emplace(new_mail);
				mails[new_mail->id] = (new_mail);
				from[new_mail->from].emplace(new_mail->id);
				to[new_mail->to].emplace(new_mail->id);
				cout << mails.size() << endl;
			}
			else {
				delete (new_mail);
				cout << "-\n";
			}
		}
		else if (input == "remove") {
			int remove_id;
			cin >> remove_id;
			if (mails.find(remove_id) != mails.end()) {
				Mail phony(remove_id);
				lengthset.erase(&phony);
				from[mails[remove_id]->from].erase(remove_id);
				to[mails[remove_id]->to].erase(remove_id);
				mails.erase(remove_id);
				cout << mails.size() << endl;
			}
			else {
				cout << "-\n";
			}
		}
		else if (input == "longest") {
			if (lengthset.size() == 0) {
				cout << "-\n";
			}
			else {
				auto iter = lengthset.begin();
				while (iter != lengthset.end() &&
					(*iter)->length == (*lengthset.begin())->length) {
					cout << (*iter)->id << (*iter)->length << '\n';
					break;
				}
			}
		}
		else if (input == "query") {
			getline(cin, input);
			int i = 0;
			while (i < input.length()) {
				if (input[i] == '-') {
					if (input[i + 1] == 'f') { ; }
					else if (input[i + 1] == 'd') { ; }
					else if (input[i + 1] == 't') { ; }
				}
				else {
					;
				}
				while (i < input.length() && input[i] != ' ')i++;
				i++;
			}
			cout << "q\n";
		}
	}
}