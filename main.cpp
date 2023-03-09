#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

//function that greets the user and returns the number of patients that will be entered
int greeting(){
    int numPatients;
    cout << "--------------------------------------------------------------------------------------" << endl
         << "                        Hospital Patient Invoice Generator" << endl
         << "--------------------------------------------------------------------------------------" << endl
         << "How many patient invoices would you like to make?: ";
    cin >> numPatients;
    if(numPatients < 0 || numPatients > 10000){
        cout << "Please enter a number between 1 and 10,000: ";
        cin >> numPatients;
    }
    return numPatients;
}

//function that will put the number of patients
void collect_nameAndID(int index, float** ID, string* names){
        cout << "   Last Name: ";
    cin >> names[index];
    cout << "   ID number: ";
    cin >> ID[index][0];
    cout << endl;
}

//function that will use the hospital costs and calculate the total for room charge
//and the grand total.
void calculate_roomCharges(int index, float** roomCost){
    cout << "Room Charges: Type 1 for Private room, 2 for Semi-private room, and 3 for ICU." << endl;
    int room_type;
    int days;

    bool flag1 = true;
    while(flag1) {
        flag1 = false;
        cout << "   Room type (1, 2, or 3): ";
        cin >> room_type;
        if (room_type != 1 && room_type != 2 && room_type != 3) {
            flag1 = true;
            cout << "Please enter a valid room type option (1, 2, or 3)" << endl;
        }
    }

    bool flag2 = true;
    while(flag2){
        flag2 = false;
        cout << "   Length of Patient's stay (# of days): ";
        cin >> days;
        if(days < 7){
            if(room_type == 1)
                roomCost[index][1] = 525 * (days);
            if(room_type == 2)
                roomCost[index][1] = 412 * (days);
            if(room_type == 3)
                roomCost[index][1] = 877 * (days);
        }else if(days < 366){
            if(room_type == 1)
                roomCost[index][1] = 525 * 6 + 462 * (days - 6);
            if(room_type == 2)
                roomCost[index][1] = 412 * 6 + 362.56 * (days - 6);
            if(room_type == 3)
                roomCost[index][1] = 877 * 6 + 771.76 * (days - 6);
        }else{
            flag2 = true;
            cout << "Please enter a patient stay that is 1 year or less." << endl;
        }
    }
}

//calculates the lab charges for the patient by asking for how many tests they had of each type and
void calculate_labCharges(int index, float** labCosts){
    int numberOfTests;
    cout << endl << "Lab Charges: How many Lab tests did patient #" << index+1 << " get?: ";
    cin >> numberOfTests;
    cout << "Please enter the level of each lab test. (1, 2, or 3)" << endl;

    int level;
    int sumOfLabs = 0;
    for(int i = 0; i < numberOfTests; i++){
        cout << "   Lab Test #" << i+1 << ": ";
        cin >> level;
        if(level == 1)
            sumOfLabs += 125;
        else if(level == 2)
            sumOfLabs += 250;
        else if(level == 3)
            sumOfLabs += 550;
        else{
            cout << "Please enter a valid Lab Test level (1, 2, or 3)" << endl;
            i--;
        }
    }
    labCosts[index][2] = sumOfLabs;
}

//function that will ask for a singular amount covering all medicine costs for their stay and discounting
//the cost by 5% when the patient's room charge subtotal is greater than 4000
void calculate_medicineCharge(int index, float** patientInfo){
    int medicineCharge;
    cout << endl << "Medicine Charges: " << endl;
    cout << "   Cost of medicine for entire stay: ";
    cin >> medicineCharge;

    if(patientInfo[index][1] < 4000)
        patientInfo[index][3] = medicineCharge;
    else
        patientInfo[index][3] = medicineCharge * .95;
}

//function that calculates the grand total for each patient and returns a float
float grandTotal(int index, float** patientInfo){
    float grandTotal = 0;
    for(int j = 1; j < 4; j++){
        grandTotal += patientInfo[index][j];
    }
    return grandTotal;
}

//function that will calculate the total charges in each category for every patient to use in hospital
//output file
void hospitalTotals(ostream& out, float** data, int numPatients, float*& totals){

    for(int i = 0; i < numPatients; i++){
        totals[0] += data[i][1];
        totals[1] += data[i][2];
        totals[2] += data[i][3];
    }
    totals[3] = totals[0] + totals[1] + totals[2];

    out << "Totals of all patient charges: " << endl
        << "--------------------------------------------------------------------------------------" << endl
        << "Grand sub total for all room charges:    $" << fixed << setprecision(2) << totals[0] << endl
        << "Grand sub total for all lab tests:       $" << fixed << setprecision(2) << totals[1] << endl
        << "Grand sub total for all medicine costs:  $" << fixed << setprecision(2) << totals[2] << endl
        << "Grand Total:                             $" << fixed << setprecision(2) << totals[3] << endl;
}

//function that will print out the format of the patient invoice to the terminal and to an external file
void patientInvoices(ostream& out, float** patientInfo, string* patientNames, int i){
    out << "--------------------------------------------------------------------------------------" << endl
        << "                              Patient #" << i+1 << "'s Invoice" << endl
        << "--------------------------------------------------------------------------------------" << endl
        << "Patient: " << endl
        << "   Last Name: " << patientNames[i] << endl
        << "   ID Number: " << fixed << setprecision(0) << patientInfo[i][0] << endl
        << "Subtotals for:" << endl
        << "   Room Charge:    $" << fixed << setprecision(2) << patientInfo[i][1] << endl
        << "   Lab Tests:      $" << fixed << setprecision(2) << patientInfo[i][2] << endl
        << "   Medicine Costs: $" << fixed << setprecision(2) << patientInfo[i][3] << endl
        << "GRAND TOTAL:       $" << fixed << setprecision(2) << grandTotal(i,patientInfo)
        << endl;
}

//function that will create an output file for the hospital totals
void hospFile(int numOfPatients, float** patientInfo, float* hospTotals){
    ofstream hospitalFile("hospital-totals.txt");
    hospitalTotals(hospitalFile, patientInfo, numOfPatients, hospTotals);
    if(!hospitalFile.is_open())
        cout << "Error creating hospital file." << endl;
    hospitalFile.close();
}

//format header in this function
void infoForPatientHeader(int i){
    cout << endl << endl
         << "                           Information for patient #" << i+1 << endl
         << "--------------------------------------------------------------------------------------" << endl;
}

int main() {
    int numOfPatients = greeting();
    string* patientLastNames = new string[numOfPatients];
    float** patientInfo = new float*[numOfPatients];
    for(int i = 0; i < numOfPatients; i++){
        patientInfo[i] = new float[4];
    }

    //for loop to collect the various data for each patient
    for(int i = 0; i < numOfPatients; i++){
        infoForPatientHeader(i);
        collect_nameAndID(i, patientInfo, patientLastNames);
        calculate_roomCharges(i, patientInfo);
        calculate_labCharges(i, patientInfo);
        calculate_medicineCharge(i, patientInfo);
    }

    //for loop to create patient invoices
    for(int i = 0; i < numOfPatients; i++) {
        patientInvoices(cout, patientInfo, patientLastNames, i);
        ofstream patientInvoice(patientLastNames[i] + "-" + to_string(int(patientInfo[i][0])) + ".txt");
        patientInvoices(patientInvoice, patientInfo, patientLastNames, i);
        if (!patientInvoice.is_open())
            cout << "Error creating invoice file." << endl;
        patientInvoice.close();
    }

    float* hospitalTotals = new float[4];
    hospFile(numOfPatients, patientInfo, hospitalTotals); //creates hospital totals file
}
