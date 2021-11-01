#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
using namespace std;

//Function Prototypes ( For General Purpose - Both Customer and Manager )...
void printHeader(string);
int readFile(int &);
void determineUser();
void printMenu();
void saveToFile();
double calcTotalPerOrder(int);

//Function Prototypes ( For Manager Purpose )...
void manager();
void checkOrder();
void updateMenu();
void updateName();
void updatePrice();
void addMenu();
void deleteMenu();
void prepareOrder(int);
void checkTotalSales();

//Function Prototypes ( For Customer Purpose )...
void customer();
void orderFood();
void checkCart();
void deleteOrder(int[]);
void changeQuantity(int[]);
void pay();


//Global Variables to be used throughout program
int numOfMenu;			//calculate number of menu of restaurant
int numOfOrder;			//calculate number of order of restaurant
int numberOfCustomerOrdered=0;	//to keep track how many food ordered by a customer
double totalSales=0;	//store totalsales of a day.
int orderFinished=0;	//store number of order finished of a day

//Parallel array for "restaurant menu", store name and price
//	maximum can store 100 different menu
string menu[100];
double price[100];

//2D arrays for customer details( name, location, numTel ),
//	customers orders( ordered food name, price, quantity )
//	maximum can store 100 different customers
string customerDetail[100][3];
int orderedQuantity[100][100];
string orderedName[100][100];
double orderedPrice[100][100];

int main(){
	
	cout<<setprecision(2)<<fixed;
	
	//after program run, calculate num of menu & num of order written in file.
	//	And store to respective arrays
	numOfMenu=readFile(numOfOrder);
	
	//after file is read, array is stored, ask user to choose type of user.
	determineUser();
	
	return 0;
}

/*
What this function do? : 
	This function will do following things:
		1. Print header based on the title given.
*/
void printHeader(string title){
	cout<<"\n-----------------------------------------------------------------\n";
	cout<<"|\t\t\t"<<left<<setw(40)<<title;
	cout<<"|\n-----------------------------------------------------------------\n"; 
}

/*
What this function do? : 
	This function will do following things:
		1. Determine type of user.
		2. Go to respective function.
*/
void determineUser(){
	//print header called "Main Menu" and ask user to choose
	printHeader("Main Menu");	
	cout<<"Please enter a number: \n\n ( 1 ) Restaurant Managers\n ( 2 ) Customers\n\n\nChoice: ";
	
	int choice;
	cin>>choice;
	
	//input validation for choice
	while(choice!=1&&choice!=2)	{
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 	
	
	//go to manager() function
	if(choice==1){
		manager();
	} 
	//else go to customer()
	else customer();
}

/*
What this function do? : 
	This function will do following things:
		1. Calculate numOfMenu and numOfOrder for later use.
		2. Read file named "restaurant.txt" that contained menus and orders
		3. Store all data (menu and order) to respective arrays
*/
int readFile(int &numOfOrder){
	
	int countMenu=0;	//index to store menu inside file to array
	int countOrder=0;	//index to store order inside file to array
	
	//open file named "restaurant.txt"
	ifstream inFile;
	inFile.open("restaurant.txt");
	
	//determine type of data collected ---- (order) or (menu)
	string type;
	
	//get type of data inside file, exp: type = "(menu)";
	while(getline(inFile, type, ':')){
		
		//execute this if type is menu
		if(type=="(menu)"){
			//add menu name and price to array, based on index(countMenu)
			getline(inFile, menu[countMenu], '\t');
			inFile>>price[countMenu];
						
			//increment the counter
			++countMenu;
			inFile.ignore();
			
		//execute this if type is order
		}else if(type=="(order)"){
			//add customer details to 2D array, based on index(countOrder)
			getline(inFile, customerDetail[countOrder][0], '\t');
			getline(inFile, customerDetail[countOrder][1], '\t');
			getline(inFile, customerDetail[countOrder][2], '\n');
				
				//store all customers orders into respective arrays using loop
				//	until encountered quantity == 0
				for(int i=0; i<100; i++){
					
					inFile>>orderedQuantity[countOrder][i];
					
					if(orderedQuantity[countOrder][i]>0){
						inFile.ignore();
						getline(inFile, orderedName[countOrder][i], '\t');
						inFile>>orderedPrice[countOrder][i];
					}else{
						inFile.ignore();
						break;
					}
				}
			//increment the counter
			++countOrder;
		}
	}
	inFile.close();	//close file
	numOfOrder=countOrder;	//update the numOfOrder as reference variable
	//cout<<ii<<endl<<mm;
	return countMenu;	//return countMenu and store to numOfMenu
}

/*
What this function do? : 
	This function will do following things:
		1. Determine type of action for manager.
		2. Go to respective function.
*/
void manager(){
	//print Manager Page header
	printHeader("Manager Menu");
	cout<<"Please enter a number: \n\n ( 1 ) Update/Change Menu\n ( 2 ) Check Orders\n ( 3 ) Check Today Total Sales\n\n\n\n ( 88 ) View Restaurant Menu\n ( 99 ) Save to File & Close System\n\n\nChoice: ";
	
	int choice;
	cin>>choice;
	
	//input validation for choice
	while(choice!=1&&choice!=2&&choice!=3&&choice!=99&&choice!=88){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	
	//go to respective function based on choice entered
	if(choice==1) updateMenu();
	else if(choice==3) checkTotalSales();
	else if(choice==99) saveToFile();
	else if(choice==88){
		printMenu();
		manager();
	} 
	else checkOrder();
}

/*
What this function do? : 
	This function will do following things:
		1. Show number of orders waiting.
		2. Print all customers' details and their orders.
		3. Ask which order to prepare
*/
void checkOrder(){
	//print Check Order Page header
	printHeader("Check Order");	
	
	//variable to print total amount for each order printed
	double total=0;
	
	//print number of orders waiting
	cout<<"Please enter a number: \n\nNumber of Orders waiting: "<<numOfOrder<<endl<<endl;
	
	//print all customers details and their orders using loop
	for(int i=0; i<numOfOrder; i++){
		cout<<" ( "<<i+1<<" ) Name: "<<customerDetail[i][0]<<"\nLocation Area: "<<customerDetail[i][1]<<"\nTelephone Number: "<<customerDetail[i][2]<<"\n\nOrder:\n";
		
		for(int j=0; j<100; j++){
			
			//print the orders if quantity is greater than 0
			if(orderedQuantity[i][j]>0){
				cout<<left<<setw(20)<<orderedName[i][j]<<"RM "<<orderedPrice[i][j]<<"\tQuantity: "<<orderedQuantity[i][j]<<endl;
				//calculate the total amount for each order
				total+=orderedQuantity[i][j]*orderedPrice[i][j];
			}
		}
		cout<<endl;		
	}
	cout<<"\n\n ( 00 ) to Go Back: \n\n\nChoice: ";
	int choice;
	cin>>choice;
		
	//input validation for choice selected
	while(choice<0||choice>numOfOrder){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	
	//go back to manager() or prepareOrder() based on choice entered	
	if(choice!=0){
		prepareOrder(choice);
	}else{
		manager();		
	}	
}

/*
What this function do? : 
	This function will do following things:
		1. Show estimated time for orders to deliver.
		2. Calculate total amount per order added with delivering fee.
		3. Move all order after selected order in front by 1 using loop
*/
void prepareOrder(int choice){
	//variable to calculate total amount per order
	double totalPerOrder=0;
	cout<<"\n-----------------------------------------------------------------\n";
	
	//determine and print estimated delivering time and delivering fee based on customer's location
	if(customerDetail[choice-1][1]=="Bukit Mertajam"){
		cout<<"\nThis order will be deliver to: "<<customerDetail[choice-1][1]<<endl;
		cout<<"Estimated Time for Delivery: within 10 minutes.\n";
		cout<<"Delivering Fee: RM 4.00\n\n";
		totalPerOrder+=4;
	}else{
		cout<<"\nThis order will be deliver to: "<<customerDetail[choice-1][1]<<endl;
		cout<<"Estimated Time for Delivery: within 40 minutes.\n\n";
		cout<<"Delivering Fee: RM 7.00\n\n";
		totalPerOrder+=7;
	}
	
	//print all prepared food based on the order
	cout<<"Prepared: \n\n";
	for(int k=0; k<100; k++){
		if(orderedQuantity[choice-1][k]!=0){
			cout<<left<<setw(20)<<orderedName[choice-1][k]<<"--RM "<<orderedPrice[choice-1][k]<<" x "<<orderedQuantity[choice-1][k]<<"\n";
		}else break;
	}
	
	//go to calcTotalPerOrdere() to calculate total amount per order
	totalPerOrder+=calcTotalPerOrder(choice);
	
	//add to total sales of a day
	totalSales+=totalPerOrder;
	
	//increment the counter: orderFinished (to keep track of number of order finished)
	++orderFinished;
	
	//This all loops is to move all customer details and their order in arrays after selected one 
	//	by bringing in front by 1 using loop
	//
	//	This can delete the selected order, because that order is delivered and should be erase from arrays.
	for(int i=choice-1; i<numOfOrder-1; i++){
		
		for(int j=0; j<100; j++){
							
			if((orderedQuantity[i+1][j]==0)&&(orderedQuantity[i][j]==0)){
				break;
			}else{
				orderedName[i][j]=orderedName[i+1][j];
				orderedQuantity[i][j]=orderedQuantity[i+1][j];
				orderedPrice[i][j]=orderedPrice[i+1][j];
			}
		}
			customerDetail[i][0]=customerDetail[i+1][0];
			customerDetail[i][1]=customerDetail[i+1][1];
			customerDetail[i][2]=customerDetail[i+1][2];
		}
		--numOfOrder;
		//print total amount based on selected order
		cout<<"\n\nTotal Per Order = RM "<<totalPerOrder<<endl;
		
		//informed that the order is being delivered
		cout<<"Order has been out for delivery. \n\n";
		
		//go back to manager()
		manager();
}

/*
What this function do? : 
	This function will do following things:
		1. Show number of order finished.
		2. Show total sales for today.
		3. Go to manager()
*/
void checkTotalSales(){
	//print Check Total Sales Today Page header
	printHeader("Check Total Sales Today");	
	
	//print number of order finsihed and total sales for today
	cout<<"\nNumber of Order Finished Today == "<<orderFinished<<endl;
	cout<<"Total Sales Today == RM "<<totalSales<<endl<<endl;
	
	//go back to manager()
	manager();
}

/*
What this function do? : 
	This function will do following things:
		1. Determine type of action for customer.
		2. Go to respective function.
*/
void customer(){
	//print Customer Page header
	printHeader("Customer Menu");	
	cout<<"Please enter a number: \n\n ( 1 ) Order Food\n ( 2 ) Check Food Cart\n ( 3 ) Pay\n\n\nChoice: ";
	int choice;
	cin>>choice;
	
	//input validation for choice
	while(choice!=1&&choice!=2&&choice!=3){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	cin.ignore();
	
	
	//go to respective function based on choice
	if(choice==1) {
		//go to orderFood() if numOfOrder havent reach 100
		if(numOfOrder<100)	orderFood();
		
		//else save to file and close system
		else{
			cout<<"Sorry... Number of Orders already exceeded maximum order...Please wait until restaurant finished some order.."<<endl;
			saveToFile();
		} 
	}
	else if(choice==3) pay();
	else checkCart();
}

/*
What this function do? : 
	This function will do following things:
		1. Display all order made by user
		2. Store all index of order list to an array
		3. Determine type of action for customer.
		4. Go to respective function with array as arguments.
*/
void checkCart(){
	
	//create array to store index of orders by customer 
	//	Pass to function to change quantity or to delete order
	//	Useful for being used as index of orders
	int index[numberOfCustomerOrdered];
	
	//bool to check if cart is empty
	bool isEmpty=true;
	
	//print Customer Page header
	printHeader("Check Cart");	
	
	//print all order made by customer
	for(int j=0, i=0; j<numOfMenu+1; j++){
		
		//check if quantity > 0, means that user did order this menu
		if(orderedQuantity[numOfOrder][j]>0){
			
			isEmpty=false;	//change isEmpty to false to indicate the cart is not empty
			index[i]=j;		//store this index (j) to array if user DID order this
			++i;			//increment if index (j) is being stored in array
							//	 int i acted as index of array
							
			//print all order list
			cout<<left<<setw(20)<<orderedName[numOfOrder][j]<<"--RM "<<orderedPrice[numOfOrder][j]<<" x "<<orderedQuantity[numOfOrder][j]<<"\n";
		}
	}
	
	int choice;
	
	//check if cart is empty
	if(!isEmpty){
		//ask user to enter type of actions
		cout<<"\n\n\n ( 88 ) Delete 1 Order\n ( 99 ) Change Quantity\n ( 00 ) Go Back to Customer Menu\n\n\nChoice: ";
		cin>>choice;
		
		//input validation for choice
		while(choice!=88&&choice!=99&&choice!=00){
			cout<<"Choice entered Invalid, enter again: ";
			cin>>choice;
		} 	
		
		//go to respective function based on choice
		if(choice==88){
			deleteOrder(index);
		}else if(choice==99){
			changeQuantity(index);
		}else{
			customer();
		}
	}
	
	//if cart is empty, print error msg and go back to customer()
	if(isEmpty) cout<<"Your cart is empty. Please add something...\n\n";
	customer();
}

/*
What this function do? : 
	This function will do following things:
		1. Display all order made by user
		2. Let user choose an order to delete
		3. Delete selected order from arrays
*/
void deleteOrder(int index[]){
	//print Delete Order Page header
	printHeader("Delete Order");
	
	//print all order using index array and ask user to select one to delete	
	cout<<"Please enter a number: \n\n";
	for(int i=0; i<numberOfCustomerOrdered; i++){
			cout<<" ( "<<i+1<<" ) "<<left<<setw(20)<<orderedName[numOfOrder][index[i]]<<"--RM "<<orderedPrice[numOfOrder][index[i]]<<" x "<<orderedQuantity[numOfOrder][index[i]]<<"\n";
	}
	cout<<"\n\n\n ( 00 ) Go Back to Customer Menu\n\n\nChoice: ";
	
	int choice;
	cin>>choice;
	
	//input validation for choice entered
	while((choice<1||choice>numberOfCustomerOrdered)&&choice!=00){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	if(choice==00) customer();
	else{
		//delete order selected from arrays and decrement number of customer ordered
		orderedName[numOfOrder][index[choice-1]]="";
		orderedPrice[numOfOrder][index[choice-1]]=0.0;
		orderedQuantity[numOfOrder][index[choice-1]]=0;
		--numberOfCustomerOrdered;
	}
	
	//go back to customer()
	customer();
}

/*
What this function do? : 
	This function will do following things:
		1. Display all order made by user
		2. Let user choose an order to change quantity
		3. Change quantity of selected order from arrays
*/
void changeQuantity(int index[]){
	//print Change Order Quantity Page header
	printHeader("Change Quantity");	
	cout<<"Please enter a number: \n\n";
	int quantity;
	
	//print all order using index array and ask user to select one to change quantity
	for(int i=0; i<numberOfCustomerOrdered; i++){
			cout<<" ( "<<i+1<<" ) "<<left<<setw(20)<<orderedName[numOfOrder][index[i]]<<"--RM "<<orderedPrice[numOfOrder][index[i]]<<" x "<<orderedQuantity[numOfOrder][index[i]]<<"\n";
	}
	cout<<"\n\n\n ( 00 ) Go Back to Customer Menu\n\n\nChoice: ";
	
	int choice;	
	cin>>choice;
	
	//input validation for choice entered
	while((choice<1||choice>numberOfCustomerOrdered)&&choice!=00){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	if(choice==00) customer();
	else{
		//input new quantity
		cout<<"Change to how many? ";
		cin>>quantity;
		
		//input validation for choice entered
		while(quantity<=0) {
			cout<<"Quantity entered Invalid, enter again: ";
			cin>>quantity;
		}	
		
		//change quantity for food selected	
		orderedQuantity[numOfOrder][index[choice-1]]=quantity;
	}
	//back to customer menu
	customer();
}

/*
What this function do? : 
	This function will do following things:
		1. Ask user to enter customer details (name, location , numTel, ...)
		2. Determine delivering fee based on location
		3. Calculate total amount need to pay to user
		4. Go to saveToFile() function to write all data into files and 
			exit program.
*/
void pay(){
	
	//print Pay Order Page header
	printHeader("Pay Order");	
	double totalPerOrder=0;
	
	//let user enter customer name and stored into arrays( customerDetail)
	cout<<"What is your name? ";
	getline(cin, customerDetail[numOfOrder][0]);
	
	//input validation for name entered
	while(customerDetail[numOfOrder][0] == ""){
		cout<<"Name entered Invalid, enter again: ";
		getline(cin, customerDetail[numOfOrder][0], '\n');
	} 
	
	//let user enter customer location and stored into arrays( customerDetail)
	cout<<"Do you live in Bukit Mertajam Area? (y/n): ";
	char ans;
	cin>>ans;
	cin.ignore();
	
	//input validation for answer entered
	while(ans!='y'&&ans!='n'){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>ans;
		cin.ignore();
	}
	
	if(ans=='y')	customerDetail[numOfOrder][1]="Bukit Mertajam";
	else {
		cout<<"Please enter location area: ";
		getline(cin, customerDetail[numOfOrder][1]);
		
		//input validation for location entered
		while(customerDetail[numOfOrder][1] == ""){
			cout<<"Location entered Invalid, enter again: ";
			getline(cin, customerDetail[numOfOrder][1]);
		} 
	}
	
	//determine delivering fee based on location entered
	if(customerDetail[numOfOrder][1]=="Bukit Mertajam") {
		totalPerOrder+=4;
		cout<<"Since you live in Bukit Mertajam Area, delivering fee will be RM 4.00."<<endl<<endl;
	}else{
		totalPerOrder+=7;
		cout<<"Since you are not living in Bukit Mertajam Area, delivering fee will be RM 7.00."<<endl<<endl;
	}
	
	//let user enter customer telephone number and stored into arrays( customerDetail)
	cout<<"What is your tel num? ";
	long int telNum;
	cin>>telNum;
	
	//input validation for tel num entered
	while(telNum < 100000000){
		cout<<"Tel num entered Invalid, enter again: ";
		cin>>telNum;
	} 
	customerDetail[numOfOrder][2]="0"+to_string(telNum);
	
	//calculate total amount per order
	totalPerOrder += calcTotalPerOrder(numOfOrder+1);
	
	//increase numOfOrder
	++numOfOrder;
	
	//print total amount to user and msg that order collected
	cout<<"Total amount: RM "<<totalPerOrder<<endl;
	cout<<"\n\nWe received your order. Please wait while your order is being processed...\n";
	saveToFile();
	
	//exit the program
	exit(0);
}

/*
What this function do? : 
	This function will do following things:
		1. Print all restaurant menu
		2. Allow user to add or order 
		3. Store to respective arrays
*/
void orderFood(){
	
	//print list of restaurant menus
	printMenu();
	cout<<"\n\n( 00 ) Return to Customer Menu\n\n\n";
	cout<<"choice: ";
	int choice;
	cin>>choice;
	
	//input validation for choice entered
	while(choice>numOfMenu||choice<0){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	
	//go to customer() if user wants to
	if(choice==0){
		customer();
	}else{
		int quantity;
		
		//check whether user already ordered this food
		if(orderedQuantity[numOfOrder][choice-1]>0){
			
			//ask user to add how many instead of reset it
			cout<<"add? ";
			cin>>quantity;
		
			//input validation for quantity
			while(quantity<=0){
				cout<<"Quantity entered Invalid, enter again: ";
				cin>>quantity;
			}
			
			//add the quantity with orderedQuantity
			orderedQuantity[numOfOrder][choice-1]+=quantity;
	
	//execute this if user havent order this food
	}else{
		//ask user how many quantity to order
		cout<<"quantity? ";
		cin>>quantity;
		
		//input validation for quantity entered
		while(quantity<=0){
			cout<<"Quantity entered Invalid, enter again: ";
			cin>>quantity;
		}
		
		//add order to respective arrays
		orderedQuantity[numOfOrder][choice-1]=quantity;
		orderedName[numOfOrder][choice-1]=menu[choice-1];
		orderedPrice[numOfOrder][choice-1]=price[choice-1];
	}
	
	//increment number of things user had ordered
	++numberOfCustomerOrdered;
	
	//go back to customer()
	customer();
	}
	
}

/*
What this function do? : 
	This function will do following things:
		1. Write all menu and orders into file, using specified format
		2. Close program after finished
*/
void saveToFile(){
	
	//open file
	ofstream outFile;
	outFile.open("restaurant.txt");
	
	//write all menu into file, using specified format
	for(int i=0; i<numOfMenu; i++){
		outFile<<"(menu):"<<menu[i]<<"\t"<<price[i]<<endl;
	}
	
	//write all orders into file, using specified format
	for(int i=0; i<numOfOrder; i++){
		outFile<<"(order):"<<customerDetail[i][0]<<"\t"<<customerDetail[i][1]<<"\t"<<customerDetail[i][2]<<endl;
		for(int j=0; j<numOfMenu; j++){
			if(orderedQuantity[i][j]>0){
				outFile<<orderedQuantity[i][j]<<"-"<<orderedName[i][j]<<"\t"<<orderedPrice[i][j]<<endl;
			}
		}
		outFile<<"0"<<endl;
	}
	
	//print Save File and System Close Page header 
	cout<<"\n-----------------------------------------------------------------\n";
	cout<<"|\t\t\t"<<left<<setw(40)<<"Save to File & Close System";
	cout<<"|\n-----------------------------------------------------------------\n"; 
	cout<<"Thanks for using our system. File has been saved. \nShutting Down......\n\n";
	
	//exit the program
	exit(0);
}

/*
What this function do? : 
	This function will do following things:
		1. Print all restaurant menu using array loaded from file
*/
void printMenu(){
	//print Restaurant Menu Page header
	printHeader("Restaurant Menu");
	cout<<endl;
	
	//print all menu stored in array using loop 
	for(int i=0; i<numOfMenu; i++){
		cout<<" ( "<<i+1<<" ) "<<left<<setw(15)<<menu[i]<<"--------- RM "<<price[i]<<endl;
	}
	
	cout<<endl;
}


/*
What this function do? : 
	This function will do following things:
		1. Determine type of action for manager update page.
		2. Go to respective function.
*/
void updateMenu(){
	//print Update Page header
	printHeader("Update/Change Menu");
	cout<<"Please enter a number: \n\n ( 1 ) Update Price\n ( 2 ) Change Name\n ( 3 ) Add New Menu\n ( 4 ) Delete a Menu\n\n\n\n ( 00 ) Go Back to Manager Menu\n ( 88 ) View Restaurant Menu\n ( 99 ) Save to File & Close System\n\n\nChoice: ";
	int choice;
	cin>>choice;
	
	//input validation for choice entered
	while(choice!=1&&choice!=2&&choice!=3&&choice!=4&&choice!=99&&choice!=88&&choice!=0){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 	
	cin.ignore();
	
	//go to respective functions based on choice entered
	if(choice==1) updatePrice();
	else if(choice==2) updateName();
	else if(choice==4) deleteMenu();
	else if(choice==99) saveToFile();
	else if(choice==0) manager();
	else if(choice==88){
		printMenu();
		updateMenu();
	} 
	else addMenu();
}

/*
What this function do? : 
	This function will do following things:
		1. Print all restaurant menu.
		2. Ask user to select one to delete
		3. Delete menu selected from arrays
*/
void deleteMenu(){
	
	//print Delete Page header
	printHeader("Delete Menu");
	cout<<"Please enter a number to delete: \n\n";
	
	//print all restaurant menu and ask user to select one to delete
	for(int i=0; i<numOfMenu; i++){
		cout<<" ( "<<i+1<<" ) "<<menu[i]<<"--"<<price[i]<<endl;
	}
	cout<<"\n\n\n( 00 ) Return to Update Menu\n\n\n";
	cout<<"Choice: ";
	int choice;
	cin>>choice;
	
	//input validation for choice entered
	while((choice<1||choice>numOfMenu)&&choice!=0){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	
	//go back to updateMenu() if user wants to
	if(choice==0) updateMenu();
	
	//ask user confirmation to delete
	cout<<"sure?(y/n) ";
	char ans;
	cin>>ans;
	
	//input validation
	while(ans!='y'&&ans!='n'){
		cout<<"Answer entered Invalid, enter again: ";
	}	cin>>ans;
	
	//delete menu from arrays if user confirmed
	if(ans=='y'){
		for(int i=choice-1; i<numOfMenu-1; i++){
			menu[i]=menu[i+1];
			price[i]=price[i+1];
		}
		--numOfMenu;
	}
	
	//go back to updateMenu()
	updateMenu();
}

/*
What this function do? : 
	This function will do following things:
		1. Print all restaurant menu.
		2. Ask user to select one to change name
		3. Change menu name selected from arrays
*/
void updateName(){
	//print Change Name Page header
	printHeader("Change Name");
	
	//user select one menu to change name
	cout<<"Please enter a number to change name: \n\n";
	for(int i=0; i<numOfMenu; i++){
		cout<<i+1<<". "<<menu[i]<<"--"<<price[i]<<endl;
	}
	cout<<"\n\n\n( 00 ) Return to Update Menu\n\n\n";
	cout<<"Choice: ";
	int choice;
	cin>>choice;

	//input validation for choice entered
	while(choice!=0&&(choice<1||choice>numOfMenu)){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	
	cin.ignore();	
	
	//go back to updateMenu() if user wants to
	if(choice==0) updateMenu();
	
	//ask user to enter new name
	cout<<"name? ";
	string newName;
	getline(cin, newName, '\n');
	
	//input validation for name entered
	while(newName == ""){
		cout<<"Name entered Invalid, enter again: ";
		getline(cin, newName, '\n');
		
	} 
	//change menu name to new name in array
	menu[choice-1]=newName;
	
	//go back to updateMenu()
	updateMenu();
}

/*
What this function do? : 
	This function will do following things:
		1. Print all restaurant menu.
		2. Ask user to select one to change price
		3. Change menu price selected from arrays
*/
void updatePrice(){
	//print Change Price Page header
	printHeader("Update Price");
	
	//user select one menu to change price
	cout<<"Please enter a number to update price: \n\n";
	for(int i=0; i<numOfMenu; i++){
		cout<<i+1<<". "<<menu[i]<<"--"<<price[i]<<endl;
	}
	cout<<"\n\n\n( 00 ) Return to Update Menu\n\n\n";
	cout<<"Choice: ";
	int choice;
	cin>>choice;
	
	//input validation for choice entered
	while(choice!=0&&(choice<1||choice>numOfMenu)){
		cout<<"Choice entered Invalid, enter again: ";
		cin>>choice;
	} 
	
	//go back to updateMenu() if user wants to
	if(choice==0) updateMenu();
	
	//ask user to enter new price
	cout<<"price? ";
	double newPrice;
	cin>>newPrice;
	
	//input validation for price entered
	while(newPrice <= 0.0){
		cout<<"Price entered Invalid, enter again: ";
		cin>>newPrice;
	} 
	
	//change menu price to new price in array
	price[choice-1]=newPrice;
	
	//go back to updateMenu()
	updateMenu();
}

/*
What this function do? : 
	This function will do following things:
		1. Allow user to add new menu and price
		2. Store new menu into arrays
*/
void addMenu(){
	//print Add New Menu Page header
	printHeader("Add New Menu");
	cout<<"Please enter: ";
	cout<<"\n\n ( 00 ) Return to Update Menu\n\n\n";
	
	//check whether numOfMenu exceed 100 (maximum number) or not
	if(numOfMenu<100){	
	
		//enter new menu name
		string newName;
		cout<<"What is Menu name? ";
		getline(cin,newName,'\n');

		//go back to updateMenu() if user want to
		if(newName=="00") updateMenu();
	
		//input validation for name
		while(newName == ""){
			cout<<"Name entered Invalid, enter again: ";
			getline(cin, newName, '\n');
		} 
		//enter new menu price
		cout<<"What is the price? ";
		double newPrice;
		cin>>newPrice;
	
		//go back to updateMenu() if user want to
		if(newPrice==0) updateMenu();
	
		while(newPrice < 0.0){
			cout<<"Price entered Invalid, enter again: ";
			cin>>newPrice;
		} 
	
		//add new menu name and price into arrays
		menu[numOfMenu]=newName;
		price[numOfMenu]=newPrice;
	
		//increment numOfOrder
		++numOfMenu;
	
	//execute this if numOfMenu exceed 100 (maximum)
	}else{
		cout<<"Sorry...Number of menu already reach maximum. Please delete some to continue...\n\n";
	}
	
	//go back to updateMenu()
	updateMenu();
}

/*
What this function do? : 
	This function will do following things:
		1. Receive an int as parameter as index
		2. Calculate total amount of an order based on index in parameter
		3. Return value to function that called this function
*/
double calcTotalPerOrder(int choice){
	double totalPerOrder=0;
	//calculate total amount of an order
	for(int k=0; k<100; k++){
			if(orderedQuantity[choice-1][k]!=0){
				totalPerOrder+=orderedPrice[choice-1][k]*orderedQuantity[choice-1][k];
			}
		}
	
	//return to function that called this function
	return totalPerOrder;
}
