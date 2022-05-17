//#include "gtest/gtest.h"
//#include "../MAPFInstance.h"
//#include "../AStarPlanner.h"
//
//using namespace std;
//
//
//TEST(task1Test, basictest){
//    MAPFInstance ins;
//    string input_file = "../test/test_1.txt";
//    if (ins.load_instance(input_file)) {
//        ins.print_instance();
//    } else {
//        cout << "Fail to load the instance " << input_file << endl;
//        exit(-1);
//    }
//
//    AStarPlanner a_star(ins);
//    vector<Path> paths(ins.num_of_agents);
//    for (int i = 0; i < ins.num_of_agents; i++) {
//        paths[i] = a_star.find_path(i); // plan paths independently by ignoring other agents
//    }
//
//    // print paths
//    cout << "Paths:" << endl;
//    int sum = 0;
//    for (int i = 0; i < ins.num_of_agents; i++) {
//        cout << "a" << i << ": " << paths[i] << endl;
//        sum += (int)paths[i].size() - 1;
//    }
//    cout << "Sum of cost: " << sum << endl;
//
//    // save paths
//    ofstream myfile (output_file.c_str(), ios_base::out);
//    if (myfile.is_open()) {
//        for (int i = 0; i < ins.num_of_agents; i++) {
//            myfile << paths[i] << endl;
//        }
//        myfile.close();
//    } else {
//        cout << "Fail to save the paths to " << output_file << endl;
//        exit(-1);
//    }
//
//
//
//    EXPECT_EQ(1, 0);
//}