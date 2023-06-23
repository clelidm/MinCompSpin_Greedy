#include <iostream>
#include <cmath>       /* tgamma */
#include <map>
#include <iomanip>

using namespace std;

/******************************************************************************/
/******************   TOOL Functions from "tools.cpp"   ***********************/
/******************************************************************************/
unsigned int Bitset_count(__int128_t bool_nb);
string int_to_bstring(__int128_t bool_nb, unsigned int n);

/******************************************************************************/
/*******************************    CONSTANTS     *****************************/
/******************************************************************************/
const __int128_t one128 = 1;

/******************************************************************************/
/**************** Log-likelihood (LogL) of a complete model  ******************/
/******************************************************************************/
// Compute the log-likelihood of a complete model on Kset:
// This function is mainly used for call by `LogL_SC_PartMCM`,
// but can also be used to compute the log-likelihood of a complete model
// Note that over a complete model, the logL is just -N*S_a (-N x entropy of the sub-data)
//
double LogL_CM(map<__int128_t, unsigned int > Kset, unsigned int N)
{
  double LogL = 0;

  map<__int128_t, unsigned int >::iterator it;
  unsigned int Ncontrol = 0; // for control
  unsigned int Ks = 0;
  double Nd = N;

  for (it = Kset.begin(); it!=Kset.end(); ++it)
  {
    Ks = (it->second);  Ncontrol += Ks;
    if (Ks == 0) {cout << "problem Ks = 0 for some mu_m" << endl; }
    LogL += (Ks * log((double) Ks / Nd) );
  }
  if (Ncontrol != N) { cout << "Error in function 'LogLikelihood_SCforMCM': Ncontrol != N" << endl;  }

  return LogL;
}

/******************************************************************************/
/***************************** Log-likelihood (LogL) **************************/
/***********************   of a sub-complete part of a MCM   ******************/
/******************************************************************************/
// Compute the log-likelihood of the sub-complete part (of an MCM) defined by Ai.
// This function could be also used directly by the user
// to compute the log-likelihood of a sub-complete model

double LogL_SubCM(map<__int128_t, unsigned int> Kset, __int128_t Ai, unsigned int N)
{
  map<__int128_t, unsigned int>::iterator it;
  map<__int128_t, unsigned int > Kset_new;

  __int128_t s;        // state
  unsigned int ks=0; // number of time state s appear in the dataset

//Build Kset_new:
  for (it = Kset.begin(); it!=Kset.end(); ++it)
  {
    s = it->first;      // initial state s 
    ks = it->second;    // # of times s appears in the data set

    s &= Ai;   // troncated state: take only the bits indicated by Ai

    Kset_new[s] += ks;
  }

  return LogL_CM(Kset_new, N);
}

/******************************************************************************/
/******************** Log-likelihood (LogL) of a MCM  *************************/
/******************************************************************************/
//check if *Partition* is an actual partition of the basis elements, 
// i.e., that no basis element appears in more than 1 part of the partition.
// i.e., that each basis element only appears in a single part of the partition.
//bool check_partition(map<uint32_t, uint32_t> Partition);

double LogL_MCM(map<__int128_t, unsigned int> Kset, map<unsigned int, __int128_t> Partition, unsigned int N, unsigned int r)
{
  //if (!check_partition(Partition)) {cout << "Error, the argument is not a partition." << endl; return 0;  }

  //else
  //{
    double LogL = 0; 
    unsigned int rank = 0;
    map<unsigned int, __int128_t>::iterator Part;

    for (Part = Partition.begin(); Part != Partition.end(); Part++)
    {
        LogL += LogL_SubCM(Kset, (*Part).second, N);
        rank += Bitset_count((*Part).second);
    }  
    return LogL - ((double) (N * (r-rank))) * log(2.);
  //}
}

double LogL_MCM_infoICC(map<__int128_t, unsigned int> Kset, map<unsigned int, __int128_t> Partition, unsigned int N, unsigned int r)
{
  //if (!check_partition(Partition)) {cout << "Error, the argument is not a partition." << endl; return 0;  }

  //else
  //{
    double LogL = 0, LogL_SCM = 0, LogL_C = 0, eta_a = 0, eta = 0, eta_denominator = 0;
    unsigned int rank = 0, rank_a = 0;
    map<unsigned int, __int128_t>::iterator Part;
    double Nd = (double) N;

    cout << setprecision(3) << fixed;
    // Complete model:
    __int128_t Part_CM = (one128 << r) - 1;
    LogL_C = LogL_CM(Kset, N);
    eta_denominator = LogL_C + ((double) (r * N)) * log(2.);

    cout << "Maximum amount of information that can be extracted = LogL_CM - LogL_0 = ";
    cout << eta_denominator << " = " << eta_denominator/Nd/log(2.) << " bits per datapoint" << endl << endl;

    cout << int_to_bstring(Part_CM, r) << "\t " << LogL_C << "\t " << -LogL_C/Nd/log(2.) << " bits per datapoint" << endl;
    cout << int_to_bstring(0, r) << "\t " << -((double) (r * N)) * log(2.) << "\t " << (int) r << " bits per datapoint" << endl << endl;

    // Each ICC:
    for (Part = Partition.begin(); Part != Partition.end(); Part++)
    {
        // ICC:
        LogL_SCM = LogL_SubCM(Kset, (*Part).second, N);
        rank_a = Bitset_count((*Part).second);
        eta_a = (LogL_SCM + ((double) (rank_a * N)) * log(2.)) / eta_denominator;

        LogL += LogL_SCM;
        rank += Bitset_count((*Part).second);
        eta += eta_a;

        cout << (*Part).first << "\t " << int_to_bstring((*Part).second, r) << "\t " << LogL_SCM << "\t " << -LogL_SCM/Nd/log(2.) << " bits per datapoint; \t";
        cout << "eta_a = " << eta_a << endl;
    }  

    // final LogL(MCM)
    LogL -= ((double) (N * (r-rank))) * log(2.);

    cout << endl;
    cout << "Maximum LogL(MCM) = " << LogL << "\t " << -LogL/((double) N)/log(2.) << " bits per datapoint \t" << endl;
    cout << "Fraction of deviance explained = " << eta << endl;

    return LogL;
  //}
}


