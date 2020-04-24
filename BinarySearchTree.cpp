// BinarySearchTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include "Entry.h"
#include "BinaryTree.h"
#include "bst.h"
#include "RuntimeException.h"
#include "AVLEntry.h"
#include "AVLTree.h"
#include <fstream>
#include <string>


using namespace std;

void printOption()
{
	cout << "\t1. Search for a record" << endl;
	cout << "\t2. Insert a record" << endl;
	cout << "\t3. Delete a record" << endl;
	cout << "\t4. List all records" << endl;
	cout << "\t5. Exit" << endl << endl;
	cout << "Option--> ";
}

int main()
{
	cout << "Welcome to my AVL Tree program!" << endl << endl;
	cout << "Reading file...." << endl;
	cout << "AVL Tree ready" << endl;
	string file = "C:\\Users\\joeje\\Desktop\\p4small.txt";
	AVLTree at1(file);
	cout << "Please select options from 1 to 5" << endl << endl;

	char yn = 'y';
	while (yn == 'y')
	{
		int code, pop;
		string name;
		int option;
		printOption();
		cin >> option;
		switch (option)
		{
		case 1: // Search
			cout << "Please enter the county/state code to search " << endl;
			cin >> code;
			at1.find(code);
			
			break;
		case 2: // Insert
			cout << "Please enter the county/state code, population, name of county and state" << endl;
			cin >> code >> pop;
			getline(cin, name);
			at1.insert(code, pop, name);
			break;
		case 3: // Delete
			cout << "Please enter the county/state code to delete " << endl;
			cin >> code;
			at1.erase(code);
			break;
		case 4: // List
			at1.print();
			break;
		case 5: // Exit
			break;
		default:
			cout << "Please enter a valid option!" << endl;
			break;
		}
		if (option == 5)
		{
			break;
		}
		cout << endl << "Do you want to continue? ";
		cin >> yn;
		cout << endl;
	}

	cout << endl << "Thank you for using my AVL Tree!" << endl;

	return 0;
}

