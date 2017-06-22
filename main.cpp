#include <iostream>
#include <queue>
#include <ncurses.h>
#include <stdlib.h> 
#include <time.h> 
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

	queue <int> kolejka1; //zasob1
	mutex kolejka1G;
	queue <int> kolejka2; //zasob2
	mutex kolejka2G;
	int** pionki;         //zasob3
	mutex pionkiG;
	int N = 10;
	int M = 20;
	int iteracje = 20;

void producent(){ //watek1
		kolejka1G.lock();
		kolejka2G.lock();
		kolejka1.push(rand()%N);
		kolejka2.push(rand()%M);
		kolejka2G.unlock();
		kolejka1G.unlock();
}
void zrobMacierz(){
	pionki = new int *[N];
	for(int i=0;i<N;i++){
		pionki[i]=new int[M];
	}
}
void przesunPionki(){
		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				if(pionki[i][j]==1&&j+1<M){
                    pionki[i][j+1]=1;
                    pionki[i][j]=0;
                    j++;
				}
			}
		}
}
void wyzerujMacierz(){
	for(int i=0;i<N;++i){
			for(int j=0;j<M;++j){
				pionki[i][j]=0;
			}
		}
}

void przetwarzacz(){ //watek2
		kolejka1G.lock();
		kolejka2G.lock();
		pionkiG.lock();
		przesunPionki();
		pionki[kolejka1.front()][kolejka2.front()] = 1;
		kolejka1.pop();
		kolejka2.pop();	
		pionkiG.unlock();
		kolejka2G.unlock();
		kolejka1G.unlock();
}
void ncurses(){ //watek3

}
void wyswietlMacierz(){ //najprostrze wyswietlanie konsolowe
	pionkiG.lock();
	for(int i=0;i<N;i++){
		for(int j=0; j<M;j++){
			cout<<pionki[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	this_thread::sleep_for(std::chrono::seconds(1));
	pionkiG.unlock();

}
int main(){
	queue <int> kolejka1; //zasob1
	queue <int> kolejka2; //zasob2
	srand(time(NULL));
	cout<<"Podaj N\n";
	cin>>N;
	cout<<"Podaj M\n";
	cin>>M;
	cout<<"Podaj liczbe iteracji\n";
	cin>> iteracje;   
	zrobMacierz(); //zasob3
	wyzerujMacierz();
	for(int i=0; i<iteracje; i++){
		auto producentThread = thread(producent);
		producentThread.join();
		auto przetwarzaczThread = thread(przetwarzacz);
		przetwarzaczThread.join();
		auto wyswietlThread = thread(wyswietlMacierz);
		wyswietlThread.join();
	}


	return 0;

}