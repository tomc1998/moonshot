#pragma once

class ScreenStack;

struct Screen {
  inline virtual void on_mount(){};
  inline virtual void on_unmount(){};
  inline virtual void on_frame(ScreenStack &stack){};
};
