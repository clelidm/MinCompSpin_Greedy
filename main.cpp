// To compile: make
// To run: or: make run
//         or: ./GreedySearch.out data_filename n
// With options:
//      ./GreedySearch.out data_filename n [-b basis_filename] [--full] [--NoCheckPoint]

#define _USE_MATH_DEFINES 
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <vector>
#include <cmath>       /* tgamma */
#include <random>

#include <ctime> // for chrono
#include <ratio> // for chrono
#include <chrono> // for chrono

using namespace std;
using namespace std::chrono;

/*****************************************************************************************/
/*************************   CONSTANT VARIABLES  *****************************************/
/*****************************************************************************************/
#include "includes/default_data.h"

/*****************************************************************************************/
/****************   GREEDY SEARCH:   Useful functions and routines    ********************/
/*****************************************************************************************/
// **** Find the best MCM, Greedy Search:
map<unsigned int, __int128_t> MCM_GreedySearch(vector<pair<__int128_t, unsigned int>> Kset, unsigned int N, unsigned int r, bool print_info = true, bool Greedy_Full_merging = false);
map<unsigned int, __int128_t> MCM_GreedySearch_AND_printInfo(vector<pair<__int128_t, unsigned int>> Kset, unsigned int N, unsigned int r, bool print_info = true, bool Greedy_Full_merging = false);

// **** Find the best MCM, Greedy Search starting from the model MCM_0:
map<unsigned int, __int128_t> MCM_GreedySearch_MCM0(vector<pair<__int128_t, unsigned int>> Kset, unsigned int N, map<unsigned int, __int128_t> MCM_0, bool print_info = true, bool Greedy_Full_merging = false);

// *** Greedy Search on Reduced dataset:
map<unsigned int, __int128_t> MCM_ReducedGreedySearch_AND_PrintInfo(vector<pair<__int128_t, unsigned int>> Kset, unsigned int K, unsigned int N, unsigned int r, bool print_it = false);

// *** Compare two MCMs:
void compare_two_MCMs_AND_printInfo(vector<pair<__int128_t, unsigned int>> Kset, unsigned int N, unsigned int r, map<unsigned int, __int128_t> fp1, map<unsigned int, __int128_t> fp2);


/*****************************************************************************************/
/*****************************   IMPORT an MCM from a FILE   *****************************/
/*****************************************************************************************/
// *** Read MCM from a file:
map<unsigned int, __int128_t> read_MCM_fromfile(string Input_MCM_file, unsigned int r);
map<unsigned int, __int128_t> read_MCM_fromfile_AND_printInfo(vector<pair<__int128_t, unsigned int>> Kset, unsigned int N, string Input_MCM_file, unsigned int r);

/******************************************************************************/
/***************************   ADD OUTPUT FOLDER    ***************************/
/******************************************************************************/

//// ** location of the output folder:
string OutputFile_Add_Location(string filename)
{
    return (OUTPUT_directory + filename);
}

/****************************************************************************************************************************************************************************/
/****************************************************************************************************************************************************************************/
/**************************************************************************   """ TUTORIAL  """    **************************************************************************/
/****************************************************************************************************************************************************************************/
/****************************************************************************************************************************************************************************/

void tutorial(vector<pair<__int128_t, unsigned int>> Nset, unsigned int N,  unsigned int n, bool Greedy_Full_merging = false)
{
    cout << endl << "*******************************************************************************************";  
    cout << endl << "******************************  CHOICE OF THE BASIS:  *************************************";
    cout << endl << "*******************************************************************************************" << endl;

// original basis of the data: this is the most natural choice a priori:
//    list<__int128_t> Basis_li = Original_Basis(n);

//// *** The basis can also be read from a file: Ex. the following files contain the best basis for the SCOTUS dataset:
//   list<__int128_t> Basis_li = Read_BasisOp_IntegerRepresentation(input_directory + basis_IntegerRepresentation_filename);
   list<__int128_t> Basis_li = Read_BasisOp_BinaryRepresentation(n, input_directory + basis_BinaryRepresentation_filename);

    PrintTerm_Basis(Basis_li, n);

    cout << endl << "*******************************************************************************************";
    cout << endl << "**********************  TRANSFORM the DATA in the CHOSEN BASIS   **************************";
    cout << endl << "**********************************   Build Kset:   ****************************************";
    cout << endl << "*******************************************************************************************" << endl;

//// *** Transform the data in the specified in Basis_SCModel[];

    vector<pair<__int128_t, unsigned int>> Kset = build_Kset(Nset, Basis_li);
    cout << "\t Kset.size() = " << Kset.size() << endl;


    cout << endl << "*******************************************************************************************";
    cout << endl << "***********************  HIERARCHICAL GREEDY MERGING: BY STEPS:  **************************";
    cout << endl << "*******************************************************************************************" << endl;

    bool print_checkpoint = true;  

//// *** Finds the best MCM:
    map<unsigned int, __int128_t> fp1 = MCM_GreedySearch(Kset, N, n, print_checkpoint, Greedy_Full_merging);

//// *** Print Log-Evidence:  
    double LogE_fp1 = LogE_MCM(Kset, fp1, N, n);
    cout << "Log-Evidence(MCM) = " << LogE_fp1 << "\t = " << LogE_fp1/((double) N)/log(2.) << " bits per datapoint \t" << endl;

//// *** Print max-Log-Likelihood:  
    double LogL_fp1 = LogL_MCM(Kset, fp1, N, n);
    cout << "Max Log-Likelihood(MCM) = " << LogL_fp1 << "\t = " << LogL_fp1/((double) N)/log(2.) << " bits per datapoint \t" << endl;

    cout << endl << "*******************************************************************************************";
    cout << endl << "******************************  READ an MCM from a FILE   *********************************";
    cout << endl << "*******************************************************************************************" << endl;

    cout << "#########  EX. READ a CHOSEN MCM:  #########" << endl;

    // the file communityfile = "INPUT/SCOTUS_Communities_inBestBasis.dat" contains the best MCM in the best basis:
    map<unsigned int, __int128_t> fp2 = read_MCM_fromfile(communityfile, n);
    Print_MCM_Partition(fp2, n);

    cout << endl << "*******************************************************************************************";
    cout << endl << "*******************************  COMPARING TWO MCMs   *************************************";
    cout << endl << "*******************************************************************************************" << endl;

    compare_two_MCMs_AND_printInfo(Kset, N, n, fp1, fp2);

    cout << endl << "*******************************************************************************************";
    cout << endl << "***************************  Decomposition of Log-E   *************************************";
    cout << endl << "*******************************   over each ICC   *****************************************";
    cout << endl << "*******************************************************************************************" << endl;

    double LogE_final = LogE_MCM_infoICC(Kset, fp1, N, n);
    //cout << "Log-Evidence(MCM) = " << LogE_final << "\t = " << LogE_final/((double) N)/log(2.) << " bits per datapoint \t" << endl;

    cout << endl << "*******************************************************************************************";
    cout << endl << "*************************  Decomposition of Max-Log-L   ***********************************";
    cout << endl << "*******************************   over each ICC   *****************************************";
    cout << endl << "*******************************************************************************************" << endl;

    double LogL_final = LogL_MCM_infoICC(Kset, fp1, N, n);
    //cout << "Max-Log-Likelihood(MCM) = " << LogL_final << "\t = " << LogL_final/((double) N)/log(2.) << " bits per datapoint \t" << endl;

    cout << endl << "*******************************************************************************************";
    cout << endl << "***************************  Working with a Reduced Dataset   *****************************";
    cout << endl << "**********   Remove from Kset all the states that occur less than K times:   **************";
    cout << endl << "*******************************************************************************************" << endl;

    // All the states that occur less than K times will be removed from the dataset:
    unsigned int K=2;
    map<unsigned int, __int128_t> fp_reduced = MCM_ReducedGreedySearch_AND_PrintInfo(Kset, K, N, n);

    cout << endl << "*******************************************************************************************";
    cout << endl << "**********************  Print information about the found MCM:  ***************************";
    cout << endl << "*******************************************************************************************" << endl;

    // Prints 1) information about the MCM; 2) the state probabilities P(s) of observed states (in the Data VS MCM); 3) the probability P(k) of observing a state with k values "+1" (in the Data VS MCM) 
    PrintFile_StateProbabilites_OriginalBasis(Nset, Basis_li, fp1, N, n, "Result");

    // Print the state probabilities P(s) of observed states (in the Data VS MCM) using the data transformed in the bew basis:
    PrintFile_StateProbabilites_NewBasis(Kset, fp1, N, n, "Result");
}


/******************************************************************************/
/*****************************   USER INTERFACE    ****************************/
/******************************************************************************/
struct RunOptions
{
    bool change_basis = false;  // by default: Search in the original basis 
    bool print_checkpoint = true;   // by default: print all the checkpoints
    
    // by default: stop Greedy merging when LogE starts decreasing 
    bool greedy_full_merging = false; // if TRUE: keep on merging until everything is merged; save best MCM along the greedy path
};

void HELP_message();
int Read_argument(int argc, char *argv[], string *datafilename, unsigned int *n, string *basis_filename, RunOptions *options);

/****************************************************************************************************************************************************************************/
/****************************************************************************************************************************************************************************/
/**************************************************************************     MAIN FUNCTION      **************************************************************************/
/****************************************************************************************************************************************************************************/
/****************************************************************************************************************************************************************************/
int main(int argc, char *argv[])
{

    cout << endl << "*******************************************************************************************";
    cout << endl << "************************************  CONFIGURATION:  *************************************";
    cout << endl << "*******************************************************************************************" << endl;

//// *** INITIAL SETTINGS:
    cout << "--->> Create the \"OUTPUT\" Folder (if needed) ";
    system(("mkdir -p " + OUTPUT_directory).c_str());
    cout << endl << endl;

    cout << "--->> Input files are in the input directory: \"" << input_directory << "\"" << endl; 
    RunOptions options;

    if ( !(Read_argument(argc, argv, &datafilename, &n, &basis_filename, &options)) )  {   return 0;   }  // error flag --> quit

    cout << endl << "*******************************************************************************************";
    cout << endl << "***********************************  READ THE DATA:  **************************************";
    cout << endl << "*******************************************************************************************" << endl;

    //cout << "Read the dataset: " << datafilename << endl;
    //cout << "Number of variables to read: n = " << n << endl;

    unsigned int N = 0; // will contain the number of datapoints in the dataset
    vector<pair<__int128_t, unsigned int>> Nset = read_datafile(&N, input_directory + datafilename, n);

    if (N == 0)     // Terminate program if the file can't be found or read, or if it is empty:
        { 
        cout << " --->> Datafile cannot be read or is empty: terminate the program." << endl << endl;
        return 0; 
        }

    cout << endl << "###### Datafile has been read successfully:" << endl;
    cout << "\t Number of datapoints: N = " << N << endl;
    cout << "\t Number of different observed states = " << Nset.size() << endl;

    vector<pair<__int128_t, unsigned int>> Kset;

    if (options.change_basis)
    {
        cout << endl << "*******************************************************************************************";  
        cout << endl << "******************************  CHOICE OF THE BASIS:  *************************************";
        cout << endl << "*******************************************************************************************" << endl;

    //// *** The basis can also be read from a file: Ex. the following files contain the best basis for the SCOTUS dataset:

        list<__int128_t> Basis_li = Read_BasisOp_BinaryRepresentation(n, input_directory + basis_filename);

        if (Basis_li.size() == 0)     // Terminate program if the file can't be found or read, or if it is empty:
            { 
            cout << " --->> Basis file cannot be read or is empty: terminate the program." << endl << endl;
            return 0; 
            }

        cout << endl << "###### Basis file has been read successfully:" << endl;
        PrintTerm_Basis(Basis_li, n);

        cout << endl << "*******************************************************************************************";
        cout << endl << "**********************  TRANSFORM the DATA in the CHOSEN BASIS   **************************";
        cout << endl << "**********************************   Build Kset:   ****************************************";
        cout << endl << "*******************************************************************************************" << endl;

        Kset = build_Kset(Nset, Basis_li);

        if (Kset.size() == 0)     // Terminate program if the file can't be found or read, or if it is empty:
        { 
            cout << " --->> Issue with conversion of the datafile in the new basis:" << endl;
            cout << " \t - Check that provided basis is actually a basis." << endl;
            cout << " \t Terminate the program." << endl << endl;
            return 0; 
        }

        cout << "###### Datafile has been converted successfully:" << endl;
        cout << "\t Number of datapoints: N = " << N << endl;
        cout << "\t Number of different observed states in the new basis = " << Kset.size() << endl;

        cout << endl << "*******************************************************************************************";
        cout << endl << "*****************************  HIERARCHICAL GREEDY MERGING:  ******************************";
        cout << endl << "**********************************  in the NEW BASIS  *************************************";
        cout << endl << "*******************************************************************************************" << endl;
    }
    else
    {
        Kset.swap(Nset);

        cout << endl << "*******************************************************************************************";
        cout << endl << "*****************************  HIERARCHICAL GREEDY MERGING:  ******************************";
        cout << endl << "********************************  in the ORIGINAL BASIS  **********************************";
        cout << endl << "*******************************************************************************************" << endl;
    }

//// *** Finds the best MCM and print information about it in the terminal:
    map<unsigned int, __int128_t> fp1 = MCM_GreedySearch_AND_printInfo(Kset, N, n, options.print_checkpoint, options.greedy_full_merging);

    cout << endl << "*******************************************************************************************";
    cout << endl << "***************************  Decomposition of Log-E   *************************************";
    cout << endl << "*******************************   over each ICC   *****************************************";
    cout << endl << "*******************************************************************************************" << endl;
    LogE_MCM_infoICC(Kset, fp1, N, n);

/*
    cout << endl << "*******************************************************************************************************************";
    cout << endl << "*******************************************************************************************************************";
    cout << endl << "************************************************  TUTORIAL:  ******************************************************";
    cout << endl << "*******************************************************************************************************************";
    cout << endl << "*******************************************************************************************************************" << endl;

    bool greedy_full_merging = false;

    tutorial(Nset, N, n, greedy_full_merging);
*/

    return 0;
}
