#include<set>
#include<map>
#include<vector>
#include<string>
#include<bitset>
#include<thread>
#include<fstream>
#include<climits>
#include<iostream>
#include<algorithm>
#include<unordered_map>
#include"expression.cpp"
#include <mutex>
#include<pthread.h>
#define print(x) cout<<(x)<<endl;
using namespace std;
//==========================================
struct mail {
	string from, to;
	int id, length;
	unsigned long long date;
	bool currently_valid;
	mail() {};
};
//==========================================
#define MAIL_SIZE 10100 
unsigned long long convert_date(const char*);
void dic_insert(const char*, mail*);
bitset<MAIL_SIZE> allid;
bitset<MAIL_SIZE> zero;
mail* container[MAIL_SIZE];
unordered_map<string, bitset<MAIL_SIZE>> dictionary;
int output_C = 0;
mutex locker;
struct length_compare {
    bool operator() (const mail* a, const mail* b) const {
        if(a ->length == b->length)return a->id < b->id;
        return a->length > b->length;
    }
};
set<mail*,length_compare> lengthset;

void TOLOW(string &s){
	for(int i=0;i<s.length();i++){
		if(s[i]<='Z' && s[i]>='A')s[i]+=32;
	}
}
void TOLOW(char *s){
	while(*s){
		if(*s<='Z' && *s>='A')*s+=32;
		s++;
	}
}
void dic_insert(const char* str, mail* M) {
	string in(str);
	dictionary[in].set(M->id, 1);
}
int add(struct mail *M){
	char path[32];
	cin >> path;
	fstream f;
	f.open(path,fstream::in);
	if(!f.is_open())return 0;//read file fail
	char trash[64];
	M->length = 0;

	f >> trash;//from
	f >> M->from;
	TOLOW(M->from);
	
	f >> trash;//date
	f.getline(trash,64);
	
	
	M->date=convert_date(trash);

	f >>trash;//ID
	f >>M->id;
	
	mail* z=container[M->id];
	if(z!=NULL){
		if (z->currently_valid == 1) {
			return 0;
		}
		else {
			return 2;
		}
	}
	f >>trash;
	f.ignore();
	string splitsb;
	getline(f,splitsb);
	TOLOW(splitsb);

	f >>trash;//To
	f >>M->to;
	TOLOW(M->to);
	
	string temp;
	for(int i=0;i<splitsb.length();i++){
		int j = i;
		while (isalnum(splitsb[j])) { 
			++j; 
		}
		if (j == i) {
			continue;
		}
		splitsb[j] = '\0';
		dic_insert(splitsb.c_str()+i,M);
		i = j;
	}

	f>>trash;
	while(getline(f,temp)){
		int i=0,j=0;
		while(j<temp.length()){
			while(isalnum(temp[j]))j++;
			if(i!=j){
				string inserting_element=string(temp.begin()+i,temp.begin()+j);
				TOLOW(inserting_element);
				dic_insert(inserting_element.c_str(),M);
				M->length+=inserting_element.length();
			}
			j++;
			i = j;
		}
	}
	f.close();
	return 1;
}
bitset<MAIL_SIZE> evaluate(vector<string> &post){
	stack<bitset<MAIL_SIZE>> calculator;
	for(string &seg:post){
		if(seg=="")continue;
		if(isalnum(seg[0])){
			if (dictionary.find(seg) == dictionary.end()) {
				bitset<MAIL_SIZE> temp;
				calculator.push(temp);
			}
			else {
				calculator.push(dictionary[seg]);
			}
		}
		else {
			if(seg=="!"){
				(calculator.top()).flip();
				calculator.top() &= allid;
			}
			else if(seg=="&" || seg=="|"){
				bitset<MAIL_SIZE> temp = calculator.top();
				calculator.pop();
				if(seg=="&"){
					calculator.top() &= temp;
				}
				else {
					calculator.top() |= temp;
				}
			}
		}
	}
	if(calculator.empty()){
		return zero;
	}
	return calculator.top();
}
void query(int id, string& condition){
	int i=0;
	TOLOW(condition);
	while (condition.back() == '\n' || condition.back() == '\r' || condition.back() == ' ') {
		condition.pop_back();
	}
	string f,t;
	bool hasf=0,hast=0,hasL=0,hasR=0;
	unsigned long long L=0,R=ULLONG_MAX;
	vector<string> ex;
	while( i < condition.length() ){
		if(condition[i]==' '||condition[i]=='\n'){
			i++;
			continue;
		}
		if (condition[i]=='-'){
			if(condition[i+1]=='f'){
				i+=3;//"
				hasf=1;
				while(condition[i]!='\"'){
					f.push_back(condition[i]);
					i++;
				}
			}
			else if (condition[i+1]=='t'){
				i+=3;//"
				hast=1;
				while(condition[i]!='\"'){
					t.push_back(condition[i]);
					i++;
				}
			}
			else if (condition[i+1]=='d'&&(condition[i+2]=='~' ||isdigit(condition[i+2]))){
					i+=2;
					if(isdigit(condition[i])){
						sscanf(condition.c_str()+i,"%llu",&L);
						hasL=1;
						while(condition[i]!='~')i++;
					}
					i++;
					if(isdigit(condition[i])){
							sscanf(condition.c_str()+i,"%llu",&R);
							hasR=1;
					}
			}
		}
		else{
			int j=i;
			while(condition[j]!=' ' && j<condition.length())j++;
			topost2(string(condition.begin()+i,condition.begin()+j),ex);
		}
		while(i<condition.length() && condition[i]!=' ')i++;
		i++;
	}
	bitset<MAIL_SIZE> re;
	if (ex.size() == 0) {
		re = allid;
	}
	else {
		re = evaluate(ex);
	}
	while (id > output_C) {
		this_thread::yield();
	}
	locker.lock();
	int n = 0;
	for (int i = 0; i < MAIL_SIZE; i++) {
		if (re[i]) {
			mail* k = container[i];
			if ( (!k->currently_valid) || (hasf && k->from != f) || (hast && k->to != t) || (hasL && k->date < L) || (hasR && k->date > R) ) {
				continue;
			}
			if (n) {
				cout << ' ';
			}
			n++;
			cout << k->id;
		}
	}
	if (n == 0) {
		cout << "-";
	}
	cout<<"\n";
	output_C = id + 1;
	locker.unlock();
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	set_precedence();
	for (int i = 0; i < MAIL_SIZE; i++) {
		container[i] = NULL;
	}
	bool parallel = false;
	bool good = true;
	string input;
	while (good){
		if (!parallel) {
			if (!(cin >> input)) {
				good = false;
				continue;
			}
		}
		else {
			parallel = false;
		}
		if(input=="add"){
			mail* t=new mail();
			int state=add(t);
			if(!state){
				delete(t);
				cout <<"-\n";
			}
			else if(state==2){
				mail* z=container[t->id];
				delete(t);
				z->currently_valid=1;
				lengthset.insert(z);
				allid.set(z->id, 1);
				cout << lengthset.size() << endl;
			}
			else{
				container[t->id]=t;
				t->currently_valid=1;
				lengthset.insert(t);
				allid.set(t->id, 1);
				cout << lengthset.size() << endl;
			}
		}
		else if (input== "remove"){
			int removing_id;
			cin >>removing_id;
			auto p = container[removing_id];
			//set iterator
			if(p==NULL || p->currently_valid==0){
				cout <<"-\n";
			}
			else{
				p->currently_valid = 0;
				lengthset.erase(p);
				cout << lengthset.size() <<endl;
				allid.set(p->id, 0);
			}
		}
		else if(input == "longest"){
			auto iter=lengthset.begin();
			if(iter!=lengthset.end()){
				cout << (*iter)->id <<" "<< (*iter)->length <<endl;
			}
			else cout <<"-\n";
		}
		else if (input=="query"){
			thread* all_thread[64];
			int counter = 0;
			output_C = 0;
			while (input == "query" && counter < 64 ) {
				getline(cin, input);
				all_thread[counter] = new thread(query,counter, ref(input));
				counter++;
				if (!(cin >> input)) {
					good = false;
					input = "GG";
				}
			}
			for (int z = 0; z < counter; z++) {
				all_thread[z]->join();
			}
			parallel = true;
		}
	}
}
unsigned long long convert_date(const char *a){
	
	unsigned long long t=0;
	int i=0;
	while(a[i]==' ')i++;
	t+=(a[i++]-'0')*10000;//day
	if(isdigit(a[i]))t*=10,t+=(a[i++]-'0')*10000;//day

	while(a[i]==' ')i++;
	// cout << t<<endl;
	unsigned long long month=0;
	if(a[i]=='J'){
		if(a[i+1]=='a')month=1;
		else if(a[i+2] =='n')month=6;
		else month=7;
	}
	else if (a[i]=='F')month=2;
	else if (a[i]=='M'){
		if(a[i+2]=='r')month=3;
		else month=5;
	}
	else if (a[i]=='A'){
		if(a[i+1]=='p')month=4;
		else month=8;
	}
	else if (a[i]=='S')month=9;
	else if (a[i]=='O')month=10;
	else if (a[i]=='N')month=11;
	else if (a[i]=='D')month=12;
	else cout <<"month error";
	t+=month*1000000;
	// cout << t<<endl;
	while( isalpha(a[i]) ||a[i]==' ' )i++;
	unsigned long long year;
	//cout << "?";
	sscanf(a+i,"%llu",&year);
	t+=year*100000000;
	//cout << t<<endl ;
	while( isdigit(a[i]) ||a[i]==' ' )i++;
	i+=3; // skip "at"
	t+=(a[i++]-'0')*1000;
	t+=(a[i++]-'0')*100;
	i++; //:
	t+=(a[i++]-'0')*10;
	t+=(a[i++]-'0');
	// cout << t<<endl;
	return t;
}