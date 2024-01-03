#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class UserDataManagement
{
    // requirements for the class
    string username;
    string password;
    string email;
    string question;

    fstream file;

public:
    void login()
    {
        string searchName;
        string searchPass;
        bool accountFound = false;

        system("CLS");
        // enter data
        cout << "Enter your Username: ";
        getline(cin, searchName);
        cout << "Enter your Password: ";
        getline(cin, searchPass);

        file.open("loginData.txt", ios::in);

        if (file)
        {
            while (getline(file, username, '|') &&
                getline(file, password, '|') &&
                getline(file, email, '|') &&
                getline(file, question, '\n'))
            {

                if (searchName == username && searchPass == password)
                {
                    system("CLS");
                    cout << "Login Success, Welcome!\n\n";
                    cout << "Username: " << username << endl;;
                    cout << "Email: " << email << endl;
                    accountFound = true;                   
                }
            }
            if (!accountFound)
            {
                system("CLS");
                cout << "incorrect password, try again\n";
            }
        }
        else
        {
            cout << "unable to open file\n";
        }
        file.close();
    }
   
    void createAccount()
    {        
        file.open("loginData.txt", ios::out | ios::app);

        system("CLS");

        cout << "Enter your Username: ";
        getline(cin, username);
        cout << "Enter your Password: ";
        getline(cin, password);
        cout << "Enter your email: ";
        getline(cin, email);
        cout << "(Security Question) What was your pets name?: ";
        getline(cin, question);

        if (file)
        {
            file << username << "|" << password << "|" << email << "|" << question << endl;
        }
        file.close();
    }

    void forgotPassword()
    {
        string searchName;
        string searchEmail;
        string searchQuestion;
        string newPassword;
        ofstream replacementFile;

        system("CLS");
        cout << "Enter your username: ";
        getline(cin, searchName);
        cout << "Enter your email: ";
        getline(cin, searchEmail);
        cout << "(Security Question) What was your pets name?: ";
        getline(cin, searchQuestion);

        replacementFile.open("tempFile.txt");
        file.open("loginData.txt", ios::in);

        if (file)
        {
            while (getline(file, username, '|') &&
                getline(file, password, '|') &&
                getline(file, email, '|') &&
                getline(file, question, '\n'))
            {
                if (searchName == username && searchEmail == email && searchQuestion == question)
                {
                    system("CLS");
                    cout << "Enter a new password: ";
                    getline(cin, newPassword);

                    password = newPassword;
                    replacementFile << username << "|" << password << "|" << email << "|" << question << "\n";

                    cout << "your new password is: " << password << endl;
                }
                else
                {                    
                    //adds remaining lines to replacement
                    replacementFile << username << "|" << password << "|" << email << "|" << question << "\n";
                }
            }
        }
        else
        {
            cout << "unable to open file\n";
        }

        file.close();
        replacementFile.close();

        remove("loginData.txt");
        rename("tempFile.txt", "loginData.txt");
    }
};

#pragma region Protoypes

char showMenu();

#pragma endregion Protoypes

int main()
{
    UserDataManagement userManagerObj;
    bool validated = false;    

    while (!validated)
    {
        switch (showMenu())
        {
        case '1':
            userManagerObj.login();
            validated = true;
            break;

        case '2':
            userManagerObj.createAccount();
            validated = true;
            break;

        case '3':
            userManagerObj.forgotPassword();
            validated = true;
            break;

        case '4':
            cout << "exiting program\n";
            validated = true;
            break;

        default:
            system("CLS");
            cout << "Invalid Selection, try again\n\n";            
            break;
        }
    }    
}

char showMenu()
{    
    char selection;

    cout << "1 - Login\n";
    cout << "2 - Create Account\n";
    cout << "3 - Forgot Username or Password\n"; 
    cout << "4 - Exit\n";

    cout << "Selection: ";
    cin >> selection;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return selection;
}
