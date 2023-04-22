#ifndef DATAFLOW_PASS_H
#define DATAFLOW_PASS_H

/** 
    @brief Dummy class used for illustration purposes. Doing something with it.

    Detailed description follows here.
    @author M. Canesche
    @date October 2022
*/

#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

#include "llvm/IR/Constants.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include <llvm/Support/raw_ostream.h>

using namespace llvm;
using namespace std;

namespace dataflow {

    struct dataflowPass : public PassInfoMixin<dataflowPass> {

        explicit dataflowPass(raw_ostream &OS) : OS(OS) {}
        
        PreservedAnalyses run(Function &F,
                              FunctionAnalysisManager &FAM);
        
        void runBasicBlock(Function::iterator &bb);
        void runInstruction(llvm::Instruction *it);
        void generate_dataflow(string nameFunction);

        string getName(const Value *v); 

        void get_map_label(Function &F);
        void BinaryNode(llvm::Instruction *I);
        void PhiNode(llvm::Instruction *I);
        void RetNode(llvm::Instruction *I);
        void LoadNode(llvm::Instruction *I);
        void BranchNode(llvm::Instruction *I);
        void SExtNode(llvm::Instruction *I);
        
        private:
            raw_ostream &OS;
            unordered_map<string, int> counter;
            unordered_map<string, string> opInstr;
            vector<tuple<string, string>> edge; 
            ofstream outFile;
            unsigned int count_phi = 0;
            unsigned int count_add = 0;
    };

} // namespace cfgPrint

#endif