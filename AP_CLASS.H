#include "gl.h"
#include "mdi.h"
#include "array.h"
#include "toolbox.h"


_CLASSDEF(Tapvendor)
 class Tapvendor:public Data4
 {
  public:
  Field4 *vendorno;
  Field4 *vdshort;
  Field4 *name;
  Field4 *address1;
  Field4 *address2;
  Field4 *address3;
  Field4 *postal;
  Field4 *phone;
  Field4 *contact;
  Field4 *ref;
  Field4 *status;

  Tglaccts(Code4 *cb,LPSTR name=0):Data4(cb,"apvendor")
  {
  tag_select(tag("vendorno"));
  top();  
  vendorno = (Field4 *) new Field4(*(Data4 *)this,"vendorno");
  vdshort = (Field4 *) new Field4(*(Data4 *)this,"vdshort");
  name     = (Field4 *) new Field4(*(Data4 *)this,"name");
  address1 = (Field4 *) new Field4(*(Data4 *)this,"address1");
  address2 = (Field4 *) new Field4(*(Data4 *)this,"address2");
  address3 = (Field4 *) new Field4(*(Data4 *)this,"address3");
  postal   = (Field4 *) new Field4(*(Data4 *)this,"postal");
  phone    = (Field4 *) new Field4(*(Data4 *)this,"phone");
  contact  = (Field4 *) new Field4(*(Data4 *)this,"contact");
  ref      = (Field4 *) new Field4(*(Data4 *)this,"ref");
  status   = (Field4 *) new Field4(*(Data4 *)this,"status");
  };
  ~Tapvendor(){close();};
};


