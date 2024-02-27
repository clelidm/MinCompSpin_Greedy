/********************************************************************/
/**************************    CONSTANTS    *************************/
/********************************************************************/

// number of binary (spin) variables:
unsigned int n = 9;  

// INPUT FOLDER:
const std::string input_directory = "INPUT/";

// INPUT DATA FILES (optional):  
// the input datafile can also be specified directly in the main() function, as an argument of the function "read_datafile()":
std::string datafilename = "SCOTUS_n9_N895_Data.dat";

// INPUT BASIS FILES (optional):
std::string basis_filename = "SCOTUS_n9_BestBasis_Binary.dat";  // must be in binary representation
const std::string basis_IntegerRepresentation_filename = "SCOTUS_n9_BestBasis_Integer.dat";        // (optional) Input basis file 
const std::string basis_BinaryRepresentation_filename = "SCOTUS_n9_BestBasis_Binary.dat";      // (optional) Input basis file

// INPUT MCM FILES (optional): // For example: choice of a community to compare with the uncovered community:
const std::string communityfile = "INPUT/SCOTUS_Communities_inBestBasis.dat"; 

// OUTPUT FOLDER:
const std::string OUTPUT_directory = "OUTPUT/";
