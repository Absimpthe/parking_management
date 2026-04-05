#include <iostream> 
#include <fstream>
#include <string>
using namespace std;

struct Student {
    string studentID;
    string password;
    string name;
    string faculty;
    string contactNumber;
    string vehicleNumber;
    string vehicleType;
    string regDate;
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
    string password;
    string contactNumber;
};

#define NO_OF_STUDENTS 100
#define NO_OF_ADMINS 10
#define MONTHLY_RATE 30.00

Student students[NO_OF_STUDENTS];
ParkingPass parkingPasses[NO_OF_STUDENTS];
Admin admins[NO_OF_ADMINS];

int studentCount = 0;
int passCount = 0;
int adminCount = 0;

void loadStudents();
void loadAdmins();
void registerStudent();
int login(int& index);

int main() 
{
    loadStudents();
    loadAdmins();

    int index = -1;
    int userType = login(index);

    if (userType == 1) {
        // Show student menu, pass students[index] around as needed
    } else if (userType == 2) {
        // Show admin menu, pass admins[index] around as needed
    }
}
/*
 * loadStudents()
 * Reads students.txt into the students[] array.
 * Each line holds pipe-delimited fields:
 *   studentID|password|name|contact|plate|vehicleType|faculty|email
 */
void loadStudents()
{
	ifstream file("students.txt");
	if (!file) return;
	string line;
	studentCount = 0;
	while (getline(file, line) && studentCount < NO_OF_STUDENTS) {
        stringstream ss(line);
        Student& s = students[studentCount];
        getline(ss, s.studentID, '|');
        getline(ss, s.password, '|');
        getline(ss, s.name, '|');
        getline(ss, s.faculty, '|');
        getline(ss, s.contactNumber, '|');
        getline(ss, s.vehicleNumber, '|');
        getline(ss, s.vehicleType, '|');
        getline(ss, s.regDate, '|');
        string activeStr;  // need this to convert string to bool
        getline(ss, activeStr, '|');
        s.isActive = (activeStr == "1");
        studentCount++;
    }
}

void loadAdmins()
{
	ifstream file("admins.txt");
	if (!file) return;
	string line;
	adminCount = 0;
	while (getline(file, line) && adminCount < NO_OF_ADMINS) {
        stringstream ss(line);
        Admin& a = admins[adminCount];
        getline(ss, a.adminID, '|');
        getline(ss, a.name, '|');
        getline(ss, a.password, '|');
        getline(ss, a.contactNumber, '|');
        adminCount++;
    }
}

/*
 * registerStudent()
 *
 * Prompts the user to enter all personal details and creates a new
 * Student record. Before saving, it checks that the student ID is
 * not already registered. The new record is appended to students[]
 * and immediately written to disk via saveStudents().
 */
void registerStudent() {
    cout << "\n===== NEW STUDENT REGISTRATION =====\n";

    Student s;
    cout << "Enter Student ID : "; cin >> s.studentID;

    // Prevent duplicate registrations
    if (findStudent(s.studentID) != -1) {
        cout << "This Student ID is already registered. Please login instead.\n";
        return;
    }

    cout << "Enter Password   : "; cin >> s.password;
    cin.ignore();  // needed for getline()
    cout << "Enter Full Name  : "; getline(cin, s.name);  // needed to read string that includes spaces
    cout << "Enter Contact No    : "; cin >> s.contactNumber;
    cout << "Enter Faculty       : "; cin >> s.faculty;
    cout << "Enter Vehicle Plate : "; cin >> s.vehicleNumber;
    cout << "Vehicle Type (Car/Motorcycle) : "; cin >> s.vehicleType;

    students[studentCount++] = s;
    saveStudents();

    cout << "\nRegistration successful! You may now login.\n";
}

/*
 * login()
 *
 * Accepts an ID and password from the user and checks against both
 * the students[] and admins[] arrays. Returns 1 if a student match
 * is found, 2 if an admin match is found, and 0 if login fails.
 * The matched index is returned via the output parameter 'index'.
 */
int login(int& index) {
    string id, password;

    cout << "\n===== LOGIN =====\n";
    cout << "Enter ID       : "; cin >> id;
    cout << "Enter Password : "; cin >> password;

    // Check students array
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentID == id && students[i].password == password) {
            index = i;
            cout << "\nLogin successful! Welcome, " << students[i].name << ".\n";
            return 1; // Student
        }
    }

    // Check admins array
    for (int i = 0; i < adminCount; i++) {
        if (admins[i].adminID == id && admins[i].password == password) {
            index = i;
            cout << "\nLogin successful! Welcome, " << admins[i].name << ".\n";
            return 2; // Admin
        }
    }

    cout << "\nInvalid ID or password. Please try again.\n";
    return 0;
}
