# TRPP
- The benchmark.zip consists of 140 instances which can be grouped into 7 sets: n=10, 20, 50, 100, 200, 500, 1000. 
- The first 6 sets are proposed by Dewilde et al. [1] based on graphs from TSPLIB.  These six sets can be downloaded from https://www.mech.kuleuven.be/en/cib/trpp.
- The last set (n=1000) is proposed by Pei et al. [2], which can be downloaded from http://www.mi.sanu.ac.rs/~nenad/trpp/


[1] T. Dewilde, D. Cattrysse, S. Coene, F. C. R. Spieksma, P. Vansteenwegen, Heuristics for the traveling repairman problem with profits, Computers & Operations Research 40 (7) (2013) 1700–1707.
[2] J. Pei, N. Mladenovi, D. Uroevi, J. Brimberg, X. Liu, Solving the traveling repairman problem with profits: A Novel variable neighborhood search approach, Information Sciences 507 (2020) 108–123.


The work is accepted by "Expert Systems With Applications". 


compile: g++ *.cpp -O3 -o TRPP 
run:     ./TRPP -i instances --seed 0 -rep 0


notes:
0. g++ with 7.5.0 version or more recent.
1. "-i" is the benchmark instance.
2. "--seed" is the seed for the random, you can change it according to your requirements.
3. "-rep" is a flag for the output file, you can ignore it if you do not use it.
