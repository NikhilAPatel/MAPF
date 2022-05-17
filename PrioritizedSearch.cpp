#include "PrioritizedSearch.h"
#include <iostream>

vector<Path> PrioritizedSearch::find_solution() {

    int num_of_agents = a_star.ins.num_of_agents;
    vector<Path> paths(num_of_agents);

    // assign priority ordering to agents
    // By default, we use the index ordering of the agents where
    // the first always has the highest priority.
    vector<int> priorities;
     for (int i = 0; i < num_of_agents; i++) {
         priorities.push_back(i);
     }

    // plan paths
    for (int i=0; i<priorities.size();i++) {
        list<Constraint> constraints;
        for(int j=0; j<i; j++){
            for(int k=0; k<paths[priorities[j]].size(); k++){
                if(k==0){
                    //First square. Can't be here at t=0
                    constraints.emplace_back(Constraint(priorities[i], paths[priorities[j]][k], -1, 0, VERTEX));
                }else{
                    //Subsequent square. Can't be there at time k
                    //And can't go from previous square to this square from time t-1 to t
                    //And can't go from current square back to previous square
                    constraints.emplace_back(Constraint(priorities[i], paths[priorities[j]][k], -1, k, VERTEX));
                    constraints.emplace_back(Constraint(priorities[i], paths[priorities[j]][k-1], paths[priorities[j]][k], k, EDGE));
                    constraints.emplace_back(Constraint(priorities[i], paths[priorities[j]][k], paths[priorities[j]][k-1], k, EDGE));
                }
            }
            for(int u=paths[priorities[j]].size(); u<1000; u++){
                constraints.emplace_back(Constraint(priorities[i], paths[priorities[j]].back(), -1, u, VERTEX));
            }
        }

        paths[priorities[i]] = a_star.find_path(priorities[i], constraints);

        if (paths[priorities[i]].empty()) {
            paths.resize(priorities[i]);
            return paths;
        }
    }
    return paths;
}


