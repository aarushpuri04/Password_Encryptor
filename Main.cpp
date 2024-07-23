#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Path to the data file

const string FILE_NAME = "D:/C++ Projects/Student Manager/Data.txt";

// Structure for subject and grades
struct SubjectGrade
{
    string subject;
    string grade;
};

// Class of student information
class Student
{
private:
    string name, rollNo, year;
    vector<SubjectGrade> subjectsGrades;

public:
    // Setters functions using const references to avoid copying strings
    void setName(const string &n) { name = n; }
    void setRollNo(const string &r) { rollNo = r; }
    void setYear(const string &y) { year = y; }

    void addSubjectGrade(const string &subject, const string &grade)
    {
        subjectsGrades.push_back({subject, grade});
    }

    // Getters fuctions for getting student information
    string getName() const { return name; }
    string getRollNo() const { return rollNo; }
    string getYear() const { return year; }
    const vector<SubjectGrade> &getSubjectsGrades() const { return subjectsGrades; }

    // Display functions for student details
    void display() const
    {
        cout << "Name: " << name << ", Year: " << year << ", Roll No: " << rollNo << endl;
        for (const auto &sg : subjectsGrades)
        {
            cout << sg.subject << ": " << sg.grade << endl;
        }
    }

    // bool function for checking subject and updating grade
    bool updateGrade(const string &subject, const string &newGrade)
    {
        for (auto &sg : subjectsGrades)
        {
            if (sg.subject == subject)
            {
                sg.grade = newGrade;
                return true;
            }
        }
        return false;
    }

    // Converting student data to a single string for file storage using stringstream
    string toString() const
    {
        stringstream ss;
        ss << name << "," << year << "," << rollNo;
        for (const auto &sg : subjectsGrades)
        {
            ss << "," << sg.subject << "," << sg.grade;
        }
        return ss.str();
    }

    // Creates a Student object from a string using class student
    static Student fromString(const string &str)
    {
        Student s;
        stringstream ss(str);
        string item;
        getline(ss, s.name, ',');
        getline(ss, s.year, ',');
        getline(ss, s.rollNo, ',');
        while (getline(ss, item, ','))
        {
            string subject = item;
            getline(ss, item, ',');
            s.addSubjectGrade(subject, item);
        }
        return s;
    }
};

vector<Student> students; // Vector to store all students

// Saves all students to a file
void saveStudentsToFile()
{
    ofstream outFile(FILE_NAME);
    if (outFile.is_open())
    {
        for (const auto &student : students)
        {
            outFile << student.toString() << endl;
        }
        outFile.close();
        cout << "Students data saved to file." << endl;
    }
    else
    {
        cout << "Unable to open file for writing." << endl;
    }
}

// Loads all students from a file
void loadStudentsFromFile()
{
    ifstream inFile(FILE_NAME);
    if (inFile.is_open())
    {
        students.clear();
        string line;
        while (getline(inFile, line))
        {
            students.push_back(Student::fromString(line));
        }
        inFile.close();
        cout << "Students data loaded from file." << endl;
    }
    else
    {
        cout << "No existing student data found." << endl;
    }
}

// Helper function to get input and add a student
void addStudent()
{
    Student s;
    string input;

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, input);
    s.setName(input);

    cout << "Enter Year: ";
    getline(cin, input);
    s.setYear(input);

    cout << "Enter Roll No: ";
    getline(cin, input);
    s.setRollNo(input);

    string more;
    do
    {
        string subject, grade;
        cout << "Enter Subject: ";
        getline(cin, subject);
        cout << "Enter Grade: ";
        getline(cin, grade);
        s.addSubjectGrade(subject, grade);

        cout << "Add another subject? (y/n): ";
        getline(cin, more);
    } while (more == "y" || more == "Y");

    students.push_back(s);
    saveStudentsToFile();
    cout << "Student added successfully!" << endl;
}

// function to Search for a student by roll number and displays their information
void searchStudent()
{
    string rollNo;
    cout << "Enter Roll No to search: ";
    cin >> rollNo; // Directly takes input into variable

    for (const auto &student : students)
    {
        if (student.getRollNo() == rollNo)
        {
            student.display();
            return;
        }
    }
    cout << "Student not found!" << endl;
}

// function to updats information for a student found by roll number
void updateStudent()
{
    string rollNo;
    cout << "Enter Roll No to update: ";
    cin >> rollNo;

    for (auto &student : students)
    {
        if (student.getRollNo() == rollNo)
        {
            student.display(); // Showing current info before updating
            cout << "1. Update existing subject" << endl;
            cout << "2. Add new subject" << endl;
            cout << "Enter your choice: ";
            int updateChoice;
            cin >> updateChoice;
            cin.ignore(); // Clearing the newline from the buffer

            if (updateChoice == 1)
            {
                string subject, newGrade;
                cout << "Enter subject to update: ";
                getline(cin, subject);
                cout << "Enter new grade: ";
                getline(cin, newGrade);
                if (student.updateGrade(subject, newGrade))
                {
                    cout << "Grade updated successfully!" << endl;
                }
                else
                {
                    cout << "Subject not found!" << endl;
                }
            }
            else if (updateChoice == 2)
            {
                string subject, grade;
                cout << "Enter new subject: ";
                getline(cin, subject);
                cout << "Enter grade for " << subject << ": ";
                getline(cin, grade);
                student.addSubjectGrade(subject, grade);
                cout << "New subject added successfully!" << endl;
            }
            else
            {
                cout << "Invalid choice!" << endl;
                return;
            }

            saveStudentsToFile(); // Saving changes to file
            return;
        }
    }
    cout << "Student not found!" << endl;
}

// function to Delete a student by roll number
void deleteStudent()
{
    string rollNo;
    cout << "Enter Roll No to delete: ";
    cin >> rollNo;

    auto it = find_if(students.begin(), students.end(), [&](const Student &s)
                      { return s.getRollNo() == rollNo; });

    if (it != students.end())
    {
        students.erase(it);
        saveStudentsToFile(); // Update file after deleting the student
        cout << "Student deleted successfully!" << endl;
    }
    else
    {
        cout << "Student not found!" << endl;
    }
}

int main()
{
    loadStudentsFromFile(); // loading student data from file before going into while

    while (true)
    {
        cout << "\nStudent Manager" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Search Student" << endl;
        cout << "3. Update Student" << endl;
        cout << "4. Delete Student" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            searchStudent();
            break;
        case 3:
            updateStudent();
            break;
        case 4:
            deleteStudent();
            break;
        case 5:
            cout << "Bye Bye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}