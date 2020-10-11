//////////////////////////////////////////////////////
//JHXie GTP Ultra                                   //
//////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
using namespace std;
#define table "index.txt"
#define BUFFERSIZE 512
class Cache{
public:
	int valid_0[8]={0};
	int block_0[32]={0};
	int valid_1[8]={0};
	int block_1[32]={0};
	int faulty_0[8]={0};
	int faulty_1[8]={0};
	vector<string> Table;
	void initialize();
	void execute();
	int HIT=0;
	int MISS=0;
	int cycle=1;
	int Quotient=0;
	int Remainder=0;
	int Set=0;
	int table_line=0;
};
vector<string> split(string delim, string tgt);
int main(){
	Cache W2S8;
	W2S8.initialize();
	while(W2S8.Table[W2S8.table_line]!="exit"){
		W2S8.execute();
	} 
	cout<<"\nHIT="<<W2S8.HIT<<"\n";
	cout<<"MISS="<<W2S8.MISS<<"\n";
	system("PAUSE");
	return 0;
}
void Cache::initialize(){
	fstream table_f;
	char line[BUFFERSIZE];
	//var
	HIT=MISS=Quotient=Remainder=Set=0;
	cycle=1;
	table_line=0;
	Table.clear();
	//load table
	table_f.open(table);
	if(table_f){
		while (table_f.getline(line,BUFFERSIZE))
			Table.push_back(line);
		cout<< "table size:" << Table.size() << "\n\n";
		table_f.close();
	}
	else
		cout<< "error: index.txt not found\n";
}
void Cache::execute(){
	vector<string> argv=split(" ",Table[table_line]);
	if(argv[0]=="F"){
		if (stoi(argv[2])==0) faulty_0[stoi(argv[1])]=1;
		else if (stoi(argv[2])==1) faulty_1[stoi(argv[1])]=1;
		else cout<< "error: index.txt format wrong\n";
	}else if(argv[0]=="C"){
		if(cycle==stoi(argv[1])){
		cout<< "\ncycle: " << cycle << "\n";
		cout<< "Table cycle: " << argv[1] <<"\n";
		cout<< "Table byte address: " << argv[2] <<"\n";
		Quotient=stoi(argv[2])/32;
		Remainder=stoi(argv[2])%32;
		Set=stoi(argv[2])%32/4;
		cout<< "Set="<< Set<< "\n";
		if((stoi(argv[2])==block_0[Remainder])&&(valid_0[Set]!=0)){
			//HIT W1
			HIT+=1;
			valid_0[Set]=cycle;
			cout<< "HIT. W1\n";
		}else if((stoi(argv[2])==block_1[Remainder])&&(valid_1[Set]!=0)){
			//HIT W2
			HIT+=1;
			valid_1[Set]=cycle;
			cout<< "HIT. W2\n";
		}else{
			//MISS
			if(((valid_1[Set]>=valid_0[Set])&&(faulty_0[Set]==0))
				||((faulty_0[Set]==0)&&(faulty_1[Set]==1))){
				//write W1
				cout<< "last used: "<< valid_0[Set]<< "\n";
				for(int i=0;i<4;i++){
					block_0[Set*4+i]=(Quotient*32)+(4*Set)+i;
				}
				MISS+=1;
				valid_0[Set]=cycle;
				cout<< "MISS. W1\n";
			}else if(((valid_1[Set]<valid_0[Set])&&(faulty_1[Set]==0))
				||((faulty_0[Set]==1)&&(faulty_1[Set]==0))){
				//WRITE w2
				cout<< "last used: "<< valid_1[Set]<< "\n";
				for(int i=0;i<4;i++){
					block_1[Set*4+i]=(Quotient*32)+(4*Set)+i;
				}
				MISS+=1;
				valid_1[Set]=cycle;
				cout<< "MISS. W2\n";
			}else {
				cout<< "Both block on the set is broken\n";
			}
		}
		cycle+=1;
	}
	else{
		cout<< "error: index.txt format wrong\n";
	}
	}else{
		cout<< "error: index.txt format wrong\n";
	}
	cout<< argv[0] << " "<< argv[1] << " " <<argv[2] << endl;
	table_line+=1;
}
vector<string> split(string delim, string tgt) {
	vector<string> result;
	char* word=NULL;
	word=strtok(const_cast<char*>(tgt.c_str()), delim.c_str());
	while (word!=NULL){
		result.push_back(word);
		word=strtok(NULL, delim.c_str());
	}
	return result;
}