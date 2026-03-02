#include <iostream> 
#include <fstream>
#include <string>
using namespace std;

struct Student {
    string studentID;
    string name;
    string email;
    string contactNumber;
    string faculty;
    string program;
    string vehicleNumber;
    string vehicleType;
    string username;
    string password;
    string registrationDate;
    bool isActive;
};

struct ParkingPass {
    string passID;
    string studentID;
    string startDate;
    string endDate;
    string status;
    string appliedDate;
    double amount;
    string paymentStatus;
    string paymentDate;
    int renewalCount;
};

struct Admin {
    string adminID;
    string name;
    string username;
    string password;
    string email;
};

#define NO_OF_STUDENTS 100
#define NO_OF_ADMINS 10

Student students[NO_OF_STUDENTS];
ParkingPass parkingPasses[NO_OF_STUDENTS];
Admin admins[NO_OF_ADMINS];

int studentCount = 0;
int passCount = 0;
int adminCount = 0;

int main()
{
	
}
