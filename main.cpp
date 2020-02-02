#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Addressee
{
    int id = 0;
    string name = "", surname = "", address = "", email = "", tel = "";
};

bool checkIfTelephoneNumberIsCorrect(string tel);

bool checkIfEmailIsCorrect(string email);

bool checkIfNameOrSurnameIsCorrect(string nameOrSurname, string whatWillBeChecked);

bool checkIfTheDataIsRepeated(vector<Addressee> &addressees, int numberOfContacts, string name, string surname, string tel, string email, string address);

void writeDataInFile(vector<Addressee> &addressees, int indexOfAddressee);

int addContact(vector<Addressee> &addressees, int numberOfContacts, int id);

void loadDataFromFile(vector<Addressee> &addressees, int numberOfContacts);

void displayDataOfAllAddressees(vector<Addressee> &addressees, int numberOfContacts);

bool checkIfThisNameExists(vector<Addressee> &addressees, string name, int numberOfContacts);

bool checkIfThisSurnameExists(vector<Addressee> &addressees, string surname, int numberOfContacts);

void searchByName(vector<Addressee> &addressees, int numberOfContacts);

void searchBySurame(vector<Addressee> &addressees, int numberOfContacts);

bool checkIfThisIdExists(vector<Addressee> &addresssees, int numberOfContacts, int id);

void reloadDataInFile(vector<Addressee> &addressees, int numberOfContacts);

void displayAddresseeById(vector<Addressee> &addressee, int numberOfContacts, int id, int &chosenAddresseePosition);

void deleteAddressee(vector<Addressee> &addressees, int numberOfContacts);

void editAddressee(vector<Addressee> &addressees, int numberOfContacts);

int getId();

int getNumberOfContacts();

int main()
{
    vector<Addressee> addressees;
    int id = getId(), numberOfContacts = getNumberOfContacts();
    char choice;

    loadDataFromFile(addressees, numberOfContacts);

    while (1)
    {
        system("cls");
        cout << "KSIAZKA ADRESOWA" << endl;
        cout << "1. Dodaj adresata" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkich adresatow" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "9. Zakoncz program" << endl;
        cout << "Twoj wybor: ";

        cin >> choice;

        switch (choice)
        {
        case '1':
            id++;
            numberOfContacts = addContact(addressees, numberOfContacts, id);
            break;
        case '2':
            searchByName(addressees, numberOfContacts);
            break;
        case '3':
            searchBySurame(addressees, numberOfContacts);
            break;
        case '4':
            displayDataOfAllAddressees(addressees, numberOfContacts);
            break;
        case '5':
            deleteAddressee(addressees, numberOfContacts);
            id = getId();
            numberOfContacts = addressees.size();
            break;
        case '6':
            editAddressee(addressees, numberOfContacts);
            break;
        case '9':
            exit(0);
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

bool checkIfTheDataIsRepeated(vector<Addressee> &addressees, int numberOfContacts, string name, string surname, string tel, string email, string address)
{
    for (int i = 0; i < numberOfContacts; i++)
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

void writeDataInFile(vector<Addressee> &addressees, int indexOfAddressee)
{
    fstream file;

    file.open("./data.txt", ios::out | ios::app);

    file << addressees[indexOfAddressee].id << "|";
    file << addressees[indexOfAddressee].name << "|";
    file << addressees[indexOfAddressee].surname << "|";
    file << addressees[indexOfAddressee].tel << "|";
    file << addressees[indexOfAddressee].email << "|";
    file << addressees[indexOfAddressee].address << "|" << endl;

    file.close();
}

int addContact(vector<Addressee> &addressees, int numberOfContacts, int id)
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

        if (!checkIfTheDataIsRepeated(addressees, numberOfContacts, name, surname, tel, email, address))
            break;
    }

    newAddresse.id = id;
    newAddresse.name = name;
    newAddresse.surname = surname;
    newAddresse.tel = tel;
    newAddresse.email = email;
    newAddresse.address = address;
    addressees.push_back(newAddresse);

    writeDataInFile(addressees, numberOfContacts);

    cout << "Dodano do kontaktow" << endl;
    Sleep(1500);

    return ++numberOfContacts;
}

void loadDataFromFile(vector<Addressee> &addressees, int numberOfContacts)
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

        idFromFile = dataOfAddressee[0];
        loadedAddressee.id = atoi(idFromFile.c_str());
        loadedAddressee.name = dataOfAddressee[1];
        loadedAddressee.surname = dataOfAddressee[2];
        loadedAddressee.tel = dataOfAddressee[3];
        loadedAddressee.email = dataOfAddressee[4];
        loadedAddressee.address = dataOfAddressee[5];
        addressees.push_back(loadedAddressee);
        dataOfAddressee.clear();
    }
    file.close();
}

void displayDataOfSingleAddressee(vector<Addressee> &addressees, int indexOfAddressee)
{
    cout << "Id: " << addressees[indexOfAddressee].id << "|";
    cout << "Imie: " << addressees[indexOfAddressee].name << "|";
    cout << "Nazwisko: " << addressees[indexOfAddressee].surname << "|";
    cout << "Numer telefonu: " << addressees[indexOfAddressee].tel << "|";
    cout << "E-mail: " << addressees[indexOfAddressee].email << "|";
    cout << "Adres: " << addressees[indexOfAddressee].address << "|" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
}

void displayDataOfAllAddressees(vector<Addressee> &addressees, int numberOfContacts)
{
    if (numberOfContacts == 0)
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

bool checkIfThisNameExists(vector<Addressee> &addressees, string name, int numberOfContacts)
{
    for (int i = 0; i < numberOfContacts; i++)
    {
        if (name == addressees[i].name)
            return true;
    }

    return false;
}

bool checkIfThisSurnameExists(vector<Addressee> &addressees, string surname, int numberOfContacts)
{
    for (int i = 0; i < numberOfContacts; i++)
    {
        if (surname == addressees[i].surname)
            return true;
    }

    return false;
}

void searchByName(vector<Addressee> &addressees, int numberOfContacts)
{
    string name;

    if (numberOfContacts == 0)
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

        if (!checkIfThisNameExists(addressees, name, numberOfContacts))
        {
            cout << "Nie ma osob o takim imieniu w ksiazce adresowej" << endl;
            Sleep(1500);
        }
        else
        {
            cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
            for (int i = 0; i < numberOfContacts; i++)
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

void searchBySurame(vector<Addressee> &addressees, int numberOfContacts)
{
    string surname;

    if (numberOfContacts == 0)
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

        if (!checkIfThisSurnameExists(addressees, surname, numberOfContacts))
        {
            cout << "Nie ma osob o takim nazwisku w ksiazce adresowej" << endl;
            Sleep(1500);
        }
        else
        {
            cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
            for (int i = 0; i < numberOfContacts; i++)
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

bool checkIfThisIdExists(vector<Addressee> &addresssees, int numberOfContacts, int id)
{
    for (int i = 0; i < numberOfContacts; i++)
    {
        if (addresssees[i].id == id)
            return true;
    }
    return false;
}

void reloadDataInFile(vector<Addressee> &addressees, int numberOfContacts)
{
    fstream file;

    file.open("./data.txt", ios::out);

    file.clear();

    for (int i = 0; i < numberOfContacts; i++)
    {
        writeDataInFile(addressees, i);
    }

    file.close();

    addressees.clear();

    loadDataFromFile(addressees, numberOfContacts);
}

void displayAddresseeById(vector<Addressee> &addressees, int numberOfContacts, int id, int &chosenAddresseePosition)
{
    for (int i = 0; i < numberOfContacts; i++)
    {
        if (id == addressees[i].id)
        {
            chosenAddresseePosition = i;
            cout << "Oto dane adresata: " << endl;
            cout << "----------------------------------------------------------------------------------------------------------------------------" << endl;
            displayDataOfSingleAddressee(addressees, i);
        }
    }
}

void deleteAddressee(vector<Addressee> &addressees, int numberOfContacts)
{
    string name, surname;
    int id;
    bool idExists;
    char choice;
    vector<Addressee>::iterator it = addressees.begin();
    fstream file;
    int chosenAddresseePosition;
    int i = 0;

    cout << "Podaj id adresata, ktorego chcesz usunac z listy: ";
    cin >> id;

    idExists = checkIfThisIdExists(addressees, numberOfContacts, id);

    if (!idExists)
    {
        cout << "Nie ma adresata o takim id" << endl;
        Sleep(1500);
        return;
    }

    cout << endl;

    displayAddresseeById(addressees, numberOfContacts, id, chosenAddresseePosition);
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
            numberOfContacts = addressees.size();

            reloadDataInFile(addressees, numberOfContacts);

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

void editAddressee(vector<Addressee> &addressees, int numberOfContacts)
{
    char choice;
    string newName, newSurname, newEmail, newAddress, newTel;
    int id;
    int chosenAddresseePosition;

    cout << endl;
    cout << "Podaj id adresata, ktorego chcesz edytowac: ";
    cin >> id;

    if (!checkIfThisIdExists(addressees, numberOfContacts, id))
    {
        cout << "Nie ma adresata o takim id" << endl;
        Sleep(1500);
        return;
    }

    displayAddresseeById(addressees, numberOfContacts, id, chosenAddresseePosition);

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
        editAddressee(addressees, numberOfContacts);
        break;
    }

    reloadDataInFile(addressees, numberOfContacts);
}

int getId()
{
    fstream file;
    string idFromFile;
    string line;
    int id = 0;

    file.open("./data.txt", ios::in);

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

int getNumberOfContacts()
{
    fstream file;
    string line;
    int numberOfContacts = 0;

    file.open("./data.txt", ios::in);

    if (file.good())
    {
        while (!file.eof())
        {
            getline(file, line);
            if (!line.empty())
                numberOfContacts++;
        }
    }

    file.close();

    return numberOfContacts;
}