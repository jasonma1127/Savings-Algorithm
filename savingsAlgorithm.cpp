#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <random>

using namespace std;
vector<vector<int>> data;
vector<vector<int>> distanceVec;
int capacity = 50;

vector<int> calculatePathSum(vector<vector<int>> _Scurrent){
	vector<vector<int>> Scurrent;
	vector<int> shortestPathVec;
	Scurrent = _Scurrent;
	
	for(int i = 0; i < Scurrent.size(); i ++){
		int pathSum = 0;
		for(int j = 0; j < Scurrent[i].size() - 1; j ++){
			pathSum += distanceVec[Scurrent[i][j] - 1][Scurrent[i][j + 1] - 1];
		}
		shortestPathVec.push_back(pathSum);
	}
	
	return shortestPathVec;
}

vector<vector<int>> randomCreateSolution(){
	vector<int> randomSolution;
	vector<vector<int>> randomSolutionVec;
	
	//create a initial solution
	for(int i = 2; i <= data.size(); i ++){
        randomSolution.push_back(i);
    }
    
    //shuffle the initial solution
    random_shuffle(randomSolution.begin(), randomSolution.end());
    
    bool flag = true;
	randomSolutionVec.push_back(randomSolution);
    while(flag){
    	int i = 0;
    	while(i <= randomSolutionVec.size()){
    		//if is the end of the randomSolutionVec then stop
    		if(i == randomSolutionVec.size()){
    			flag = false;
    			break;
			} else {
				
				//caculate the sum of the demand
				int sum = 0;
				for(int j = 0; j < randomSolutionVec[i].size(); j++){
					sum += data[randomSolutionVec[i][j] - 1][3];
				}
				
				if(sum > capacity) {
					vector<int> temp;
					randomSolutionVec.push_back(temp);
					
					//split the route in half
					int mid = (randomSolutionVec[i].size()) / 2;
					for(int j = mid; j < randomSolutionVec[i].size(); j++){
						randomSolutionVec[randomSolutionVec.size()-1].push_back(randomSolutionVec[i][j]);
					}
					randomSolutionVec[i].erase(randomSolutionVec[i].begin() + mid, randomSolutionVec[i].end());
					i = i;
					
				} else {
					i ++;
				}
			}
		}
	}
	
	for(int i = 0; i < randomSolutionVec.size(); i ++){
		randomSolutionVec[i].insert(randomSolutionVec[i].begin(), 1);
		randomSolutionVec[i].push_back(1);
	}
	
	return randomSolutionVec;    
}

void savingsAlgorithm(){
	vector<vector<int>> savingsVec;
	vector<vector<int>> routeDemandLength;
	
	//caculate savingsVec
	for(int i = 2; i < data.size(); i ++){
		for(int j = i + 1; j < data.size() + 1; j ++){
			vector<int> temp;
			savingsVec.push_back(temp);
			
			int savings = distanceVec[i - 1][0] + distanceVec[0][j - 1] - distanceVec[i - 1][j - 1];
			savingsVec[savingsVec.size() - 1].push_back(savings);
			savingsVec[savingsVec.size() - 1].push_back(i);
			savingsVec[savingsVec.size() - 1].push_back(j);
		}
	}
	
	//sort the savingsVec by savings
	sort(savingsVec.rbegin(), savingsVec.rend());
	
	//create initial solution
	for(int i = 2; i < data.size() + 1; i ++){
		vector<int> temp;
		routeDemandLength.push_back(temp);
		routeDemandLength[i - 2].push_back(1);
		routeDemandLength[i - 2].push_back(i);
		routeDemandLength[i - 2].push_back(1);
		routeDemandLength[i - 2].push_back(data[i - 1][3]);
		routeDemandLength[i - 2].push_back(2 * distanceVec[i - 1][0]);		
	}
	
	cout << "Initial routeDemandLength" << endl;
	for(int i = 0; i < routeDemandLength.size(); i ++){
		for(int j = 0; j < routeDemandLength[i].size(); j ++){
			cout << routeDemandLength[i][j] << " ";
		}
		cout << endl;
	}
	
	int shortestPath = 0;
	for(int i = 0; i < routeDemandLength.size(); i ++){
		shortestPath += routeDemandLength[i][routeDemandLength[i].size() - 1];
	}
	cout << "shortestPath => " << shortestPath << endl;
	cout << endl;
	
	//merge the route accroding to the savingsVec
	for(int i = 0; i < savingsVec.size(); i ++){
		int customer_1 = savingsVec[i][1];
		int customer_2 = savingsVec[i][2];
		int route_1;
		int route_2;
		
		for(int j = 0; j < routeDemandLength.size(); j ++){
			for(int k = 0; k < routeDemandLength[j].size() - 2; k ++){
							 
				 if(routeDemandLength[j][k] == customer_1) {
				 	route_1 = j;
				 }
				 if(routeDemandLength[j][k] == customer_2) {
				 	route_2 = j;
				 }
			}
		}
		if(route_1 == route_2) continue;
		
		int tempRoute;
		if(route_1 > route_2) {
			tempRoute = route_1;
			route_1 = route_2;
			route_2 = tempRoute;
		}
				
		int demandSum = 0;
		for(int j = 0; j < routeDemandLength[route_1].size() - 2; j ++){
			demandSum += data[routeDemandLength[route_1][j] - 1][3];
		}
		for(int j = 0; j < routeDemandLength[route_2].size() - 2; j ++){
			demandSum += data[routeDemandLength[route_2][j] - 1][3];
		}
		
		if(demandSum > capacity) continue;
		
		vector<int> greaterRoute;
		vector<int> lowerRoute;
		
		if(routeDemandLength[route_1].size() >= routeDemandLength[route_2].size()) {
			greaterRoute = {routeDemandLength[route_1].begin(), routeDemandLength[route_1].end() - 3};
			lowerRoute = {routeDemandLength[route_2].begin() + 1, routeDemandLength[route_2].end() - 2};
		} else {
			greaterRoute = {routeDemandLength[route_2].begin(), routeDemandLength[route_2].end() - 3};
			lowerRoute = {routeDemandLength[route_1].begin() + 1, routeDemandLength[route_1].end() - 2};
		}
		
		//merge two route
		greaterRoute.insert(greaterRoute.end(), lowerRoute.begin(), lowerRoute.end());
		
		int origPathSum = routeDemandLength[route_1][routeDemandLength[route_1].size() - 1] + routeDemandLength[route_2][routeDemandLength[route_2].size() - 1];

		int mergePathSum = 0;
	    for(int j = 0; j < greaterRoute.size() - 1; j ++){
	        mergePathSum += distanceVec[greaterRoute[j] - 1][greaterRoute[j + 1] - 1];
	    }
	    
	    if(origPathSum < mergePathSum) continue;
	    
	    routeDemandLength.erase(routeDemandLength.begin() + route_2);
		routeDemandLength.erase(routeDemandLength.begin() + route_1);
		
		greaterRoute.push_back(demandSum);
		greaterRoute.push_back(mergePathSum);
		
		routeDemandLength.push_back(greaterRoute);
		
	}
	
	cout << "Final routeDemandLength" << endl;
	for(int i = 0; i < routeDemandLength.size(); i ++){
		for(int j = 0; j < routeDemandLength[i].size() - 2; j ++){
			cout << routeDemandLength[i][j] << " ";
		}
		cout << endl;
	}
	
	shortestPath = 0;
	for(int i = 0; i < routeDemandLength.size(); i ++){
		shortestPath += routeDemandLength[i][routeDemandLength[i].size() - 1];
	}
	cout << "shortestPath => " << shortestPath << endl;
	
}

int main(){
	ifstream dataFile;
	dataFile.open("Solomon's_instance_1_depot_10_customers.txt");
	
	//read file
	if(dataFile.is_open()){
		cout << "File open successed" << endl;
		
		string line;
		int dataIndex = 0;
		while(getline(dataFile, line)){
			float num;
			istringstream istr(line);
			vector<int> temp;
			data.push_back(temp);
			
			while(istr >> num){
				data[dataIndex].push_back(num);
			}
			dataIndex ++;
		}
	} else {
		cout << "File open failed" << endl;
	}

	//caculate distance between the customer
	for(int i = 0; i < data.size(); i ++){
		vector<int> temp;
		distanceVec.push_back(temp);
		for(int j = 0; j < data.size(); j ++){
			distanceVec[i].push_back(sqrt(pow(data[i][1] - data[j][1], 2) + pow(data[i][2] - data[j][2], 2)));
		}
	}
	
	//execute Savings Algorithm
	savingsAlgorithm();	
}




