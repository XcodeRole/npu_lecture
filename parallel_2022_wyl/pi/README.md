(1)π的并行计算算法
莱布尼茨公式
$$
\pi=4*\sum_{n=0}^\infty\frac{(-1)^n}{2n+1}
$$
利用该公式，对π进行求解，项数越多，π值的求解越精确。
要求：
1）实现计算π的并行算法；
分析比较不同线程数或进程数、不同n值时的程序运行时间。

运行:
./run.sh <arguments>
arguments:
-h,--help   echo usage
-s          run serial code
-t          run mul_threads using from 1 to 32 threads
-p          run mul_processes using from 1 to 32 processes

