//
//  assignment.h
//  prioritizer
//
//  Created by Nithali Sridhar on 1/17/19.
//  Copyright © 2019 Nithali Sridhar. All rights reserved.
//

#ifndef assignment_h
#define assignment_h
#include <stdio.h>
#include <queue>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


#endif /* assignment_h */
enum Type {hw, project, reading, catch_up};

class Assignment{
public:
    //constructor -- requires this
    Assignment(string class_in, string name_in, int type, int time_est, int due_date){
        class_name = class_in;
        name = name_in;
        switch (type){
            case 0:
                type = hw;
            case 1:
                type = project;
            case 2:
                type = reading;
            case 3:
                type = catch_up;
        }
        time_to_complete = time_est;
        days_due = due_date;
    }
    
    Assignment(string name_in){
        class_name = "misc.";
        name = name_in;
        type = hw;
        time_to_complete = 0;
        days_due = 0;
    }
    
    void updateDue(int new_due){
        days_due += new_due; //input extension
    }
    
    bool didWork(int time_worked){
        time_to_complete -= time_worked;
        if(time_to_complete == 0){
            string finished;
            cout << "Have you completed this assignment? Y/N ";
            cin >> finished;
            if(finished == "Y") return true;
            else return false;
        }
        else return false;
    }
    
    string class_name;
    string name;
    Type type;
    int time_to_complete; //difficulty
    int days_due;
    friend class Order;
    friend class Assignment_Notebook;
};

ostream & operator<<(ostream &os, Assignment const task){
    os << task.name << " for " << task.class_name << " due in " << task.days_due << " days with " <<
    task.time_to_complete << " hours left" << endl;
    return os;
    }
