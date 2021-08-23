#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

//Weapon struct declaration
struct Weapon {
    Weapon(string weaponName, int powerRating, float powerCons);
    int powerRating;
    float powerCons;
    string weaponName;
};

//For the creation of weapon objects
Weapon::Weapon(string weaponName, int powerRating, float powerCons) {
    this->weaponName = weaponName;
    this->powerRating = powerRating;
    this->powerCons = powerCons;
}

//Ship class declaration
class Ship {
    string name;
    string shipClass;
    int shipLength;
    int capacity;
    float warpSpeed;
    unsigned int numWeapons;
public:
    vector<Weapon> weapons;
    vector<Ship> ships;
    Ship(string name, string shipClass, int shipLength, int capacity, float warpSpeed, int numWeapons);
    void LoadData(string fileName);
    void Print();
    int GetPower();
    string GetName() { return name; }
    string GetClass() { return shipClass; }
    int GetCapacity() { return capacity; }
    float GetWarpSpeed() { return warpSpeed; }
    int GetNumWeapons() { return numWeapons; }
    int GetWeaponPower(int i) { return weapons.at(i).powerRating; }
};

//For the creation of ship objects
Ship::Ship(string name, string shipClass, int shipLength, int capacity, float warpSpeed, int numWeapons) {
    this->name = name;
    this->shipClass = shipClass;
    this->shipLength = shipLength;
    this->capacity = capacity;
    this->warpSpeed = warpSpeed;
    this->numWeapons = numWeapons;
}

//To help determine the strongest and weakest ships
int Ship::GetPower() {
    int power = 0;
    for (unsigned int i = 0; i < numWeapons; i++)
        power = power + (weapons.at(i).powerRating);
    return power;
}

vector<Ship> LoadData(string fileName) {
    vector<Ship> tempShips;
    unsigned int count = 0;
    unsigned int nameLength = 0;
    string name = "";
    unsigned int classLength = 0;
    string shipClass = "";
    unsigned int shipLength = 0;
    unsigned int capacity = 0;
    float warpSpeed = 0.0;
    unsigned int numWeapons = 0;
    unsigned int weaponNameLength = 0;
    string weaponName = "";
    int powerRating = 0;
    float powerCons = 0.0;

    ifstream inFile(fileName, ios_base::binary);
    if (inFile.is_open()) {
        inFile.read((char*)&count, 4);
        for (unsigned int i = 0; i < count; i++) {
            //1. Length of the ship name
            inFile.read((char*)&nameLength, 4);

            //2. Ship name (string)
            char* temp = new char[nameLength + 1];
            inFile.read(temp, nameLength * sizeof(temp[0]));
            name = temp;
            delete[] temp;

            //3. Length of the ship's class
            inFile.read((char*)&classLength, 4);

            //4. Ship's class name (string)
            temp = new char[classLength + 1];
            inFile.read(temp, classLength * sizeof(temp[0]));
            shipClass = temp;
            delete[] temp;

            //5. Ship's length
            inFile.read((char*)&shipLength, 2);

            //6. Ship's shield capacity
            inFile.read((char*)&capacity, 4);

            //7. Ship's warp speed
            inFile.read((char*)&warpSpeed, 4);

            //8. Number of weapons
            inFile.read((char*)&numWeapons, 4);

            Ship newShip = Ship(name, shipClass, shipLength, capacity, warpSpeed, numWeapons);

            if (numWeapons != 0) {
                for (unsigned int i = 0; i < numWeapons; i++) {
                    //9. Length of the weapon name
                    inFile.read((char*)&weaponNameLength, 4);

                    //10. Weapon name (string)
                    temp = new char[weaponNameLength + 1];
                    inFile.read(temp, weaponNameLength * sizeof(temp[0]));
                    weaponName = temp;
                    delete[] temp;

                    //11. Power rating
                    inFile.read((char*)&powerRating, 4);

                    //12. Power consumption
                    inFile.read((char*)&powerCons, 4);

                    newShip.weapons.push_back(Weapon(weaponName, powerRating, powerCons));
                }
            }
            tempShips.push_back(newShip);
        }
        inFile.close();
    }
    return tempShips;
}

//General-purpose print function
void Ship::Print() {
    cout << "Name: " << name << endl;
    cout << "Class: " << shipClass << endl;
    cout << "Length: " << shipLength << endl;
    cout << "Shield capacity: " << capacity << endl;
    cout << "Maximum Warp: " << warpSpeed << endl;
    cout << "Armaments: " << endl;

    //Assuming there are weapons available
    if (numWeapons != 0) {
        for (unsigned int i = 0; i < numWeapons; i++) {
            cout << weapons.at(i).weaponName << ",";
            cout << " " << weapons.at(i).powerRating << ",";
            cout << " " << weapons.at(i).powerCons << endl;
        }
        cout << "Total firepower: " << GetPower() << endl;
    }
    else {
        cout << "Unarmed" << endl;
    }
    cout << endl;
}

int main() {
    vector<Ship> ships;
    cout << "Which file(s) to open?\n";
    cout << "1. friendlyships.shp" << endl;
    cout << "2. enemyships.shp" << endl;
    cout << "3. Both files" << endl;
    int option;
    cin >> option;

    /* Load files here */
    switch (option) {
    case 1:
        ships = LoadData("friendlyships.shp");
        break;
    case 2:
        ships = LoadData("enemyships.shp");
        break;
    case 3:
        ships = LoadData("friendlyships.shp");
        vector<Ship> shipsAppend = LoadData("enemyships.shp");
        ships.insert(ships.end(), shipsAppend.begin(), shipsAppend.end());
        //Note: Idea for appending vector to vector from "Stack Overflow"
        break;
    }

    cout << "1. Print all ships" << endl;
    cout << "2. Starship with the strongest weapon" << endl;
    cout << "3. Strongest starship overall" << endl;
    cout << "4. Weakest ship (ignoring unarmed)" << endl;
    cout << "5. Unarmed ships" << endl;

    cin >> option;

    /* Work your magic here */
    switch (option) {
        //1. Print all ships
    case 1: {
        for (unsigned int i = 0; i < ships.size(); i++)
            ships.at(i).Print();
        break;
    }

          //2. Print most powerful weapon
    case 2: {
        int currentPower;
        int index = 0;
        int bestWeapon = 0;
        for (unsigned int i = 0; i < ships.size(); i++) {
            //If there are weapons present
            if (ships.at(i).GetNumWeapons() > 0) {
                for (int k = 0; k < ships.at(i).GetNumWeapons(); k++) {
                    currentPower = ships.at(i).GetWeaponPower(k);
                    if (currentPower > bestWeapon) {
                        bestWeapon = currentPower;
                        index = i;
                    }
                }
            }
        }
        ships.at(index).Print();
        break;
    }

          //3. Print most powerful ship (reuse code)
    case 3: {
        int index = 0;
        int bestShip = 0;
        int currentPower = 0;
        for (unsigned int i = 0; i < ships.size(); i++) {
            //If there are weapons present
            if (ships.at(i).GetNumWeapons() > 0) {
                currentPower = ships.at(i).GetPower();
                if (currentPower > bestShip) {
                    bestShip = currentPower;
                    index = i;
                }
            }
        }
        ships.at(index).Print();
        break;
    }

          //4. Print weakest ship (reuse code)
    case 4: {
        int index = 0;
        int worstShip = 100000; //High number to find ships lower than this threshold
        int currentPower = 0;
        for (unsigned int i = 0; i < ships.size(); i++) {
            //If there are weapons present
            if (ships.at(i).GetNumWeapons() > 0) {
                currentPower = ships.at(i).GetPower();
                if (currentPower < worstShip) {
                    worstShip = currentPower;
                    index = i;
                }
            }
        }
        ships.at(index).Print();
        break;
    }

          //5. Print unarmed ships (0 weapons)
    case 5:
        for (unsigned int i = 0; i < ships.size(); i++) {
            if (ships.at(i).GetNumWeapons() == 0) {
                ships.at(i).Print();
            }
        }
        break;
    }
    return 0;
}