#include <iostream> 
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
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

#define NO_OF_STUDENTS 200
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

void saveParkingPasses();
void loadParkingPasses();
void viewUpdateProfile(Student &s);
string getCurrentDate(); 
void applyParkingPass(Student &s);
void renewParkingPass(Student &s);
void viewMyPassHistory(Student &s);
void viewPendingApplications();
void approveRejectPass();

int findStudent(string id);
void saveStudents();
void checkExpirationAlert(Student &s);
void generateAnalytics();
void studentMenu(Student &s);
void adminMenu(Admin &a);

int main() 
{
    loadStudents();
    loadAdmins();
    loadParkingPasses();
    
    int mainChoice;
    do {
        cout << "\n=== MPKJ Monthly Car Parking Pass System ===\n";
        cout << "1. Login\n";
        cout << "2. Register New Student\n";
        cout << "3. Exit System\n";
        cout << "Enter your choice: ";
        cin >> mainChoice;

        if (mainChoice == 1) {
            int index = -1;
            int userType = login(index);

            if (userType == 1) {
                // Show student menu, pass students[index] around as needed
                studentMenu(students[index]);
            } else if (userType == 2) {
                // Show admin menu, pass admins[index] around as needed
                adminMenu(admins[index]);
            }
        } else if (mainChoice == 2) {
            registerStudent();
        } else if (mainChoice == 3) {
            cout << "Exiting system. Goodbye!\n";
        } else {
            cout << "Invalid choice.\n";
        }
    } while (mainChoice != 3);
    
    return 0;
}

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

void registerStudent() {
    cout << "\n===== NEW STUDENT REGISTRATION =====\n";

    Student s;
    
    // Validate ID
    while (true) {
        cout << "Enter Student ID (7 digits, starting with 2): ";
        cin >> s.studentID;
        if (s.studentID.length() == 7 && s.studentID[0] == '2') {
            bool allDigits = true;
            for (int i = 0; i < 7; i++) {
                if (!isdigit(s.studentID[i])) { allDigits = false; break; }
            }
            if (allDigits) break;
        }
        cout << "Invalid Student ID. Please enter again.\n";
    }

    // Prevent duplicate registrations
    if (findStudent(s.studentID) != -1) {
        cout << "This Student ID is already registered. Please login instead.\n";
        return;
    }

    // Validate password
    while (true) {
        cout << "Enter Password (at least 6 characters): ";
        cin >> s.password;
        if (s.password.length() >= 6) break;
        cout << "Password too short. Please enter again.\n";
    }
    
    // Validate full name
	cin.ignore();
	while (true) {
	    cout << "Enter Full Name: ";
	    getline(cin, s.name);
	    bool validName = s.name.length() >= 2;
	    for (int i = 0; i < s.name.length(); i++) {
	        if (!isalpha(s.name[i]) && s.name[i] != ' ') {
	            validName = false;
	            break;
	        }
	    }
	    if (validName) break;
	    cout << "Invalid name. Name must be at least 2 characters and can only contain letters and spaces.\n";
	}
	    
    // Validate contact number
	while (true) {
	    cout << "Enter Contact Number (10 digits): ";
	    cin >> s.contactNumber;
	    if (s.contactNumber.length() == 10) {
	        bool allDigits = true;
	        for (int i = 0; i < s.contactNumber.length(); i++) {
	            if (!isdigit(s.contactNumber[i])) { allDigits = false; break; }
	        }
	        if (allDigits) break;
	    }
	    cout << "Invalid contact number. Please enter again.\n";
	}
    
    // Validate faculty
	string validFaculties[] = {"FCI", "FAM", "LKCFES", "FEGT", "MKFMHS", "FED"};
	int numValidFaculties = 6;
	
	while (true) {
	    cout << "\nSelect Faculty:\n";
	    cout << "1. FCI\n";
	    cout << "2. FAM\n";
	    cout << "3. LKCFES\n";
	    cout << "4. FEGT\n";
	    cout << "5. MKFMHS\n";
	    cout << "6. FED\n";
	    cout << "Enter choice (1-6): ";
	
	    int facChoice;
	    cin >> facChoice;
	
	    if (facChoice >= 1 && facChoice <= numValidFaculties) {
	        s.faculty = validFaculties[facChoice - 1];
	        break;
	    }
	    cin.clear();
    	cin.ignore(1000, '\n');
	    cout << "Invalid choice. Please select a number between 1 and 6.\n";
	}
	cin.ignore();
	
    // Validate Vehicle Plate
	while (true) {
	    cout << "Enter Vehicle Plate Number (3 letters followed by 4 digits, e.g. ABC1234): ";
	    cin >> s.vehicleNumber;
	
	    if (s.vehicleNumber.length() == 7) {
	        bool valid = true;
	
	        // Check first 3 characters are letters
	        for (int i = 0; i < 3; i++) {
	            if (!isalpha(s.vehicleNumber[i])) { valid = false; break; }
	        }
	
	        // Check last 4 characters are digits
	        for (int i = 3; i < 7; i++) {
	            if (!isdigit(s.vehicleNumber[i])) { valid = false; break; }
	        }
	
	        if (valid) {
	            // Convert letters to uppercase before saving
	            for (int i = 0; i < 3; i++) {
	                s.vehicleNumber[i] = toupper(s.vehicleNumber[i]);
	            }
	            break;
	        }
	    }
	    cout << "Invalid plate number. Please enter again.\n";
	}
    
    // Validate vehicle type
    while (true) {
        cout << "Enter Vehicle Type (Car/Motorcycle): ";
	    cin >> s.vehicleType;
	
	    // Convert input to lowercase for comparison
	    string lowerType = s.vehicleType;
	    for (int i = 0; i < lowerType.length(); i++) {
	        lowerType[i] = tolower(lowerType[i]);
	    }
	
	    if (lowerType == "car") { s.vehicleType = "Car"; break; }
	    else if (lowerType == "motorcycle") { s.vehicleType = "Motorcycle"; break; }
	    else cout << "Invalid vehicle type. Please enter again.\n";
    }
    
    s.regDate = getCurrentDate(); 
    s.isActive = true;             

    students[studentCount++] = s;
    saveStudents();

    cout << "\nRegistration successful! You may now login.\n";
}

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

//QY
void saveParkingPasses() {
    ofstream file("parking_passes.txt");
    for (int i = 0; i < passCount; i++) {
        ParkingPass &p = parkingPasses[i];
        file << p.passID << "|"
             << p.studentID << "|"
             << p.startDate << "|"
             << p.endDate << "|"
             << p.status << "|"
             << p.appliedDate << "|"
             << p.amount << "|"
             << p.paymentStatus << "|"
             << p.paymentDate << "|"
             << p.renewalCount << endl;
    }
    file.close();
}

void loadParkingPasses() {
    ifstream file("parking_passes.txt");
    if (!file) return;
    
    string line;
    passCount = 0;
    while (getline(file, line) && passCount < NO_OF_STUDENTS) {
        stringstream ss(line);
        ParkingPass &p = parkingPasses[passCount];
        getline(ss, p.passID, '|');
        getline(ss, p.studentID, '|');
        getline(ss, p.startDate, '|');
        getline(ss, p.endDate, '|');
        getline(ss, p.status, '|');
        getline(ss, p.appliedDate, '|');
        ss >> p.amount;
        ss.ignore();
        getline(ss, p.paymentStatus, '|');
        getline(ss, p.paymentDate, '|');
        ss >> p.renewalCount;
        passCount++;
    }
    file.close();
}

void viewUpdateProfile(Student &s) {
    int choice;
    do {
        cout << "\n=== MY PROFILE ===\n";
        cout << "Student ID     : " << s.studentID << endl;
        cout << "Full Name      : " << s.name << endl;
        cout << "Faculty        : " << s.faculty << endl;
        cout << "Contact Number : " << s.contactNumber << endl;
        cout << "Vehicle Plate  : " << s.vehicleNumber << endl;
        cout << "Vehicle Type   : " << s.vehicleType << endl;
        cout << "Registered On  : " << s.regDate << endl;

        cout << "\n--- What would you like to update? ---\n";
        cout << "1. Contact Number\n";
        cout << "2. Faculty\n";
        cout << "3. Vehicle Plate Number\n";
        cout << "4. Vehicle Type\n";
        cout << "5. Password\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter new contact number: ";
                cin >> s.contactNumber;
                cout << "Contact number updated successfully.\n";
                break;

            case 2:
                cout << "Enter new faculty: ";
                cin.ignore();
                getline(cin, s.faculty);
                cout << "Faculty updated successfully.\n";
                break;

            case 3:
                cout << "Enter new vehicle plate number: ";
                cin >> s.vehicleNumber;
                cout << "Vehicle plate number updated successfully.\n";
                break;

            case 4: {
                cout << "Enter new vehicle type (Car/Motorcycle): ";
                cin >> s.vehicleType;
                // Simple validation
                while (s.vehicleType != "Car" && s.vehicleType != "Motorcycle" &&
                       s.vehicleType != "car" && s.vehicleType != "motorcycle") {
                    cout << "Invalid type. Please enter Car or Motorcycle: ";
                    cin >> s.vehicleType;
                }
                cout << "Vehicle type updated successfully.\n";
                break;
            }

            case 5: {
                string oldPassword;
                cout << "Enter current password: ";
                cin >> oldPassword;
                if (oldPassword != s.password) {
                    cout << "Incorrect current password. Failed to update.\n";
                    break;
                }
                string newPassword, confirmPassword;
                cout << "Enter new password: ";
                cin >> newPassword;
                cout << "Confirm new password: ";
                cin >> confirmPassword;
                if (newPassword != confirmPassword) {
                    cout << "Passwords do not match. Failed to update.\n";
                    break;
                }
                s.password = newPassword;
                cout << "Password updated successfully.\n";
                break;
            }

            case 6:
                cout << "Returning to main menu...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

        // Save to file after every successful update
        if (choice >= 1 && choice <= 5) {
            saveStudents();
        }

    } while (choice != 6);
}

string getCurrentDate() {
    time_t t = time(0);
    tm* now = localtime(&t);

    int year  = now->tm_year + 1900;  // tm_year counts from 1900
    int month = now->tm_mon + 1;      // tm_mon is 0-indexed (0 = January)
    int day   = now->tm_mday;

    // Manual formatting to get YYYY-MM-DD
    stringstream ss;
    ss << year
       << "-"
       << (month < 10 ? "0" : "") << month   // pad single digit months
       << "-"
       << (day < 10 ? "0" : "") << day;       // pad single digit days

    return ss.str();
}

void applyParkingPass(Student &s) {
    // Check if there are already active monthly passes.
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].studentID == s.studentID && 
            (parkingPasses[i].status == "Active" || parkingPasses[i].status == "Pending")) {
            cout << "You already have an active or pending application!\n";
            return;
        }
    }
    
    ParkingPass newPass;
    stringstream ss;
	ss << "P" << (passCount + 1001);
	newPass.passID = ss.str();
    newPass.studentID = s.studentID;
    newPass.startDate = "";
    newPass.endDate = "";
    newPass.status = "Pending";
    newPass.appliedDate = getCurrentDate();
    newPass.amount = MONTHLY_RATE;
    newPass.paymentStatus = "Unpaid";
    newPass.paymentDate = "";
    newPass.renewalCount = 0;
    
    parkingPasses[passCount] = newPass;
    passCount++;
    saveParkingPasses();
    
    cout << "\nApplication submitted! Pass ID: " << newPass.passID << endl;
    cout << "Please wait for admin approval.\n";
}

// Student renew parking pass
void renewParkingPass(Student &s) {
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].studentID == s.studentID && 
            parkingPasses[i].status == "Active") {
            
            cout << "Current pass expires on: " << parkingPasses[i].endDate << endl;
            cout << "Renew for another month? (y/n): ";
            char choice;
            cin >> choice;
            
            if (choice == 'y' || choice == 'Y') {
                // Update new end date 
                string newEndDate;
                cout << "Enter new end date (YYYY-MM-DD): ";
                cin >> newEndDate;
                parkingPasses[i].endDate = newEndDate;
                parkingPasses[i].renewalCount++;
                parkingPasses[i].status = "Pending";  // Needs re-approval
                saveParkingPasses();
                cout << "Renewal request submitted for approval.\n";
            }
            return;
        }
    }
    cout << "No active parking pass found to renew.\n";
}

void viewMyPassHistory(Student &s) {
    bool found = false;
    cout << "\n=== Your Parking Pass History ===\n";
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].studentID == s.studentID) {
            found = true;
            ParkingPass &p = parkingPasses[i];
            cout << "Pass ID: " << p.passID << endl;
            cout << "Applied: " << p.appliedDate << endl;
            cout << "Start: " << (p.startDate.empty() ? "N/A" : p.startDate) << endl;
            cout << "End: " << (p.endDate.empty() ? "N/A" : p.endDate) << endl;
            cout << "Status: " << p.status << endl;
            cout << "Amount: RM " << p.amount << endl;
            cout << "Payment: " << p.paymentStatus << endl;
            cout << "Renewals: " << p.renewalCount << endl;
            cout << "------------------------\n";
        }
    }
    if (!found) {
        cout << "No transaction history found.\n";
    }
}

void viewPendingApplications() {
    bool hasPending = false;
    cout << "\n=== Pending Applications ===\n";
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].status == "Pending") {
            hasPending = true;
            cout << "Pass ID: " << parkingPasses[i].passID 
                 << " | Student ID: " << parkingPasses[i].studentID
                 << " | Applied: " << parkingPasses[i].appliedDate << endl;
        }
    }
    if (!hasPending) {
        cout << "No pending applications.\n";
    }
}

void approveRejectPass() {
    string passID;
    cout << "Enter Pass ID to process: ";
    cin >> passID;
    
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].passID == passID && 
            parkingPasses[i].status == "Pending") {
            
            cout << "1. Approve\n2. Reject\nChoice: ";
            int choice;
            cin >> choice;
            
            if (choice == 1) {
                parkingPasses[i].status = "Active";
                cout << "Enter start date (YYYY-MM-DD): ";
                cin >> parkingPasses[i].startDate;
                cout << "Enter end date (YYYY-MM-DD): ";
                cin >> parkingPasses[i].endDate;
                cout << "Pass approved!\n";
            } else if (choice == 2) {
                parkingPasses[i].status = "Rejected";
                cout << "Pass rejected!\n";
            }
            saveParkingPasses();
            return;
        }
    }
    cout << "Pending pass not found.\n";
}

//HY
int findStudent(string id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentID == id) {
            return i;
        }
    }
    return -1;
}

void saveStudents() {
    ofstream file("students.txt");
    if (!file) {
        cout << "ERROR: Could not open students.txt for writing!\n";
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        Student &s = students[i];
        file << s.studentID << "|"
             << s.password << "|"
             << s.name << "|"
             << s.faculty << "|"
             << s.contactNumber << "|"
             << s.vehicleNumber << "|"
             << s.vehicleType << "|"
             << s.regDate << "|"
             << (s.isActive ? "1" : "0") << "\n";
    }
    file.close();
}

// 1. Expiration Alert
void checkExpirationAlert(Student &s) {
    bool hasActive = false;
    string endDateStr = "";
    
    for(int i = 0; i < passCount; i++) {
        if(parkingPasses[i].studentID == s.studentID && parkingPasses[i].status == "Active") {
            hasActive = true;
            endDateStr = parkingPasses[i].endDate;
            break;
        }
    }
    
    if(hasActive) {
        cout << "\n================== SYSTEM ALERT ==================" << endl;
        cout << " [REMINDER] You have an ACTIVE parking pass." << endl;
        cout << " Expiry Date: " << endDateStr << endl;
        cout << " Please remember to renew before approaching month end!" << endl;
        cout << "==================================================\n" << endl;
    }
}

// 2. Analytics & Summary Report
void generateAnalytics() {
    cout << "\n========== MPKJ PARKING PASS ANALYTICS ==========\n";
    
    int totalActive = 0;
    int totalPending = 0;
    int totalRenewals = 0;
    
    int facultyCount[20] = {0};
    string faculties[20];
    int numFaculties = 0;

    for(int i = 0; i < passCount; i++) {
        if(parkingPasses[i].status == "Active") totalActive++;
        else if(parkingPasses[i].status == "Pending") totalPending++;

        totalRenewals += parkingPasses[i].renewalCount;

        int sIdx = findStudent(parkingPasses[i].studentID);
        if(sIdx != -1) {
            string fac = students[sIdx].faculty;
            bool found = false;
            for(int j = 0; j < numFaculties; j++) {
                if(faculties[j] == fac) {
                    facultyCount[j]++;
                    found = true; 
                    break;
                }
            }
            if(!found && numFaculties < 20) {
                faculties[numFaculties] = fac;
                facultyCount[numFaculties] = 1;
                numFaculties++;
            }
        }
    }

    cout << "[General Overview]" << endl;
    cout << "Total Active Passes  : " << totalActive << endl;
    cout << "Total Pending Passes : " << totalPending << endl;
    cout << "Total Renewal Times  : " << totalRenewals << endl;

    cout << "\n[Application by Faculty/Institute]" << endl;
    if(numFaculties == 0) {
        cout << "No data available." << endl;
    } else {
        for(int i = 0; i < numFaculties; i++) {
            cout << "- " << faculties[i] << " : " << facultyCount[i] << " passes" << endl;
        }
    }

    int totalCapacity = 500;
    double utilization = 0.0;
    if(totalCapacity > 0) {
        utilization = (double)totalActive / totalCapacity * 100.0;
    }
    
    cout << "\n[Car Park Utilization (Sungai Long Compound)]" << endl;
    cout << "Total Capacity     : " << totalCapacity << " spots" << endl;
    cout << "Current Occupied   : " << totalActive << " spots" << endl;
    cout << "Utilization Rate   : " << utilization << " %" << endl;
    cout << "=================================================\n";
}

// 3. Student Menu
void studentMenu(Student &s) {
    checkExpirationAlert(s);
    
    int choice;
    do {
        cout << "\n=== STUDENT MENU ===\n";
        cout << "1. Apply New Parking Pass\n";
        cout << "2. Renew Parking Pass\n";
        cout << "3. View My Pass History\n";
        cout << "4. View / Update My Profile\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: applyParkingPass(s); break;
            case 2: renewParkingPass(s); break;
            case 3: viewMyPassHistory(s); break;
            case 4: viewUpdateProfile(s); break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while(choice != 5);
}

// 4. Admin Menu
void adminMenu(Admin &a) {
    int choice;
    do {
        cout << "\n=== ADMIN MENU ===\n";
        cout << "1. View Pending Applications\n";
        cout << "2. Approve / Reject Pass\n";
        cout << "3. Generate Analytics & Summary Report\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: viewPendingApplications(); break;
            case 2: approveRejectPass(); break;
            case 3: generateAnalytics(); break;
            case 4: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while(choice != 4);
}
