#include<bits/stdc++.h>
using namespace std;
struct mail{
	string from,to,subject;
	int id,length;
	unsigned long long date;
	//ex:   19 May 2011 at 16:50
	//   -> 201105191650
	set<string> content;
};
struct date_compare {
    bool operator() (const mail* a, const mail* b) const {
        if(a ->date == b->date)return a->id < b->id;
        return a->date < b->date;
    }
};
struct id_compare {
    bool operator() (const mail* a, const mail* b) const {
        return a->id < b->id;
    }
};
struct from_compare {
    bool operator() (const mail* a, const mail* b) const {
        if(a ->from == b->from)return a->id < b->id;
        return a->from < b->from;
    }
};
struct to_compare {
    bool operator() (const mail* a, const mail* b) const {
        if(a ->to == b->to)return a->id < b->id;
        return a->to < b->to;
    }
};
struct subject_compare {
    bool operator() (const mail* a, const mail* b) const {
        if(a ->subject == b->subject)return a->id < b->id;
        return a->subject < b->subject;
    }
};
struct length_compare {
    bool operator() (const mail* a, const mail* b) const {
        if(a ->length == b->length)return a->id < b->id;
        return a->length > b->length;
    }
};
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
bool add(struct mail *M){
	char path[32];
	cin >> path;
	fstream f;
	f.open(path,fstream::in);
	if(!f.is_open())return 0;//read file fail
	char trash[64];
	
	f >> trash;//from
	f >> M->from;
	// cout <<"sender "<< M->from << endl;
	
	f >> trash;//date
	f.getline(trash,64);
	M->date=convert_date(trash);
	//convert date to number
	// cout<<"date " << M->date << endl;

	f >>trash;//ID
	f >>M->id;
	// cout <<"id "<< M->id << endl;

	f >>trash;
	f.ignore();
	getline(f,M->subject);
	// cout <<"subject "<< M->subject<<endl;

	f >>trash;//To
	f >>M->to;
	// cout << M->to << " to\n";

	M->length=-7;
	string temp;
	while(getline(f,temp)){
		int i=0,j=0;
		while(j<temp.length()){
			while(isalpha(temp[j]) || isdigit(temp[j]))j++;
			if(i!=j){
				//this is segment of content
				string inserting_element=string(temp.begin()+i,temp.begin()+j);
				// cout << "inserted " << inserting_element << endl;
				M->content.insert(inserting_element);
				M->length+=inserting_element.length();
			}
			i=++j;

		}
	}
	//f >>
	f.close();
	return 1;
}

set<mail*,date_compare> dateset;
set<mail*,id_compare> idset;
set<mail*,from_compare> fromset;
set<mail*,to_compare> toset;
set<mail*,subject_compare> subjectset;
//increasing order

set<mail*,length_compare> lengthset;//decreasing

int priority(const string& input)
{
	if(input == "!") return 3;
	if(input == "&") return 10;
	if(input == "|") return 12;
	if(input == "(") return 16;
	return 0;
}

void verify(stack<string>& s, const string& input, deque<string>& output1)
{
	if(!s.empty() && !(priority(input) == 3&&priority(s.top()) == 3  ))
		while(!s.empty() && (priority(s.top()) <= priority(input))){
			output1.push_back(s.top());
			s.pop();
		}
	s.push(input);
}

/*void eval(deque<string>& output1)
{
	stack<string> s;
	vector<int> ansid;
	while(!output1.empty()){
		if(output1.front()=='!')
			do_one(s, output1);
		else if(output1.front()=='&'||output1.front()=='|')
			do_binary(s, output1);
		else
			s.push(output1.front().c_str());
		output1.pop_front();
	}
}*/

void query(string &condition){
	//cout<<condition;
	set<mail*,id_compare> ansid;
	auto iter=lengthset.begin();
	while(iter!=lengthset.end() ){
		ansid.insert(*iter);
		iter++;
	}

	int i=0;
	string from={},datebegin={},dateend={},to={},allkey={};
	while( i < condition.length() ){
		if (condition[i]=='-'){
			if(condition[i+1]=='f'){
				printf("1");
				int j=i+3,k=0;
				while(condition[j]!='\"'){
					from+=condition[j];
					k++;j++;
				}
				auto iter=ansid.begin();
				while(iter!=ansid.end() ){
					if((*iter)->from != from){
						ansid.erase(*iter);
					}
					iter++;
				}
			}
			else if (condition[i+1]=='d'){
				if(condition[i+2]=='~'){
					for(int j=1;j<=12;j++){
						dateend+=condition[i+2+j];
					}
					auto iter=ansid.begin();
					while(iter!=ansid.end() ){
						if((*iter)->date >stoi(dateend)){
							ansid.erase(*iter);
						}
						iter++;
					}
				}else{
					for(int j=1;j<=12;j++){
						datebegin+=condition[i+1+j];
					}
					auto iter=ansid.begin();
						while(iter!=ansid.end() ){
							if((*iter)->date <stoi(datebegin)){
								ansid.erase(*iter);
							}
							iter++;
						}	
					if(condition[i+14]=='~'){
						if(isdigit(condition[i+15])){
							for(int j=1;j<=12;j++){
								dateend+=condition[i+14+j];
							}	
						}
						auto iter=ansid.begin();
						while(iter!=ansid.end() ){
							if((*iter)->date >stoi(dateend)){
								ansid.erase(*iter);
							}
							iter++;
						}	
					}
				}
			}
			else if (condition[i+1]=='t'){
				int j=i+3,k=0;
				while(condition[j]!='\"'){
					to+=condition[j];
					k++;j++;
				}
				auto iter=ansid.begin();
						while(iter!=ansid.end() ){
							if((*iter)->to !=to){
								ansid.erase(*iter);
							}
							iter++;
						}	
			}
			
		}
		else{
			if(condition[i]!=' '){
				allkey+=condition[i];
			}
		}
		//while(i<condition.length() && condition[i]!=' ')i++;
		i++;
	}
	//cout<<from<<datebegin<<dateend<<to;
	//cout<<allkey;
	iter=ansid.begin();
	while(iter!=ansid.end() ){
		cout<<(*iter)->id<<" ";
		iter++;
	}
	stack<string> s;
	deque<string> output1;
	for(i=0;i<allkey.length();i++){
		if(allkey[i] == '&'){
			verify(s, "&", output1);
		}else if(allkey[i] == '|'){
			verify(s, "|", output1);
		}else if(allkey[i] == '!'){
			verify(s, "!", output1);
		}else if(allkey[i] == '('){
			s.push("(");
		}else if(allkey[i] == ')'){
			while(!s.empty() && s.top() != "("){
				output1.push_back(s.top());
				s.pop();
			}
			if(!s.empty())
				s.pop();
		}else{
			string t;
			for(int j = 0; isalpha(allkey[i]); i++, j++)
				t += allkey[i];
			i--;
			output1.push_back(t);
		}
	}
	//eval(output1);
	while(!s.empty()){
		output1.push_back(s.top());
		s.pop();
	}
	/*for(int i=0;i<output1.size();i++){
		cout<<output1[i]<<' ';
	}*/
	
}

int main(){
	ios_base::sync_with_stdio(0);
	//cin.tie(0);
	string input;
	/*vector<mail*> m_vec;
	m_vec.reserve(10000);*/

	while (cin >> input){
		//getline(cin,input);
		//cout << convert_date(input.c_str());
		if(input=="add"){
			mail* t=new mail();//testing 
			bool state=add(t);
			if(!state || idset.find(t)!=idset.end()){
				delete(t);
				cout <<"-\n";
			}
			else{
				dateset.insert(t);
				fromset.insert(t);
				subjectset.insert(t);
				lengthset.insert(t);
				toset.insert(t);
				idset.insert(t);
				cout << idset.size() <<'\n';
			}
		}
		
		else if (input== "remove"){
			int removing_id;
			cin >>removing_id;

			mail fake_target;
			fake_target.id=removing_id;
			auto p=idset.find(&fake_target);
			//set iterator
			if(p==idset.end()){
				cout <<"-\n";
			}
			else{
				/*if(!dateset.erase(*p));
				if(!fromset.erase(*p));
				if(!subjectset.erase(*p));
				if(!lengthset.erase(*p));
				if(!toset.erase(*p));
				if(!idset.erase(p));*/
				dateset.erase(*p);
				fromset.erase(*p);
				subjectset.erase(*p);
				lengthset.erase(*p);
				toset.erase(*p);
				idset.erase(p);
				cout << idset.size() <<'\n';

			}
		}
		else if(input == "longest"){
			if(lengthset.size()==0){
				cout <<"-\n";
			}
			else{
				auto iter=lengthset.begin();
				while(iter!=lengthset.end() ){
					if((*iter)->length == (*lengthset.begin())->length){
						cout << (*iter)->id <<' '<<(*iter)->length<< '\n';
						break;
					}
					iter++;
				}
			}
		}
		else if (input=="query"){
			getline(cin,input);
			query(input);
			//cout << "q\n";
		}
	}
	
}
