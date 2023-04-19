set -e

# Benchmarks
BENCH=(
	#sum_simple
    #mult_sum_simple
    #sum_loop
    #cond_1
    #cond_2
    #pointer_1
    loop_while
)

LLVM_INSTALL_DIR="/home/canesche/git/llvm-16/build" # </path/to/llvm/>, if you use binary installation, you can leave empty this string 
LLVM_OPT=$LLVM_INSTALL_DIR/bin/opt # </path/to/opt>
CLANG=$LLVM_INSTALL_DIR/bin/clang++ # </path/to/clang>

PATH_LIB="build/lib/libhlsmc.so" 

# Create the build
mkdir -p build

# Create the results
mkdir -p results

echo "Building the cfgPrinter pass"
cmake -DLLVM_INSTALL_DIR=$LLVM_INSTALL_DIR -G "Unix Makefiles" -B build/ .
cd build
cmake --build .
cd ..

OPT="-p instnamer,mem2reg"

for ((i = 0; i < ${#BENCH[@]}; i++)); do

    EXAMPLE=bench/${BENCH[i]}

    python3 scripts/collectCode.py $EXAMPLE.cpp  

    echo "Executing the pass for bench: ${BENCH[i]}"

    $CLANG -Wno-everything -fno-discard-value-names -Xclang -disable-O0-optnone -S -emit-llvm $EXAMPLE"_to_cgra.cpp" -o $EXAMPLE".ll"
    $LLVM_OPT -S $OPT $EXAMPLE".ll" -o $EXAMPLE"_opt.ll"

    $LLVM_OPT -load-pass-plugin $PATH_LIB -passes="dfg" $EXAMPLE"_opt.ll" -disable-output
    
    $LLVM_OPT -p dot-cfg $EXAMPLE"_opt.ll"

    mv *.dot "results/${BENCH[i]}.dot"

    xdot "results/${BENCH[i]}.dot"
done

#rm bench/*.ll
