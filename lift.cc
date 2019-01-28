#include "evmjit/libevmjit/Compiler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>

using namespace std;
using namespace dev::eth::jit;

struct Code {
  string name;
  byte* code;
  ssize_t code_size;

  static Code load(const char* fname) {
    ifstream f(fname);
    stringstream buf;
    buf << f.rdbuf();
    string contents = buf.str();

    Code res;
    res.name = "foo";
    res.code_size = buf.str().length()/2;
    res.code = new byte[res.code_size];
    char strbuf[3] = {0,0,0};

    for (ssize_t i = 0; i < res.code_size; i++) {
      strbuf[0] = contents[i*2];
      strbuf[1] = contents[i*2+1];
      res.code[i] = strtol(strbuf, NULL, 16);
    }

    return res;
  }
};

unique_ptr<llvm::Module> compile(llvm::LLVMContext &ctx, const Code& evm_code) {
  evmc_revision rev;
  bool staticCall = false;

  Compiler c({}, rev, staticCall, ctx);
  return c.compile(evm_code.code, evm_code.code + evm_code.code_size, evm_code.name);
}

int main(int argc, char** argv) {
  llvm::LLVMContext ctx;
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " file.bin" << endl;
    return -1;
  }

  auto c = Code::load(argv[1]);
  auto m = compile(ctx, c);

  llvm::raw_os_ostream lcout{cout};
  m->print(lcout, nullptr);
}
