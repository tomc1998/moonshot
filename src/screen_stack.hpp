#pragma once

#include <memory>

class ScreenStack {
  std::vector<std::unique_ptr<Screen>> stack;

public:
  inline void push(std::unique_ptr<Screen> &&s) {
    s->on_mount();
    stack.push_back(std::move(s));
  }
  inline void pop() {
    assert(stack.size() > 0);
    stack.back()->on_unmount();
    stack.pop_back();
  }
  inline Screen &last() {
    assert(stack.size() > 0);
    return *stack.back();
  }
};
