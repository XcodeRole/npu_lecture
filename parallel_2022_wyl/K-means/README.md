(3)并行k-means算法
K-means是最常用的基于欧式距离的聚类算法，其认为两个目标的距离越近，相似度越大。
K-means 有一个著名的解释：牧师—村民模型：
有四个牧师去郊区布道，一开始牧师们随意选了几个布道点，并且把这几个布道点的情况公告给了郊区所有的村民，于是每个村民到离自己家最近的布道点去听课。
听课之后，大家觉得距离太远了，于是每个牧师统计了一下自己的课上所有的村民的地址，搬到了所有地址的中心地带，并且在海报上更新了自己的布道点的位置。
牧师每一次移动不可能离所有人都更近，有的人发现A牧师移动以后自己还不如去B牧师处听课更近，于是每个村民又去了离自己最近的布道点……
就这样，牧师每个礼拜更新自己的位置，村民根据自己的情况选择布道点，最终稳定了下来。
K-means算法的步骤为：
1)选择初始化的k个样本作为初始聚类中心a=a1,a2,,,,ak；
2)针对数据集中每个样本xi，计算它到k个聚类中心的距离，并将其分到距离最小的聚类中心所对应的类中；
3)针对每个类别Cj，重新计算它的聚类中心（即属于该类的所有样本的质心）；
4)重复上面的2)、3)两步操作，直到达到某个终止条件（迭代次数、最小误差变化等）。

要求：
1）基于MPI或OpenMP并行编程，设计并实现k-means算法的并行程序。
2）采用UCI的用于聚类的数据集（https://archive.ics.uci.edu/ml/index.php）。
3）测试并分析并行程序的时间性能。

运行代码：
usage: ./run.sh <arguments> [num_cores]
num_cores:    using num_cores to parallel
arguments:
-h,--help     echo usage
-s            run serial code
-t num_cores  run mul_threads using num_cores threads
-p num_cores  run mul_processes using num_cores processes