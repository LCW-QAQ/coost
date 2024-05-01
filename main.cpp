#include "include/co/all.h"
#include "src/co/context/context.h"

void main_loop(tb_context_from_t from) {
  // 证明from就是main函数的上下文
  auto str = ((char *)from.priv);
  std::cout << "main loop, priv: " << str << std::endl;
  int64_t a = 16;
  int32_t b = 10;
  int32_t c = 10;
  int32_t d = 10;
  printf("a = %d, b = %d, c = %d, d = %d\n", a, b, c, d);
  tb_context_jump(from.ctx, nullptr);
}

int main() {
  // 本质是void*指针，指向协程的上下文（包含栈空间）
  tb_context_t ctx{};
  // 包含tb_context_t和一个const void*类型的用户数据指针
  tb_context_from_t from{};
  // 函数指针一个void(tb_context_from_t)类型的函数指针
  // tb_context_func_t func;

  size_t stack_size = 1024 * 1024;
  char *stack = new char[stack_size];

  // 微软x64调用约定，规定返回值小于等于8字节，通过rax返回，
  // 否则由调用者在栈上分配空间，并将指向返回值的指针作为函数的第一个参数（隐式传递）。
  // 微软x64调用约定，规定函数的前四个参数分别由RCX、RDX、r8、r9传递（浮点型对应XMM0、XMM1
  // ......），后续参数入栈。

  // 创建一个协程上下文，运行时的函数为main_loop
  ctx = tb_context_make(stack, stack_size, main_loop);

  std::cout << "before jump" << std::endl;

  // 跳转到指定协程，第一参数表示目标协程上下文，第二个参数为用户数据
  char *str = new char[]{"hello world"};
  from = tb_context_jump(ctx, str);

  std::cout << "after jump" << std::endl;
  std::cout << &from << std::endl;

  delete[] str;
  delete[] stack;
  str = nullptr;
  stack = nullptr;
  return 0;
}