# Greedy Search<br>for the best Minimally Complex Models (MCM)

This program allows the **detection of community structures in binary data**, while taking into account possible **high-order patterns of the data** in detecting the communities (i.e., possible high-order correlations between the variables). The idea of the algorithm is based on performing statistical inference with a family of spin models (maximum entropy models for binary data) that have minimal information-theoretic complexity. These models are called **Minimally Complex Models (MCM)**. We use the fact that each partition of the variables (i.e., a community structure) can be mapped to an MCM.

One huge advantage of Minimally Complex Models is that the model evidence (integration of the likelihood over the model parameter) has a known analytic expression that is easy to compute numerically. As a consequence, the computation of the model evidence doesn’t require fitting the parameters of the model nor computing a numerical integral, which allows to significantly accelerate the comparison between models. The selected model can also be used as a generative model for data. Details can be found in the paper [*Statistical Inference of Minimally Complex Models*](https://arxiv.org/abs/2008.00520) [1].

This repository contains a code initially developed for the paper Ref. [1] and later optimized for the paper Ref.[2]. **The program performs a greedy search** for the best Minimally Complex Spin Model (MCM) on a basis provided by the user. The comparison between models is based on their evidence (i.e., posterior probability that the model generates the data, integrated over its parameter values). The code performs a hierarchical merging procedure to find an optimal MCM in the basis chosen by the user. The code can run for systems with up to `n=128` variables. 

This program is complementary to the program available in [github.com/clelidm/MinCompSpin](https://github.com/clelidm/MinCompSpin), which performs an **exhaustive search** for the best community. We recommend using the greedy search over the exhaustive search when the number of variables exceeds `15`. A **simulated annealing version** of the optimization procedure can also be found in [github.com/ebokai/MinCompSpin_SimulatedAnnealing](https://github.com/ebokai/MinCompSpin_SimulatedAnnealing), which can find solutions closer to the global optimal.


[1]  C. de Mulatier, P. P. Mazza, M. Marsili, *Statistical Inference of Minimally Complex Models*, [arXiv:2008.00520](https://arxiv.org/abs/2008.00520)

[2] E. Peerbooms, S. Kamphof, J.-H. Lestang, C. de Mulatier, *Statistical Modeling of High-Order Community Structures in Binary Data*

## Different programs for searching for the best MCM:

The comparison between models is based on their evidence (posterior probability that the model generates the data, integrated over its parameter values). All these programs aim at finding the models with the largest log-evidence. 

 - **Exhaustive search:** The program available [here](https://github.com/clelidm/MinCompSpin) performs an exhaustive search for the best community. The code goes through all possible MCMs of a given rank `r`, and finds the one with the largest evidence (exact optimal model). The limiting factor of this code is that the space of possible MCMs increases almost exponentially with the number `r` of variables (the number of MCMs on a chosen basis is given by the Bell number of `r`). We recommend using a different strategy when the number of variables exceeds `~13` to `15`.

 - **Greedy search VS Simulated annealing:** 
We recommend using the *simulated annealing algorithm* if you are interested in finding a solution that is as close as possible to the global optimal, or when the local greedy procedure fails to merge the initial spin variables into communities (this could be due to the absence of strong pairwise patterns, for example). We recommend the *Greedy algorithm* if you are looking for a fast converging algorithm, as it will allow finding near-optimal solutions in reasonable times when the search space becomes too large.

----
# General information

The code performs a hierarchical merging procedure to try to find the optimal Minimally Complex Model (MCM) in the basis chosen by the user.

 - **Chosen Basis:** The code starts by re-writing the data in the basis provided by the user. By default, this basis is the original basis of the data.

 - **Greedy Search:** To perform the hierarchical merging procedure, the code starts with `r` initial communities, and then successively merges the two communities that lead to the largest increase in the log-Evidence of the model. The code iterates this procedure until no more communities can be merged without decreasing the log-Evidence. **By default, the `n` initial communities are taken to be such that there is only one variable in each community.**  This procedure returns the MCM that achieves the maximal value of the evidence along the hierarchical merging process, as the number of communities varies from `n` to `1`.

----
# Usage

## Requirements

The code uses the C++11 version of C++.

## Format of the input datafiles:
The data file and the basis file must be placed in the `INPUT` folder and have the following format:
- **Data file:** The dataset must be provided in a text file in which the state of each variable is either a `'0'` or a `'1'`. The file must have:
   - `n` columns, where `n` is the number of variables;
   - `N` lines, each corresponding to a new datapoint (where `N` is the total number of datapoints).
     
  See the example file `MNIST11.sorted` which is a dataset with `n=121` variables and `N=60 000` binary datapoints.

- **Basis file:** The basis must be provided in a text file, in which basis operators are defined in each line by a binary string of size `n`:
  - a `'0'` at the i-th index indicates that the i-th spin is not involved in the operator;
  - a `'1'` at the i-th index indicates that the i-th spin contributes to the operator.
    
  Here, spins are ordered in the same order as in the original datafile, i.e. the i-th index from the left in the basis file corresponds to the i-th spin from the left in the datafile.
  
  See the example file `MNIST11.sorted_BestBasis_k4_Binary` which contains a basis with `n=121` basis operators, each encoded over the `n=121` original basis variables. Each operator corresponds to a new line. Note that the text appearing after the `n`-th columns is simply ignored by the function reading the basis. Similarly, empty lines or lines starting with characters other than `'0'` or `'1'` are ignored (see for example the line starting with `'#'`).

## Usage without Makefile:

 - **To compile:**  Type in your Terminal from the main folder:
   ```bash
   g++ -std=c++11 -O3 includes/main_routines.cpp main.cpp src/MCM/*.cpp -include includes/library.hpp -o MCM_Greedy.out
   ```
   or use the bash script `job_compile.sh` by running in your Terminal:
   ```bash
   bash job_compile.sh
   ```

 - **To execute:** The data and basis files be placed in the `INPUT` folder. In the following commands, replace `[datafilename]` by the name of your datafile, `[n]` by the number of variables, and `[basisfilename]` by the name of the file containing a chosen new basis (optional).

   - **Greedy Search in the original basis:** This will run the greedy search in the basis in which the dataset is provided:
     ```bash
     ./GreedySearch.out  [datafilename] [n]
     ```
   - **Greedy Search in a chosen basis:** You can run the greedy search in another chosen basis by using the option `-b` followed by the name of the basis file `[basisfilename]`:
     ```bash
     ./GreedySearch.out  [datafilename] [n] -b [basisfilename]
     ```

   **Other options:** You can also use the following options: `--full` and `--NoCheckPoint`
    - **Full merging:** the option `--full` will run the greedy merging until everything is merged into a single complete ICC, and print the evolution of the LogE along the merging path. The best MCM along the greedy path is saved and returned at the end. By default (i.e., without this option), the greedy algorithm will stop as soon as any additional merging starts decreasing the LogE.
    - **Remove checkpoints:** By default, the program prints out the intermediate values of LogE along the Greedy path. Using the option `--NoCheckPoint` will stop printing this information. This may save time, when printing is not needed, for datasets with a large number of variables and a long greedy path.
    - The options `-b`, `--full` and `--NoCheckPoint` can be combined in any order. Note that options `--full` and `--NoCheckPoint` are incompatible: `--full` will always print the checkpoints and therefore take over `--NoCheckPoint`.
   
   **Summary table:**
   | Run  | Command | Comment |
   | --- | --- | --- |
   | Help | `./MCM_Greedy.out -h` | |
   | Run default example:<br>MNIST dataset | `./MCM_Greedy.out`| See Example 1<br>in "Examples" section below|
   | Greedy Search<br>in the Original Basis<br>of the data | `./MCM_Greedy.out [datafilename] [n]` |  |
   | Greedy Search<br>in the chosen New Basis | `./MCM_Greedy.out [datafilename] [n] -b [basisfilename]`| |
   | Full Greedy merging<br>until getting a single ICC | `./MCM_Greedy.out [datafilename] [n] --full` | prints LogE<br>along the entire Greedy path |
   | Don't print checkpoints | `./MCM_Greedy.out [datafilename] [n] --NoCheckPoint` | slightly faster for datasets<br>with large `n` |

 - **To execute using a bash script:**
   Open the script file `job_run.sh` and replace `datafilename` by the name of your datafile, `n` by your number of variables in the file `job_run.sh`, and (if needed) `basisfilename` by the name of your basis file. Select/comment the command line that applies to your case.
   
   Use the script `job_run.sh` by running in your Terminal:
   ```bash
   bash job_run.sh
   ```
   All the options in the previous section also apply here. 

## Usage with Makefile:

Run the following commands in your terminal, from the main folder (folder containing the `makefile` document):

 - **To compile:** `make`
  
 - **To clean:**
   - `make clean` removes all the intermediate files created during compilation and not used later;
   - `make delete` will delete the final executable (to use only once you're done using the code).

 - **To Execute:**
    - `make run` to run the greedy search in the original basis;
    - `make run-newBasis` to run the greedy search in the specified new basis.
   
   This will use the datafiles specified at the beginning of the `makefile`. To change them, open the `makefile` and replace the values of the following variables at the very top of the file:
   >  - `datafilename`: name of your datafile; this file must be placed in the `INPUT` folder.
   >  - `n`: number of variables in your file; maximum possible value `n = 128`.
   >  - `basisfilename`: (option) name of the file containing the new basis; this file must be placed in the `INPUT` folder.
   
   You can also execute the code by running it directly from your terminal, as in the previous section ("Usage without Makefile"). For example, you can run the command:
   ```bash
   ./GreedySearch.out [datafilename] [n]
   ```
   where you must replace `[datafilename]` by the name of your datafile and `[n]` by your number of variables.

   **Other options:** All the options specified in the previous section, "Usage without Makefile", still apply here.


## Examples

In the `INPUT` folder, we provided the following examples:
  - **Example 1: MNIST dataset.** The binary dataset `MNIST11.sorted`: this is the binarized version of the MNIST dataset [3] used as an example in Ref.[1] (see Fig.~7). The dataset has `n=121` variables and `N=60 000` datapoints. The file `MNIST11.sorted_BestBasis_k4_Binary.dat` contains the binary representation of the best basis found with the recursive procedure described in Ref.[1] up to order `k=4` (using the program available [here](https://github.com/clelidm/MinCompSpin_BasisSearch128)).
  - **Example 2: Big 5 dataset.** The binary dataset `Big5PT.sorted`: this is the binarized version of the Big 5 dataset [3] used as an example in Ref.[1]. The dataset has `n=50` variables and `N = 1 013 558` datapoints. Important: This dataset is given as a zip file (due to its large size) and must be decompressed before use.
The file `MNIST11.sorted_BestBasis_k4_Binary.dat` contains the binary representation of the best basis found with the recursive procedure described in Ref.[1] up to order `k=4` (using the program available [here](https://github.com/clelidm/MinCompSpin_BasisSearch128)).
  - **Example 3:** The binary dataset `SCOTUS_n9_N895_Data.dat`, which is the dataset of votes of the US Supreme Court analyzed in Ref.[4] and used as an example in Ref.[1].
    
Each of these datasets can be analyzed by running the program with the `makefile` or the `job_run.sh` after commenting/uncommenting the correct datafile choices at the beginning of these files.

[2] LeCun, L Bottou, Y Bengio, P Haffner, *Gradient-based learning applied to document recognition*. Proc. IEEE 86, 2278–2324 (1998).

[3] Raw data from [Open-Source Psychometrics Project](https://openpsychometrics.org/_rawdata/) in the line indicated as *"Answers to the IPIP Big Five Factor Markers"*; [here](https://openpsychometrics.org/_rawdata/IPIP-FFM-data-8Nov2018.zip) is a direct link to the same zip file.

[4] E.D. Lee, C.P. Broedersz, W. Bialek, *Statistical Mechanics of the US Supreme Court*. [J Stat Phys 160, 275–301 (2015)](https://link.springer.com/article/10.1007/s10955-015-1253-6).


## Output and format of the returned MCMs:

 - **Working in the original basis:** The (How to read MCM in the original and in the new basis.)

 - **Working in the new basis:**


**Results:** See Ref.[1] for results and discussions on the best MCMs obtained for these datasets. You can also find, in the `OUTPUT` folder, the log returned when running the greedy code on these datasets. 
The log returned when running the greedy code on the example datasets can be found in the `OUTPUT` folder.

## Important information:

### Basis change:
To change the basis of the data to a chosen basis and apply the MCM search in this new basis:
 1. Specify the basis elements in a list of integers `list<__int128_t> basis_li = ` using one of the available functions.
 2. Transform the dataset `Nset` into the new basis (transformed data is in `Kset`) using the function `build_Kset`. This defines the new transformed data, stored in `Kset`:
```c++
vector<pair<__int128_t, unsigned int>> Kset = build_Kset(Nset, Basis_li);
```

**!! Important!!**
when performing this basis transformation, basis operators are placed from right to left in the new basis, 
i.e. the rightmost bit (lowest bit) corresponds to the first operator in `list<__int128_t> Basis`.

This is very important for properly interpreting the output of the MCM algorithm after basis transformation.

----
## Advanced use:

You can also directly work with the C++ code, by calling functions directly from the `main()` function of the `main.cpp` file. For hands-on simple use of available functions, see the function `tutorial()` in the `main.cpp` file (you can call this function by uncommenting the last lines in the function `main()`).

All the functions that can be called from `int main()` are declared at the beginning of the `main.cpp` file or in the file `library.hpp`. The most useful functions are described below. 

 - **Basis Choice:** The basis is encoded in the variable `Basis_li`; by default, it is the original basis of the data. A different basis can be specified in a file by the user and read with the functions `Read_BasisOp_IntegerRepresentation()` or the function `Read_BasisOp_BinaryRepresentation()` depending on the format of the basis file (see examples in the `INPUT` folder, the files `SCOTUS_n9_BestBasis_Binary.dat` and `SCOTUS_n9_BestBasis_Integer.dat` which are encoding the best basis for the US Supreme Court data used as an example in Ref.[1]).

 - **Basis Transformation:** blabla

 - **Greedy Search:** The Greedy search can be done using the functions `MCM_GreedySearch()` or `MCM_GreedySearch_AND_printInfo()` (the only difference is the latter function also prints some information about the found best MCM). 

 - **Starting the Greedy search from a chosen MCM:** By default, the greedy search starts from an MCM in which each ICC only contains a single variable. The search can also be started from a different initial MCM, using the function `MCM_GreedySearch_MCM0()`. This can be used for instance to run a short greedy merging at the end of a simulated annealing search.

----
## License

This code is an open-source project under the GNU GPLv3.


