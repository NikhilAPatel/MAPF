#include "AStarPlanner.h"
#include <queue>
#include <unordered_map>
#include <algorithm> // reverse
#include <iostream>

ostream& operator<<(ostream& os, const Path& path)
{
    for (auto loc : path) {
        os << loc << " ";
    }
    return os;
}

Path AStarPlanner::make_path(const AStarNode* goal_node) const {
    Path path;
    const AStarNode* curr = goal_node;
    while (curr != nullptr) {
        path.push_back(curr->location);
        curr = curr->parent;
    }
    std::reverse(path.begin(),path.end());
    return path;
}

Path AStarPlanner::find_path(int agent_id, const list<Constraint>& constraints) {
    int start_location = ins.start_locations[agent_id];
    int goal_location = ins.goal_locations[agent_id];

    // Open list
    priority_queue<AStarNode*, vector<AStarNode*>, CompareAStarNode> open;

    // Unordered map is an associative container that contains key-value pairs with unique keys.
    // The following unordered map is used for duplicate detection, where the key is the location of the node.
    unordered_map<pair<int, int>, AStarNode*, hash_pair> all_nodes;

    int h = ins.get_Manhattan_distance(start_location, goal_location); // h value for the root node
    auto root = new AStarNode(start_location, 0, h, nullptr, 0);
    open.push(root);

    Path path;
    while (!open.empty()) {
        auto curr = open.top();
        open.pop();

        if(curr->timestep>500){
            return {};
        }

        // goal test
        if (curr->location == goal_location && no_future_constraints(curr, agent_id, constraints)) {
            path = make_path(curr);
            break;
        }

        // generate child nodes
        auto child_nodes = ins.get_adjacent_locations(curr->location);
        child_nodes.emplace_back(curr->location);
        for (auto next_location : child_nodes) {
            auto it = all_nodes.find(make_pair(next_location, curr->timestep+1));
            if (it == all_nodes.end()) {// the location has not been visited before
                int next_g = curr->g + 1;
                int next_h = ins.get_Manhattan_distance(next_location, goal_location);
                auto next = new AStarNode(next_location, next_g, next_h, curr, curr->timestep+1);

                //Check to make sure that this node follows constraints
                if(satisfies_constraints(next, curr->location, agent_id, constraints)) {
                    open.push(next);
                    all_nodes[make_pair(next_location, curr->timestep + 1)] = next;
                }
            }
            // Note that if the location has been visited before,
            // next_g + next_h must be greater than or equal to the f value of the existing node,
            // because we are searching on a 4-neighbor grid with uniform-cost edges.
            // So we don't need to update the existing node.
        }
    }

    // release memory
    for (auto n : all_nodes)
        delete n.second;

    return path;
}

bool AStarPlanner::no_future_constraints(const AStarNode* node, int agent_id, const list<Constraint>& constraints){
    for(Constraint c: constraints){
        if(agent_id==get<0>(c) && node->timestep<=get<3>(c) && (node->location==get<1>(c) && get<4>(c)==VERTEX)){
            return false;
        }
    }

    return true;
}

bool AStarPlanner::satisfies_constraints(const AStarNode* node, int current_location, int agent_id, const list<Constraint>& constraints){
    for (Constraint c: constraints){
        //Checks if this constraint applies to the current agent
        if(agent_id!=get<0>(c)){
            continue;
        }

        //Check constraints
        if(get<4>(c)==VERTEX){
            if(node->location==get<1>(c) && node->timestep==get<3>(c)){
                return false;
            }
        }else if(get<4>(c) == EDGE){
            if(node->location==get<2>(c) && current_location==get<1>(c) && node->timestep==get<3>(c)){
                return false;
            }
        }
    }

    return true;
}
