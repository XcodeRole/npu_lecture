usage(){
    echo "usage: ./run.sh <arguments> [num_cores]"
    echo "num_cores:    using num_cores to parallel"
    echo "arguments:"
    echo "-h,--help     echo usage"
    echo "-s            run serial code"
    echo "-t num_cores  run mul_threads using num_cores threads"
    echo "-p num_cores  run mul_processes using num_cores processes"
    exit
}

trap "onCtrlC" SIGINT
function onCtrlC () {
    rm a.out
    exit
}

if [ $1 = "-s" ];then
    echo "serial code:"
    g++ serial.cpp && ./a.out
elif [ $# != 2 ] || [ $1 = "-h" ] || [ $1 = "--help" ];then
    usage
elif [ $1 = "-t" ];then
    g++ mul_threads.cpp -fopenmp
    echo "Multithreading: "
    # for i in {2..32}
    # do
        echo "use threads:$2"
        OMP_NUM_THREADS=$2 ./a.out
    # done
elif [ $1 = "-p" ];then
    # mpic++ mul_processes.cpp
    # echo "multiprocessing:"
    # for i in {2..32}
    # do
    #     echo "use processes:$i"
    #     mpirun -np $i ./a.out
    # done
    echo "这个程序没写出来，请运行多线程并行程序，命令：./run -t 4"
    echo "4表示线程数，可更改"
else
    usage
fi

if [ -f a.out ];then
    rm a.out
fi
