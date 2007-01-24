#include "dataclasses/I3Tree.h"

struct I3MMCInfo{
  int minor_pid;
  long major_pid;

  double xi;
  double yi;
  double zi;
  double ti;
  double Ei;

  double xc;
  double yc;
  double zc;
  double tc;
  double Ec;

  double xf;
  double yf;
  double zf;
  double tf;
  double Ef;

  I3MMCInfo() {}
  
  virtual ~I3MMCInfo();

  template <class Archive> void serialize(Archive & ar, unsigned version);
};

typedef I3Tree<I3MMCInfo> I3MMCInfoTree;
I3_POINTER_TYPEDEFS(I3MMCInfo);
I3_POINTER_TYPEDEFS(I3MMCInfoTree);
