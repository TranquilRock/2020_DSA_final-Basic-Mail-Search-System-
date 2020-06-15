#include<bits/stdc++.h>
using namespace std;

bool compareChar(const char & c1,const char & c2)
{
	if (c1 == c2)
		return true;
	else if (std::toupper(c1) == std::toupper(c2))
		return true;
	return false;
}
 
bool caseInSensStringCompare(std::string const& str1, std::string const&str2)
{
	return ( (str1.size() == str2.size() ) &&
			 std::equal(str1.begin(), str1.end(), str2.begin(), &compareChar) );
}

struct mail{
	string from,to;
	int id,length;
	unsigned long long date;
	//ex:   19 May 2011 at 16:50
	//   -> 201105191650
	set<string> subject;
	set<string> content;
};
struct intlist{
	vector<int> list;
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
	string temp2;
	getline(f,temp2);
	int i=0,j=0;
		while(j<temp2.length()){
			while(isalpha(temp2[j]) || isdigit(temp2[j]))j++;
			if(i!=j){
				//this is segment of content
				string inserting_element=string(temp2.begin()+i,temp2.begin()+j);
				// cout << "inserted " << inserting_element << endl;
				M->subject.insert(inserting_element);
				
			}
			i=++j;

		}
	// cout <<"subject "<< M->subject<<endl;

	f >>trash;//To
	f >>M->to;
	// cout << M->to << " to\n";

	M->length=0;
	string temp;
	while(getline(f,temp)){
		i=0,j=0;
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
//set<mail*,subject_compare> subjectset;
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

void do_one(stack<intlist>& s, deque<string>& output1,intlist& ansid){
	intlist a;
	a=s.top();
	s.pop();
	intlist w;
	vector<int> v(100000);                      
  	vector<int>::iterator it;
  	it=set_difference (ansid.list.begin(), ansid.list.end(), a.list.begin(), a.list.end(), v.begin());
  	v.resize(it-v.begin()); 
  	w.list=v;
  	s.push(w);
}

void do_binary(stack<intlist>& s, deque<string>& output1,const string& out)
{
	intlist  a, b;
	b = s.top();
	s.pop();
	a = s.top();
	s.pop();
	intlist w;
	if(out=="&"){
		//printf("w");
		vector<int> v(100000);                      
  		vector<int>::iterator it;
  		it=set_intersection (b.list.begin(), b.list.end(), a.list.begin(), a.list.end(), v.begin());
  		//printf("w");
  		v.resize(it-v.begin()); 
  		w.list=v;
	}
	if(out=="|"){
		vector<int> v(100000);                      
  		vector<int>::iterator it;
  		it=set_union (b.list.begin(), b.list.end(), a.list.begin(), a.list.end(), v.begin());
  		v.resize(it-v.begin()); 
  		w.list=v;
	}
	s.push(w);
}

void eval(deque<string>& output1,set<mail*,id_compare>& ansid)
{
	stack<intlist> s;
	auto iter3=ansid.begin();
	intlist metoo;
	while(iter3!=ansid.end() ){
		metoo.list.push_back((*iter3)->id);
		iter3++;
	}
	if(output1.empty()){
		if(metoo.list.size()==0){
			cout<<"-";
		}else{
			for(int i=0;i<metoo.list.size();i++){
				cout<<metoo.list[i]<<" ";
			}
		}
		cout<<"\n";
	}
	//vector<int> ansid;
	else{
	while(!output1.empty()){
		if(output1.front()=="!")
			do_one(s, output1,metoo);
		else if(output1.front()=="&"||output1.front()=="|")
			do_binary(s, output1,output1.front());
		else{
			set<mail*,id_compare> tmp=ansid;
			auto iter=tmp.begin();
			while(iter!=tmp.end() ){
				bool flag=false;
				//if(caseInSensStringCompare((*iter)->subject,output1.front()))flag=true;
				
				auto iter10=(*iter)->subject.begin();
				while(iter10!=(*iter)->subject.end()){
					if(caseInSensStringCompare(*(iter10),output1.front())){
						flag=true;
						//cout<<*iter2;
					}
					iter10++;
				}
				
				auto iter2=(*iter)->content.begin();
				while(iter2!=(*iter)->content.end()){
					if(caseInSensStringCompare(*(iter2),output1.front())){
						flag=true;
						//cout<<*iter2;
					}
					iter2++;
				}
				if(flag==false){
					tmp.erase(*iter);
				}
				iter++;
			}
			iter=tmp.begin();
			intlist me;
			while(iter!=tmp.end() ){
				me.list.push_back((*iter)->id);
				iter++;
			}
			s.push(me);
			/*for(int i=0;i<me.list.size();i++){
				cout<<me.list[i];
			}*/
		}
			
		output1.pop_front();
	}
	intlist jack=s.top();
	if(jack.list.size()==0){
		cout<<"-";
	}else{
		for(int i=0;i<jack.list.size();i++){
			cout<<jack.list[i]<<" ";
		}
	}
	cout<<"\n";
	}
}




void query(string &condition){
	//cout<<condition<<"\n";
	int i=1;string from={},datebegin={},dateend={},to={},allkey={};
	set<mail*,id_compare> ansid;
	unsigned long long comdate;
	auto iter=idset.begin();
	while(iter!=idset.end() ){
		ansid.insert(*iter);
		iter++;
	}
	while( i < condition.length() ){
		if (condition[i]=='-'&&condition[i-1]==' '){
			bool put2all=false;
			if(condition[i+1]=='f'&&condition[i+2]=='\"'){
				int j=i+3,k=0;
				int count=0;bool ok =true;
				while(condition[j]!='\"'){
					from+=condition[j];
					k++;j++;count++;
					if(count>60){
						ok=false;put2all=true;
						break;
						
					}
				}
				if(ok){
					iter=ansid.begin();
					while(iter!=ansid.end() ){
						if(!caseInSensStringCompare((*iter)->from,from)){
							ansid.erase(*iter);
						}
						iter++;
					}
				}
				
			}
			else if (condition[i+1]=='d'){
				if(condition[i+2]=='~'){
					bool okdate=true;
					for(int j=1;j<=12;j++){
						if(!isdigit(condition[i+2+j])){
							okdate=false;put2all=true;
						}
						dateend+=condition[i+2+j];
					}
					if(okdate){
						iter=ansid.begin();
						//cout<<dateend;
						comdate=stoull(dateend,nullptr,10);
						while(iter!=ansid.end() ){
							if((*iter)->date >=comdate){
								ansid.erase(*iter);
							}
							iter++;
						}
					}
					
				}else{
					bool okdate=true;
					for(int j=1;j<=12;j++){
						if(!isdigit(condition[i+1+j])){
							okdate=false;put2all=true;
						}
						datebegin+=condition[i+1+j];
					}
					//cout<<datebegin;
					if(okdate){
						comdate=stoull(datebegin);
						iter=ansid.begin();
						while(iter!=ansid.end() ){
							
							if((*iter)->date <=comdate){
								ansid.erase(*iter);
							}
							iter++;
						}
					}
						
					if(condition[i+14]=='~'){
						if(isdigit(condition[i+15])){
							okdate=true;
							for(int j=1;j<=12;j++){
								if(!isdigit(condition[i+14+j])){
									okdate=false;put2all=true;
								}
								dateend+=condition[i+14+j];
							}	
							//cout<<dateend;
							if(okdate){
								comdate=stoull(dateend,nullptr,10);
								iter=ansid.begin();
								while(iter!=ansid.end() ){
									if((*iter)->date >=comdate){
										ansid.erase(*iter);
									}
									iter++;
								}	
							}
							
						}
						
					}
				}
			}
			else if (condition[i+1]=='t'&&condition[i+2]=='\"'){
				int j=i+3,k=0;
				int count=0;bool ok=true;
				while(condition[j]!='\"'){
					to+=condition[j];
					k++;j++;count++;
					if(count>60){
						ok=false;put2all=true;
						break;
					}
				}
				if(ok){
					iter=ansid.begin();
					while(iter!=ansid.end() ){
						if(!caseInSensStringCompare((*iter)->to,to)){
							ansid.erase(*iter);
						}
						iter++;
					}
				}
					
			}
			if(put2all){
				while(i<condition.length() && condition[i]!=' '){
					allkey+=condition[i];
					i++;
				}
			}else{
				while(i<condition.length() && condition[i]!=' ')i++;
			}
			
		}
		else{
			if(condition[i]!=' '){
				allkey+=condition[i];
			}
		}
		
		i++;
	}
	//cout<<"hwllo"<<from<<" "<<datebegin<<" "<<dateend<<" "<<to<<" "<<allkey<<"\n";
	
	
	stack<string> s;
	deque<string> output1;
	for(int z=0;z<allkey.size();z++){
		if(isalpha(allkey[z])||isdigit(allkey[z])){
			string t;
			for(int j = 0; isalpha(allkey[z])||isdigit(allkey[z]); z++, j++)
				t += allkey[z];
			z--;
			output1.push_back(t);
		}
		else if(allkey[z] == '&'){
			verify(s, "&", output1);
		}else if(allkey[z] == '|'){
			verify(s, "|", output1);
		}else if(allkey[z] == '!'){
			verify(s, "!", output1);
		}else if(allkey[z] == '('){
			s.push("(");
		}else if(allkey[z] == ')'){
			while(!s.empty() && s.top() != "("){
				output1.push_back(s.top());
				s.pop();
			}
			if(!s.empty())
				s.pop();
		}/*else{
			string t;
			for(int j = 0; isalpha(allkey[z])||isdigit(allkey[z]); z++, j++)
				t += allkey[z];
			z--;
			output1.push_back(t);
		}*/
	}
	while(!s.empty()){
		output1.push_back(s.top());
		s.pop();
	} 
	/*for(int i=0;i<output1.size();i++){
		cout<<output1[i]<<' ';
	}*/
	eval(output1,ansid);
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
				//subjectset.insert(t);
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
				//subjectset.erase(*p);
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
