#include<bits/stdc++.h>
#include "ternary2.cpp"
#include "expression.cpp"
using namespace std;

struct length_compare {
    bool operator() (const mail* a, const mail* b) const {
        if(a ->length == b->length)return a->id < b->id;
        return a->length > b->length;
    }
};
set<mail*,id_compare>allid;
unordered_map<int,mail*> container;
unsigned long long convert_date(const char *);
Node *TST_root=NULL;
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
int add(struct mail *M){
	char path[32];
	cin >> path;
	// cout <<"WTF"<<path <<"asd"<<'\n' <<"fu";
	fstream f;
	f.open(path,fstream::in);
	//cout << f.is_open();
	if(!f.is_open())return 0;//read file fail
	char trash[64];

	M->length=0;

	f >> trash;//from
	f >> M->from;
	TOLOW(M->from);
	//M->length+=M->from.length();
	// cout <<"sender "<< M->from << endl;
	
	f >> trash;//date
	f.getline(trash,64);
	
	//M->length+=strlen(trash)-5;
	
	M->date=convert_date(trash);
	//convert date to number
	// cout<<"date " << M->date << endl;

	f >>trash;//ID
	f >>M->id;
	
	//for(int i=M->id;i!=0;i/=10)M->length++;
	// cout <<"id "<< M->id << endl;
	auto z=container.find(M->id);
	if(z!=container.end()){
		if (z->second->currently_valid==1)return 0;
		else {
			return 2;
		};
	}
	f >>trash;
	f.ignore();
	getline(f,M->subject);
	TOLOW(M->subject);
	// cout <<"subject "<< M->subject<<endl;

	f >>trash;//To
	f >>M->to;
	TOLOW(M->to);
	//M->length+=M->to.length();
 	// cout << M->to << " to\n";

	
	string temp;
	//TST_insert(&TST_root,M->from.c_str(),M);
	//TST_insert(&TST_root,M->to.c_str(),M);

	char splitsb[128];
	strncpy(splitsb,M->subject.c_str(),128);
	for(int i=0;i<M->subject.length();i++){
		int j=i;
		while(splitsb[j]!=' ' && splitsb[j]!='\0')++j;
		splitsb[j]='\0';
		TST_insert(&TST_root,splitsb+i,M);
		//M->length+=j-i;
		i=j;
	}
	//TST_insert(&TST_root,M->subject.c_str(),M);
	f>>trash;
	while(getline(f,temp)){
		int i=0,j=0;
		while(j<temp.length()){
			while(isalnum(temp[j]))j++;
			if(i!=j){
				//this is segment of content
				string inserting_element=string(temp.begin()+i,temp.begin()+j);
				TOLOW(inserting_element);
				//cout << "inserted " << inserting_element << endl;
				// M->content.insert(inserting_element);
				TST_insert(&TST_root,inserting_element.c_str(),M);
				M->length+=inserting_element.length();
			}
			i=++j;

		}
	}
	//f >>
	f.close();
	return 1;
}
void intersec(set<mail*,id_compare>&a,set<mail*,id_compare>&b,set<mail*,id_compare>&c){
	auto i=a.begin();
	auto j=b.begin();
	while(i!=a.end() && j!=b.end()){
		if((*i)->id > (*j)->id)j++;
		else if((*i)->id < (*j)->id)i++;
		else {c.insert(*i);i++,j++;}
	}
	return ;
}
void uni(set<mail*,id_compare>&a,set<mail*,id_compare>&b,set<mail*,id_compare>&c){
	c=a;
	for(auto t:b){
		c.insert(t);
	}
	return;
}
void diff(set<mail*,id_compare>&a,set<mail*,id_compare>&b,set<mail*,id_compare>&c){
	// a/b
	c=a;
	for(auto t:b){
		c.erase(t);
	}
}
set<mail*,id_compare> evaluate(vector<string> &post){
	stack< set<mail*,id_compare> >num;
	//for(auto k:post)cout << k <<"_ ";
	//cout << "fuck";
	for(string &seg:post){
		//assert(seg!="");
		if(seg=="")continue;
		if(isalnum(seg[0])){
			Node* ss=searchTST(TST_root,seg.c_str());
			if(ss==NULL){
				//cout << "??"<<seg.c_str()<<"??";
				num.push(set<mail*,id_compare>());
			}
			else num.push(ss->TST_set);
		}
		else {
			
			//assert(prec.find(seg)!=prec.end());

			if(seg=="!"){
				set<mail*,id_compare> temp;
				//set_difference(allid.begin(),allid.end(),
				//num.top().begin(),num.top().end(),std::inserter(temp,temp.begin()));
				diff(allid,num.top(),temp);
				num.top()=temp;
				//else assert(seg=="u+");
			}
			else if(seg=="&" || seg=="|"){
				assert(num.size()>=2);
				set<mail*,id_compare> a=num.top();
				num.pop();
				set<mail*,id_compare> b=num.top();
				num.pop();
				/*for (auto k:a)cout <<k<<"* ";
				for (auto k:b)cout <<k<<"@ ";*/
				if(seg=="&"){
					num.push(set<mail*,id_compare>());
					// vector<mail*>wtf;
					// wtf.resize(100);
					intersec(a,b,num.top());
					//set_intersection(a.begin(),a.end(),b.begin(),b.end(),std::inserter(num.top(),num.top().begin()));
					//for (auto k:num.top())cout <<k->id<<"@* ";
				}
				else {
					num.push(set<mail*,id_compare>());
					uni(a,b,num.top());
					//set_union(a.begin(),a.end(),b.begin(),b.end(),std::inserter(num.top(),num.top().begin()));
				}

				}
			}

	}
	//assert(num.size()==1);
	if(num.empty()){
		return set<mail*,id_compare>();
	}
	return num.top();
}
void query(string &condition){
	int i=0;
	TOLOW(condition);
	/*cout << condition << endl;
	auto r=topost2(condition);*/
	//for(auto &k:r)cout << k <<' ';
	//cout <<condition<<"newline?";
	if(condition.back()=='\n' || condition.back()=='\r')condition.pop_back();
	//cout <<condition<<"newline?";
	string f,t;
	unsigned long long L=0,R=ULLONG_MAX;
	vector<string> ex;
	//cout << endl;
	while( i < condition.length() ){
		if(condition[i]==' '||condition[i]=='\n'){i++;continue;}
		if (condition[i]=='-'){
			if(condition[i+1]=='f'){
				i+=3;//"
				while(condition[i]!='\"'){
					f.push_back(condition[i]);
					i++;
				}
			}
			else if (condition[i+1]=='t'){
				i+=3;//"
				while(condition[i]!='\"'){
					t.push_back(condition[i]);
					i++;
				}
			}
			else if (condition[i+1]=='d'&&(condition[i+2]=='~' ||isdigit(condition[i+2]))){
					i+=2;

					if(isalnum(condition[i])){
						sscanf(condition.c_str()+i,"%llu",&L);
						while(condition[i]!='~')i++;
					}
					i++;
					if(condition[i]!=' '&&condition[i]!='\0' && condition[i]!='\n')
							sscanf(condition.c_str()+i,"%llu",&R);
			}
			
		}
		else{
			int j=i;
			while(condition[j]!=' ' && j<condition.length())j++;
			ex=topost2(string(condition.begin()+i,condition.begin()+j));
		}
		while(i<condition.length() && condition[i]!=' ')i++;
		i++;
	}
	//cout << ex.size()<<"**" ;
	//if(R!=ULLONG_MAX)cout <<"@@";
	/*cout <<"f" <<f <<endl;
	cout <<"t" <<t <<endl;
	cout <<"L" <<L <<endl;
	cout <<"R" <<R <<endl;*/
	/*for(auto&k:ex)cout << k <<" ";
	cout <<endl;
	cout <<"okhere1\n";*/
	set<mail*,id_compare> ret;
	if(ex.size()!=0){
		ret=evaluate(ex);
		int n=0;
		for(auto k:ret){
			if(!k->currently_valid)continue;
			if(f!="" && k->from!=f)continue;
			if(t!="" && k->to!=t)continue;
			if(L!=0 && k->date <L)continue;
			if(R!=ULLONG_MAX && k->date>R)continue;
			if(n++)cout <<' ';
			cout << k->id;
		}
		if(n==0)cout <<"-";
		cout <<'\n';
	}
	else{
		int n=0;
		//cout <<"no ex" << f <<"f"<<t;
		for(auto k:allid){
			if(!k->currently_valid)continue;
			if(f!="" && k->from!=f)continue;
			if(t!="" && k->to!=t)continue;
			if(L!=0 && k->date <L)continue;
			if(R!=ULLONG_MAX && k->date>R)continue;
			if(n++)cout <<' ';
			cout << k->id;
		}
		if(n==0)cout <<"-";
		cout <<'\n';
	}
}

int main(){
	//ios_base::sync_with_stdio(0);
	//cin.tie(0);
	string input;
	/*vector<mail*> m_vec;
	m_vec.reserve(10000);*/
	// cout <<"foog\n";
	int mailnum=0;
	int j=0;
	while (cin >> input){
		//getline(cin,input);
		//cout << convert_date(input.c_str());
		//cout << ++j <<"  "<<endl;
		if(input=="add"){
			mail* t=new mail();//testing 
			 // cout <<"goin";
			int state=add(t);
			 // cout <<"??";
			if(!state){
				delete(t);
				cout <<"-\n";
			}
			else if(state==2){
				auto z=container.find(t->id)->second;
				delete(t);
				z->currently_valid=1;
				cout <<++mailnum <<endl;
				lengthset.insert(z);
				allid.insert(z);
			}
			else{
				container.insert(make_pair(t->id,t));
				t->currently_valid=1;
				lengthset.insert(t);
				allid.insert(t);
				cout <<++mailnum <<endl;
			}
		}
		
		else if (input== "remove"){
			int removing_id;
			cin >>removing_id;
			auto p=container.find(removing_id);
			//set iterator
			if(p==container.end() || p->second->currently_valid==0){
				cout <<"-\n";
			}
			else{
				p->second->currently_valid=0;
				lengthset.erase(p->second);
				cout << --mailnum <<endl;
				allid.erase(p->second);
			}
		}
		else if(input == "longest"){
			if(lengthset.size()==0){
				cout <<"-\n";
			}
			else{
				auto iter=lengthset.begin();
				if(iter!=lengthset.end()){
					//cout <<"l\n";
					cout << (*iter)->id <<" "<< (*iter)->length <<endl;
				}
			}
		}
		else if (input=="query"){
			getline(cin,input);
			//cout <<"q";
			//cout << "q" <<input<<endl;
			//cout <<"**"<< (input.find('\n') ==string::npos)<<"hello"<<endl;
			query(input);
			
		}
	}
	//traverseTST(TST_root);
	/*cout <<sizeabc<<endl;*/
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