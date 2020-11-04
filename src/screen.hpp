#pragma once

struct Screen {
  inline virtual void on_mount(){};
  inline virtual void on_unmount(){};
  inline virtual void on_frame(){};
};
