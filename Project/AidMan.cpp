
#include <iostream>
#include <fstream>
#include "Utils.h"
#include "AidMan.h"

using namespace std;

namespace sdds
{
	// Constructor
	void AidMan::save()
	{
		if (m_file != nullptr)
		{
			ofstream file;
			file.open(m_file);
			for (int i = 0; i < m_iProductItems; i++)
			{
				m_ptr[i]->save(file) << endl;
			}
		}

	}
	// Destructor
	// It deallocates the memory that was allocated dynamically
	void AidMan::deallocate()
	{
		for (int i = 0; i < m_iProductItems; i++)
		{
			delete m_ptr[i];
		}
		delete[] m_file;
		m_file = nullptr;
		m_iProductItems = 0;

	}
	// It loads the data from the file
	bool AidMan::load(const char* tempFileName)
	{
		bool isDone = false;
		bool done = false;
		int count = 0;
		save();
		deallocate();
		ut.alocpy(m_file, tempFileName);
		ofstream newFile;
		ifstream file;
		file.open(m_file);
		// If the file is not open, it will ask the user if they want to create a new file
		if (!file.is_open())
		if (!file.is_open())
		{
			cout << "Failed to open " << m_file << " for reading!";
			cout << "Would you like to create a new data file?";
			cout << "1- Yes!\n0- Exit\n>";

			int tempNum = ut.getint();
			switch (tempNum)
			{
			case 1:
				newFile.open(m_file);
				done = true;
				break;
			case 0:
				break;
			default:
				break;
			}
		}
		// If the file is open,it will read the data from the file
		if (file.is_open() && done != true)
		{
			// It will read the data from the file until it reaches the end of the file
			while (!file.eof())
			{
				char peek = file.peek();
				if (peek == '1' || peek == '2' || peek == '3')
				{
					m_ptr[m_iProductItems] = new Perishable;

					m_ptr[m_iProductItems]->load(file);
					if (m_ptr[m_iProductItems]->operator bool())
					{
						m_iProductItems++;
						count++;
					}
					else
					{
						delete m_ptr[m_iProductItems];
					}
				}
				else if (peek == '4' || peek == '5' || peek == '6' || peek == '7' || peek == '8' || peek == '9')
				{
					m_ptr[m_iProductItems] = new Item;
					m_ptr[m_iProductItems]->load(file);
					if (m_ptr[m_iProductItems]->operator bool())
					{
						m_iProductItems++;
						count++;
					}
					else
					{
						delete m_ptr[m_iProductItems];
					}
				}
				else if (!isdigit(peek))
				{
					file.setstate(ios::badbit);
				}
			}
		}
		if (m_iProductItems > 0)
		{
			isDone = true;
		}
		return isDone;
	}

	int AidMan::search(int sku) const
	{
		int j = -1;
		bool done = false;
		for (int i = 0; i < m_iProductItems && done != true; i++)
		{
			if (m_ptr[i]->operator==(sku))
			{
				j = i;
				done = true;
			}
		}
		return j;
	}

	void AidMan::addItem()
	{
		if (m_iProductItems > sdds_max_num_items)
		{
			cout << "Database full!" << endl;
		}
		else
		{
			bool done = false;
			iProduct* perishable;
			iProduct* item;

			int tempSku = 0;
			cout << "1- Perishable\n2- Non-Perishable\n-----------------\n0- Exit\n> ";
			int num = 0;
			cin >> num;

			switch (num)
			{
			case 1:
				perishable = new Perishable;
				cout << "SKU: ";
				cin >> tempSku;
				for (int i = 0; i < m_iProductItems && done != true; i++)
				{
					if (m_ptr[i]->operator==(tempSku))
					{
						cout << "Sku: " << tempSku << " is already in the system, try updating quantity instead.\n" << endl;
						done = true;
						delete perishable;
					}
				}

				if (done == false)
				{
					perishable->read(cin);
					cout << endl;
					if (perishable->operator bool())
					{
						m_ptr[m_iProductItems] = perishable;
						m_iProductItems++;
					}
					else
					{
						perishable->display(cout);
						delete perishable;
					}
				}
				break;
			case 2:
				item = new Item;
				cout << "SKU: ";
				cin >> tempSku;
				for (int i = 0; i < m_iProductItems && done != true; i++)
				{
					if (m_ptr[i]->operator==(tempSku))
					{
						cout << "Sku: " << tempSku << " is already in the system, try updating quantity instead.\n" << endl;
						done = true;
						delete item;
					}
				}
				if (done == false)
				{
					item->read(cin);
					cout << endl;
					if (item->operator bool())
					{
						m_ptr[m_iProductItems] = item;
						m_iProductItems++;
					}
					else
					{
						item->display(cout);
						delete item;
					}
				}
				break;
			case 0:
				cout << "Aborted\n";
				break;
			default:
				break;
			}
		}

	}

	void AidMan::remove()
	{
		bool done = false;
		int index = 0;
		char tempDescription[100] = { "\0" };
		cout << "Item description: ";
		cin.ignore(1000, '\n');
		cin.getline(tempDescription, 100, '\n');
		int num = list(tempDescription);
		if (num > 0)
		{
			int tempSku = ut.getint("Enter SKU: ");
			for (int i = 0; i < m_iProductItems && done != true; i++)
			{
				if (m_ptr[i]->operator==(tempSku))
				{
					cout << "Following item will be removed: " << endl;
					m_ptr[i]->linear(false);
					m_ptr[i]->display(cout) << endl;
					cout << "Are you sure?\n1- Yes!\n0- Exit\n> ";
					int num = 0;
					cin >> num;
					switch (num)
					{
					case 1:
						index = i;
						delete m_ptr[i];
						for (int j = index; j < m_iProductItems; j++)
						{
							m_ptr[j] = m_ptr[j + 1];
						}
						m_iProductItems--;
						cout << "Item removed!" << endl << endl;
						done = true;
						break;

					case 0:
						cout << "Aborted!" << endl;
						break;
					default:
						break;
					}
				}
			}
		}
	}

	void AidMan::updateQuantity()
	{
		bool done = false;
		char tempDescription[100] = { "\0" };
		cout << "Item description: ";
		cin.ignore(1000, '\n');
		cin.getline(tempDescription, 100, '\n');

		int num = list(tempDescription);
		if (num > 0)
		{
			int tempSku = ut.getint("Enter SKU: ");
			for (int i = 0; i < m_iProductItems && done != true; i++)
			{
				if (m_ptr[i]->operator==(tempSku))
				{
					int value = 0;
					cout << "1- Add\n2- Reduce\n0- Exit\n> ";
					cin >> value;
					switch (value)
					{
					case 1:
						if (m_ptr[i]->qty() >= m_ptr[i]->qtyNeeded())
						{
							cout << "Quantity Needed already fulfilled!\n" << endl;
						}
						else
						{
							int addValue = ut.getint(1, m_ptr[i]->qtyNeeded(), "Quantity to add: ");
							m_ptr[i]->operator+=(addValue);
							cout << addValue << " items added!\n" << endl;
						}
						break;
					case 2:
						if (m_ptr[i]->qty() == 0)
						{
							cout << "Quaintity on hand is zero!\n" << endl;
						}
						else
						{
							int reduceValue = ut.getint(1, m_ptr[i]->qtyNeeded(), "Quantity to reduce: ");
							m_ptr[i]->operator-=(reduceValue);
							cout << reduceValue << " items removed!\n" << endl;
						}
						break;
					case 0:
						cout << "Aborted!\n" << endl;
						break;
					default:
						break;
					}
					done = true;
				}
			}

			if (done != true)
			{
				cout << "SKU not found!\n";
			}

		}
		else
		{
			cout << "No matches found!\n";
		}
	}
	void AidMan::sort()
	{

		for (int i = 0; i < m_iProductItems - 1; i++)
		{

			for (int j = 0; j < m_iProductItems - i - 1; j++)
			{
				int left_Qty = m_ptr[j]->qty();
				int leftQtyNeeded = m_ptr[j]->qtyNeeded();
				int leftValue = leftQtyNeeded - left_Qty;
				int right_Qty = m_ptr[j + 1]->qty();
				int rightQtyNeeded = m_ptr[j + 1]->qtyNeeded();
				int rightValue = rightQtyNeeded - right_Qty;
				if (leftValue < rightValue)
				{
					iProduct* temp;
					temp = m_ptr[j];
					m_ptr[j] = m_ptr[j + 1];
					m_ptr[j + 1] = temp;
				}
			}
		}
		cout << "Sort completed!\n" << endl;

	}
	void AidMan::shipItems()
	{
		int shippingCount = 0;
		int day = 0;
		int month = 0;
		int year = 0;
		ut.getSystemDate(&year, &month, &day);
		ofstream file;
		file.open("shippingOrder.txt");
		file << "Shipping Order, Date: " << year << "/0" << month << "/" << day << endl;
		file << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry" << endl;
		file << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;
		for (int i = 0; i <  m_iProductItems; i++)
		{
			if (m_ptr[i]->qty() == m_ptr[i]->qtyNeeded())
			{
				m_ptr[i]->linear(true);
				shippingCount++;
				file << " ";
				file.setf(ios::right);
				file.width(3);
				file << shippingCount;
				file.unsetf(ios::right);
				file << " | ";
				m_ptr[i]->display(file) << endl;
				removeIndex(i);
			}
		}
		file << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;

		cout <<"Shipping Order for " << shippingCount << " times saved!\n" << endl;
	}
	int AidMan::removeIndex(int index)
	{
		int delete_Index = index;
		delete m_ptr[index];
		for (int j = delete_Index; j <  m_iProductItems; j++)
		{
			m_ptr[j] = m_ptr[j + 1];
		}
		 m_iProductItems--;
		return 0;
	}

	unsigned int AidMan::menu() const
	{
		int year = 0;
		int month = 0;
		int day = 0;
		ut.getSystemDate(&year, &month, &day);
		cout << "Aid Management System" << endl;
		cout << "Date: " << year << "/0" << month << "/" << day << endl;
		cout << "Data file: ";
		if (m_file != nullptr)
		{
			cout << m_file << endl;
		}
		else
		{
			cout << "No file" << endl;
		}
		cout << "---------------------------------" << endl;
		int  num = Menu::run();
		return num;
	}

	AidMan::AidMan(unsigned int num, const char* str) :Menu(num, str)
	{
		m_file = nullptr;
	}

	AidMan::~AidMan()
	{
		deallocate();
	}

	void AidMan::run()
	{
		int num = 0;
		char tempChar[30] = { "\0" };
		do
		{
			num = menu();
			if (m_file == nullptr && num != 0)
			{
				switch (num)
				{
				default:
					cout << "\n****New/Open Aid Database****" << endl;
					cout << "Enter file name: ";
					char tempChar[30] = { "\0" };
					cin >> tempChar;
					if (load(tempChar))
					{
						cout << m_iProductItems << " records loaded!" << endl << endl;
					}
					break;
				}
			}
			else
			{
				switch (num)
				{
				case 0:
					cout << "Exiting Program!" << endl;
					save();
					break;
				case 1:
					cout << "\n****List Items****" << endl;
					list();
					break;
				case 2:
					cout << "\n****Add Item****" << endl;
					addItem();
					break;
				case 3:
					cout << "\n****Remove Item****" << endl;
					remove();
					break;
				case 4:
					cout << "\n****Update Quantity****" << endl;
					updateQuantity();
					break;
				case 5:
					cout << "\n****Sort****" << endl;
					sort();
					break;
				case 6:
					cout << "\n****Ship Items****" << endl;
					shipItems();
					break;
				case 7:
					cout << "\n****New/Open Aid Database****\n" << endl;
					cout << "Enter file name: ";

					cin >> tempChar;
					if (load(tempChar))
					{
						cout << m_iProductItems << " records loaded!" << endl;
					}
					break;
				default:
					break;
				}
			}
		} while (num != 0);
	}

	int AidMan::list(const char* sub_desc)
	{
		int count = 0;
		bool isRow = true;
		if (m_iProductItems > 0) {
			cout << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry" << endl;
			cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;
			if (sub_desc == nullptr)
			{

				for (int i = 0; i < m_iProductItems; i++)
				{
					cout << " ";
					cout.setf(ios::right);
					cout.width(3);
					cout.fill(' ');
					cout << i + 1;
					cout.unsetf(ios::right);
					cout << " | ";
					m_ptr[i]->linear(true);
					m_ptr[i]->display(cout);
					cout << endl;
					count++;
				}
			}
			else
			{
				for (int i = 0; i < m_iProductItems; i++)
				{
					if (m_ptr[i]->operator==(sub_desc))
					{
						cout << " ";
						cout.setf(ios::right);
						cout.width(3);
						cout.fill(' ');
						cout << i + 1;
						cout.unsetf(ios::right);
						cout << " | ";
						m_ptr[i]->linear(true);
						m_ptr[i]->display(cout);
						cout << endl;
						count++;
						isRow = false;
					}
				}
			}
			cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;

			if (isRow) {
				cout << "Enter row number to display details or <ENTER> to continue:\n> ";
				cin.ignore();
				if (cin.peek() != '\n') {
					char temp[4] = "";
					cin >> temp;
					int tempNum = atoi(temp);
					m_ptr[tempNum - 1]->linear(false);
					m_ptr[tempNum - 1]->display(cout);
				}
				cout << endl;
			}
		}
		else
		{
			cout << "The list is emtpy!" << endl;
		}
		return count;
	}



}