#include<stdlib.h>
#include<set>
#include<map>
#include<stack>
#include<string>
#include<vector>
#include<limits.h>
#include<string.h>
#include<fstream> 
#include<iostream>
#include<algorithm>
#ifndef NUM
#define NUM 10000
#endif // !NUM


using namespace std;
vector<unsigned long long> date;
vector<int> id;
vector<string> from;
vector<string> to;
vector<string> word;
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
bool add(const char path[32]) {
	fstream f;
	f.open(path, ios::in);
	if (!f.is_open()) {
		return 0;//read file fail
	}
	char trash[64];
	string push;
	f >> trash;//from
	f >> push;
	from.push_back(push);

	f >> trash;//date
	f.getline(trash, 64);
	date.push_back(convert_date(trash));

	f >> trash;//ID
	int t_id;
	f >> t_id;
	id.push_back(t_id);

	push = "";
	f >> trash;
	f.ignore();
	getline(f, push);
	int s = 0, count = 0;
	for (int z = 0; z < push.length(); z++) {
		count++;
		if (!(isalpha(push[z]) || isdigit(push[z]))) {
			push[z] = ' ';
			if (count > 1) {
				word.push_back(push.substr(s, count - 1));
			}
			s = z + 1;
			count = 0;
		}
	}
	

	push = "";
	f >> trash;//To
	f >> push;
	to.push_back(push);

	string temp;
	while (getline(f, temp)) {
		int i = 0, j = 0;
		while (j < temp.length()) {
			while (isalpha(temp[j]) || isdigit(temp[j])) { 
				j++; 
			}
			if (i != j) {//this is segment of content
				string inserting_element = string(temp.begin() + i, temp.begin() + j);
				word.push_back(inserting_element);
			}
			j++;
			i = j;
		}
	}
	f.close();
	return 1;
}
int main() {
	srand(time(NULL));
	id.reserve(10000);
	date.reserve(10000);
	from.reserve(10000);
	to.reserve(10000);
	word.reserve(2000000);
	for (int z = 1; z <= 10000; z++) {
		if (z % 500 == 0) {
			cout << z << endl;
		}
		string path = "./mail";
		path += to_string(z);
		add(path.c_str());
	}
	for (int z = 0; z < NUM; z++) {	
		int command = rand() % 4;
		if (command == 0) {
			cout << "add " << "./mail" << to_string(rand() % 10000 + 1) << endl;
		}
		else if (command == 1) {
			cout << "remove " << id[rand()%id.size()] << endl;
		}
		else if (command == 2) {
			cout << "longest\n";
		}
		else {
			cout << "query ";
			if (rand() % 2) {//f
				cout << "-f\"" << from[rand() % from.size()]<<"\" ";
			}
			if (rand() % 2) {//t
				cout << "-t\"" << to[rand() % to.size()] <<"\" ";
			}
			if (rand() % 2) {//d
				bool flag = true;
				unsigned long long start = 0 ,end = 0;
				cout << "-d";
				if (rand() % 2) {
					start = date[rand() % date.size()];
					flag = false;
				}
				
				if (rand() % 2 || flag) {
					end = date[rand() % date.size()];
					if (end < start) {
						swap(end, start);
					}
				}
				if (start != 0) {
					cout << start;
				}
				cout << "~";
				if (end != 0) {
					cout << end;
				}
				cout << " ";
			}
			switch (rand() % 3) {
			case 0:
				cout << "!" <<word[rand()%word.size()]<< endl;
				break;
			case 1:
				cout << word[rand() % word.size()] << "&" << word[rand() % word.size()] << endl;
				break;
			case 2:
				cout << word[rand() % word.size()] << "|" << word[rand() % word.size()] << endl;
				break;
			}
		}
	}
	return 0;
}