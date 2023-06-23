#include <iostream>
#include <cmath>       /* lgamma */
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

pair<bool, unsigned int> check_partition(map<unsigned int, __int128_t> Partition);

/******************************************************************************/
/**************** Log-Evidence (LogE) of a sub-complete model  ****************/
/******************************************************************************/
// Compute the log-evidence of a sub-complete model based on m basis elements
// ! Kset must have been previously reduced to these m basis elements !
// This function is mainly used for call by `LogE_PartMCM`,
// but can also be used to compute the log-likelihood of a complete model
//
double LogE_SubC_forMCM(map<__int128_t, unsigned int> Kset, unsigned int m, unsigned int N)
{
  double LogE = 0;

  map<__int128_t, unsigned int>::iterator it;
  unsigned int Ncontrol = 0; // for control
  unsigned int Ks = 0;

  for (it = Kset.begin(); it!=Kset.end(); ++it)
  {
    Ks = (it->second);  Ncontrol += Ks;
    if (Ks == 0) {cout << "problem Ks = 0 for some mu_m" << endl; }
    LogE += lgamma(Ks + 0.5);
  }
  if (Ncontrol != N) { cout << "Error Likelihood function: Ncontrol != N" << endl;  }

//  return LogE - GeomComplexity_SubCM(m) - lgamma( (double)( N + (one128 << (m-1)) ) );
  return LogE + lgamma((double)( one128 << (m-1) )) - (Kset.size()/2.) * log(M_PI) - lgamma( (double)( N + (one128 << (m-1)) ) ); 
}

/******************************************************************************/
/*********  Log-Evidence (LogE) of a sub-complete part of a MCM   *************/
/******************************************************************************/
// Compute the log-evidence of the sub-complete part (of an MCM) defined by Ai.
// This function could be also used directly by the user
// to compute the log-likelihood of a sub-complete model
// Rem: the function compute the LogE as if the space were reduced to the sub-space defined by the model

double LogE_SubCM(map<__int128_t, unsigned int> Kset, __int128_t Ai, unsigned int N)
{
  map<__int128_t, unsigned int>::iterator it;
  map<__int128_t, unsigned int> Kset_new;

  __int128_t s;        // state
  unsigned int ks=0;   // number of times state s appears in the dataset

//Build Kset:
  for (it = Kset.begin(); it!=Kset.end(); ++it)
  {
    s = it->first;      // initial state s 
    ks = it->second;    // # of times s appears in the data set

    s &= Ai;   // troncated state: take only the bits indicated by Ai
    Kset_new[s] += ks;
  }

  return LogE_SubC_forMCM(Kset_new, Bitset_count(Ai), N);
}

/******************************************************************************/
/****************************   LogE of a MCM   *******************************/
/******************************************************************************/

double LogE_MCM(map<__int128_t, unsigned int> Kset, map<unsigned int, __int128_t> Partition, unsigned int N, unsigned int r)
{
  //if (!check_partition(Partition)) {cout << "Error, the argument is not a partition." << endl; return 0;  }

  //else
  //{
    double LogE = 0; 
    unsigned int rank = 0;
    map<unsigned int, __int128_t>::iterator Part;

    for (Part = Partition.begin(); Part != Partition.end(); Part++)
    {
      LogE += LogE_SubCM(Kset, (*Part).second, N);
      rank += Bitset_count((*Part).second);
    }  
    return LogE - ((double) (N * (r-rank))) * log(2.);
  //}
  return 0;
}


double LogE_MCM_infoICC(map<__int128_t, unsigned int> Kset, map<unsigned int, __int128_t> Partition, unsigned int N, unsigned int r)
{
  //if (!check_partition(Partition)) {cout << "Error, the argument is not a partition." << endl; return 0;  }

  //else
  //{
    double LogE = 0, LogE_SCM = 0, LogE_CM = 0, eta = 0; 
    unsigned int rank = 0;
    map<unsigned int, __int128_t>::iterator Part;
    double Nd = (double) N;

    cout << setprecision(3) << fixed;
    // Complete model:
    __int128_t Part_CM = (one128 << r)-1;
    LogE_CM = LogE_SubCM(Kset, Part_CM, N);
    cout << int_to_bstring(Part_CM, r) << "\t " << LogE_CM << "\t " << -LogE_CM/Nd/log(2.) << " bits" << endl << endl;

    // Each ICC:
    for (Part = Partition.begin(); Part != Partition.end(); Part++)
    {
      LogE_SCM = LogE_SubCM(Kset, (*Part).second, N);
      LogE += LogE_SCM;
      rank += Bitset_count((*Part).second);
      //eta = ((LogE_SCM/Nd/log(2.)) + r) / (r + (LogE_CM/Nd/log(2.)));
      cout << (*Part).first << "\t " << int_to_bstring((*Part).second, r) << "\t " << LogE_SCM << "\t " << -LogE_SCM/Nd/log(2.) << " bits \t" << endl;
    }  
    cout << endl;
    return LogE - ((double) (N * (r-rank))) * log(2.);
  //}
  return 0;
}


