########################################################################################################################
####################################      CONSTANT TO SPECIFY     ######################################################
########################################################################################################################
#### If the following two variables are left empty:           ##########################################################
####     then the program will automatically use the filename and number of variables specifed in data.h    ############
########################################################################################################################

#### Datafiles must be placed in the "INPUT" folder:
datafilename := MNIST11.sorted #Big5PT.sorted   # datafile name
n := 121 #50		# number of binary variables in the datafile 
basisfilename := MNIST11.sorted_BestBasis_k4_Binary.dat  #Big5PT.sorted_BestBasis_Binary.dat 

########################################################################################################################
######## ENTER THE FOLLOWING IN YOUR TERMINAL:
#### TO COMPILE:  	make
#### TO RUN: 		make run
#### TO CLEAN:  	make clean    --> to use only when you are completely done
########################################################################################################################


########################################################################################################################
##########################################      DO NOT MODIFY     ######################################################
########################################################################################################################

CC = g++ 	# Flag for implicit rules: used for linker
CXX = g++ 	# Flag for implicit rules: compilation of c++ files
CXXFLAGS = -std=c++11 -Wall   #Extra flags to give to the C++ compiler

# !!!! Implicit compilation of the all the objects !!!!
# Compilation -- Implicite rule:   
#%.o : %.c
#		$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

### MCM Files:
DIR_MCM = Libraries/MCM
objects_MCM = tools.o LogE_LogL.o Complexity.o MCM_info.o Basis_Choice.o P_s.o Operations_OnData.o info_quant.o BestMCM_GreedySearch.o User_Interface.o

### Libraries:
DIR_LIB = Libraries

# String substitution: add the directory name: # As an example, hello.o turns into ./MCM/hello.o
OBJS := $(objects_MCM:%=$(DIR_MCM)/%)


MCM_Greedy.out: $(OBJS) $(DIR_LIB)/main.o $(DIR_LIB)/main_routines.o $(DIR_LIB)/library.hpp.gch
	g++ $(CXXFLAGS) $(DIR_LIB)/main.o $(DIR_LIB)/main_routines.o $(OBJS) -o MCM_Greedy.out

$(DIR_LIB)/main.o: main.cpp data.h $(DIR_LIB)/library.hpp.gch
	g++ $(CXXFLAGS) -c main.cpp -include $(DIR_LIB)/library.hpp -o $(DIR_LIB)/main.o   # Compile main.cpp

$(DIR_LIB)/main_routines.o: $(DIR_LIB)/main_routines.cpp $(DIR_LIB)/library.hpp.gch
	g++ $(CXXFLAGS) -c $(DIR_LIB)/main_routines.cpp -include $(DIR_LIB)/library.hpp -o $(DIR_LIB)/main_routines.o

$(DIR_LIB)/library.hpp.gch: $(DIR_LIB)/library.hpp
	g++ $(CXXFLAGS) -c $(DIR_LIB)/library.hpp

run: MCM_Greedy.out
	time ./MCM_Greedy.out $(datafilename) $n 

run-newBasis: MCM_Greedy.out
	time ./MCM_Greedy.out $(datafilename) $n -b $(basisfilename)

clean:
	rm -f $(OBJS) $(DIR_LIB)/library.hpp.gch $(DIR_LIB)/main_routines.o $(DIR_LIB)/main.o MCM_Greedy.out
