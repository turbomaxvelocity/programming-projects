//Space Clock ADT (Abstract Data Time)
//This represents a fictional planet with only 7 hours is a day

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

struct SpaceClock {
    //data members(the domain and range of values)
    int hours;
    int minutes;
    int seconds;
    //constructor
    //this intializes a new space clock with starting values
    SpaceClock(int h =0, int m = 0, int s = 0) {
        //modulo operator ensures they stay in range
        hours = h % 7;
        minutes = m % 60;
        seconds = s % 60;
    }
    void tick() {
        // step 1: increment seconds using modular arthemetic
        seconds = (seconds + 1) % 60;
        //step 2: check is seconds wrapped to 00
        if (seconds == 0) {
            minutes = (minutes + 1) % 60;
        }
        //step 3: check if minutes wrapped to 00
        if (minutes == 0) {
            hours = (hours + 1) % 7;
        }
    }
    //set time operation
    //allows manual setting to a specfic time
    void setTime(int h, int m, int s) {
        hours = h % 7;
        minutes = m % 60;
        seconds = s % 60;
    }

    //display operation
    void display() {
        //print hours
        cout << hours << ":" ;
        //set fill pads with zeros
        cout << setfill('0')<<setw (2)<< minutes << ":";
        cout << setfill('0')<<setw (2) << seconds;


    }
};
//earth clock abstract data time
struct EarthClock {
    int hours;
    int minutes;
    int seconds;
    EarthClock(int h =0, int m = 0, int s = 0) {
        hours = h % 24;
        minutes = m % 60;
        seconds = s % 60;
    }
    void tick() {
        seconds = (seconds + 1) % 60;
        if (seconds == 0) {
            minutes = (minutes + 1) % 60;
        }
        if (minutes == 0) {
            hours = (hours + 1) % 24;
        }
    }
    void display() {
        cout << setfill('0') << setw(2) << hours << ":";
        cout << setfill('0') << setw(2) << minutes << ":";
        cout << setfill('0') << setw(2) << seconds;
    }
};


int main()
{

//create clock instances
    SpaceClock spaceClock(6,59,55);
    EarthClock earthClock(23,59,55);
    cout << "Space Planet (7-hour day)  |  Earth (24-hour day)" << endl;
    cout << "---------------------------------------------------" << endl;
    //run both clocks together
    for (int i = 0; i < 30; i++) {
        // run for 30 seconds
        //moves cursor back to start of line (carriage return)
        cout << "\r        ";
        spaceClock.display();
        cout << "          |        ";
        earthClock.display();
        cout << flush;  // make sure it displays immediately

        // wait 1 second
        this_thread::sleep_for(chrono::seconds(1));

        // tick both clocks
        spaceClock.tick();
        earthClock.tick();
    }

    cout << endl; 
   return 0;   }
