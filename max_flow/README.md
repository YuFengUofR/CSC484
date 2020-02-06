# Max Flow

In this project, I implemented 2 different algorithms: Ford-Fulkerson and Push-relabel.

In Ford-Fulkerson, I used a simple DFS to find a augmented path and then compute the residual graph. Both algorithms' results are in `*_result.txt` and the runtime is in `*.runtime.txt`.

Overall, push relabel is much faster in my implementation. To compile both algoorithms, please

```
 $ g++ -std=c++11 -O3  -W -o [SOLVER]  [SOLVER].cpp
```

and to reproduce the results, please edit the `runner.sh` first (to choose the solver) and then, run
```
 $ ./runner.sh    
```
