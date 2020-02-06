#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Addressee
{
    int addresseeId = 0;
    string name = "", surname = "", address = "", email = "", tel = "";
};

struct User
{
    int userId = 0;
    string userName = "", password = "";
};

bool checkIfTelephoneNumberIsCorrect(string tel);

bool checkIfEmailIsCorrect(string email);

bool checkIfNameOrSurnameIsCorrect(string nameOrSurname, string whatWillBeChecked);

bool checkIfTheDataIsRepeated(vector<Addressee> &addressees, string name, string surname, string tel, string email, string address);

void writeDataInFile(vector<Addressee> &addressees, int indexOfAddressee, int userId, string fileName);

int addContact(vector<Addressee> &addressees, int numberOfContacts, int addresseeId, int userId);

void loadDataFromFile(vector<Addressee> &addressees, int &numberOfContacts, int idOfLoggedUser);

void displayDataOfAllAddressees(vector<Addressee> &addressees);

bool checkIfThisNameExists(vector<Addressee> &addressees, string name);

bool checkIfThisSurnameExists(vector<Addressee> &addressees, string surname);

void searchByName(vector<Addressee> &addressees);

void searchBySurame(vector<Addressee> &addressees);

bool checkIfThisIdExists(vector<Addressee> &addresssees, int addresseeId);

void reloadDataInFile(vector<Addressee> &addressees, int &numberOfContacts, int userId, int &chosenAddresseePosition, int chosenId, string mode);

void displayAddresseeById(vector<Addressee> &addressee, int addresseeId, int &chosenAddresseePosition);

void deleteAddressee(vector<Addressee> &addressees, int &numberOfContacts, int userId);

void editAddressee(vector<Addressee> &addressees, int numberOfContacts, int userId);

int getId(string fileName);

int getNumberOfContactsOrUsers(string fileName);

void loadUsersFromFile(vector<User> &users, int numberOfUsers);

int registerNewUser(vector<User> &users, int numberOfUsers, int userId);

bool login(vector<User> &users, int numberOfUsers, int &idOfLoggedUser);

int main()
{
    vector<Addressee> addressees;
    vector<User> users;
    int addresseeId = getId("./data.txt"), numberOfContacts = getNumberOfContactsOrUsers("./data.txt");
    int numberOfUsers = getNumberOfContactsOrUsers("./users.txt"), userId = getId("./users.txt");
    char choice;
    bool isUserLoggedIn = false;
    int idOfLoggedUser = 0;

    loadUsersFromFile(users, numberOfUsers);

    while (1)
    {
        while (!isUserLoggedIn)
        {
            system("cls");
            cout << "PANEL UZYTKOWNIKA" << endl;
            cout << "1. Logowanie" << endl;
            cout << "2. Rejestracja" << endl;
            cout << "3. Zamknij program" << endl;
            cout << "Twoj wybor: ";

            cin >> choice;

            switch (choice)
            {
            case '1':
                isUserLoggedIn = login(users, numberOfUsers, idOfLoggedUser);
                break;
            case '2':
                userId++;
                numberOfUsers = registerNewUser(users, numberOfUsers, userId);
                break;
            case '3':
                exit(0);
            default:
                cout << "Nie ma takiej opcji w menu!" << endl;
                Sleep(1500);
                break;
            }
            loadDataFromFile(addressees, numberOfContacts, idOfLoggedUser);
        }

        system("cls");
        cout << "KSIAZKA ADRESOWA" << endl;
        cout << "1. Dodaj adresata" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkich adresatow" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "7. Wyloguj sie" << endl;
        cout << "Twoj wybor: ";

        cin >> choice;

        switch (choice)
        {
        case '1':
            addresseeId++;
            numberOfContacts = addContact(addressees, numberOfContacts, addresseeId, idOfLoggedUser);
            break;
        case '2':
            searchByName(addressees);
            break;
        case '3':
            searchBySurame(addressees);
            break;
        case '4':
            displayDataOfAllAddressees(addressees);
            break;
        case '5':
            deleteAddressee(addressees, numberOfContacts, idOfLoggedUser);
            addresseeId = getId("./data.txt");
            break;
        case '6':
            editAddressee(addressees, numberOfContacts, idOfLoggedUser);
            break;
        case '7':
            isUserLoggedIn = false;
            addressees.clear();
            break;
        default:
            cout << "Nie ma takiej opcji w menu!" << endl;
            Sleep(1500);
            break;
        }
    }

    return 0;
}

bool checkIfTelephoneNumberIsCorrect(string tel)
{
    int length = tel.length();

    for (int i = 0; i < length; i++)
    {
        if (!(tel[i] >= '0' && tel[i] <= '9' && length == 9))
        {
            cout << "Nieprawidlowy format!" << endl;
            cout << "Wprowadz numer 9-cyfrowy bez liter i znakow specjalnych" << endl;
            Sleep(1500);
            return false;
        }
    }

    return true;
}

bool checkIfEmailIsCorrect(string email)
{
    int length = email.length();

    for (int i = 0; i < length; i++)
    {
        if (email[i] == '@')
            return true;
    }

    cout << "Nieprawidlowy format!" << endl;
    cout << "Podaj e-mail ze znakiem '@' w addressie" << endl;
    Sleep(1500);

    return false;
}

bool checkIfNameOrSurnameIsCorrect(string nameOrSurname, string whatWillBeChecked)
{
    int length = nameOrSurname.length();
    bool result = true;
    string whatHasBeenChecked;

    if (whatWillBeChecked == "name")
    {
        whatHasBeenChecked = "Imie";
    }
    else
    {
        whatHasBeenChecked = "Nazwisko";
    }

    if (!(nameOrSurname[0] >= 'A' && nameOrSurname[0] <= 'Z'))
        result = false;

    for (int i = 1; i < length; i++)
    {
        if (!(nameOrSurname[i] >= 'A' && nameOrSurname[i] <= 'Z' || nameOrSurname[i] >= 'a' && nameOrSurname[i] <= 'z'))
            result = false;
    }

    if (!result)
    {
        cout << "Nieprawidlowy format!" << endl;
        cout << whatHasBeenChecked << " musi zaczynac sie od wielkiej litery i nie moze zawierac znakow specjalnych" << endl;
        Sleep(1500);
    }

    return result;
}

bool checkIfTheDataIsRepeated(vector<Addressee> &addressees, string name, string surname, string tel, string email, string address)
{
    for (int i = 0; i < addressees.size(); i++)
    {
        if (name == addressees[i].name && surname == addressees[i].surname && tel == addressees[i].tel && email == addressees[i].email && address == addressees[i].address)
        {
            cout << "Wprowadzone dane juz znajduja sie w ksiazce adresowej" << endl;
            Sleep(1500);
            return true;
        }
    }
    return false;
}

void writeDataInFile(vector<Addressee> &addressees, int indexOfAddressee, int userId, string fileName)
{
    fstream file;
    string line;

    file.open(fileName, ios::in);
    // sprawdzam czy ostatnia linijka jest pusta
    while (!file.eof())
    {
        getline(file, line);
    }

    file.close();

    file.open(fileName, ios::out | ios::app);

    if (!line.empty())
    {
        file << endl;
    }

    file << addressees[indexOfAddressee].addresseeId << "|";
    file << userId << "|";
    file << addressees[indexOfAddressee].name << "|";
    file << addressees[indexOfAddressee].surname << "|";
    file << addressees[indexOfAddressee].tel << "|";
    file << addressees[indexOfAddressee].email << "|";
    file << addressees[indexOfAddressee].address << "|" << endl;

    file.close();
}

int addContact(vector<Addressee> &addressees, int numberOfContacts, int addresseeId, int userId)
{
    string name, surname, address, email, tel;
    Addressee newAddresse;

    while (1)
    {
        do
        {
            cout << "Podaj imie: ";
            cin.sync();
            getline(cin, name);
        } while (!checkIfNameOrSurnameIsCorrect(name, "name"));
        do
        {
            cout << "Podaj nazwisko: ";
            cin.sync();
            getline(cin, surname);
        } while (!checkIfNameOrSurnameIsCorrect(surname, "surname"));
        do
        {
            cout << "Podaj numer telefonu: ";
            cin >> tel;
        } while (!checkIfTelephoneNumberIsCorrect(tel));
        do
        {
            cout << "Podaj adres e-mail: ";
            cin >> email;
        } while (!checkIfEmailIsCorrect(email));
        cin.sync();
        cout << "Podaj adres: ";
        getline(cin, address);

        if (!checkIfTheDataIsRepeated(addressees, name, surname, tel, email, address))
            break;
    }

    newAddresse.addresseeId = addresseeId;
    newAddresse.name = name;
    newAddresse.surname = surname;
    newAddresse.tel = tel;
    newAddresse.email = email;
    newAddresse.address = address;
    addressees.push_back(newAddresse);

    writeDataInFile(addressees, addressees.size() - 1, userId, "./data.txt");

    cout << "Dodano do kontaktow" << endl;
    Sleep(1500);

    return ++numberOfContacts;
}

void loadDataFromFile(vector<Addressee> &addressees, int &numberOfContacts, int idOfLoggedUser)
{
    fstream file;
    string line, idFromFile;
    Addressee loadedAddressee;
    string partOfLine;
    vector<string> dataOfAddressee;

    file.open("./data.txt", ios::in);

    for (int i = 0; i < numberOfContacts; i++)
    {
        getline(file, line);
        int lenghtOfLine = line.length();
        for (int j = 0; j < lenghtOfLine; j++)
        {
            if (line[j] != '|')
            {
                partOfLine += line[j];
            }
            else
            {
                dataOfAddressee.push_back(partOfLine);
                partOfLine.clear();
            }
        }

        if (atoi(dataOfAddressee[1].c_str()) == idOfLoggedUser)
        {
            idFromFile = dataOfAddressee[0];
            loadedAddressee.addresseeId = atoi(idFromFile.c_str());
            loadedAddressee.name = dataOfAddressee[2];
            loadedAddressee.surname = dataOfAddressee[3];
            loadedAddressee.tel = dataOfAddressee[4];
            loadedAddressee.email = dataOfAddressee[5];
            loadedAddressee.address = dataOfAddressee[6];
            addressees.push_back(loadedAddressee);
        }
        dataOfAddressee.clear();
    }
    file.close();
}

void displayDataOfSingleAddressee(vector<Addressee> &addressees, int indexOfAddressee)
{
    cout << "Id: " << addressees[indexOfAddressee].addresseeId << "|";
    cout << "Imie: " << addressees[indexOfAddressee].name << "|";
    cout << "Nazwisko: " << addressees[indexOfAddressee].surname << "|";
    cout << "Numer telefonu: " << addressees[indexOfAddressee].tel << "|";
    cout << "E-mail: " << addressees[indexOfAddressee].email << "|";
    cout << "Adres: " << addressees[indexOfAddressee].address << "|" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
}

void displayDataOfAllAddressees(vector<Addressee> &addressees)
{
    if (addressees.size() == 0)
    {
        cout << "Ksiazka adresowa jest pusta" << endl;
        Sleep(1500);
    }
    else
    {
        cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
        for (int i = 0; i < addressees.size(); i++)
        {
            displayDataOfSingleAddressee(addressees, i);
        }
        system("pause");
    }
}

bool checkIfThisNameExists(vector<Addressee> &addressees, string name)
{
    for (int i = 0; i < addressees.size(); i++)
    {
        if (name == addressees[i].name)
            return true;
    }

    return false;
}

bool checkIfThisSurnameExists(vector<Addressee> &addressees, string surname)
{
    for (int i = 0; i < addressees.size(); i++)
    {
        if (surname == addressees[i].surname)
            return true;
    }

    return false;
}

void searchByName(vector<Addressee> &addressees)
{
    string name;

    if (addressees.size() == 0)
    {
        cout << "Ksiazka adresowa jest pusta" << endl;
        Sleep(1500);
    }
    else
    {
        do
        {
            cout << "Podaj imie: ";
            cin.sync();
            getline(cin, name);
        } while (!checkIfNameOrSurnameIsCorrect(name, "name"));

        if (!checkIfThisNameExists(addressees, name))
        {
            cout << "Nie ma osob o takim imieniu w ksiazce adresowej" << endl;
            Sleep(1500);
        }
        else
        {
            cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
            for (int i = 0; i < addressees.size(); i++)
            {
                if (name == addressees[i].name)
                {
                    displayDataOfSingleAddressee(addressees, i);
                }
            }
            system("pause");
        }
    }
}

void searchBySurame(vector<Addressee> &addressees)
{
    string surname;

    if (addressees.size() == 0)
    {
        cout << "Ksiazka adresowa jest pusta" << endl;
        Sleep(1500);
    }
    else
    {
        do
        {
            cout << "Podaj nazwisko: ";
            cin.sync();
            getline(cin, surname);
        } while (!checkIfNameOrSurnameIsCorrect(surname, "surname"));

        if (!checkIfThisSurnameExists(addressees, surname))
        {
            cout << "Nie ma osob o takim nazwisku w ksiazce adresowej" << endl;
            Sleep(1500);
        }
        else
        {
            cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
            for (int i = 0; i < addressees.size(); i++)
            {
                if (surname == addressees[i].surname)
                {
                    displayDataOfSingleAddressee(addressees, i);
                }
            }
            system("pause");
        }
    }
}

bool checkIfThisIdExists(vector<Addressee> &addresssees, int addresseeId)
{
    for (int i = 0; i < addresssees.size(); i++)
    {
        if (addresssees[i].addresseeId == addresseeId)
            return true;
    }
    return false;
}

void reloadDataInFile(vector<Addressee> &addressees, int &numberOfContacts, int userId, int &chosenAddresseePosition, int chosenId, string mode)
{
    fstream file, temporaryFile;
    string line, partOfLine;
    vector<string> dataOfAddressee;
    bool doesThisIdExist = checkIfThisIdExists(addressees, chosenId);
    int end = 0;

    file.open("./data.txt", ios::in);

    temporaryFile.open("./data_temp.txt", ios::out | ios::app);

    if (mode == "edit")
    {
        end = numberOfContacts;
    }
    else
    {
        end = numberOfContacts + 1;
    }

    for (int i = 0; i < end; i++)
    {
        getline(file, line);
        if (!line.empty())
        {
            for (int j = 0; j < line.length(); j++)
            {
                if (line[j] != '|')
                {
                    partOfLine += line[j];
                }
                else
                {
                    dataOfAddressee.push_back(partOfLine);
                    partOfLine.clear();
                }
            }
        }

        if (doesThisIdExist)
        {
            if (chosenId == atoi(dataOfAddressee[0].c_str()))
            {
                writeDataInFile(addressees, chosenAddresseePosition, userId, "./data_temp.txt");
            }
            else
            {
                temporaryFile << line << endl;
            }
        }
        else
        {
            if (chosenId != atoi(dataOfAddressee[0].c_str()))
            {
                temporaryFile << line << endl;
            }
        }

        dataOfAddressee.clear();
    }

    file.close();
    temporaryFile.close();

    remove("./data.txt");
    rename("./data_temp.txt", "./data.txt");

    addressees.clear();

    loadDataFromFile(addressees, numberOfContacts, userId);
}

void displayAddresseeById(vector<Addressee> &addressees, int addresseeId, int &chosenAddresseePosition)
{
    for (int i = 0; i < addressees.size(); i++)
    {
        if (addresseeId == addressees[i].addresseeId)
        {
            chosenAddresseePosition = i;
            cout << "Oto dane adresata: " << endl;
            cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
            displayDataOfSingleAddressee(addressees, i);
        }
    }
}

void deleteAddressee(vector<Addressee> &addressees, int &numberOfContacts, int userId)
{
    string name, surname;
    int addresseeId;
    char choice;
    vector<Addressee>::iterator it = addressees.begin();
    fstream file;
    int chosenAddresseePosition;
    int i = 0;

    cout << "Podaj id adresata, ktorego chcesz usunac z listy: ";
    cin >> addresseeId;

    if (!checkIfThisIdExists(addressees, addresseeId))
    {
        cout << "Nie ma adresata o takim id" << endl;
        Sleep(1500);
        return;
    }

    cout << endl;

    displayAddresseeById(addressees, addresseeId, chosenAddresseePosition);
    cout << endl;

    while (1)
    {
        cout << "Czy na pewno chcesz usunac wybrany kontakt? (t/n)" << endl;
        cout << "Twoj wybor: ";

        cin >> choice;

        if (choice == 't')
        {
            while (i < chosenAddresseePosition)
            {
                it++;
                i++;
            }

            addressees.erase(it);
            numberOfContacts--;

            reloadDataInFile(addressees, numberOfContacts, userId, chosenAddresseePosition, addresseeId, "delete");

            cout << "Kontakt zostal usuniety" << endl;
            Sleep(1500);
            break;
        }
        else if (choice == 'n')
        {
            cout << "Kontakt nie zostal usuniety" << endl;
            Sleep(1500);
            break;
        }
        else
        {
            cout << "Nie ma takiej opcji!" << endl;
        }
    }
}

void editAddressee(vector<Addressee> &addressees, int numberOfContacts, int userId)
{
    char choice;
    string newName, newSurname, newEmail, newAddress, newTel;
    int addresseeId;
    int chosenAddresseePosition;

    cout << endl;
    cout << "Podaj id adresata, ktorego chcesz edytowac: ";
    cin >> addresseeId;

    if (!checkIfThisIdExists(addressees, addresseeId))
    {
        cout << "Nie ma adresata o takim id" << endl;
        Sleep(1500);
        return;
    }

    displayAddresseeById(addressees, addresseeId, chosenAddresseePosition);

    cout << endl;
    cout << "MENU EDYCJI" << endl;
    cout << "1. Imie" << endl;
    cout << "2. Nazwisko" << endl;
    cout << "3. Numer telefonu" << endl;
    cout << "4. E-mail" << endl;
    cout << "5. Adres" << endl;
    cout << "6. Powrot do menu" << endl;
    cout << "Twoj wybor: ";

    cin >> choice;

    switch (choice)
    {
    case '1':
        cout << "Wpisz nowe imie: ";
        cin.sync();
        getline(cin, newName);
        addressees[chosenAddresseePosition].name = newName;
        cout << "Imie zostalo zmienione";
        Sleep(1500);
        break;
    case '2':
        cout << "Wpisz nowe nazwisko: ";
        cin.sync();
        getline(cin, newSurname);
        addressees[chosenAddresseePosition].surname = newSurname;
        cout << "Nazwisko zostalo zmienione";
        Sleep(1500);
        break;
    case '3':
        cout << "Wpisz nowy numer telefonu: ";
        cin >> newTel;
        addressees[chosenAddresseePosition].tel = newTel;
        cout << "Numer telefonu zostal zmieniony";
        Sleep(1500);
        break;
    case '4':
        cout << "Wpisz nowy adres e-mail: ";
        cin >> newEmail;
        addressees[chosenAddresseePosition].email = newEmail;
        cout << "Adres e-mail zostal zmieniony";
        Sleep(1500);
        break;
    case '5':
        cout << "Wpisz nowy adres: ";
        cin.sync();
        getline(cin, newAddress);
        addressees[chosenAddresseePosition].address = newAddress;
        cout << "Adres zostal zmieniony";
        Sleep(1500);
        break;
    case '6':
        return;
    default:
        cout << "Nie ma takiej opcji w menu!";
        Sleep(1500);
        editAddressee(addressees, numberOfContacts, userId);
        break;
    }

    reloadDataInFile(addressees, numberOfContacts, userId, chosenAddresseePosition, addresseeId, "edit");
}

int getId(string fileName)
{
    fstream file;
    string idFromFile;
    string line;
    int id = 0;

    file.open(fileName, ios::in);

    if (file.good())
    {
        while (!file.eof())
        {
            getline(file, line);
            if (!line.empty())
            {
                idFromFile = line[0];
                id = atoi(idFromFile.c_str());
            }
        }
    }

    return id;
}

int getNumberOfContactsOrUsers(string fileName)
{
    fstream file;
    string line;
    int numberOfContactsOrUsers = 0;

    file.open(fileName, ios::in);

    if (file.good())
    {
        while (!file.eof())
        {
            getline(file, line);
            if (!line.empty())
                numberOfContactsOrUsers++;
        }
    }

    file.close();

    return numberOfContactsOrUsers;
}

void loadUsersFromFile(vector<User> &users, int numberOfUsers)
{
    fstream file;
    string line;
    string idFromFile, userName, password, partOfLine;
    User loadedUser;
    vector<string> dataOfUser;

    file.open("./users.txt", ios::in);

    for (int i = 0; i < numberOfUsers; i++)
    {
        getline(file, line);
        for (int j = 0; j < line.length(); j++)
        {
            if (line[j] != '|')
            {
                partOfLine += line[j];
            }
            else
            {
                dataOfUser.push_back(partOfLine);
                partOfLine.clear();
            }
        }
        idFromFile = dataOfUser[0];
        loadedUser.userId = atoi(idFromFile.c_str());
        loadedUser.userName = dataOfUser[1];
        loadedUser.password = dataOfUser[2];
        users.push_back(loadedUser);
        dataOfUser.clear();
    }
    file.close();
}

int registerNewUser(vector<User> &users, int numberOfUsers, int userId)
{
    User newUser;
    string userName, password;
    fstream file;

    cout << "Podaj nazwe uzytkownika: ";
    cin >> userName;

    cout << "Podaj haslo: ";
    cin >> password;

    newUser.userId = userId;
    newUser.userName = userName;
    newUser.password = password;

    users.push_back(newUser);

    file.open("./users.txt", ios::out | ios::app);

    file << users[numberOfUsers].userId << "|";
    file << users[numberOfUsers].userName << "|";
    file << users[numberOfUsers].password << "|" << endl;

    file.close();

    cout << "Rejestracja przebiegla pomyslnie" << endl;
    Sleep(1500);

    return ++numberOfUsers;
}

bool login(vector<User> &users, int numberOfUsers, int &idOfLoggedUser)
{
    string userName, password;

    cout << "Podaj nazwe uzytkownika: ";
    cin >> userName;

    cout << "Podaj haslo: ";
    cin >> password;

    for (int i = 0; i < numberOfUsers; i++)
    {
        if (userName == users[i].userName && password == users[i].password)
        {
            cout << "Logowanie zakonczone sukcesem" << endl;
            idOfLoggedUser = users[i].userId;
            Sleep(1500);
            return true;
        }
    }

    cout << "Nieprawidlowa nazwa uzytkownika lub haslo" << endl;
    Sleep(1500);
    return false;
}