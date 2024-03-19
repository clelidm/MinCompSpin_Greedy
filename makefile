########################################################################################################################
####################################      CONSTANT TO SPECIFY     ######################################################
########################################################################################################################
#### If the following two variables are left empty:           ##########################################################
####     then the program will automatically use the filename and number of variables specifed in default_data.h   #####
####     DEFAULT EXAMPLE:   MNIST dataset            ###################################################################
########################################################################################################################

### IMPORTANT: -- the datafile and the basis-file must be in the 'INPUT' folder
### 		   -- the MCM-file (for sampling) must be in the 'OUTPUT' folder

###############################      EXAMPLE 1: MNIST dataset:     #####################################################

#datafilename := MNIST11.sorted # datafile name, must be in INPUT folder
#n := 121 		# number of binary variables in the datafile 
#basisfilename := MNIST11.sorted_BestBasis_k4_Binary.dat   # must be in INPUT folder 

#mcmfilename :=    # MCM-filename (to specify), must be in OUTPUT folder 
#N = 1000

###############################      EXAMPLE 2: Big5 dataset:     ######################################################

#datafilename := Big5PT.sorted   # datafile name, must be in INPUT folder
#n := 50		# number of binary variables in the datafile 
#basisfilename := Big5PT.sorted_BestBasis_Binary.dat  # must be in INPUT folder 

#mcmfilename :=    # MCM-filename (to specify), must be in OUTPUT folder 
#N = 1000

###############################      EXAMPLE 3: SCOTUS dataset:     ####################################################

datafilename := SCOTUS_n9_N895_Data.dat    # datafile name, must be in INPUT folder
n := 9      # number of binary variables in the datafile 
basisfilename := SCOTUS_n9_BestBasis_Binary.dat  # must be in INPUT folder 

mcmfilename := SCOTUS_n9_N895_Data_MCM_inBnew.dat  # must be in OUTPUT folder 
                                                   # --> this is the best MCM overall, i.e. the one found by the exhaustive search in the best basis
N=1000          ## number of samples

########################################################################################################################
######## ENTER THE FOLLOWING IN YOUR TERMINAL:
#### TO COMPILE:  	    make
#### TO RUN GREEDY:     make run
#### TO RUN SAMPLING:   make sample
#### TO CLEAN:  	    make clean    --> to use only when you are completely done
########################################################################################################################


########################################################################################################################
##########################################      DO NOT MODIFY     ######################################################
########################################################################################################################

CC = g++ 	# Flag for implicit rules: used for linker
CXX = g++ 	# Flag for implicit rules: compilation of c++ files
CXXFLAGS = -std=c++11 -O3 #-Wall #-Wall #-O3   #Extra flags to give to the C++ compiler

# !!!! Implicit compilation of the all the objects !!!!
# Compilation -- Implicite rule:   
#%.o : %.c
#		$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

### MCM Files:
DIR_MCM = src/MCM
objects_MCM = tools.o LogE_LogL.o Complexity.o MCM_info.o Basis_Choice.o P_s.o Sampling_fromMCM.o Operations_OnData.o info_quant.o BestMCM_GreedySearch.o User_Interface.o BasisTools.o

### Libraries:
DIR_LIB = includes

# String substitution: add the directory name: # As an example, hello.o turns into ./MCM/hello.o
OBJS := $(objects_MCM:%=$(DIR_MCM)/%)

MCM_Greedy.out: $(OBJS) $(DIR_LIB)/main.o $(DIR_LIB)/main_routines.o $(DIR_LIB)/library.hpp.gch
	g++ $(CXXFLAGS) $(DIR_LIB)/main.o $(DIR_LIB)/main_routines.o $(OBJS) -o MCM_Greedy.out

$(DIR_LIB)/main.o: main.cpp $(DIR_LIB)/default_data.h $(DIR_LIB)/library.hpp.gch
	g++ $(CXXFLAGS) -c main.cpp -include $(DIR_LIB)/library.hpp -o $(DIR_LIB)/main.o   # Compile main.cpp

$(DIR_LIB)/main_routines.o: $(DIR_LIB)/main_routines.cpp $(DIR_LIB)/library.hpp.gch
	g++ $(CXXFLAGS) -c $(DIR_LIB)/main_routines.cpp -include $(DIR_LIB)/library.hpp -o $(DIR_LIB)/main_routines.o

$(DIR_LIB)/library.hpp.gch: $(DIR_LIB)/library.hpp
	g++ $(CXXFLAGS) -c $(DIR_LIB)/library.hpp


run: #MCM_Greedy.out
	time ./MCM_Greedy.out $(datafilename) $n 

run-newBasis: #MCM_Greedy.out
	time ./MCM_Greedy.out $(datafilename) $n -b $(basisfilename)

sample: #MCM_Greedy.out
	time ./MCM_Greedy.out $(datafilename) $n --sample $(mcmfilename) -N $N

sample-newBasis: #MCM_Greedy.out
	time ./MCM_Greedy.out $(datafilename) $n -b $(basisfilename) --sample $(mcmfilename) -N $N

clean:
	rm -f $(OBJS) $(DIR_LIB)/library.hpp.gch $(DIR_LIB)/main_routines.o $(DIR_LIB)/main.o

delete:
	rm MCM_Greedy.out
