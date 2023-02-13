usage(){
    echo "usage: ./run.sh <arguments>"
    echo "arguments:"
    echo "-h,--help   echo usage"
    echo "-s          run serial code"
    echo "-t          run mul_threads using from 1 to 32 threads"
    echo "-p          run mul_processes using from 1 to 32 processes"
    exit
}
if [ $# != 1 ] || [ $1 = "-h" ] || [ $1 = "--help" ];then
    usage
fi

trap "onCtrlC" SIGINT
function onCtrlC () {
    rm a.out
    exit
}

if [ $1 = "-s" ];then
    echo "serial code:"
    g++ serial.cpp && ./a.out
elif [ $1 = "-t" ];then
    g++ mul_threads.cpp -fopenmp
    echo "Multithreading: "
    for i in {2..32}
    do
        echo "use threads:$i"
        OMP_NUM_THREADS=$i ./a.out
    done
elif [ $1 = "-p" ];then
    mpic++ mul_processes.cpp
    echo "multiprocessing:"
    for i in {2..32}
    do
        echo "use processes:$i"
        mpirun -np $i ./a.out
    done
else
    usage
fi

if [ -f a.out ];then
    rm a.out
fi
