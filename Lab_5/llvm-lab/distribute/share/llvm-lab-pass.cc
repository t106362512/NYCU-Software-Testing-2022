#include "llvm/Analysis/CFGPrinter.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Pass.h"

using namespace llvm;

namespace
{

  class ExamplePass : public ModulePass
  {

  public:
    static char ID;
    ExamplePass() : ModulePass(ID) {}

    bool doInitialization(Module &M) override;
    bool runOnModule(Module &M) override;
  };

} // namespace

char ExamplePass::ID = 0;

bool ExamplePass::doInitialization(Module &M)
{

  return true;
}

bool ExamplePass::runOnModule(Module &M)
{

  errs() << "runOnModule\n";

  for (auto &F : M)
  {

    /* add you code here */
    errs() << F.getName() << "\n";
  }

  return true;
}

static void registerExamplePass(const PassManagerBuilder &,
                                legacy::PassManagerBase &PM)
{

  PM.add(new ExamplePass());
}

static RegisterStandardPasses RegisterExamplePass(
    PassManagerBuilder::EP_OptimizerLast, registerExamplePass);

static RegisterStandardPasses RegisterExamplePass0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerExamplePass);
