#include "dataflowPass.h"

using namespace llvm;

#define DEBUG true

namespace dataflow {

    // get label of register and set on number map 
    string dataflowPass::getName(const Value *v) {
        StringRef name(v->getName());

        string name_str = name.str();

        // correction with the string have "."
        if (name_str.find(".") != std::string::npos)
            name_str = "\"" + name_str + "\"";
             
        if (opInstr.count(name_str) > 0) {
            return opInstr[name_str];
        }
        return name_str;
    }

    // get number map of all instruction and basicblock
    void dataflowPass::get_map_label(Function &F) {
        string s;
        for (auto &BB : F) {
            for (auto &I : BB) {
                if (DEBUG)  errs() << I << " " << I.getOpcode() << "\n";
                
                s = getName(&I);
                if (opInstr.count(s) > 0) {
                    continue;
                } else if (CmpInst* CI = dyn_cast<CmpInst>(&I)) {
                    opInstr[s] = I.getOpcodeName();
                    opInstr[s].append("_"); 
                    opInstr[s] += CI->getPredicateName(CI->getPredicate()).str();
                    if (DEBUG) errs() << s << " -> " << opInstr[s] << "\n";
                } else if (isa<GetElementPtrInst>(I)) {
                    opInstr[s] = "getelemptr";
                } else if (isa<BranchInst>(I)) {
                    errs() << I << "\n";
                } else if (!isa<SExtInst>(I) && !isa<BranchInst>(I) && !isa<PHINode>(I) 
                    && !isa<ReturnInst>(I) && !isa<StoreInst>(I)) {
                    opInstr[s] = I.getOpcodeName();
                    if (DEBUG) errs() << s << " -> " << opInstr[s] << "\n";
                }
            }
        }
    }

    void dataflowPass::generate_dataflow(string nameFunction){
        outFile.open(nameFunction + ".dot");
        outFile << "digraph G {\n";
        for (auto e : edge) {
            outFile << get<0>(e) << " -> " << get<1>(e) << "\n";
        }
        outFile << "}\n";
        outFile.close();
    }

    PreservedAnalyses dataflowPass::run(Function &F, FunctionAnalysisManager &FAM) {

        // Counter for Instruction and branch
        get_map_label(F);

        for (auto &BB : F) {
            for (auto &I : BB) {
                runInstruction(&I);
            }
        }
        
        generate_dataflow(F.getName().str());
        
        return PreservedAnalyses::all();
    }

    // Executing Instruction
    void dataflowPass::runInstruction(llvm::Instruction *I) {

        // get the opcode name of each instruction
        int opcode = I->getOpcode();
        errs() << opcode << "\n";

        switch (opcode) {
            case Instruction::Ret: // Return
                RetNode(I); 
                break;
            case Instruction::Store: // load 
                LoadNode(I);
                break;
            case Instruction::Br:
                BranchNode(I);
                break;
            default: // other type
                BinaryNode(I); 
                break;
        }
    }

    void dataflowPass::BinaryNode(llvm::Instruction *I) {
        for (int i = 0; i < I->getNumOperands(); ++i) {
            if (const ConstantInt *CI = dyn_cast<ConstantInt>(I->getOperand(i))) {
                string c = "\"" + to_string(CI->getSExtValue()) + "\"";
                edge.push_back(make_tuple(c, getName(I)));
            } else {
                edge.push_back(make_tuple(getName(I->getOperand(i)), getName(I)));
            }
        }
    }

    void dataflowPass::BranchNode(llvm::Instruction *I) {
        for (int i = 1; i < I->getNumOperands(); ++i) {
            if (isa<ConstantInt>(I->getOperand(i))) {
                string c = "const_" + to_string(dyn_cast<ConstantInt>(I->getOperand(i))->getZExtValue());
                edge.push_back(make_tuple(c, getName(I->getOperand(0))));
            } else {
                errs() << getName(I->getOperand(i)) << " " << getName(I->getOperand(0)) << "\n";
                edge.push_back(make_tuple(getName(I->getOperand(i)), getName(I->getOperand(0))));
            }
        }
    }

    void dataflowPass::LoadNode(llvm::Instruction *I) {

        for (int i = 0; i < I->getNumOperands(); ++i) {
            if (isa<ConstantInt>(I->getOperand(i))) {
                string c = "const_" + to_string(dyn_cast<ConstantInt>(I->getOperand(i))->getZExtValue());
                edge.push_back(make_tuple(c, I->getOpcodeName()));
            } else {
                edge.push_back(make_tuple(getName(I->getOperand(i)), I->getOpcodeName()));
            }
        }

        //errs() << li->getName() << "\n";
    }

    void dataflowPass::RetNode(llvm::Instruction *I) {
        
        errs() << I << "\n";

        if (I->getNumOperands() == 0) return;
        else if (isa<Constant>(I->getOperand(0))) return;
        else {
            edge.push_back(make_tuple(getName(I->getOperand(0)), "out"));
        }
    }

    void dataflowPass::SExtNode(llvm::Instruction *I) {
        opInstr[getName(&(*I))] = getName(I->getOperand(0));
    }
}
