#include "CBS.h"
#include <iostream>
#include <queue>
#include <unordered_map>

vector<Path> CBS::find_solution() {
    priority_queue<CBSNode*, vector<CBSNode*>, CompareCBSNode> open; // open list

    /* generate the root CBS node */
    auto root = new CBSNode();
    all_nodes.push_back(root);  // whenever generating a new node, we need to
    // put it into all_nodes
    // so that we can release the memory properly later in ~CBS()

    // find paths for the root node
    root->paths.resize(a_star.ins.num_of_agents);
    for (int i = 0; i < a_star.ins.num_of_agents; i++) {
        // TODO: if you change the input format of function find_path()
        //  you also need to change the following line to something like
        //  root->paths[i] = a_star.find_path(i, list<Constraint>());
        root->paths[i] = a_star.find_path(i);

        if (root->paths[i].empty()) {
            cout << "Fail to find a path for agent " << i << endl;
            return vector<Path>(); // return "No solution"
        }
    }
    // compute the cost of the root node
    for (const auto& path : root->paths)
        root->cost += (int)path.size() - 1;

    // put the root node into open list
    open.push(root);

    int i=0;

    while (!open.empty()) {
        i++;
        auto cur = open.top();
        open.pop();

        vector<Collision> collisions = find_collisions(cur->paths);

        //If no collisions, then found the goal
        if(collisions.empty()){
            return cur->paths;
        }

        vector<Constraint> newConstraints;
        for(Collision c: collisions){
            if(c.type==VERTEX){
                newConstraints.emplace_back(Constraint(c.collider1, c.location, -1, c.timestamp, VERTEX));
                newConstraints.emplace_back(Constraint(c.collider2, c.location, -1, c.timestamp, VERTEX));
            }else{
                newConstraints.emplace_back(Constraint(c.collider1, c.location, c.location2, c.timestamp, EDGE));
                newConstraints.emplace_back(Constraint(c.collider2, c.location2, c.location, c.timestamp, EDGE));
            }
        }

        for(Constraint c: newConstraints){
            auto Q = new CBSNode();
            list<Constraint> QConstraints(cur->constraints);
            QConstraints.emplace_back(c);
            Q->constraints = QConstraints;
            Q->paths = cur->paths;
            bool notallempty = true;
            for(int i =0; i<a_star.ins.num_of_agents; i++){
                auto path = a_star.find_path(i, Q->constraints);

                if(!path.empty()){
                    Q->paths[i] = path;
                }else{
                    notallempty = false;
                }

            }
            int sum=0;
            for (int i = 0; i < a_star.ins.num_of_agents; i++) {
                sum += (int)Q->paths[i].size() - 1;
            }
            Q->cost = sum;

            if(notallempty) {
                open.push(Q);
            }
        }
    }

    return vector<Path>(); // return "No solution"
}

//Takes in a list of paths and returns a list of all collisions
vector<Collision> CBS::find_collisions(vector<Path> paths){
    vector<Collision> collisions;


    int largest_path = 0;
    for(auto p: paths){
        if(largest_path<p.size()){
            largest_path = p.size();
        }
    }

    //Augment the paths list so each agent stays at the goal location until everyone is finished
    //This allows for collision detections after an agent has reached their goal
    for(int i=0; i< paths.size(); i++){
        int last_location = paths[i].back();
        while(paths[i].size()<largest_path){
            paths[i].emplace_back(last_location);
        }
    }

    //Checking for vertex collisions
    //i is the current timestamp
    //j is the current agent
    for(int i =0; i<largest_path; i++){
        //location -> agent at location (boolean)
        unordered_map<int, int> locations;
        for(int j =0; j<paths.size(); j++){
            //If no agent is at this vertex at this timestamp
            if(!locations.count(paths[j][i])){
                locations[paths[j][i]] = j;
            }else{
                //Found vertex collision between agent j and agent locations[[paths[j][i]]
                collisions.emplace_back(Collision(j, locations[paths[j][i]], paths[j][i], VERTEX, i));
                return collisions;
            }
        }
    }


    //edge collisions
    for(int p=0; p<paths.size(); p++){
        for(int i =1; i<paths[p].size(); i++){
            int location = paths[p][i];

            for(int pa=0; pa<paths.size(); pa++){
                if(paths[pa][i-1]==location && paths[p][i-1]==paths[pa][i] && pa!=p){
                    collisions.emplace_back(Collision(p, pa,paths[p][i-1], paths[p][i], EDGE, i));
                    return collisions;
                }
            }
        }
    }

    return collisions;
}


CBS::~CBS() {
    // release the memory
    for (auto n : all_nodes)
        delete n;
}
