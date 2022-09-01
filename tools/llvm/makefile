.POSIX:

LLVM_SRC_URL=https://github.com/stuffandahalf/llvm-project/releases/download/llvmorg-12.0.0-alix-beta2/llvm-project-12.0.0-alix-beta2.tar.xz

all: llvm

llvm: build configure
	cd build && $(MAKE) all install

configure: src build
	cd build && cmake ../src/llvm -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/Users/ganorton/git/alix/obj/tools -DLLVM_ENABLE_PROJECTS=clang
