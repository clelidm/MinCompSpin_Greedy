
### DATA:

datafilename=MNIST11.sorted   #Big5PT.sorted
n=121  #50
basisfilename=MNIST11.sorted_BestBasis_k4_Binary.dat  #Big5PT.sorted_BestBasis_Binary.dat 


### RUN in ORIGINAL BASIS:

time ./MCM_Greedy.out $datafilename $n 

### RUN in chosen NEW BASIS:

#time ./MCM_Greedy.out $datafilename $n -b MNIST11.sorted_BestBasis_k4_Binary.dat