//
//  prioritizer.cpp
//  prioritizer
//
//  Created by Nithali Sridhar on 12/26/18.
//  Copyright © 2018 Nithali Sridhar. All rights reserved.
//

#include <stdio.h>
#include <queue>
#include <vector>
#include <iostream>
#include <fstream>
#include "assignment.h"
using namespace std;

//want to take in how many hours you have to work
//want to look through your assignments and see what you can do
//will have two modes -- are you pressed for time
//if pressed for time -- finish the assignments that are due this week
//way to order -- by due date, then by difficulty and number of hours

//need to have an update function -- with key words

//projects and hw beat reading and catch_up

struct Course{
    string name;
    int class_num;
};

struct Date {
    int month;
    int day;
};

struct Order{
    bool operator() (const Assignment &lhs, const Assignment &rhs){
        
        //lhs needs to be less priority
        
        //is it just catch up or reading
        if(lhs.type == reading | lhs.type == catch_up) return true;
        
        //is it due sooner
        else if(lhs.days_due > rhs.days_due) return true;
        
        //now we need to order by what difficulty
        else if(lhs.time_to_complete < rhs.time_to_complete) return true;
        
        else if(rhs.type == project) return true;
        
        else return false;
    }
};

class Assignment_Notebook{
public:
    Assignment_Notebook(){
        boot_notebook();
    }
    void boot_notebook(); //done
    int update(int curr_month, int curr_day); //done
    void modify_class(); //done
    string choose_class();
    void add_assignment(); //done
    size_t search(string class_name, string name); //done
    void modify_assignment(); //done
    void print_notebook();
    void save_notebook(); //done
    void run_stressed();
    void run();
private:
    vector<Assignment> agenda;
    vector<Course> courses;
    priority_queue<Assignment, vector<Assignment>, Order> schedule;
    Date date;
    //run stressed -- means finish everything that is due within two days
    //run less stressed -- requires math
    
};

void Assignment_Notebook::boot_notebook(){
    fstream fs;
    fs.open("scheduler.txt", fstream::in);
    int assignment_num, month, day;
    string class_in, name_in;
    int type, time_est, due_date;
    fs >> assignment_num;
    if(assignment_num == 0){
        int num_classes;
        string name;
        cout << "How many classes are you taking? ";
        cin >> num_classes;
        for(int j = 0; j < num_classes; ++j){
            Course temp;
            cout << "Course name: ";
            cin >> name;
            temp.name = name;
            temp.class_num = j;
            courses.push_back(temp);
        }
        cout << num_classes << " classes added!" << endl;
    }
    else{
        fs >> month >> day;
        date.month = month;
        date.day = day;
        int class_num;
        fs >> class_num;
        for(int i = 0; i < class_num; ++i){
            Course temp;
            string course_name;
            fs >> course_name;
            temp.name = course_name;
            temp.class_num = i;
            courses.push_back(temp);
        }
    }
    for(int i = 0; i < assignment_num; ++i){
        fs >> class_in >> name_in >> type >> time_est >> due_date;
        Assignment temp(class_in, name_in, type, time_est, due_date);
        agenda.push_back(temp);
    }
    cout << assignment_num << " assignments currently in notebook." << endl;
    fs.close();
    int curr_month, curr_day;
    cout << "Enter date as M and D: ";
    cin >> curr_month >> curr_day;
    int days_due = update(curr_month, curr_day);
    days_due = days_due * -1;
    for(size_t i = 0; i < agenda.size(); ++i){
        agenda[i].updateDue(days_due);
    }
    date.month = curr_month;
    date.day = curr_day;
    cout << "Notebook updated." << endl;
}

int Assignment_Notebook::update(int curr_month, int curr_day){
    int compl_month = 0;
    if(curr_month != date.month){
        if(curr_month == 1 | curr_month == 3 | curr_month == 5 | curr_month == 7 | curr_month == 8 | curr_month == 10 | curr_month == 12){
            compl_month = 31 - date.day;
        }
        else if(curr_month == 2){
            char year;
            cout << "Is it a leap year Y/N? ";
            cin >> year;
            if(year  == 'Y'){
                compl_month = 29 - date.day;
            }
            else{
                compl_month = 28 - date.day;
            }
        }
        else{
            compl_month = 30 - date.day;
        }
        date.month++;
    }
    int month_diff = curr_month - date.month;
    if(month_diff > 0){
        //approx math calculations here
        compl_month += (month_diff * 30); //later time figure out math to be super precise here lol
    }
    else{
        compl_month = curr_day - date.day;
    }
    return compl_month;
}

void Assignment_Notebook::modify_class(){
    cout << "Would you like to add (A) or drop (D) a class? ";
    char answer;
    cin >> answer;
    string class_name;
    cout << "What is the name of the class? ";
    cin >> class_name;
    if(answer == 'A'){
        Course temp;
        temp.name = class_name;
        temp.class_num = (int)courses.size();
        courses.push_back(temp);
        return;
    }
    else{
        for(size_t i = 0; i < courses.size(); ++i){
            if(courses[i].name == class_name){
                courses.erase(courses.begin() + i);
                return;
            }
        }
    }
}

string Assignment_Notebook::choose_class(){
    int num;
    for(size_t i = 0; i < courses.size(); ++i){
        cout << "Choose class: ";
        cout << courses[i].name << " (" << courses[i].class_num << ") ";
    }
    cin >> num;
    return courses[num].name;
}

void Assignment_Notebook::add_assignment(){
    int num_to_add;
    cout << "How many assignments do you want to add? ";
    cin >> num_to_add;
    string class_in, name_in;
    int type, time_est, due_date_month, due_date_day;
    for(int i = 0; i < num_to_add; ++i){
        class_in = choose_class();
        cout << "Assignment: ";
        cin >> name_in;
        cout << "Due Date: ";
        cin >> due_date_month >> due_date_day;
        int due_date = update(due_date_month, due_date_day);
        cout << "Expected Work Time in Hours: ";
        cin >> time_est;
        cout << "Project Type [hw (1), project (2), reading (3), catch_up (4)]: ";
        cin >> type;
        Assignment temp(class_in, name_in, type, time_est, due_date);
        agenda.push_back(temp);
        cout << "Assignment added!" << endl;
    }
}

size_t Assignment_Notebook::search(string class_name, string name){
    for(size_t i = 0; i < agenda.size(); ++i){
        if(agenda[i].class_name == class_name && agenda[i].name == name){
           return i;
        }
    }
    cout << "Error: no assignment found" << endl;
    return courses.size();
}

void Assignment_Notebook::modify_assignment(){
    cout << "Enter the class followed by the name of the assignment: ";
    string cl_name, as_name;
    cin >> cl_name >> as_name;
    size_t index = search(cl_name, as_name); //do not need to create = operator
    char answer;
    cout << "Have you worked on the assignment (W) or would you like to change the due date (D)? ";
    cin >> answer;
    if(answer == 'W'){
        cout << "How many hours did you spend working? ";
        int num;
        cin >> num;
        bool to_pop = agenda[index].didWork(num);
        if(to_pop){
            courses.erase(courses.begin() + index);
        }
    }
    else{
        cout << "How does the due date change? ";
        int num;
        cin >> num;
        agenda[index].updateDue(num);
    }
    return;
}

void Assignment_Notebook::print_notebook(){
    for(size_t j = 0; j < agenda.size(); ++j){
        schedule.push(agenda[j]);
    }
    for(size_t k = 0; k < agenda.size(); ++k){
        cout << schedule.top();
        schedule.pop();
    }
}

void Assignment_Notebook::save_notebook(){
    fstream fs;
    fs.open("scheduler.txt", fstream::out);
    fs << agenda.size() << endl;
    fs << date.month << " " << date.day << endl;
    fs << courses.size();
    for(size_t i = 0; i < courses.size(); ++i){
        fs << " " << courses[i].name;
    }
    fs << endl;
    for(size_t j = 0; j < agenda.size(); ++j){
        schedule.push(agenda[j]);
    }
    for(size_t k = 0; k < agenda.size(); ++k){
        int type;
        switch (schedule.top().type){
            case hw:
                type = 0;
            case project:
                type = 1;
            case reading:
                type = 2;
            case catch_up:
                type = 3;
        }
        fs << schedule.top().class_name << " " << schedule.top().name << " " << type << " " << schedule.top().time_to_complete << " " << schedule.top().days_due << endl;
        schedule.pop();
    }
    fs.close();
}



int main(){
    cout << "Welcome to the Assignment Notebook!" << endl;
    Assignment_Notebook schedule; //open the filesteam upon creation...?
    char command;
    cout << "USAGE: Add Assignments (A), Modify Assignments (M), Create Schedule (S), View Assignments (V), Modify Classes (C), Usage (U), Quit (Q)"
    << endl << "> ";
    cin >> command;
    while(command != 'Q'){
        switch(command){
            case 'A':
                schedule.add_assignment();
                break;
            case 'M':
                schedule.modify_assignment();
                break;
            /*case 'S':
                char stressed;
                int work_time;
                cout << "How many hours do you plan to spend working? ";
                cin >> work_time;
                cout << "Are you pressed for time? Y/N ";
                cin >> stressed;
                if(stressed == 'Y'){
                    schedule.run_stressed();
                }
                else{
                    schedule.run();
                }*/
            case 'V':
                schedule.print_notebook();
                break;
            case 'C':
                schedule.modify_class();
                break;
            case 'U':
                cout << "USAGE: Add Assignments (A), Modify Assignments (M), Create Schedule (S), View Assignments (V), Modify Classes (C), Usage (U), Quit (Q)" << endl;
                break;
        }
        cout << "> ";
        cin >> command;
    }
    if(command == 'Q'){
        schedule.save_notebook();
    }
    return 0;
}
