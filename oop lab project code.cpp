#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX = 30;

class Vehicle
{
protected:
    string number;
    string owner;
    int hours;

public:
    Vehicle()
    {
        number= "";
        owner= "";
        hours= 0;
    }

    void setData(string n, string o, int h)
    {
        number = n;
        owner =o;
        hours = h;
    }

    string getNumber()
    {
        return number;
    }

    string getOwner()
    {
        return owner;
    }

    int getHours()
    {
        return hours;
    }

    virtual int calculateFee()
    {
        return hours*50;
    }

    virtual string getType()
    {
        return "Vehicle";
    }
};

class Car : public Vehicle
{
public:
    int calculateFee()
    {
        return hours * 100;
    }

    string getType()
    {
        return "Car";
    }
};

class Bike : public Vehicle
{
public:
    int calculateFee()
    {
        return hours* 50;
    }

    string getType()
    {
        return "Bike";
    }
};

class Truck : public Vehicle
{
public:
    int calculateFee()
    {
        return hours * 150;
    }

    string getType()
    {
        return "Truck";
    }
};

class EVCar : public Vehicle
{
public:
    int calculateFee()
    {
        return hours*100;
    }

    string getType()
    {
        return "EV Car";
    }
};

class Ticket
{
private:
    int ticketId;

public:
    Ticket()
    {
        ticketId= 0;
    }

    void setTicket(int id)
    {
        ticketId =id;
    }

    int getTicket()
    {
        return ticketId;
    }
};

class Service
{
public:
    int washCharge()
    {
        return 300;
    }

    int chargingCharge()
    {
        return 500;
    }
};

class Report
{
private:
    int totalRevenue;

public:
    Report()
    {
        totalRevenue=0;
    }

    void addRevenue(int amount)
    {
        totalRevenue=totalRevenue+amount;
    }

    friend void showRevenue(Report r);
};

void showRevenue(Report r)
{
    cout << "Total Revenue: " << r.totalRevenue << endl;
}

class ParkingLot
{
private:
    string vehicleNo[MAX];
    string ownerName[MAX];
    string vehicleType[MAX];
    
    int vehicleHours[MAX];
    bool occupied[MAX];
    bool wash[MAX];
    
    bool charging[MAX];
    int slotNo[MAX];
    
    Ticket tickets[MAX];

    int totalVehicles;
    Report report;
    Service service;

public:
    ParkingLot()
    {
        totalVehicles= 0;

        for(int i = 0; i<MAX; i++)
        {
            occupied[i] = false;
            wash[i] = false;
            
            charging[i] = false;
            slotNo[i] = i + 1;
            vehicleNo[i] = "";
            
            ownerName[i] = "";
            vehicleType[i] = "";
            vehicleHours[i] = 0;
        }
    }

    int generateTicket()
    {
        return 1000 + totalVehicles + 1;
    }

    int findSlot()
    {
        for (int i = 0; i < MAX; i++)
        {
            if (occupied[i] == false)
            {
                return i;
            }
        }

        return -1;
    }

    void vehicleEntry()
    {
        int choice;
        
        string num;
        string owner;
        int h;

        int index = findSlot();

        if (index== -1)
        {
            cout <<"Parking Full\n";
            return;
        }

        cout << "1. Car\n";
        cout << "2. Bike\n";
        cout << "3. Truck\n";
        cout << "4. EV Car\n";
        cout << "Enter Vehicle Type: ";
        cin >> choice;

        cout << "Enter Vehicle Number: ";
        cin >> num;

        cout << "Enter Owner Name: ";
        cin >> owner;

        cout << "Enter Hours: ";
        cin >> h;

        Vehicle *v;

        Car c;
        Bike b;
        
        Truck t;
        EVCar e;

        if (choice == 1)
        {
            c.setData(num, owner, h);
            v = &c;
        }
        else if (choice == 2)
        {
            b.setData(num, owner, h);
            v = &b;
        }
        else if (choice == 3)
        {
            t.setData(num, owner, h);
            v = &t;
        }
        else
        {
            e.setData(num, owner, h);
            
            v = &e;
        }

        vehicleNo[index] = v->getNumber();
        
        ownerName[index] = v->getOwner();
        vehicleType[index] = v->getType();
        
        vehicleHours[index] = v->getHours();

        occupied[index]=true;
        wash[index] =false;
        charging[index]= false;

        tickets[index].setTicket(generateTicket());

        totalVehicles++;

        cout << "Vehicle Parked Successfully\n";
        
        cout << "Slot Number: " << slotNo[index] << endl;
        cout << "Ticket ID: " << tickets[index].getTicket() << endl;

        ofstream file("records.txt", ios::app);
        file << vehicleNo[index] << " ";
        file << ownerName[index] << " ";
        
        file << vehicleType[index] << " ";
        file << slotNo[index] << endl;
        
        file.close();
    }

    void vehicleExit()
    {
        string num;
        int bill= 0;

        cout << "Enter Vehicle Number: ";
        cin >>num;

        for (int i =0; i <MAX; i++)
        {
            if (vehicleNo[i] == num && occupied[i] == true)
            {
                if (vehicleType[i] == "Car")
                {
                    bill = vehicleHours[i] * 100;
                }
                else if (vehicleType[i] == "Bike")
                {
                    bill = vehicleHours[i] * 50;
                }
                else if (vehicleType[i] == "Truck")
                {
                    bill = vehicleHours[i] * 150;
                }
                else if (vehicleType[i] == "EV Car")
                {
                    bill = vehicleHours[i] * 100;
                }

                if (wash[i] == true)
                {
                    bill = bill + service.washCharge();
                }

                if (charging[i] == true)
                {
                    bill = bill + service.chargingCharge();
                }

                report.addRevenue(bill);

                cout << "Vehicle Exit Successful\n";
                cout << "Total Bill: " << bill << endl;

                occupied[i] = false;
                wash[i] = false;
                charging[i] = false;
                vehicleNo[i] = "";
                ownerName[i] = "";
                vehicleType[i] = "";
                vehicleHours[i] = 0;

                return;
            }
        }

        cout << "Vehicle Not Found\n";
    }

    void showSlots()
    {
        for (int i= 0;i<MAX; i++)
        {
            cout << "Slot " << slotNo[i] << " : ";

            if (occupied[i] == true)
            {
                cout << "Occupied\n";
            }
            else
            {
                cout << "Available\n";
            }
        }
    }

    void searchVehicle()
    {
        string num;

        cout <<"Enter Vehicle Number: ";
        cin>> num;

        for (int i = 0; i < MAX; i++)
        {
            if (vehicleNo[i] == num && occupied[i] == true)
            {
                cout << "Vehicle Found\n";
                cout << "Owner Name: " << ownerName[i] << endl;
                
                cout << "Vehicle Type: " << vehicleType[i] << endl;
                cout << "Slot Number: " << slotNo[i] << endl;
                
                cout << "Ticket ID: " << tickets[i].getTicket() << endl;
                return;
            }
        }

        cout << "Vehicle Not Found\n";
    }

    void ticketInfo()
    {
        string num;

        cout << "Enter Vehicle Number: ";
        cin >> num;

        for (int i = 0; i < MAX; i++)
        {
            if (vehicleNo[i] == num && occupied[i] == true)
            {
                cout <<"Ticket ID: "<< tickets[i].getTicket() << endl;
                return;
            }
        }

        cout << "Vehicle Not Found\n";
    }

    void reports()
    {
        cout << "Total Vehicles Parked: " << totalVehicles << endl;
        showRevenue(report);
    }

    void saveReport()
    {
        ofstream file("report.txt");

        file << "Total Vehicles Parked: " <<totalVehicles <<endl;

        file.close();

        cout << "Report Saved\n";
    }

    void evCharging()
    {
        string num;

        cout<< "Enter Vehicle Number: ";
        cin >> num;

        for (int i = 0; i < MAX; i++)
        {
            if (vehicleNo[i] == num && occupied[i] == true)
            {
                if (vehicleType[i] == "EV Car")
                {
                    charging[i] = true;
                    cout << "EV Charging Added\n";
                }
                else
                {
                    cout<<"Only EV Car Allowed\n";
                }

                return;
            }
        }

        cout << "Vehicle Not Found\n";
    }

    void carWash()
    {
        string num;

        cout<< "Enter Vehicle Number: ";
        cin >>num;

        for (int i = 0; i < MAX; i++)
        {
            if (vehicleNo[i] == num && occupied[i] == true)
            {
                wash[i] = true;
                cout << "Car Wash Added\n";
                return;
            }
        }

        cout<< "Vehicle Not Found\n";
    }
};

int main()
{
    ParkingLot p;
    int choice;

    do
    {
        cout << "Smart Parking Lot System\n";
        cout << "1. Vehicle Entry\n";
        cout << "2. Vehicle Exit\n";
        cout << "3. Auto Slot Status\n";
        cout << "4. Search Vehicle\n";
        cout << "5. Parking Ticket Info\n";
        cout << "6. Reports\n";
        cout << "7. File Save Report\n";
        cout << "8. EV Charging Facility\n";
        cout << "9. Car Wash Facility\n";
        cout << "10. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        if (choice ==1)
        {
            p.vehicleEntry();
        }
        else if (choice== 2)
        {
            p.vehicleExit();
        }
        else if (choice ==3)
        {
            p.showSlots();
        }
        else if (choice== 4)
        {
            p.searchVehicle();
        }
        else if (choice== 5)
        {
            p.ticketInfo();
        }
        else if (choice ==6)
        {
            p.reports();
        }
        else if (choice== 7)
        {
            p.saveReport();
        }
        else if (choice==8)
        {
            p.evCharging();
        }
        else if (choice==9)
        {
            p.carWash();
        }

    } while (choice != 10);

    return 0;
}
