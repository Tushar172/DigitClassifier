#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <climits>
#include <cassert>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define pb push_back
#define mp make_pair
#define si(n) scanf("%d",&n)
#define sl(n) scanf("%lld",&n)
#define fr first
#define sc second
#define sz size()
#define ln length()
#define forr(i,a,b)                 for(int i=a;i<b;i++)
#define rep(i,n)                    forr(i,0,n) 
#define all(v)                      v.begin(),v.end()    
#define uniq(v)                     sort(all(v));v.erase(unique(all(v)),v.end())
#define clr(a)                      memset(a,0,sizeof a)
#define MAX 28

double matrix_match[10][10];
double testimg[MAX][MAX];
double like[10][MAX][MAX];
double number[10];
double prior_prob[10];
int totl = 5000 , nm;
int cnt = 0.0 , total_testdata = 1000;
void initialise(){

	rep(i,10){
		rep(j,28){
			rep(k,28){
				like[i][j][k] = 1.0;   //its 1 bcause log(1)=0 in further processing
			}
		}
	}
}
void calculate_likelihood(ifstream& f1 , ifstream&f2){
	string s1 , s;
	int num ;
	for(int i=1;i<=5000;i++){
		getline(f2,s1);                      //read a testlabel
		num = (s1[0] - '0');
		for(int j=0;j<28;j++){
			getline(f1,s);                   // reading image line by line
			for(int k=0;k<28;k++){
				if(s[k] == '+' || s[k] == '#')
					like[num][j][k] += 1.0;               // incremant occurence of event (j,k) for class(i)
			}
		}
		number[num]++;                                     // total count of class(num)
	}
}
void likelihood_of_evidence(){             // i.e    P((i,j)| C=num)
	rep(i,10){
		rep(j,28){
			rep(k,28){
				like[i][j][k] = (double)like[i][j][k]/(double)number[i] ;
			}
		}
	}
}
void check_class(int nm){                // check the class of to with nm matches most
	double prob[10] ;
	clr(prob);
	int mx = 0;
	rep(i,10){
		prior_prob[i] = ((double)(number[i])/(double)(totl)) ;   //prior probability
		prob[i] +=  (double)log(prior_prob[i]);
		rep(j,28){
			rep(k,28){
				if(testimg[j][k] == 0)
					prob[i] += (double)log(1-like[i][j][k]);     // prob that it (i,j) did not occur and it belongs to class(i)
				else
					prob[i] += (double)log(like[i][j][k]);    //prob that it (i,j) occured and it belongs to class(i)
			}
		}
		if(prob[i] >= prob[mx])    // maximum posterior prob
			mx = i;
	}
	if(mx == nm)
		cnt++;                      // count of total correct matches
	matrix_match[mx][nm]++;         // entry (i,j) stores count of :- how many times nm has beed detected as mx
}
void printmatrix(){
	double total_nm= 0;
	rep(j,10){
		total_nm = 0;
		rep(i,10)
			total_nm += matrix_match[i][j];            // total times (j) actually ocured while matching from(0-9)
		rep(i,10)
			matrix_match[i][j] /= total_nm;            
	}
	rep(i,10){
		rep(j,10){
			printf("%.1lf\t",matrix_match[i][j]*100);   // express occurence of a digit as percent
		}
		printf("\n");
	}
	cout<<endl;
	cout<<"################################################################################"<<endl;
}
void print_results(){
	cout<<"###########  DIGIT CLASSIFIACTION   #####################\n\n";
	cout<<"CorrectCount/TotalTestData =  " << cnt<<"/"<<total_testdata<<endl<<endl;
	cout<<"Accuracy Percent := " << ((double)(cnt)/(double)(1000))*100 << endl<<endl;
	cout<<"###########  Matrix Expressing Classification of each digit  #############"<<endl<<endl;
	printmatrix();
}
void readimage(ifstream& f3 , ifstream& f4){
	string s1,s;
	int nm ;
	for(int i=1;i<=1000;i++){
		getline(f4,s1);
		clr(testimg);
		nm = (s1[0]-'0');
		for(int j=0;j<28;j++){
			getline(f3,s);
			for(int k=0;k<28;k++){
				if((s[k] == '+') || (s[k] == '#'))
					testimg[j][k] = 1;
			}
		}
		check_class(nm);
	}
	print_results();
}
int main(){
	ifstream f1("trainingimages");
	ifstream f2("traininglabels");
	ifstream f3("testimages");
	ifstream f4("testlabels");
	initialise();
	calculate_likelihood(f1,f2);
	likelihood_of_evidence();
	/* now test images */
	readimage(f3,f4);
	return 0;
}
