
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class Item
{
    int itemNo;
    char itemName[50];
    double itemPrice;
    int quantity;

public:
    void getData();
    void showData();
    int retItemNo();
    double retItemPrice();
    char *retItemName();
    void writeToFile();
    void displayAll();
    void displaySpecific(int n);
    void deleteRecord();
    void menu();
    void modifyItem();
    void placeOrder();
    void searchItem();
    void makePayment(double);
};

fstream file;
Item item;

void Item::getData()
{
    do
    {
        cout << "Enter Item Number (positive value): ";
        cin >> itemNo;
        if (itemNo <= 0)
        {
            cout << "Please enter a positive value for Item Number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    } while (itemNo <= 0);

    cout << "Enter Item Name: ";
    cin.ignore();
    cin.getline(itemName, 50);

    cout << "Enter Item Price: ";
    cin >> itemPrice;

    // cout << "Enter Item Quantity: ";
    // cin >> quantity; // Added line
    cin.ignore();
}

void Item::showData()
{
    cout << "\n";
    cout << "\nItem Number: " << itemNo;
    cout << "\nItem Name: " << itemName;
    cout << "\nItem Price: " << itemPrice;
    // cout << "\nItem Quantity: " << quantity; // Added line
}

int Item::retItemNo()
{
    return itemNo;
}

double Item::retItemPrice()
{
    return itemPrice;
}

char *Item::retItemName()
{
    return itemName;
}

void Item::writeToFile()
{
    int found = 0;
    fstream file("store.txt", ios::in | ios::out | ios::app);

    while (file.read((char *)&item, sizeof(item)))
    {
        if (item.retItemNo() == itemNo)
        {
            found = 1;
            break;
        }
    }
    file.clear();

    if (!found)
    {
        file.close();
        file.open("store.txt", ios::app);
        getData();
        file.write((char *)&item, sizeof(item));
        file.close();
        cout << "\nItem has been added to the inventory.";
    }
    else
    {
        Item newItem;
        newItem.getData();

        if (item.retItemNo() == newItem.retItemNo() &&
            strcmp(item.retItemName(), newItem.retItemName()) == 0 &&
            item.retItemPrice() != newItem.retItemPrice())
        {
            cout << "\nNew item details are the same as the existing item.\n";
            cout << "Do you want to update it (Y/N)? ";
            char choice;
            cin >> choice;

            if (choice == 'Y' || choice == 'y')
            {
                fstream tempFile("temp.txt", ios::out | ios::binary);

                file.seekg(0, ios::beg);
                while (file.read((char *)&item, sizeof(item)))
                {
                    if (item.retItemNo() != itemNo)
                    {
                        tempFile.write((char *)&item, sizeof(item));
                    }
                }
                file.close();
                tempFile.close();

                remove("store.txt");
                rename("temp.txt", "store.txt");

                file.open("store.txt", ios::app);
                newItem.getData();
                file.write((char *)&newItem, sizeof(newItem));
                file.close();

                cout << "\nItem has been updated in the inventory.";
            }
            else
            {
                cout << "\nItem remains unchanged in the inventory.";
            }
        }
        else
        {
            file.seekp(-static_cast<int>(sizeof(item)), ios::cur);
            file.write((char *)&newItem, sizeof(newItem));
            file.close();
            cout << "\nItem has been updated in the inventory.";
        }
    }
}

void Item::displayAll()
{
    ifstream file("store.txt", ios::in);
    file.seekg(0, ios::end);
    if (file.tellg() == 0)
    {
        cout << "No records found. The file is empty.\n";
        file.close();
        return;
    }
    if (!file)
    {
        cout << "Failed to open the file." << endl;
        return;
    }

    cout << "\nDisplaying All Items\n";
    file.seekg(0, ios::beg);
    while (file.read((char *)&item, sizeof(item)))
    {
        item.showData();
        cout << endl;
    }

    file.close();
}

void Item::displaySpecific(int n)
{
    int flag = 0;
    file.open("store.txt", ios::in);
    while (file.read((char *)&item, sizeof(item)))
    {
        if (item.retItemNo() == n)
        {
            item.showData();
            flag = 1;
            break;
        }
    }
    file.close();
    if (flag == 0)
    {
        cout << "\nRecord not found!";
    }
}

void Item::modifyItem()
{
    int no, found = 0;
    cout << "\nEnter Item Number to Modify: ";
    cin >> no;
    file.open("store.txt", ios::in | ios::out);
    while (file.read((char *)&item, sizeof(item)) && found == 0)
    {
        if (item.retItemNo() == no)
        {
            item.showData();
            cout << "\nEnter New Details: ";
            getData();
            int pos = -1 * static_cast<int>(sizeof(item));
            file.seekp(pos, ios::cur);
            file.write((char *)&item, sizeof(item));
            cout << "\nRecord modified successfully!";
            found = 1;
        }
    }
    file.close();
    if (found == 0)
    {
        cout << "\nRecord not found!";
    }
}

void Item::deleteRecord()
{
    int no;
    cout << "\nEnter Item Number to Delete: ";
    cin >> no;
    file.open("store.txt", ios::in);
    ofstream tempFile("temp.txt", ios::out);
    file.seekg(0, ios::beg);
    while (file.read((char *)&item, sizeof(item)))
    {
        if (item.retItemNo() != no)
            tempFile.write((char *)&item, sizeof(item));
    }
    file.close();
    tempFile.close();
    remove("store.txt");
    rename("temp.txt", "store.txt");
    cout << "\nRecord deleted successfully!";
}

void Item::searchItem()
{
    int no;
    cout << "\nEnter Item Number to Search: ";
    cin >> no;
    displaySpecific(no);
}

void Item::makePayment(double totalAmount)
{
    double userInputAmount = 0.0;

    cout << "\nTotal Amount to be paid: " << totalAmount << endl;
    int paymentMethod;

    do
    {
        cout << "Enter the amount to pay: ";
        cin >> userInputAmount;
        if (userInputAmount == totalAmount)
        {
            cout << "Payment successful! Thank you for your purchase.";
            break;
        }
        else
        {
            cout << "Incorrect amount entered. Please try again." << endl;
        }
    } while ((paymentMethod < 1 || paymentMethod > 3) || userInputAmount != totalAmount);
}

void Item::placeOrder()
{
    int order_arr[50], quan[50], c = 0;
    double amt, total = 0;
    char ch = 'Y';
    cout << "\n\nPlace your Order\n";
    do
    {
        cout << "\nEnter the Item Number: ";
        cin >> order_arr[c];
        cout << "Quantity: ";
        cin >> quan[c];
        c++;
        cout << "\nDo you want to continue (Y/N)? ";
        cin >> ch;
    } while (ch == 'Y' || ch == 'y');
    double totalAmount = 0;
    cout << "\nThank You for Placing the Order\n";
    cout << "\n******************* INVOICE *******************\n";
    cout << "\nItem No.\tName\tQuantity\tPrice\n";
    cout << "-----------------------------------------------\n";
    file.open("store.txt", ios::in);
    for (int x = 0; x < c; x++)
    {
        while (file.read((char *)&item, sizeof(item)))
        {
            if (item.retItemNo() == order_arr[x])
            {
                amt = item.retItemPrice() * quan[x];
                cout << order_arr[x] << "\t\t" << item.retItemName() << "\t     " << quan[x] << "\t\t" << item.retItemPrice() << endl;
                total += amt;
            }
        }
        file.clear();
        file.seekg(0, ios::beg);
    }
    file.close();
    cout << "-----------------------------------------------\n";
    cout << "Total Amount = " << total << endl;
    cout << "-----------------------------------------------\n";
    makePayment(total);
}

void Item::menu()
{
    int choice;
    do
    {
        cout << "\n\n******** Inventory Management System ********";
        cout << "\n1. Add Item";
        cout << "\n2. Display All Items";
        cout << "\n3. Display Specific Item";
        cout << "\n4. Modify Item";
        cout << "\n5. Delete Item";
        cout << "\n6. Search Item";
        cout << "\n7. Place Order";
        cout << "\n8. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            item.writeToFile();
            break;
        case 2:
            item.displayAll();
            break;
        case 3:
            int num;
            cout << "\nEnter Item Number: ";
            cin >> num;
            item.displaySpecific(num);
            break;
        case 4:
            item.modifyItem();
            break;
        case 5:
            item.deleteRecord();
            break;
        case 6:
            item.searchItem();
            break;
        case 7:
            item.placeOrder();
            break;
        case 8:
            cout << "\nExiting...";
            break;
        default:
            cout << "\nInvalid choice. Try again!";
        }
    } while (choice != 8);
}

int main()
{
    item.menu();

    return 0;
}
