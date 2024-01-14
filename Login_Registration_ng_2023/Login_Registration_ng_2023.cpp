#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;

class UserLoginManagement
{
    // best to keep members within a class private if possible
private:
    string searchUserName;
    string searchPassWord;
    string searchEmail;
    string searchQuestion;

    //fstream can be used based on ios state for reading and writing
    fstream file;
public:
    void createAccount()
    {
        string accountName;
        string passWord;
        string email;
        string question;
        int id = 1 + lastID("userData.txt");

        cout << "Enter a username: ";
        getline(cin, accountName);

        if (usernameTaken(accountName))
        {
            cout << "that username isn't available." << endl;
        }
        else
        {
            cout << "Enter a password: ";
            getline(cin, passWord);

            cout << "Enter your email: ";
            getline(cin, email);

            cout << "(Security Question) What was your pets name?: ";
            getline(cin, question);

            // saves data to file as singular string to user (within saveFile, it ends the line).
            saveFile("|" + accountName + "|" + passWord + "|" + email + "|" + question + "|", id, "userData.txt");
        }
    }

    void accountLogin()
    {
        int accountID;
        int initalID;
        string currentData;

        cout << "Enter your username: ";
        getline(cin, searchUserName);

        accountID = checkFile(1, searchUserName, currentData);
        initalID = accountID;

        if (searchUserName == currentData && accountID == initalID)
        {
            cout << "Enter your password: ";
            getline(cin, searchPassWord);

            if (isValidInput(2, initalID, searchPassWord))
            {
                system("CLS");
                cout << "Welcome " << searchUserName << "!" << endl << endl;

                displayUserData(3, "Email: ");
                displayUserData(4, "Your favorite pet!: ");
            }
            else
            {
                cout << "Password incorrect" << endl;
            }
        }
        else
        {
            cout << "Username not found" << endl;
        }
    }

    void forgotPassword()
    {
        int accountID;
        int initalID;
        string currentData;
        string newPassword;

        cout << "Enter your username: ";
        getline(cin, searchUserName);

        accountID = checkFile(1, searchUserName, currentData);
        initalID = accountID;

        if (searchUserName == currentData && accountID == initalID)
        {
            cout << "Enter your email: ";
            getline(cin, searchEmail);

            if (isValidInput(3, initalID, searchEmail))
            {
                cout << "(Security Question) What was your pets name?: ";
                getline(cin, searchQuestion);

                if (isValidInput(4, initalID, searchQuestion))
                {
                    system("CLS");

                    cout << "Enter a new password: ";
                    getline(cin, newPassword);

                    replacePassword(newPassword);
                }
            }
            else
            {
                cout << "Email not found:" << endl;
            }
        }
        else
        {
            cout << "Username not found" << endl;
        }
    }

    void replacePassword(string newPass)
    {
        ifstream inFile("userData.txt");
        ofstream tempFile("tempFile.txt");
        string line;
        string currentChar;
        string oldPass;
        long long eChar = 0;
        int id = 0;

        checkFile(2, searchUserName, oldPass);

        while (inFile >> currentChar)
        {
            if (currentChar.find("ID:") != string::npos)
            {
                size_t startPos = line.find(oldPass);
                if (startPos != string::npos)
                {
                    line.replace(startPos, oldPass.length(), newPass);

                    id = 1 + lastID("tempFile.txt");
                    saveFile(line, id, "tempFile.txt");
                }
                else
                {
                    id = 1 + lastID("tempFile.txt");
                    saveFile(line, id, "tempFile.txt");
                }
                line.clear();
            }
            else
            {
                istringstream(currentChar) >> eChar;
                line += (char)decrypt(eChar);
                currentChar.clear();
            }
        }
        tempFile.close();
        inFile.close();

        remove("userData.txt");
        rename("tempFile.txt", "userData.txt");
    }

    bool isValidInput(const int dataID, int initialID, string attempt)
    {
        string currentData;
        int accountID;

        accountID = checkFile(dataID, attempt, currentData);

        if (attempt == currentData && accountID == initialID)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void displayUserData(const int dataID, string text)
    {
        string currentData;

        checkFile(dataID, searchUserName, currentData);

        cout << text << "" << currentData << endl;
    }

    bool usernameTaken(string accountName)
    {
        string currentData;

        int accountID = checkFile(1, accountName, currentData);       

        if (accountID != 0)
        {
            return true;
        }
        else if (accountID == 0)
        {
            return false;
        }
    }

    void saveFile(string line, const int accountID, string fileName)
    {
        file.open(fileName, ios::out | ios::app);
        file.seekg(0, ios::end);

        if (file.tellg() != 0)
        {
            file << endl;
        }

        file.seekg(0, ios::beg);

        if (file.is_open())
        {
            for (int i = 0; i < line.length(); i++)
            {
                file << encrypt(line[i]);
                file << endl;
            }
           
            file << "ID:" << accountID;
        }

        file.close();
    }

    int checkFile(const int dataID, string attempt, string& currentData)
    {
        string currentChar;
        string line;
        int currentDataID = 0;
        int accountID = 0;
        long long eChar;

        file.open("userData.txt", ios::in);

        if (dataID > 4 || dataID <= 0)
        {
            cout << "non existant ID" << endl;
            file.close();
            return 0;
        }

        while (file >> currentChar)
        {
            if (currentChar.find("ID:") != string::npos)
            {
                if (line.find(attempt) != string::npos)
                {
                    file.close();
                    currentChar.erase(0, 3);
                    istringstream(currentChar) >> accountID;

                    for (int i = 0; i < line.length(); i++)
                    {
                        if (line[i] == '|')
                        {
                            currentDataID++;
                        }
                        if (currentDataID >= dataID && currentDataID < dataID + 1)
                        {
                            currentData += line[i];
                        }
                    }
                    // erase the first delimiter                    
                    currentData.erase(0, 1);
                    return accountID;
                }
                else
                {
                    line.clear();
                }
            }
            else
            {
                istringstream(currentChar) >> eChar;
                line += (char)decrypt(eChar);
                currentChar.clear();
                accountID = 0;
            }
        }
        if (file.peek() == EOF)
        {
            file.close();
            return accountID;
        }
    }

    int lastID(string fileName)
    {
        string searchId;
        string userId;
        int id;

        file.open(fileName, ios::in);
        file.seekg(0, ios::end);

        if (file.tellg() == 0)
        {
            id = 0;
            file.close();
            return id;
        }

        file.seekg(0, ios::beg);

        while (getline(file, searchId))
        {
            size_t pos = searchId.find('I');

            if (pos != string::npos)
            {
                userId = searchId;
            }
        }

        file.close();
        userId.erase(0, 3);

        istringstream(userId) >> id;
        return id;
    }

    long long encrypt(int letter)
    {
        return powf(letter, 6) * 3 - 10;
    }

    int decrypt(long long letter)
    {
        return powf((letter + 10) / 3, 1 / 6.f);
    }
};

char showMenu()
{   
    char selection;

    cout << "Nathan Gessner 2024: Login and Regsistration Program" << endl << endl;

    cout << "1 - Login\n";
    cout << "2 - Create Account\n";
    cout << "3 - Forgot Username or Password\n";
    cout << "4 - Exit\n";

    cout << "Selection: ";
    cin >> selection;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return selection;
}

int main()
{
    ofstream outFile("userData.txt");
    UserLoginManagement loginManagerObj;

    bool validated = false;
    if (outFile.is_open())
    {
        outFile.close();

        while (!validated)
        {
            switch (showMenu())
            {
            case '1':
                system("CLS");
                loginManagerObj.accountLogin();
                validated = true;
                break;

            case '2':
                system("CLS");
                loginManagerObj.createAccount();
                validated = true;
                break;

            case '3':
                system("CLS");
                loginManagerObj.forgotPassword();
                validated = true;
                break;

            case '4':
                system("CLS");
                cout << "exiting program" << endl;
                validated = true;
                break;

            default:
                system("CLS");
                cout << "Invalid Selection, try again" << endl << endl;
                break;
            }
        }
    }  
    else
    {
        cout << "Error opening file" << endl;
    }

}