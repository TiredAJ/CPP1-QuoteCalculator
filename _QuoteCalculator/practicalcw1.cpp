#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
//#include <Windows.h>
using namespace std;

const string FileLoc = "floorscustomer.txt";	//file name
const string CurrencySym = "\x9C";	//£			//currency symbol
//the handle for Graphics() - UNUSED
//another note, as codegrade is linux based, the Windows.h header 
//is commented out so that the CG compiler runs
//const HANDLE ConsControl = GetStdHandle(STD_OUTPUT_HANDLE);
const int SC_Option = 0;	//for ScreenCleanerTM

//prototypes
//void Graphics(int Option);
void ScreenCleanerTM(int Option);
string CommaRemover(string Input, int SpacesQ);

struct Address
{//to handle the customer's address
	string HouseNum = "#";
	string StreetName = "## St.";
	string City = "##";
	string County = "##";
	string Postcode = "##";

	//displays address to the console
	void DisplayAddress()
	{
		string FormatLine;
		cout << "-Address" << FormatLine.assign(47, '-') << endl;
		cout << "\t" << HouseNum << ", " << StreetName << endl << "\t" << City << endl;
		cout << "\t" << County << endl << "\t" << Postcode << endl;
		cout << FormatLine.assign(55, '-') << endl;
		//Graphics(0);
	}

	//returns the address as a string (file writing)
	string GetAddrs()
	{
		string TempString;
		TempString = HouseNum + ", " + StreetName + ", " + City + ", ";
		TempString += County + ", " + Postcode;

		return TempString;
	}

	//sets address (file reading)
	void FillAddr(string Block)
	{	//block is a string of data taken from the file
		int Commas[4], Counter = 0;

		for (int i = 0; i < Block.size(); i++)		//saves the locations of the commas
		{
			if (Block[i] == ',')
			{
				Commas[Counter] = i;
				Counter++;
			}
		}

		//uses the comma locations to cut text out in one chunk
		//could use CommaRemover here if I hadn't forgotten about it
		HouseNum = Block.substr(0, Commas[0]);
		StreetName = Block.substr(Commas[0] + 2, (Commas[1] - (Commas[0] + 2)));
		City = Block.substr(Commas[1] + 2, (Commas[2]- (Commas[1]+2)));
		County = Block.substr(Commas[2] + 2, (Commas[3] - (Commas[2]+2)));
		Postcode = Block.substr(Commas[3] + 2);
	}

	//when creating a customer, each piece of their address in inputted
	//individually with an accompanying index
	void AddPiece(string Value, int Index)
	{	
		switch (Index)
		{
		case 0:
			HouseNum = Value;
			break;
		case 1:
			StreetName = Value;
			break;
		case 2:
			City = Value;
			break;
		case 3:
			County = Value;
			break;
		case 4:
			Postcode = Value;
			break;
		default:
			cout << "Invalid index" << endl;
			break;
		}
	}

	//clears the stored address
	void Clear()
	{
		HouseNum = "#";
		StreetName = "## St.";
		City = "##";
		County = "##";
		Postcode = "##";
	}
};

struct Material
{//to handle the different types of materials
	string Name = "Default Material";
	double Cost = 0.0;
	double InstCost = 0.0;
	int SizeUnit = 0;

	//to create a new material
	void CreateMaterial(string Name, double Cost, double InstCost, int SizeUnit)
	{	
		//can be used for custom materials in the future
		this->Name = Name;
		this->Cost = Cost;
		this->InstCost = InstCost;
		this->SizeUnit = SizeUnit;
	}

	//for displaying material details to the screen
	void DisplayMaterial()			//necessary?	perhaps
	{
		string FormatLine;
		cout << Name << " " << FormatLine.assign(47, '-');
		cout << "cost: " << CurrencySym << Cost << "Per m^2" << endl;
		cout << "Installation: " << CurrencySym << InstCost;
		cout << "per " << SizeUnit << "m^2" << endl;
	}
};

struct Shape
{//struct to handle shapes that make up a floor
	double LengthA;
	double LengthB;
	double Area;

	//base constructor
	Shape()
	{
		LengthA = 0.0;
		LengthB = 0.0;
	}

	//takes length inputs
	void SetLengths(double LenA, double LenB)
	{
		LengthA = LenA;
		LengthB = LenB;
		Area = (LengthA * LengthB);
	}

	//takes length and area inputs
	void SetLengths(double LenA, double LenB, double Area)
	{
		LengthA = LenA;
		LengthB = LenB;
		this->Area = Area;
	}
};

class Floor
{//this handles an individual floor, which can be made up of 1+ *shapes*
public:
	//default constructor
	Floor()
	{
		Area = 0;
		FloorTypes[0].CreateMaterial("Carpet", 5, 50, 5);
		FloorTypes[1].CreateMaterial("Wood", 10, 75, 5);
		FloorTypes[2].CreateMaterial("Tiled", 15, 100, 5);
	}

	//parameterised constructor
	Floor(double LenA, double LenB, double Area)
	{
		Shape TempShape;
		TempShape.SetLengths(LenA, LenB, Area);

		Shapes.push_back(TempShape);
	}

	//creates a new shape for the floor
	void AddShapes()
	{
		double TempLenA, TempLenB;
		string Command;
		do
		{
			cout << "Please enter the first length" << endl << "> ";	//obtains x and y
			cin >> TempLenA;
			cout << "Please enter the second length" << endl << "> ";
			cin >> TempLenB;

			TempShape.SetLengths(TempLenA, TempLenB);
			Shapes.push_back(TempShape);		//adds shapes to the vector

			//for repeating and adding multiple shapes
			cout << endl << "would you like to add another shape? [yes/no]" << endl << "> ";
			cin >> Command;

		} while (Command != "no" || Command == "yes");

		UpdateTotalArea();		
	}

	//for reading from a file
	void AddShapes(string Block)
	{	
		int Pos; double TempLenA, TempLenB, TempArea;
		Pos = Block.find(',');					//it find the comma
		TempLenA = stod(Block.substr(0, Pos));	//substr's up to the comma
		Block = Block.substr(Pos + 2);			//substr's after the comma (and space)
												//could probably use CommaRemover()
		Pos = Block.find(',');
		TempLenB = stod(Block.substr(0, Pos));
		Block = Block = Block.substr(Pos + 2);

		TempArea = stod(Block);		//area is the last bit left of the block

		TempShape.SetLengths(TempLenA, TempLenB, TempArea);	//creates the shape
		Shapes.push_back(TempShape);	//adds to the vector of shapes
		UpdateTotalArea();		//updates the total area of the floor
	}

	//calculates the total area of all shapes
	void UpdateTotalArea()
	{
		Area = 0;				//forgot about this. resets the area value
		for (auto X : Shapes)	//goes through all the shapes in <Shapes>
		{
			if (X.Area != -1)
			{
				Area += X.Area;	//and adds to the area
			}
		}
	}

	//displays the available material options and allows the user to choose from them
	void SetMaterials()
	{
		int Command;

		cout << "New Floor" << endl << "Please select a floor material [1-3]" << endl;	//"table" headings
		cout << setw(15) << left << "  Name" << setw(10) << left << "Cost";
		cout << setw(15) << "Installation" << endl;

		for (int i = 0; i < 3; i++)	//retreives and displays all material details
		{
			cout << i + 1 << ")" << setw(13) << left << FloorTypes[i].Name << CurrencySym << setw(9);
			cout << left << FloorTypes[i].Cost << CurrencySym << FloorTypes[i].InstCost;
			cout << " " << FloorTypes[i].SizeUnit << "m^2" << endl;
		}
		cout << "> ";

		do			//checks input is within bounds
		{
			cin >> Command;
		} while ((Command-1) > 3 || (Command-1) < 0);

		ChosenMaterial = FloorTypes[Command - 1];

		//cout << endl << "DEBUG::ChosenMaterial=" << ChosenMaterial.Name << endl;		
	}

	//sets the material
	void SetMaterials(Material Input)
	{
		ChosenMaterial = Input;
	}

	//sets the material (file reading)
	void SetMaterials(string Input)
	{	
		if (Input == "Carpet")
		{
			ChosenMaterial = FloorTypes[0];
		}
		else if (Input == "Wood")
		{
			ChosenMaterial = FloorTypes[1];
		}
		else if (Input == "Wood")
		{
			ChosenMaterial = FloorTypes[2];
		}
		else
		{
			cout << endl << "error" << endl;
		}
	}

	//used for displaying material options
	void DisplayMaterials()
	{
		GetTotalArea();	//updates total area

		double MatCost, InstCost;

		cout << "Demo Floor: " << Area << "m^2..." << endl;

		for (int i = 0; i < 3; i++)
		{			
			cout << "In " << FloorTypes[i].Name << ": " << endl;
			//material cost
			MatCost = Area * FloorTypes[i].Cost;
			cout << setw(24) << left << "\t Materials: " << CurrencySym;
			cout << MatCost << endl;

			//installation cost
			InstCost = (Area / FloorTypes[i].SizeUnit) * FloorTypes[i].InstCost;
			cout << setw(24) << left << "\t Installation: " << CurrencySym;
			cout << InstCost << endl;

			//total cost
			cout << setw(24) << left << "\t Total: " << CurrencySym;
			cout << (MatCost + InstCost) << endl;
		}
	}
	
	//displays all the shapes in a floor
	void DisplayShapes()
	{
		//Graphics(0);

		for (int i = 0; i < Shapes.size(); i++)
		{
			cout << "\t" << Shapes[i].LengthA << "m x " << Shapes[i].LengthB << "m";
			cout << " = " << Shapes[i].Area << "m^2" << endl;
		}
		cout << endl;
	}
	
	//deletes shapes
	void ClearShapes()
	{
		Shapes.clear();
	}

	//for writing to a file
	string GetWholeFloorDetails()
	{
		string TempString;

		//*acts* like a CSV
		TempString += (to_string(Shapes.size()) + ", " + ChosenMaterial.Name + "\n");

		for (int i = 0; i < Shapes.size(); i++)
		{
			TempString += to_string(Shapes[i].LengthA) + ", " + to_string(Shapes[i].LengthB) + ", ";
			TempString += to_string(Shapes[i].Area) + "\n";
		}

		return TempString;
	}

	//gets number of lines for file writing
	int GetLines()
	{
		return Shapes.size();
	}

	//returns the total area
	double GetTotalArea()
	{return Area;}
	
	//returns the chosen material
	Material GetMaterial()
	{return ChosenMaterial;}

private:
	double Area;
	Shape TempShape;
	vector <Shape> Shapes;
	Material FloorTypes[3], ChosenMaterial;
};

class Customer
{//class handles individual customer details
public:
	Customer()
	{CustID = "DB#####";};

	//prompts user to make a new customer
	void CreateCustomer()
	{
		string TempString;

		CustID = "DB#####";		//DEBUG

		//in a later revision, I'd like to automate the creation of Customer IDs
		//as the names break if anything less than 7 characters are used for IDs
		cout << "Please enter an ID code for the Customer" << endl;
		do
		{
			cout << "(7 characters)" << endl << "> ";
			getline(cin >> ws, TempString);

		} while (TempString.size() < 7);
				
		CustID = TempString;
		TempString.clear();

		//rest is just obtaining customer details
		cout << "New Customer" << endl << "Please enter the customer's name" << endl << "> ";
		getline(cin >> ws, TempString);
		CustName = TempString;
		TempString.clear();

		cout << "please enter the customer's phone number (no spaces)" << endl << "> ";
		getline(cin >> ws, TempString);
		CustPhone = TempString;
		TempString.clear();

		//this is where AddPiece from Address is used
		for (int i = 0; i < 5; i++)
		{
			cout << "Please enter the customer's " << AddrPieces[i] << endl << "> ";
			getline(cin >> ws, TempString);
			CustAddr.AddPiece(TempString, i);
			TempString.clear();
		}		
	}

	//block for file reading
	void CreateCustomer(string Block)
	{	
		int Pos;

		CustID = Block.substr(0, 7);	//splits block up

		Block = Block.substr(9);

		Pos = Block.find(',');
		CustName = Block.substr(0, Pos);
		Block = Block.substr(Pos + 2);

		Pos = Block.find(',');
		CustPhone = Block.substr(0, Pos);
		Block = Block.substr(Pos + 2);

		CustAddr.FillAddr(Block);
	}

	//for displaying cust details to screen
	void DisplayCustdetails()
	{
		//Graphics(1);
		cout << "Customer: " << CustID << endl << CustName << endl << CustPhone << endl;
		CustAddr.DisplayAddress();
		//Graphics(3);
	}

	//function junction	this is used for file writing
	string GetCustDetails()
	{
		string TempString;
		TempString = CustID + ", " + CustName + ", " + CustPhone + ", " + CustAddr.GetAddrs();
		return TempString;
	}
	
	//returns the number of lines needed for customer details
	int GetLines()
	{return 1;}

	//clears the customer
	void Clear()
	{	
		CustID = "DB#####";
		CustName = "";
		CustAddr.Clear();
		CustPhone.clear();
	}

private:
	string CustID;
	string CustName;
	string CustPhone;	//11 digits
	string AddrPieces[5] = {"House No./name (no commas)", "Street Name", "City", "County", "Postcode"};
	Address CustAddr;
};

class Job
{//Job class ties customers to floors. One job has a customer and 1+ floors
public:
	Job()
	{TotalCost = 0.0;}

	//creates a new job & customer
	void CreateJob()
	{
		string Command;

		//creates a customer beforehand
		Cust.CreateCustomer();

		do		//goes through and adds new floors (and consequently, shapes)
		{
			AddFloor();

			cout << endl << "Would you like to add another floor? [yes/no]" << endl << "> ";
			cin >> Command;

		} while (Command != "no" || Command == "yes");
		
	}

	//for handling file reading
	void CreateJob(string ID, string Block)
	{	
		int NewlinePos, NoFloors, NewlineCounter = 0, ShapeCounter;
		string CustBlock, FloorBlock, TempBlock;

		JobID = ID;	//ID is already read

		NewlinePos = Block.find('\n');	//cuts at newlines for a large part

		CustBlock = Block.substr(0, NewlinePos);	//seperates the customer details from
		FloorBlock = Block.substr(NewlinePos);		//floor details

		Cust.CreateCustomer(CustBlock);		//sends off the Customer block

		NoFloors = FloorBlock[1] - '0';		//takes the first character as the number of floors
		FloorBlock = FloorBlock.substr(3);	//removes ^ and the trialing comma and WS
		
		//stuck here for ages
		for (int i = 0; i < NoFloors; i++)
		{
			ShapeCounter = FloorBlock[0] - '0';		//counds the number of shapes in a floor

			for (int j = 0; j < FloorBlock.size(); j++)
			{	//this loop goes through every shape in a floor by keeping track of how many
				//newlines it encounters in respect to how many it should be expecting
				if (FloorBlock[j] == '\n')
				{
					NewlineCounter++;
				}
				if (NewlineCounter == ShapeCounter + 1)
				{
					AddFloor(TempBlock);		//Sends off the temporary floor
					NewlineCounter = 0;			//resets the newline counter
					TempBlock.clear();
					NewlinePos = j;				//sets the position to the end of the last shape
					break;
				}
				TempBlock += FloorBlock[j];		//adds character [j] to TempFloor
			}
			FloorBlock = FloorBlock.substr(NewlinePos + 1);	//substr's the Floor block to remove
															//the last floor
		}
	}

	//for creating a new job *without* a floor
	void CreateJob(string Input)
	{	
		JobID = to_string(stoi(Input) + 1);

		while (JobID.size() < 7)	//ensures the ID has  digits
		{
			JobID = "0" + JobID;
		}

		TotalCost = 0.0;
		Cust.CreateCustomer();	//creates a new customer	
	}

	//for creating a job for an existing customer
	void CreateJob(Customer Input)
	{
		string Command;

		Cust = Input;	//existing customer

		do
		{
			AddFloor();

			cout << endl << "Would you like to add another floor? [yes/no]" << endl << "> ";
			cin >> Command;

		} while (Command != "no" || Command == "yes");

	}

	//for throwing job details onto the console
	void DisplayJobDetails()
	{	
		string FormatLine;

		//Graphics(2);
		cout << "JobID: " << JobID << endl;
		//Graphics(3);

		Cust.DisplayCustdetails();

		//Graphics(3);

		cout << endl << "No. Floors: " << Floors.size() << endl;

		for (int i = 0; i < Floors.size(); i++)
		{	//goes through and shows the materials and shapes for each floor
			cout << "Floor " << i + 1 << " : " << Floors[i].GetMaterial().Name << " (";
			cout << Floors[i].GetTotalArea() << "m^2)" << endl;
			Floors[i].DisplayShapes();
		}

		GetTotalCost();

		cout << "Total cost: " << CurrencySym << TotalCost << endl;
	}

	//displays customer details on the console
	void DisplayCustomerDetails()
	{	
		Cust.DisplayCustdetails();
		GetTotalCost();

		cout << "Total cost: " << CurrencySym << TotalCost << endl;
	}

	//creates a new floor
	void AddFloor()
	{
		Floor TempFloor;

		ScreenCleanerTM(SC_Option);

		TempFloor.SetMaterials();
		
		TempFloor.AddShapes();
		Floors.push_back(TempFloor);		
	}

	//for reading from a file
	void AddFloor(string Block)
	{	
		Floor TempFloor;
		int NoShapes, Pos;

		NoShapes = Block[0] - '0';	//takes the number of shapes
		Block = Block.substr(3);	//removes ^ and trailing comma and WS
		Pos = Block.find('\n');		//finds the newline
		
		//sets material to a substr of the block upto the newline ^
		TempFloor.SetMaterials(Block.substr(0, Pos));

		Block = Block.substr(Pos + 1); //removes newline & material

		for (int i = 0; i < NoShapes; i++)	//goes through and adds each shape
		{
			Pos = Block.find('\n');
			TempFloor.AddShapes(Block.substr(0, Pos));
			Block = Block.substr(Pos + 1);
		}
		Floors.push_back(TempFloor);	//adds the temporary floor to the list of floors
	}

	//displays floors in different materials
	void MaterialOptions()
	{

		if (Floors.size() < 1)
		{
			cout << "This customer has no floors" << endl;

		}
		else
		{
			for (int i = 0; i < Floors.size(); i++)
			{
				cout << "Floor " << i + 1 << endl;
				Floors[i].DisplayMaterials();
				cout << endl;
			}
		}		
	}

	//allows the user to edit a customer's details
	void EditCust()
	{
		ScreenCleanerTM(SC_Option);

		Customer TempCust;
		
		//shows current details
		cout << "Current customer details: " << endl;
		Cust.DisplayCustdetails();

		//and allows the user to make a new account
		TempCust.CreateCustomer();

		//this overrides the data previously stored
		Cust = TempCust;
	}

	//allows the user to edit a floor
	void EditFloor()
	{	//alike editing a customer, it makes a temporay
		//then replaces the original.
		string Command, FormatLine; Floor TempFloor;
		Material TempMaterial;

		ScreenCleanerTM(SC_Option);

		//presents the job's floors
		for (int i = 0; i < Floors.size(); i++)
		{
			cout << i << ")";
			Floors[i].DisplayShapes();
		}
				
		cout << "which floor would you like to edit? [0-" << Floors.size()-1 << "]";
		cout << endl << "> ";
		cin >> Command;

		ScreenCleanerTM(SC_Option);

		//displays current shapes
		Floors[stoi(Command)].DisplayShapes();
		
		//takes material of the original
		TempFloor.SetMaterials(Floors[stoi(Command)].GetMaterial());

		cout << FormatLine.assign(55, '=') << endl;

		//making new shapes for the new floor
		TempFloor.AddShapes();

		//overwrites old floor with new floor
		Floors[stoi(Command)] = TempFloor;

		//re-calculates total
		CalculateTotal();	
	}

	//retrieves the total cost of a job
	double GetTotalCost()
	{	
		if (TotalCost < 1.0)	//the if statement is to double-check the total
		{						//cost. It can be let off as some jobs don't
			CalculateTotal();	//have floors, just an ID and customer
			return TotalCost;
		}
		else
		{
			return TotalCost;
		}		
	}

	//returns the customer's details (file writing)
	string GetCustomerDetails()
	{return(Cust.GetCustDetails());}

	//returns the details of all floors (file writing)
	string GetAllFloorDetails()
	{
		string TempString;

		TempString += (to_string(Floors.size()) + "\n");	//marks no. of floors

		for (int i = 0; i < Floors.size(); i++)
		{
			TempString += Floors[i].GetWholeFloorDetails();	//should obtain chosen
		}										//materials as well as all the shapes
		
		return TempString;
	}

	//returns ID (file writing)
	string GetID()
	{return JobID;}

	//this returns the total amount of lines needed for a job
	int GetLines()
	{
		//this includes all the customer details, floors and their
		//shapes, materials etc.
		//in a later version I'd like to save a quoted price as I
		//forgot to include it
		int TempInt = 1;	//+1 for JobID
		TempInt += Cust.GetLines();

		for (int i = 0; i < Floors.size(); i++)
		{
			TempInt += (Floors[i].GetLines() + 1);		//+1 for the floor itself
		}

		return TempInt;
	}

	//returns how many floors are included in the job
	int GetNoFloors()
	{return Floors.size();}

	//erases the details of the job
	void Clear()
	{
		Floors.clear();
		Cust.Clear();
		JobID = "#######";
		TotalCost = 0.0;
	}

	//returns customer details as a customer object
	Customer GetCust()
	{return Cust;}
private:
	string JobID = "#######";
	Customer Cust;
	vector <Floor> Floors;
	double TotalCost;

	//calculates a job's total
	void CalculateTotal()
	{
		double TempCost = 0;
		Material TempMaterial;

		for (int i = 0; i < Floors.size(); i++)
		{	//loop goes through each floor in the list, retrieves it's area
			//then caclulates the cost
			TempMaterial = Floors[i].GetMaterial();

			TempCost += Floors[i].GetTotalArea() * TempMaterial.Cost;
			TempCost += (Floors[i].GetTotalArea() / TempMaterial.SizeUnit) * TempMaterial.InstCost;
		}
		TotalCost = TempCost;
	}
};

class Archive
{	//Archive allows the user to view all current/past jobs.
	//Also handles I/O
public:
	//overwrites the file with whatever's currently stored in JobsList
	void WriteAllJobs()
	{
		string TempString;
		
		/*The way this operates, it creates a long string that contains 
		all the details of one job. It calls the Job, Floor and Customer
		objects to return respective strings, containing their stored data
		I had tried another method, but I wasn't able to get it to work, 
		it wouldn't write all the details and random bits would be excluded.
		
		in a future refinement, I'd like to pass the writer object itself
		and have the objects write to the file*/
		
		//adds how many jobs the program should expect when reading
		TempString += to_string(JobsList.size()) + "\n";

		//goes through every entry in the JobsList vector
		for (int i = 0; i < JobsList.size(); i++)
		{
			TempString += to_string(JobsList[i].GetLines()) + ", ";
			TempString += JobsList[i].GetID() + "\n";
			
			//calls for the string from the Customer object
			TempString += JobsList[i].GetCustomerDetails() + "\n";
			
			//calls for the floor string from the Job object
			TempString += JobsList[i].GetAllFloorDetails();
		}
		WriteJob(TempString);	//Writes it all to the file
	}

	//reads the whole file
	void ReadJobs()
	{
		Job TempJob; int NoLines; string ID, Block, TempBlock;

		Reader.open(FileLoc);

		Reader >> NoJobs;	//firstly nabs the number of jobs to expect

		for (int i = 0; i < NoJobs; i++)
		{	//runs the loop for every job entry in the file
			
			//ensures there's a blank job object to populate
			TempJob.Clear();

			Reader >> NoLines;		//gets how many lines the current job is
			getline(Reader, ID);	//obtains JobID
			ID = CommaRemover(ID, 1);	//original use for CommaRemover

			//creates the job block
			for (int i = 0; i < NoLines; i++)
			{
				getline(Reader, TempBlock);
				Block += TempBlock + "\n";
			}

			TempJob.CreateJob(ID, Block);	//sends the block off
			Block.clear();			//clears the block

			JobsList.push_back(TempJob);	//sends off the temporary job object
		}

		Reader.close();		//closes the reader by default
	}

	//displays all the saved jobs
	void DisplayJobs()
	{
		int ChosenJob;

		do
		{
			//goes through the JobsList vector
			for (int i = 0; i < JobsList.size(); i++)
			{
				cout << FormatLine.assign(55, '=') << endl;
				cout << i << ")" << endl;
				JobsList[i].DisplayJobDetails();
				cout << endl;
			}

			cout << FormatLine.assign(55, '=') << endl;

			cout << "Would you like to edit a job's floor? [yes/no]" << endl << "> ";
			cin >> Command;
			if (Command == "yes")
			{	//provides the option to edit a floor
				cout << "Please enter the index of the job you'd like to edit" << endl << "> ";
				cin >> ChosenJob;

				JobsList[ChosenJob].EditFloor();
			}

			do
			{
				cout << "Would you like to [return] to the menu or [repeat] the operation?";
				cout << endl << "> ";
				cin >> Command;

			} while (Command != "return" && Command == "repeat");

		} while (Command == "repeat");
	}

	//displays all the customers saved in JobsList
	void DisplayCustomers(int Option)
	{	
		int ChosenCust;

		ScreenCleanerTM(SC_Option);

		if (Option == 0)
		{	//option determines whether just the customers should
			//be displayed or if there should be options displayed
			
			for (int i = 0; i < JobsList.size(); i++)
			{
				cout << FormatLine.assign(55, '=') << endl;
				cout << i << ")" << endl;
				JobsList[i].DisplayCustomerDetails();
				cout << endl;
			}
			cout << FormatLine.assign(55, '=') << endl;
		}
		else if (Option == 1)
		{
			do
			{
				for (int i = 0; i < JobsList.size(); i++)
				{
					cout << FormatLine.assign(55, '=') << endl;
					cout << i << ")" << endl;
					JobsList[i].DisplayCustomerDetails();
					cout << endl;
				}
				cout << FormatLine.assign(55, '=') << endl;

				do
				{	//provides the option to sort the displayed customers
					cout << "Would you like the list sorted? [yes/no]" << endl << "> ";
					cin >> Command;
					if (Command == "yes")
					{
						SortJobs();
						break;
					}
				} while (Command != "yes" && Command != "no");

				cout << "Would you like to edit a customer? [yes/no]" << endl << "> ";
				cin >> Command;
				if (Command == "yes")
				{	//provides the option to edit a customer
					cout << "Please enter the index of the customer you'd like to edit" << endl << "> ";
					cin >> ChosenCust;

					JobsList[ChosenCust].EditCust();
				}

				do
				{
					cout << "Would you like to [return] to the menu or [repeat] the operation?";
					cout << endl << "> ";
					cin >> Command;

				} while (Command != "return" && Command != "repeat");
			} while (Command == "repeat");
		}		
	}

	//displays sorted customers
	void DisplayCustomers(vector <Job> Input)
	{
		ScreenCleanerTM(SC_Option);

		for (int i = 0; i < JobsList.size(); i++)
		{
			cout << FormatLine.assign(55, '=') << endl;
			cout << i << ")" << endl;
			Input[i].DisplayCustomerDetails();
			cout << endl;
		}

		cout << FormatLine.assign(55, '=') << endl;
	}

	//for creating a new customer
	void NewCustomer()
	{	
		do
		{
			Job TempJob; string LastJob; int Temp;
			
			//generates a new JobID
			Temp = JobsList.size() - 1;
			LastJob = JobsList[Temp].GetID();
			TempJob.CreateJob(LastJob);	//creates a temp job
			JobsList.push_back(TempJob);	//adds it to the list

			do
			{
				cout << "Would you like to [return] to the menu or [repeat] the operation?";
				cout << endl << "> ";
				cin >> Command;

			} while (Command != "return" && Command != "repeat");
		} while (Command == "repeat");
	}

	//sorts the jobs by total price for display
	void SortJobs()
	{	
		
		//find out what the sort is called, it's just my default
		vector <Job> SortedJobsList; bool Sorted = false;
		Job TempJob;

		SortedJobsList = JobsList;

		do
		{
			for (int i = 0; i < SortedJobsList.size() -1; i++)
			{	//goes through each index and compares it with the next, setting Sorted
				//to false if a swap is needed. If it can run through the whole vector
				//without Sorted being switched to false, the vector is sorted
				if (SortedJobsList[i].GetTotalCost() < SortedJobsList[i + 1].GetTotalCost())
				{
					TempJob = SortedJobsList[i];
					SortedJobsList[i] = SortedJobsList[i + 1];
					SortedJobsList[i + 1] = TempJob;
					Sorted = false;
				}
				else
				{
					Sorted = true;
				}
			}

		} while (Sorted == false);

		DisplayCustomers(SortedJobsList);
	}

	//for making a new job from scratch
	void CreateJob()
	{	
		Job TempJob;
		Customer TempCust;
		char Command; int ChosenCust;

		do
		{
			cout << "Is the job for an existing customer? [y/n]" << endl << "> ";
			cin >> Command;
			if (Command == 'y')
			{
				//allows the user to choose what customer the new job'll be for
				DisplayCustomers(0);
				cout << "Please choose a customer (enter cust ID[0-" << (JobsList.size()-1) << "])" << endl << "> ";
				cin >> ChosenCust;

				//sets the temporary customer to the chosen customer
				TempCust = JobsList[ChosenCust].GetCust();

				//creates a new job with the customer
				TempJob.CreateJob(TempCust);

				//adds it to the JobsList
				JobsList[ChosenCust] = TempJob;
			}
			else if (Command == 'n')
			{
				//creates a new job and customer
				TempJob.CreateJob();
				JobsList.push_back(TempJob);
			}

		} while (Command != 'y' && Command != 'n');		
	}

	//displays prices for floors in different materials
	void MaterialOptions()
	{		
		int ChosenCust; char Command; string StrCommand;
		
		//allows the user to choose between making a demo floor or using 
		//an existing customer's floor(s)
		
		do
		{
			ScreenCleanerTM(SC_Option);

			cout << "Would you like to use an existing customer? [y/n]" << endl << "> ";
			cin >> Command;

			if (Command == 'y')
			{	//allows the user to choose a job 
				for (int i = 0; i < JobsList.size(); i++)
				{
					cout << FormatLine.assign(55, '=') << endl;
					cout << i << ")" << endl;
					JobsList[i].DisplayCustomerDetails();
					cout << endl;
				}
				cout << FormatLine.assign(55, '=') << endl;
				cout << endl << "please enter the index of the chosen customer" << endl << "> ";
				cin >> ChosenCust;

				if (JobsList[ChosenCust].GetNoFloors() < 1)
				{
					cout << "This customer has no floors, please select another" << endl;
				}
				else
				{
					JobsList[ChosenCust].MaterialOptions();
				}			
			}
			else
			{	//allows the user to make a temporary floor
				Floor TempFloor;
				TempFloor.ClearShapes();
				TempFloor.AddShapes();

				TempFloor.DisplayMaterials();
			}

			cout << endl;
			cout << "Would you like to [return] to the menu or [repeat] the operation?";
			cout << endl << "> ";
			cin >> StrCommand;

		} while (StrCommand != "return" && StrCommand == "repeat");


	}
	
	//closes the R/W objects
	void CloseArchive()
	{	
		Writer.close();
		Reader.close();
	}

private:
	vector <Job> JobsList;
	ifstream Reader;
	ofstream Writer;
	int NoLines = 0;
	int NoJobs;
	string FormatLine, Command;

	//writes a specified Job to the file
	void WriteJob(string Input)
	{
		Writer.open(FileLoc);
		Writer << Input;
	}
};

int main()
{
	char Command;
	Archive MainStore;
	MainStore.ReadJobs();

	do
	{	//menu
		ScreenCleanerTM(SC_Option);

		cout << "Hello user" << endl << "What would you like to do?" << endl;
		cout << "\t [V]iew Customers" << endl << "\t [A]dd Customer" << endl;
		cout << "\t View [J]obs" << endl << "\t [C]reate Job" << endl;
		cout << "\t Ex[p]lore material options" << endl << "\t E[x]it Program" << endl << "> ";
		cin >> Command;

		//menu options
		switch (Command)
		{	//I've duplicated the upper and lower case for each character
		case 'V':
		{
			cout << "Loading customers for display..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.DisplayCustomers(1);
			break;
		}
		case 'A':
		{
			cout << "Loading new customer..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.NewCustomer();
			break;
		}
		case 'J':
		{
			cout << "Loading jobs for display..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.DisplayJobs();
			break;
		}
		case 'C':
		{
			cout << "Loading new job..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.CreateJob();
			break;
		}
		case 'P':
		{
			cout << "Loading material options..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.MaterialOptions();
			break;
		}
		case 'X':
		{	//exit case		
			//allows the user to decide whether to save the current data in the program
			cout << "Would you like to save all data? [y/n]" << endl << "> ";
			cin >> Command;
			if (Command == 'y')
			{
				MainStore.WriteAllJobs();
			}
			Command = 'x';
			break;
		}
		case 'v':
		{
			cout << "Loading customers for display..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.DisplayCustomers(1);
			break;
		}
		case 'a':
		{
			cout << "Loading new customer..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.NewCustomer();
			break;
		}
		case 'j':
		{
			cout << "Loading jobs for display..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.DisplayJobs();
			break;
		}
		case 'c':
		{
			cout << "Loading new job..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.CreateJob();
			break;
		}
		case 'p':
		{
			cout << "Loading material options..." << endl;
			ScreenCleanerTM(SC_Option);
			MainStore.MaterialOptions();
			break;
		}		
		case 'x':
		{
			cout << "Would you like to save all data? [y/n]" << endl << "> ";
			cin >> Command;
			if (Command == 'y')
			{
				MainStore.WriteAllJobs();
			}
			Command = 'x';
			break;
		}
		default:
		{
			cout << "Please enter a valid command" << endl;
			break;
		}
		}

	} while (Command != 'x');

	MainStore.CloseArchive();

	//Graphics(1);
}

void ScreenCleanerTM(int Option)
{
	switch (Option)		//Option is globally set using SC_Option
	{					//this allows for either the screen to be cleared
						//or to have a plain "ScreenCleaner TM" message
						//pop-up for debugging
	case 0:
	{
		system("cls");	//ScreenCleaner TM
		break;
	}
	default:
	{	//doesn't use the Graphics() function so I can definitely see the 
		//ugly green, whether Graphics is enabled or not
		//SetConsoleTextAttribute(ConsControl, 45);
		cout << "ScreenCleaner TM" << endl;// << endl;
		//SetConsoleTextAttribute(ConsControl, 15);
		cout << endl;
		break;
	}
	}
}

string CommaRemover(string Input, int Option)
{	//0 to remove commas, 1 to remove commas & WS
	//this function is only used in specific locations
	//and probably would either be removed or improved
	//in a later revision
	string Result;

	switch (Option)
	{
	case 0:
	{
		for (int i = 0; i < Input.length(); i++)	//loops through and re-writes
		{											//the string
			if (Input[i] != ',')					//ignoring commas
			{
				Result += Input[i];
			}
		}
		break;
	}
	case 1:
	{
		for (int i = 0; i < Input.length(); i++)	//same here, but ignoring spaces too
		{
			if (Input[i] != ',' && Input[i] != ' ')
			{
				Result += Input[i];
			}
		}
		break;
	}
	default:
	{

		break;
	}		
	}
	return Result;
}

/*void Graphics(int Option)			//Probably going to have to cut this out
{
	//so, hello.
	//this was going to handle changing the colour (fore/background) and the >
	//borders of text, but in practice, it didn't behave and colours would >
	//leak across lines. Feel free to test it by uncommenting it and any lines >
	//"Graphics(#)" where # is 1, 0 etc.
	switch (Option)
	{
	case 0:
	{
		SetConsoleTextAttribute(ConsControl, 15);
		break;
	}

	case 1:
	{
		SetConsoleTextAttribute(ConsControl, 112);
		break;
	}
	case 2:
	{
		SetConsoleTextAttribute(ConsControl, 17928);
		break;
	}
	case 3:
	{
		SetConsoleTextAttribute(ConsControl, 15);
		break;
	}
	default:
	{
		SetConsoleTextAttribute(ConsControl, 15);
		break;
	}
	}
}*/