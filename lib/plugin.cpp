#include "cfgPrintPass.h"
#include "dataflowPass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

bool registerPipeline(StringRef Name, FunctionPassManager &FPM,
                      ArrayRef<PassBuilder::PipelineElement>) {
    
    if (Name == "cfgPrint") {
        FPM.addPass(cfgPrint::cfgPrintPass(errs()));
        return true;
    }
    if (Name == "dfg") {
        FPM.addPass(dataflow::dataflowPass(errs()));
        return true;
    }

    return false;
}

PassPluginLibraryInfo getCFGConstPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "hlsmc", LLVM_VERSION_STRING, 
            [](PassBuilder &PB){
        PB.registerPipelineParsingCallback(registerPipeline);
    }};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getCFGConstPluginInfo();
}
