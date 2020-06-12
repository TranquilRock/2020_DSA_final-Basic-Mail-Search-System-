#include<set>
#include<map>
#include<stack>
#include<string>
#include<vector>
#include<limits.h>
#include<fstream> 
#include<iostream>
#include<algorithm>
using namespace std;
typedef unsigned long long Date_type;
#define MAX_DATE 999999999999
#define print(x) cout<<(x);
class Mail{
public:
	int id, length;
	Date_type date;
	set<string> content;
	string from, to, subject;
	Mail(int ID = 0,int L = 0,int D = 0) :id(ID),length(L),date(D) {
	}
};
void Tolower(string& s) {
	for (auto z = s.begin(); z != s.end(); z++) {
		*z = tolower(*z);
	}
}
unsigned long long convert_date(const char* a) {
	unsigned long long t = 0;
	int i = 0;
	while (a[i] == ' ') { 
		i++; 
	}
	t += (a[i++] - '0') * 10000;//day
	if (isdigit(a[i])) {//day
		t *= 10, t += (a[i++] - '0') * 10000;
	}
	while (a[i] == ' ') {
		i++;
	}
	unsigned long long month = 0;
	if (a[i] == 'J') {
		if (a[i + 1] == 'a') {
			month = 1;
		}
		else if (a[i + 2] == 'n') {
			month = 6;
		}
		else {
			month = 7;
		}
	}
	else if (a[i] == 'F') {
		month = 2;
	}
	else if (a[i] == 'M') {
		if (a[i + 2] == 'r') {
			month = 3;
		}
		else {
			month = 5;
		}
	}
	else if (a[i] == 'A') {
		if (a[i + 1] == 'p') {
			month = 4;
		}
		else {
			month = 8;
		}
	}
	else if (a[i] == 'S') {
		month = 9;
	}
	else if (a[i] == 'O') {
		month = 10;
	}
	else if (a[i] == 'N') {
		month = 11;
	}
	else if (a[i] == 'D') {
		month = 12;
	}
	else {
		print("month error");
	}
	t += month * 1000000;
	while (isalpha(a[i]) || a[i] == ' ') { 
		i++; 
	}
	unsigned long long year;
	sscanf(a + i, "%llu", &year);
	t += year * 100000000;
	while (isdigit(a[i]) || a[i] == ' ') {
		i++;
	}
	i += 3; //"at"
	t += (a[i++] - '0') * 1000;
	t += (a[i++] - '0') * 100;
	i++; //:
	t += (a[i++] - '0') * 10;
	t += (a[i++] - '0');
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
	Tolower(M.from);

	f >> trash;//date
	f.getline(trash, 64);
	M.date = convert_date(trash);

	f >> trash;//ID
	f >> M.id;

	f >> trash;
	f.ignore();
	getline(f, M.subject);
	Tolower(M.subject);
	//
	M.content.emplace(M.subject);
	//

	f >> trash;//To
	f >> M.to;
	Tolower(M.to);

	M.length = 0;
	string temp;
	while (getline(f, temp)) {
		int i = 0, j = 0;
		while (j < temp.length()) {
			while (isalpha(temp[j]) || isdigit(temp[j])) { 
				j++; 
			}
			if (i != j) {//this is segment of content
				string inserting_element = string(temp.begin() + i, temp.begin() + j);
				Tolower(inserting_element);
				M.content.emplace(inserting_element);
				M.length += inserting_element.length();
			}
			j++;
			i = j;
		}
	}
	f.close();
	return 1;
}
struct length_compare {
	bool operator() (const Mail* a, const Mail* b) const {
		if ((a->length) == (b->length)) {
			return a->id < b->id;
		}
		return (a->length) > (b->length);
	}
};
struct date_compare {
	bool operator() (const Mail* a, const Mail* b) const {
		if (a->date == b->date) {
			return a->id < b->id;
		}
		return a->date < b->date;
	}
	bool operator() (const Mail* a, const Date_type& b) const {
		return a->date < b;
	}
};
struct id_compare {
	bool operator() (const Mail* a, const Mail* b) const {
		return a->id < b->id;
	}
};
int main() {
	map<string, set<Mail*, id_compare>> from;
	map<string, set<Mail*, id_compare>> to;
	map<int, Mail*> mails;
	set<Mail*, length_compare> length;
	set<Mail*, date_compare> date;
	//ios_base::sync_with_stdio(0); //sscanf
	cin.tie(0);
	string input;
	while (cin >> input) {
		if (input == "add") {
			Mail* new_mail = new Mail;
			if (add(*new_mail) && (mails.find(new_mail->id) == mails.end())) {
				length.emplace(new_mail);
				from[new_mail->from].emplace(new_mail);
				to[new_mail->to].emplace(new_mail);
				date.emplace(new_mail);
				mails[new_mail->id] = (new_mail);
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
				Mail* target = mails[remove_id];
				length.erase(target);
				from[mails[remove_id]->from].erase(target);
				to[mails[remove_id]->to].erase(target);
				date.erase(target);
				mails.erase(remove_id);
				cout << mails.size() << endl;
			}
			else {
				cout << "-\n";
			}
		}
		else if (input == "longest") {
			if (length.size() == 0) {
				cout << "-\n";
			}
			else {
				cout << (*length.begin())->id << (*length.begin())->length << endl;
			}
		}
		else if (input == "query") {
			bool flag = false;
			set<Mail*,id_compare> candidate;
			while (cin >> input) {
				Tolower(input);
				if (input[0] == '-') {
					if (input[1] == 'f') {//from
						string name(input.begin() + 2, input.end());
						if (!flag) {
							candidate = from[name];
						}
						else {
							print("error\n");
						}
					}
					else if (input[1] == 't') {//to
						string name(input.begin() + 2, input.end());
						if (!flag) {
							candidate = from[name];
						}
						else {
							set<Mail*, id_compare> temp;
							set_intersection(candidate.begin(), candidate.end(), to[name].begin(), to[name].end(), inserter(temp,temp.begin()));
							candidate = temp;
						}
					}
					else if (input[1] == 'd') {//date
						Date_type start = 0, end = 0;
						int i = 2;
						while (i < input.length() && input[i] != '~') {
							start += input[i] - '0';
							i++;
						}
						i++;//~
						if (i >= input.length()) {
							end = MAX_DATE;
						}
						else {
							while (i < input.length()) {
								end += input[i] - '0';
								i++;
							}
						}
						Mail lower(0, 0, start);
						Mail upper(INT_MAX, 0, end);
						set<Mail*,id_compare> temp(lower_bound(date.begin(), date.end(),&lower), upper_bound(date.begin(), date.end(),&upper));
						if (!flag) {
							candidate = temp;
						}
						else{
							set<Mail*,id_compare>temp2;
							set_union(temp.begin(), temp.end(), candidate.begin(), candidate.end(), inserter(temp2, temp2.begin()));
							candidate = temp2;
						}
					}
					flag = true;
				}
				else {
					vector<string> calculator;
					stack<string> operand;
					for (int z = 0; z < input.length(); z++) {
						if (isalpha(input[z])) {
							string key;
							while (z < input.length() && isalpha(input[z])) {
								key += input[z];
								z++;
							}
							calculator.push_back(key);
						}
						else {
							string to_push;
							if (input[z] == '(') {
								to_push = "(";
							}
							else if (input[z] == '!') {
								to_push = "!";
							}
							else if (input[z] == ')') {
								while (operand.top() != "(") {
									calculator.push_back(operand.top());
									operand.pop();
								}
								operand.pop();
							}
							else{//& |
								while ((!operand.empty()) && (operand.top() != "(")) {
									calculator.push_back(operand.top());
									operand.pop();
								}
							}
							operand.push(to_push);
						}
					}
					while ((!operand.empty())) {
						calculator.push_back(operand.top());
						operand.pop();
					}
					bool nothing = true;
					for (auto c = candidate.begin(); c != candidate.end(); c++) {
						bool space_flag = false;
						stack<bool> result;
						for (int z = 0; z < calculator.size(); z++) {
							if (isalpha(calculator[z][0])) {
								if ((*c)->content.find(calculator[z]) != (*c)->content.end()) {
									result.push(true);
								}
								else {
									result.push(false);
								}
							}
							else if (calculator[z] == "!") {
								bool p = result.top();
								result.pop();
								result.push(!p);
							}
							else if (calculator[z] == "&") {
								bool p1 = result.top();
								result.pop();
								bool p2 = result.top();
								result.pop();
								result.push(p1 && p2);
							}
							else if (calculator[z] == "|") {
								bool p1 = result.top();
								result.pop();
								bool p2 = result.top();
								result.pop();
								result.push(p1 || p2);
							}
						}
						if (result.top()) {
							if (space_flag) {
								cout << " ";
							}
							else {
								space_flag = true;
								nothing = false;
							}
							cout << (*c)->id;
						}
						
					}
					if (nothing) {
						cout << "-\n";
					}
					break;
				}
			}
			
		}
	}
}