#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;


struct studentInfo {
    int studentID;
    string answers;
    int score;
    float percent;
    char grade;
};


bool isDigits(const string& str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}


char calculateGrade(float percent) {
    if (percent >= 90.0f) return 'A';
    else if (percent >= 80.0f) return 'B';
    else if (percent >= 70.0f) return 'C';
    else if (percent >= 60.0f) return 'D';
    else return 'F';
}


void calculateScore(studentInfo* student, const string& correctAnswers) {
    int score = 0;
    for (size_t i = 0; i < correctAnswers.size() && i < student->answers.size(); ++i) {
        char studentAnswer = student->answers[i];
        char correctAnswer = correctAnswers[i];

        if (studentAnswer == ' ') {
            
            continue;
        }
        else if (studentAnswer == correctAnswer) {
            score += 2;
        }
        else {
            score += 1;
        }
    }

    student->score = score;
    student->percent = static_cast<float>(score) / 40.0f * 100.0f;
    student->grade = calculateGrade(student->percent);
}

int main() {
    ifstream inputFile("student.txt");
    ofstream errorFile("error.txt");
    ofstream reportFile("report.txt");

    if (!inputFile || !errorFile || !reportFile) {
        cerr << "Error opening one or more files." << endl;
        return 1;
    }

    string correctAnswers;
    getline(inputFile, correctAnswers);  

    vector<studentInfo*> students;
    string line;

    while (getline(inputFile, line)) {
        istringstream ss(line);
        string idStr;
        ss >> idStr;

        
        if (!isDigits(idStr)) {
            errorFile << "Error found in this line: " << line << endl;
            continue;
        }

        
        string word, studentAnswers;
        while (ss >> word) {
            studentAnswers += word;
        }

        
        while (studentAnswers.length() < correctAnswers.length()) {
            studentAnswers += ' ';  
        }

        studentInfo* student = new studentInfo;
        student->studentID = stoi(idStr);
        student->answers = studentAnswers;

        calculateScore(student, correctAnswers);
        students.push_back(student);
    }

    \
    reportFile << left
        << setw(10) << "STUD ID"
        << setw(25) << "STUDENT ANSWERS"
        << setw(10) << "SCORE/40"
        << setw(10) << "PERCENT"
        << "GRADE" << endl;

    for (studentInfo* student : students) {
        reportFile << left
            << setw(10) << student->studentID
            << setw(25) << student->answers
            << setw(10) << student->score
            << setw(10) << fixed << setprecision(2) << student->percent
            << student->grade << endl;
    }

    
    for (studentInfo* s : students) {
        delete s;
    }

    inputFile.close();
    errorFile.close();
    reportFile.close();

    cout << "Grading complete. Check 'report.txt' and 'error.txt' for results." << endl;
    return 0;
}

