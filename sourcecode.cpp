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
    string securityPhrase;
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
    string securityPhrase;
};

#define NO_OF_STUDENTS 500
#define NO_OF_ADMINS 50
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
void registerAdmin();
string validateStudentID();
string validatePassword();
string validateName();
string validateContactNumber();
string validateFaculty();
string validateVehiclePlate();
string validateVehicleType();
string validateSecurityPhrase();
string validateAdminID();
int findAdmin(string id);
void resetPassword(const string &id);
int login(int& index);
void saveParkingPasses();
void loadParkingPasses();
void viewUpdateProfile(Student &s);
void viewUpdateAdminProf(Admin &a);
void makePayment(Student &s);
string getCurrentDate(); 
void applyParkingPass(Student &s);
bool parseDate(const string &date, int &y, int &m, int &d);
bool isLeapYear(int y);
int daysInMonth(int y, int m);
string formatDate(int y, int m, int d);
string addOneDay(const string &date);
string addMonthsMinusOneDay(const string &startDate, int months);
void renewParkingPass(Student &s);
void viewMyPassHistory(Student &s);
void viewPendingApplications();
void approveRejectPass();
void viewCurrentAppStatus(Student &s);
int findStudent(string id);
void saveStudents();
void saveAdmins();
void checkExpirationAlert(Student &s);
void generateAnalytics();
void studentMenu(Student &s);
void adminMenu(Admin &a);

int main() 
{
    loadStudents();
    loadAdmins();
    loadParkingPasses();
    
    int mainChoice = 0;
    do {
        cout << "\n=== MPKJ Monthly Car Parking Pass System ===\n";
        cout << "1. Login\n";
        cout << "2. Register New Student\n";
        cout << "3. Register New Admin\n";
        cout << "4. Exit System\n";
        cout << "Enter your choice: ";

        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            mainChoice = 0;
        }

        switch (mainChoice) {
            case 1: {
                int index = -1;
                int userType = login(index);
                if (userType == 2) {
                    studentMenu(students[index]);
                } else if (userType == 1) {
                    adminMenu(admins[index]);
                }
                break;
            }
            case 2:
                registerStudent();
                break;
            case 3:
            	registerAdmin();
            	break;
            case 4:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid input. Please enter 1 to 4.\n";
        }
    } while (mainChoice != 4);
    
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
        // Try to read security answer
		if (!getline(ss, s.securityPhrase, '|')) {
		    s.securityPhrase = ""; // if no answer, default to null
		}
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
        if (!getline(ss, a.securityPhrase, '|')) {
            a.securityPhrase = "";
        }
        adminCount++;
    }
}

void registerStudent() {
    cout << "\n===== NEW STUDENT REGISTRATION =====\n";

    Student s;

    s.studentID = validateStudentID();

    if (findStudent(s.studentID) != -1) {
        cout << "This Student ID is already registered. Please login instead.\n";
        return;
    }

    s.password      = validatePassword();
    s.name          = validateName();
    s.contactNumber = validateContactNumber();
    s.faculty       = validateFaculty();
    s.vehicleNumber = validateVehiclePlate();
    s.vehicleType   = validateVehicleType();
    s.regDate       = getCurrentDate();
    s.securityPhrase   = validateSecurityPhrase();
    s.isActive      = true;

    students[studentCount++] = s;
    saveStudents();

    cout << "\nRegistration successful! You may now login.\n";
}

void registerAdmin() {
    cout << "\n===== NEW ADMIN REGISTRATION =====\n";

    if (adminCount >= NO_OF_ADMINS) {
        cout << "Admin limit reached. Cannot register more admins.\n";
        return;
    }

    Admin a;

    a.adminID = validateAdminID();

    if (findAdmin(a.adminID) != -1) {
        cout << "This Admin ID is already registered.\n";
        return;
    }

    a.password = validatePassword();
    a.name = validateName();
    a.contactNumber = validateContactNumber();
    a.securityPhrase = validateSecurityPhrase();

    admins[adminCount++] = a;
    saveAdmins();

    cout << "\nAdmin registration successful!\n";
}

int findAdmin(string id) {
    for (int i = 0; i < adminCount; i++) {
        if (admins[i].adminID == id) return i;
    }
    return -1;
}

string validateStudentID() {
    string id;
    while (true) {
        cout << "Enter Student ID (7 digits, starting with 2): ";
        cin >> id;
        if (id.length() == 7 && id[0] == '2') {
            bool allDigits = true;
            for (int i = 0; i < 7; i++) {
                if (!isdigit(id[i])) { allDigits = false; break; }
            }
            if (allDigits) return id;
        }
        cout << "Invalid Student ID. Please enter again.\n";
    }
}

string validateAdminID() {
    string id;
    while (true) {
        cout << "Enter Admin ID (starts with 1, total 7 digits): ";
        cin >> id;

        if (id.length() == 7 && id[0] == '1') {
            bool allDigits = true;
            for (int i = 0; i < 7; i++) {
                if (!isdigit(id[i])) {
                    allDigits = false;
                    break;
                }
            }
            if (allDigits) return id;
        }

        cout << "Invalid Admin ID. Please enter again.\n";
    }
}

string validatePassword() {
    string password;
    while (true) {
        cout << "Enter Password (at least 6 characters): ";
        cin >> password;
        if (password.length() >= 6) return password;
        cout << "Password too short. Please enter again.\n";
    }
}

string validateName() {
    string name;
    cin.ignore();
    while (true) {
        cout << "Enter Full Name: ";
        getline(cin, name);
        bool valid = name.length() >= 2;
        for (int i = 0; i < name.length(); i++) {
            if (!isalpha(name[i]) && name[i] != ' ') { valid = false; break; }
        }
        if (valid) return name;
        cout << "Invalid name. Name must be at least 2 characters and contain only letters and spaces.\n";
    }
}

string validateContactNumber() {
    string contact;
    while (true) {
        cout << "Enter Contact Number (10 digits): ";
        cin >> contact;
        if (contact.length() == 10) {
            bool allDigits = true;
            for (int i = 0; i < contact.length(); i++) {
                if (!isdigit(contact[i])) { allDigits = false; break; }
            }
            if (allDigits) return contact;
        }
        cout << "Invalid contact number. Please enter again.\n";
    }
}

string validateFaculty() {
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
        if (facChoice >= 1 && facChoice <= numValidFaculties) return validFaculties[facChoice - 1];
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice. Please select a number between 1 and 6.\n";
    }
}

string validateVehiclePlate() {
    string plate;
    while (true) {
        cout << "Enter Vehicle Plate Number (3 letters followed by 4 digits, e.g. ABC1234): ";
        cin >> plate;
        if (plate.length() == 7) {
            bool valid = true;
            for (int i = 0; i < 3; i++) {
                if (!isalpha(plate[i])) { valid = false; break; }
            }
            for (int i = 3; i < 7; i++) {
                if (!isdigit(plate[i])) { valid = false; break; }
            }
            if (valid) {
                for (int i = 0; i < 3; i++) plate[i] = toupper(plate[i]);
                return plate;
            }
        }
        cout << "Invalid plate number. Please enter again.\n";
    }
}

string validateVehicleType() {
    string type;
    while (true) {
        cout << "Enter Vehicle Type (Car/Motorcycle): ";
        cin >> type;
        string lower = type;
        for (int i = 0; i < lower.length(); i++) lower[i] = tolower(lower[i]);
        if (lower == "car") return "Car";
        if (lower == "motorcycle") return "Motorcycle";
        cout << "Invalid vehicle type. Please enter again.\n";
    }
}

string validateSecurityPhrase() {
    string ans;
    cin.ignore();
    while (true) {
        cout << "Please enter a security phrase. This will be used to reset your password.\n";
        getline(cin, ans);

        // basic validation: at least 2 chars, letters/spaces only
        bool valid = ans.length() >= 2;
        for (int i = 0; i < (int)ans.length(); i++) {
            if (!isalpha(ans[i]) && ans[i] != ' ') {
                valid = false;
                break;
            }
        }

        if (valid) {
            // normalize to lowercase for easier matching
            for (int i = 0; i < (int)ans.length(); i++) {
                ans[i] = tolower(ans[i]);
            }
            return ans;
        }

        cout << "Invalid answer. Use letters/spaces only.\n";
    }
}

void resetPassword(const string &id) {
    if (id.empty()) {
        cout << "Invalid ID.\n";
        return;
    }

    string ans;
    cin.ignore();
    cout << "Please enter security phrase to reset password. ";
    getline(cin, ans);
    for (int i = 0; i < (int)ans.length(); i++) ans[i] = tolower(ans[i]);

    // Student reset
    if (id[0] == '2') {
        int sIdx = findStudent(id);
        if (sIdx == -1) {
            cout << "Student ID not found.\n";
            return;
        }

        if (students[sIdx].securityPhrase.empty()) {
            cout << "No security phrase set for this student account.\n";
            return;
        }

        if (ans != students[sIdx].securityPhrase) {
            cout << "Security phrase incorrect. Reset cancelled.\n";
            return;
        }

        cout << "Identity verified.\n";
        students[sIdx].password = validatePassword();
        saveStudents();
        cout << "Password reset successful. Please login again.\n";
        return;
    }

    // Admin reset
    if (id[0] == '1') {
        int aIdx = -1;
        for (int i = 0; i < adminCount; i++) {
            if (admins[i].adminID == id) {
                aIdx = i;
                break;
            }
        }

        if (aIdx == -1) {
            cout << "Admin ID not found.\n";
            return;
        }

        if (admins[aIdx].securityPhrase.empty()) {
            cout << "No security phrase set for this admin account.\n";
            return;
        }

        if (ans != admins[aIdx].securityPhrase) {
            cout << "Security phrase incorrect. Reset cancelled.\n";
            return;
        }

        cout << "Identity verified.\n";
        admins[aIdx].password = validatePassword();
        saveAdmins(); // make sure this exists
        cout << "Password reset successful. Please login again.\n";
        return;
    }

    cout << "Unknown ID type. Reset cancelled.\n";
}

int login(int& index) {
    string id, password;

    cout << "\n===== LOGIN =====\n";
    cout << "Enter ID       : "; 
    cin >> id;

    cout << "Enter Password (enter -1 to reset): "; 
    cin >> password;

    // Trigger reset flow
    if (password == "-1") {
        resetPassword(id);
        return 0;
    }

    // Student login
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentID == id && students[i].password == password) {
            index = i;
            cout << "\nLogin successful! Welcome, " << students[i].name << ".\n";
            return 2;
        }
    }

    // Admin login
    for (int i = 0; i < adminCount; i++) {
        if (admins[i].adminID == id && admins[i].password == password) {
            index = i;
            cout << "\nLogin successful! Welcome, " << admins[i].name << ".\n";
            return 1;
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
    int choice = 0;
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

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                s.contactNumber = validateContactNumber();
                cout << "Contact number updated successfully.\n";
                break;
            case 2:
                s.faculty = validateFaculty();
                cout << "Faculty updated successfully.\n";
                break;
            case 3:
                s.vehicleNumber = validateVehiclePlate();
                cout << "Vehicle plate number updated successfully.\n";
                break;
            case 4:
                s.vehicleType = validateVehicleType();
                cout << "Vehicle type updated successfully.\n";
                break;
            case 5: {
                string oldPassword;
                cout << "Enter current password: ";
                cin >> oldPassword;
                if (oldPassword != s.password) {
                    cout << "Incorrect current password. Update cancelled.\n";
                    break;
                }
                s.password = validatePassword();
                cout << "Password updated successfully.\n";
                break;
            }
            case 6:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid input. Please enter 1 to 6.\n";
        }

        if (choice >= 1 && choice <= 5) {
            saveStudents();
        }

    } while (choice != 6);
}

void viewUpdateAdminProf(Admin &a) {
    int choice = 0;
    do {
        cout << "\n=== ADMIN PROFILE ===\n";
        cout << "Admin ID       : " << a.adminID << endl;
        cout << "Full Name      : " << a.name << endl;
        cout << "Contact Number : " << a.contactNumber << endl;

        cout << "\n--- What would you like to update? ---\n";
        cout << "1. Contact Number\n";
        cout << "2. Password\n";
        cout << "3. Back to Admin Menu\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                a.contactNumber = validateContactNumber();
                cout << "Contact number updated successfully.\n";
                saveAdmins();
                break;
            case 2: {
                string oldPassword;
                cout << "Enter current password: ";
                cin >> oldPassword;

                if (oldPassword != a.password) {
                    cout << "Incorrect current password. Update cancelled.\n";
                    break;
                }

                a.password = validatePassword();
                cout << "Password updated successfully.\n";
                saveAdmins();
                break;
            }
            case 3:
                cout << "Returning to admin menu...\n";
                break;
            default:
                cout << "Invalid input. Please enter 1 to 3.\n";
        }

    } while (choice != 3);
}

void makePayment(Student &s) {
    int targetIndex = -1;

    // Find latest approved but unpaid pass for this student
    for (int i = passCount - 1; i >= 0; i--) {
        if (parkingPasses[i].studentID == s.studentID &&
            parkingPasses[i].status == "Active" &&
            parkingPasses[i].paymentStatus == "Unpaid") {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        cout << "\nNo approved unpaid pass found.\n";
        cout << "Either you have no approved application, or payment is already completed.\n";
        return;
    }

    ParkingPass &p = parkingPasses[targetIndex];

    cout << "\n===== PAYMENT =====\n";
    cout << "Pass ID      : " << p.passID << endl;
    cout << "Student ID   : " << p.studentID << endl;
    cout << "Pass Period  : " << (p.startDate.empty() ? "N/A" : p.startDate)
         << " to " << (p.endDate.empty() ? "N/A" : p.endDate) << endl;
    cout << fixed << setprecision(2);
    cout << "Amount Due   : RM " << p.amount << endl;
    cout << "Status       : " << p.paymentStatus << endl;

    int methodChoice = 0;
    cout << "\nSelect Payment Method (Simulation):\n";
    cout << "1. FPX Online Banking\n";
    cout << "2. Debit/Credit Card\n";
    cout << "3. E-Wallet\n";
    cout << "4. Cancel\n";
    cout << "Enter choice: ";

    if (!(cin >> methodChoice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Payment cancelled.\n";
        return;
    }

    if (methodChoice == 4) {
        cout << "Payment cancelled.\n";
        return;
    }

    if (methodChoice < 1 || methodChoice > 3) {
        cout << "Invalid payment method. Payment cancelled.\n";
        return;
    }

    char confirm;
    cout << "Confirm payment of RM " << p.amount << "? (Y/N): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        p.paymentStatus = "Paid";
        p.paymentDate = getCurrentDate();
        saveParkingPasses();

        cout << "\nPayment successful (SIMULATED).\n";
        cout << "Payment Date : " << p.paymentDate << endl;
        cout << "Thank you.\n";
    } else {
        cout << "Payment not completed.\n";
    }
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
	// Capacity check
    if (passCount >= NO_OF_STUDENTS) {
        cout << "System cannot accept more parking pass records.\n";
        return;
    }
    
    // Check if there are already active monthly passes
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

bool parseDate(const string &date, int &y, int &m, int &d) {
    // Expected format: YYYY-MM-DD
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    string ys = date.substr(0, 4);
    string ms = date.substr(5, 2);
    string ds = date.substr(8, 2);

    // Ensure all are digits
    for (int i = 0; i < (int)ys.length(); i++) {
        if (!isdigit(ys[i])) return false;
    }
    for (int i = 0; i < (int)ms.length(); i++) {
        if (!isdigit(ms[i])) return false;
    }
    for (int i = 0; i < (int)ds.length(); i++) {
        if (!isdigit(ds[i])) return false;
    }

    stringstream yss(ys), mss(ms), dss(ds);
    yss >> y;
    mss >> m;
    dss >> d;

    if (yss.fail() || mss.fail() || dss.fail()) return false;

    if (m < 1 || m > 12) return false;

    int dim = daysInMonth(y, m);
    if (d < 1 || d > dim) return false;

    return true;
}

bool isLeapYear(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

int daysInMonth(int y, int m) {
    int dm[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && isLeapYear(y)) return 29;
    return dm[m - 1];
}

string formatDate(int y, int m, int d) {
    stringstream ss;
    ss << y << "-"
       << (m < 10 ? "0" : "") << m << "-"
       << (d < 10 ? "0" : "") << d;
    return ss.str();
}

string addOneDay(const string &date) {
    int y, m, d;
    if (!parseDate(date, y, m, d)) return "";

    d++;
    int dim = daysInMonth(y, m);
    if (d > dim) {
        d = 1;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }
    return formatDate(y, m, d);
}

string addMonthsMinusOneDay(const string &startDate, int months) {
    int y, m, d;
    if (!parseDate(startDate, y, m, d) || months < 1) return "";

    // target month after adding `months`
    int ty = y;
    int tm = m + months;
    while (tm > 12) { tm -= 12; ty++; }

    // Same day-of-month if possible, otherwise clamp to month end
    int targetDay = d;
    int dimTarget = daysInMonth(ty, tm);
    if (targetDay > dimTarget) targetDay = dimTarget;

    string sameDayAfterMonths = formatDate(ty, tm, targetDay);

    // End date = day before that
    int ey, em, ed;
    parseDate(sameDayAfterMonths, ey, em, ed);
    ed--;
    if (ed < 1) {
        em--;
        if (em < 1) {
            em = 12;
            ey--;
        }
        ed = daysInMonth(ey, em);
    }

    return formatDate(ey, em, ed);
}

// Student renew parking pass
void renewParkingPass(Student &s) {
    // Block duplicate pending renewal/application
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].studentID == s.studentID &&
            parkingPasses[i].status == "Pending") {
            cout << "You already have a pending application/renewal.\n";
            return;
        }
    }

    // Find latest active pass by endDate 
    int activeIdx = -1;
    string latestEnd = "";
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].studentID == s.studentID &&
    	parkingPasses[i].status == "Active" &&
    	parkingPasses[i].paymentStatus == "Paid") {
            if (activeIdx == -1 || parkingPasses[i].endDate > latestEnd) {
                activeIdx = i;
                latestEnd = parkingPasses[i].endDate;
            }
        }
    }

    if (activeIdx == -1) {
        cout << "No eligible parking pass found to renew.\n";
        cout << "You must have an active and paid pass for renewal.\n";
        return;
    }

    // Validate active pass end date
    int y, m, d;
    if (!parseDate(parkingPasses[activeIdx].endDate, y, m, d)) {
        cout << "Current active pass has invalid end date. Please contact admin.\n";
        return;
    }

    int months = 0;
    cout << "Current active pass ends on: " << parkingPasses[activeIdx].endDate << endl;
    cout << "How many month(s) to renew? (1-3): ";
    if (!(cin >> months) || months < 1 || months > 3) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Renewal months must be between 1 and 3.\n";
        return;
    }

    // Create new renewal request
    ParkingPass renewal;
    stringstream ss;
    ss << "P" << (passCount + 1001);
    renewal.passID = ss.str();

    renewal.studentID = s.studentID;
    // Current active ends on some month's last day.
	// Renewal starts on the 1st of the next month.
	int ey, em, ed;
	if (!parseDate(parkingPasses[activeIdx].endDate, ey, em, ed)) {
	    cout << "Current active pass has invalid end date. Please contact admin.\n";
	    return;
	}
	
	// Renewal starts the day after current active pass ends
	string newStart = addOneDay(parkingPasses[activeIdx].endDate);
	string newEnd   = addMonthsMinusOneDay(newStart, months);
	
	if (newStart.empty() || newEnd.empty()) {
	    cout << "Date calculation error. Please contact admin.\n";
	    return;
	}
	
	renewal.startDate = newStart;
	renewal.endDate   = newEnd;
	renewal.amount = MONTHLY_RATE * months;
    renewal.status = "Pending";
    renewal.appliedDate = getCurrentDate();
    renewal.paymentStatus = "Unpaid";
    renewal.paymentDate = "";
    renewal.renewalCount = parkingPasses[activeIdx].renewalCount + 1;

    if (passCount >= NO_OF_STUDENTS) {
        cout << "System cannot accept more pass records.\n";
        return;
    }

    parkingPasses[passCount++] = renewal;
    saveParkingPasses();

    cout << "\nRenewal request submitted successfully.\n";
    cout << "Pass ID       : " << renewal.passID << endl;
    cout << "Renewal Period: " << renewal.startDate << " to " << renewal.endDate << endl;
    cout << "Amount        : RM " << renewal.amount << endl;
    cout << "Status        : Pending (waiting admin approval)\n";
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

            cout << "1. Approve\n";
            cout << "2. Reject\n";
            cout << "Choice: ";
            int choice = 0;

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Pass not processed.\n";
                return;
            }

            if (choice == 1) {
			    string today = getCurrentDate();
			
			    if (parkingPasses[i].startDate == "" || parkingPasses[i].endDate == "") {
				    string newStart = addOneDay(today); // day after approval
				    string newEnd   = addMonthsMinusOneDay(newStart, 1); // 1-month validity
				
				    if (newStart.empty() || newEnd.empty()) {
				        cout << "Date calculation error. Pass not processed.\n";
				        return;
				    }
				
				    parkingPasses[i].startDate = newStart;
				    parkingPasses[i].endDate   = newEnd;
				}
			
			    parkingPasses[i].status = "Active";
			    cout << "Pass approved and activated (calendar-month).\n";
			    cout << "Start Date: " << parkingPasses[i].startDate << endl;
			    cout << "End Date  : " << parkingPasses[i].endDate << endl;
			}
            else if (choice == 2) {
                parkingPasses[i].status = "Rejected";
                cout << "Pass rejected.\n";
            }
            else {
                cout << "Invalid input. Pass not processed.\n";
                return;
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
        cout << "Error: Could not open students.txt for writing\n";
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
             << (s.isActive ? "1" : "0") << "|"
             << s.securityPhrase << "\n";
    }
    file.close();
}

void saveAdmins() {
    ofstream file("admins.txt");
    if (!file) {
        cout << "Error: Could not open admins.txt for writing\n";
        return;
    }
    for (int i = 0; i < adminCount; i++) {
        Admin &a = admins[i];
        file << a.adminID << "|"
             << a.name << "|"
             << a.password << "|"
             << a.contactNumber << "|"
             << a.securityPhrase << "\n";
    }
    file.close();
}

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

void studentMenu(Student &s) {
    checkExpirationAlert(s);
    
    int choice;
    do {
        cout << "\n=== STUDENT MENU ===\n";
        cout << "1. Apply New Parking Pass\n";
		cout << "2. Renew Parking Pass\n";
		cout << "3. Make Payment\n";
		cout << "4. View Current Application Status\n";
		cout << "5. View My Pass History\n";
		cout << "6. View / Update My Profile\n";
		cout << "7. Logout\n";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
        }

        switch(choice) {
		    case 1: applyParkingPass(s); break;
		    case 2: renewParkingPass(s); break;
		    case 3: makePayment(s); break;
		    case 4: viewCurrentAppStatus(s); break;
		    case 5: viewMyPassHistory(s); break;
		    case 6: viewUpdateProfile(s); break;
		    case 7: cout << "Logging out...\n"; break;
		    default: cout << "Invalid input. Please enter 1 to 7.\n";
		}
    } while(choice != 7);
}

void adminMenu(Admin &a) {
    int choice;
    do {
        cout << "\n=== ADMIN MENU ===\n";
        cout << "1. View Pending Applications\n";
        cout << "2. Approve / Reject Pass\n";
        cout << "3. Generate Analytics\n";
        cout << "4. View / Update My Profile\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
        }

        switch(choice) {
            case 1: viewPendingApplications(); break;
            case 2: approveRejectPass(); break;
            case 3: generateAnalytics(); break;
            case 4: viewUpdateAdminProf(a); break;
    		case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid input. Please enter 1 to 5.\n";
        }
    } while(choice != 5);
}

void viewCurrentAppStatus(Student &s) {
    int latestIdx = -1;

    // Find latest record for this student (most recent by appliedDate)
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].studentID == s.studentID) {
            if (latestIdx == -1 || parkingPasses[i].appliedDate > parkingPasses[latestIdx].appliedDate) {
                latestIdx = i;
            }
        }
    }

    if (latestIdx == -1) {
        cout << "\n=== CURRENT APPLICATION STATUS ===\n";
        cout << "No parking pass application/renewal found.\n";
        cout << "Please apply for a new parking pass first.\n";
        return;
    }

    ParkingPass &p = parkingPasses[latestIdx];

    cout << "\n=== CURRENT APPLICATION STATUS ===\n";
    cout << "Pass ID        : " << p.passID << endl;
    cout << "Applied Date   : " << p.appliedDate << endl;
    cout << "Status         : " << p.status << endl;
    cout << "Pass Period    : "
         << (p.startDate.empty() ? "N/A" : p.startDate)
         << " to "
         << (p.endDate.empty() ? "N/A" : p.endDate) << endl;
    cout << fixed << setprecision(2);
    cout << "Amount         : RM " << p.amount << endl;
    cout << "Payment Status : " << p.paymentStatus << endl;
    cout << "Payment Date   : " << (p.paymentDate.empty() ? "N/A" : p.paymentDate) << endl;

    cout << "\nNext Action: ";
    if (p.status == "Pending") {
    	char cancelChoice;
        cout << "Please wait for admin approval.\n";
        cout << "You may cancel a pending application at any time. To cancel, input 'Y', otherwise press any key to return.\n";
        cin >> cancelChoice;
        cin.ignore(1000, '\n'); // discard everything else until newline
        if (cancelChoice == 'Y' || cancelChoice == 'y') {
            p.status = "Cancelled";
            saveParkingPasses();
            cout << "Your pending application has been cancelled.\n";
        }
    } else if (p.status == "Rejected") {
        cout << "Your application was rejected. Please submit a new application.\n";
    } else if (p.status == "Active" && p.paymentStatus == "Unpaid") {
        cout << "Your pass is approved. Please proceed to 'Make Payment'.\n";
    } else if (p.status == "Active" && p.paymentStatus == "Paid") {
        cout << "No action needed. Your pass is active and paid.\n";
    } else {
        cout << "Please apply for a new pass.\n";
    }
}

void checkExpirationAlert(Student &s) {
    // Find latest active pass (by endDate)
    int activeIdx = -1;
    string latestEnd = "";

    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].studentID == s.studentID &&
            parkingPasses[i].status == "Active") {
            if (activeIdx == -1 || parkingPasses[i].endDate > latestEnd) {
                activeIdx = i;
                latestEnd = parkingPasses[i].endDate;
            }
        }
    }

    if (activeIdx == -1) return;

    ParkingPass &p = parkingPasses[activeIdx];

    // Only alert near month end
    time_t nowT = time(0);
    tm *now = localtime(&nowT);
    int dayOfMonth = now->tm_mday;

    if (dayOfMonth < 25) return;

    cout << "\n================== SYSTEM ALERT ==================\n";
    cout << " [REMINDER] Approaching month end.\n";
    cout << " Your active parking pass will end on: " << p.endDate << "\n";

    bool hasPending = false;
    for (int i = 0; i < passCount; i++) {
        if (parkingPasses[i].studentID == s.studentID &&
            parkingPasses[i].status == "Pending") {
            hasPending = true;
            break;
        }
    }

    if (hasPending) {
        cout << " You already have a pending renewal/application.\n";
        cout << " Please wait for admin approval.\n";
    } else {
        cout << " Please renew for next month if you need continued access.\n";
    }

    cout << "==================================================\n\n";
}
