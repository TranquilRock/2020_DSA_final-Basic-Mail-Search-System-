#include<bits/stdc++.h>
using namespace std;

unordered_map <string,int> prec;

void set_precedence();

void topost2(const string &in,vector<string>&post){
	//cout << in <<" this";
	stack<string> op;
	//vector<string> post;
	
	for(int i=0;i<in.length();i++){
		if(in[i]==' ' || in[i]=='\n')continue;
		if(isalnum(in[i])){
			int j=i+1;
			while (isalnum(in[j]) )j++;
			post.push_back(string(in.begin()+i,in.begin()+j));
			i=j-1;
		}
		else if(in[i]=='('){
			op.push("(");
			continue;
		}
		else if(in[i]==')'){
			while(op.top()!="("){
				post.push_back(op.top());
				op.pop();
			}
			op.pop();
		}
		else {
			string temp;
			temp.push_back(in[i]);
			//cout <<temp <<"???\n";
			//if ( (in[i]=='-' || in[i]=='+') && (i>0 && !isdigit(in[i-1])) )temp='u'+in[i];
			//cout << in[i]<<endl;
			// switch (in[i]){
			// 	case '&':
			// 		if (i+1<in.length() && in[i+1]==in[i]){temp="&";i++;}
			// 		break;
			// 	case '|':
			// 		if (i+1<in.length() && in[i+1]==in[i]){temp="|";i++;}
			// 		break;
			// 	case '!':
			// 		if (in[i+1]=='='){temp="!=";i++;}
			// 		break;
			// 	default :
			// 		break;
			// 	};
			//assert(prec.find(temp)!=prec.end());
			while (!op.empty() && op.top()!="(" &&prec[op.top()] <= prec[temp] && prec[temp]>1){
				post.push_back(op.top());
				//cout << op.top() << ' ' << temp <<endl;
				op.pop();
			}
			op.push(temp);
			//cout << temp <<"__"<<i <<endl;
		}
	}
	while(!op.empty()){
		post.push_back(op.top());
		op.pop();
	}
	return ;
}


// int main(int argc,char *argv[]){
// 	string input;
// 	set_precedence();
// 	while(getline(cin,input)){
// 		//cout << input <<"__read\n" << prec.size() << '\n';


// 		vector<string> out2=topost2(input);

// 		/*for(int i=0;i<out2.size();i++){
// 			cout << out2[i];
// 			cout << (i==out2.size()-1?'\n':' ');
// 		}*/

// 		cout <<"RESULT: "<< evaluate(out2) <<endl;

// 	}
// 	return 0;
// }
void set_precedence(){
	prec["("]=prec[")"]=0;
	prec["!"]=1;
	// prec["~"]=prec["u+"]=prec["u-"]=1;
	// prec["*"]=prec["/"]=prec["%"]=2;
	// prec["+"]=prec["-"]=3;
	// prec[">>"]=prec["<<"]=4;
	// prec["!="]=prec["=="]=5;
	prec["&"]=6;
	// prec["^"]=7;
	prec["|"]=8;
	// prec["&&"]=9;
	// prec["||"]=10;
	return ;
}

/*

()
! + -(unary) ~
% * /
+ -
>> <<
== !=
&
^
|
&&
||

*/
