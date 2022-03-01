/*
Paul Scala
Z23561522
COP4610
Programming Assignment 1
SJF Algorithm 
*/
#include<iostream>
#include<bits/stdc++.h>

using namespace std;

typedef pair<int,int> pi;
void sjf(vector<vector<int>> processes, priority_queue< pi, vector<pi>,greater<pi> > readyQueue, vector<int> processBurstIndex);

 void printCompleteProcesses(vector<int> complete);
 void printQueue(priority_queue< pi, vector<pi>, greater<pi> > q, vector<vector<int>> processes, vector<int> processBurstIndex);
 void printContextSwitchData(vector<vector<int>> processes,int process_index,priority_queue< pi, vector<pi>, greater<pi> > readyQueue,vector<int> processBurstIndex, int time);

 int main(){

 	int numProcesses = 8;
 	vector<vector<int>> processes(numProcesses);
 	// queue <int> readyQueueFCFS;
 	priority_queue< pi, vector<pi>, greater<pi> > readyQueueSJF;
 	vector<int> processBurstIndex(numProcesses,0);
	processes[0] = {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 5};
	processes[1] = {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8};
	processes[2] = {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6};
	processes[3] = {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3};
	processes[4] = {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4};
	processes[5] = {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8};
	processes[6] = {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10};
	processes[7] = {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6};

 	for(int i=0;i<processes.size();i++){
 		// readyQueueFCFS.push(i);
 		readyQueueSJF.push(make_pair(processes[i][processBurstIndex[i]],i));
 	}
 	cout<<endl<<"----SJF----"<<endl;
 	sjf(processes,readyQueueSJF,processBurstIndex);
 }


 void sjf(vector<vector<int>> processes, priority_queue< pi, vector<pi>, greater<pi> > readyQueue,
vector<int> processBurstIndex){

 	vector<pair<int,int>> ioreturn; //array of pair<return time,process id>
 	vector<int> arrivalTime(processes.size(),0);
 	vector<int> waitingTime(processes.size(),0);
 	vector<int> tat(processes.size(),0);
 	vector<int> responseTime(processes.size(),-1);
 	vector<int> complete(processes.size(),0);

 	for(int i=0;i<processes.size();i++){
 		ioreturn.push_back(make_pair(INT_MAX,i));
 	}
	 int time = 0;
 	int idle_Cpu_Time=0;
 	while(!readyQueue.empty()){

 	int process_index = readyQueue.top().second;
	 readyQueue.pop();

 	//waiting time in ready queue
 	waitingTime[process_index] += time - arrivalTime[process_index];

 	//takes value of first waiting time and then resets flag so the if condition never executes again
 	if(responseTime[process_index] == -1){
 		responseTime[process_index] = time - arrivalTime[process_index];
 	}	
 	printContextSwitchData(processes,process_index,readyQueue,processBurstIndex, time);
 	time += processes[process_index][processBurstIndex[process_index]]; // process getting cpu burst time

 	if(processBurstIndex[process_index] <= processes[process_index].size()-3){
 		for(int i=0;i<processes.size();i++){
 			if(ioreturn[i].second == process_index){
			 ioreturn[i].first = processes[process_index][processBurstIndex[process_index]+1] + time;
			 break;
		 	}
 		}
 	}else{
 		complete[process_index] = 1;
 	}
	printCompleteProcesses(complete);
 	processBurstIndex[process_index] += 2;
 	sort(ioreturn.begin(), ioreturn.end());

 	if(readyQueue.empty()){
 		while(ioreturn[0].first != INT_MAX && ioreturn[0].first > time){
			 time++;
 			idle_Cpu_Time++;
 		}
 	}

 	for(int i=0;i<processes.size();i++){
 		if(ioreturn[i].first <= time){
 			readyQueue.push(make_pair(processes[ioreturn[i].second]
[processBurstIndex[ioreturn[i].second]],ioreturn[i].second)); //pushing process id into readyQueue
 	arrivalTime[ioreturn[i].second] = ioreturn[i].first;
 	ioreturn[i].first = INT_MAX;
 		}
 	   		}
    	}
 	int burst_time = time - idle_Cpu_Time;

 	for(int i=0;i<processes.size();i++){
 		int total_burst_time=0;
 		for(int j=0;j<processes[i].size();j++){
 			total_burst_time += processes[i][j];
		}
 		tat[i] += total_burst_time + waitingTime[i];
		}

 		//printing results
 cout<<"Code by Paul Scala"<<endl;
 cout<<"******************************************************** "<<endl;
 cout<<"Total time required to complete all the 8 processes: "<<time<<endl;
 cout<<"CPU Utilization: "<<(float)burst_time/time*100<<endl<<endl;
 cout<<"******************************************************** "<<endl;
 cout<<"******************************************************** "<<endl;
 cout<<"WAITING TIME"<<endl;
 int twt = 0;
 for(int i=0;i<processes.size();i++){
 cout<<"Process: "<<i<<", Waiting Time: "<<waitingTime[i]<<endl;
 twt += waitingTime[i];
 }
 cout<<"Average Waiting Time: "<<(float)twt/processes.size()<<endl<<endl;
 cout<<"******************************************************** "<<endl;
 cout<<"******************************************************** "<<endl;

 cout<<"TURNAROUND TIME"<<endl;
 int ttat = 0;
 for(int i=0;i<processes.size();i++){
 cout<<"Process: "<<i<<", Turnaround Time: "<<tat[i]<<endl;
 ttat += tat[i];
 }
 cout<<"Average Turnaround Time: "<<(float)ttat/processes.size()<<endl<<endl;
 cout<<"******************************************************** "<<endl;
 cout<<"******************************************************** "<<endl;

 cout<<"RESPONSE TIME"<<endl;
 int trt = 0;
 for(int i=0;i<processes.size();i++){
 cout<<"Process: "<<i<<", Response Time: "<<responseTime[i]<<endl;
 trt += responseTime[i];
 }
 cout<<"Average Response Time: "<<(float)trt/processes.size()<<endl;
 }

 void printQueue(priority_queue< pi, vector<pi>, greater<pi> > q, vector<vector<int>> processes, vector<int> processBurstIndex)
 {
 //printing content of queue
 cout<<setfill(' ')<<setw(10)<<"Process"<<setw(10)<<"Burst"<<endl;
 if(!q.empty()){
 while (!q.empty()){
 int pid = q.top().second;
 cout<<setfill(' ')<<setw(10)<<"P"<<pid<<setw(10)<<processes[pid]
[processBurstIndex[pid]]<<endl;
 q.pop();
 }
 }else{
 cout<<setfill(' ')<<setw(10)<<"[empty]"<<setw(10)<<"N/A"<<endl;
 }
 cout<<endl;
 }

 void printCompleteProcesses(vector<int> complete){

 cout<<"Complete Processes: ";
 for(int i=0;i<complete.size();i++){
 if(complete[i] == 1){
 cout<<"P"<<i<<" ";
 }
 }cout<<endl;
 cout<<setfill('-')<<setw(40)<<"-"<<endl;
 }

 void printContextSwitchData(vector<vector<int>> processes,int process_index,priority_queue
< pi, vector<pi>, greater<pi> > readyQueue,vector<int> processBurstIndex, int time){

 cout<<setfill('-')<<setw(40)<<"-"<<endl;
 cout<<"Current Execution Time: "<<time<<endl<<endl;
 cout<<"Next Process on the CPU: P"<<process_index<<endl;
 cout<<setfill('-')<<setw(40)<<"-"<<endl<<endl;
 cout<<"Ready queue:"<<endl;
 printQueue(readyQueue,processes,processBurstIndex);
 cout<<setfill('-')<<setw(40)<<"-"<<endl;
 }