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

    // 把要用的 Context 和 Type 搞出乃
    LLVMContext &C = M.getContext();
    Type *VoidTy = Type::getVoidTy(C);
    IntegerType *Int32Ty = IntegerType::getInt32Ty(C);

    if (F.getName() == "main")
    {
      // 先挖出 Builder, 以修改行為
      BasicBlock::iterator IP = F.getEntryBlock().getFirstInsertionPt();
      IRBuilder<> IRB(&(*IP));

      // 1. 找出沒有回傳值, 且第一個參數是 int 類別的 debug 函式, 並且注入第一個值為 9527 之參數後執行它
      FunctionType *FnTy = FunctionType::get(VoidTy, {Int32Ty}, false);
      FunctionCallee Fn = M.getOrInsertFunction("debug", FnTy);
      IRB.CreateCall(Fn, ConstantInt::get(Int32Ty, 9527));

      // 2. 在確認前, 使 argc 為 9487, 並使輸出符合作業預期結果
      F.getArg(0)->replaceAllUsesWith(ConstantInt::get(Int32Ty, 9487));

      // 3. 在確認前, 使 argv[1] 為 "aesophor is ghost !!!", 然後存到 Ptr 指定之位址
      Value *argv1Ptr = IRB.CreateGEP(F.getArg(1), ConstantInt::get(Int32Ty, 1));
      Value *strPtr = IRB.CreateGlobalStringPtr("aesophor is ghost !!!");
      IRB.CreateStore(strPtr, argv1Ptr);
    }

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
