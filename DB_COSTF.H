/* Cost File - Resource Costs records the costs for all resources for a particular period.
   Update the resource structure cost file from:

   Actual Labour costs - Job time reports.
   Actual Production amounts - Manufacturing result file
   Actual Overhead - General Ledger and Allocaition dbf
   Standard rates for each resource come from the resource file.
   Historical amounts are created from the previous months actual amounts.
   Material usage for each raw material comes from the item master file.
    On hand quantity + purchases - previous months on hand quantity

*/  
_CLASSDEF(TCostf)
class TCostf : public Data4
{
 public:
 Field4 *fCode;			// Resource master file
 Field4 *fFulln;		// Resource master file
 Field4 *fPeriod;		// Profile file
 Field4 *fCosttype;		// Resource file
 Field4 *fElement;		// Resource cost structure master 
 Field4 *fUnitPrice;		// Depends on the type of cost and element
 Field4 *fQuantity;		// Depends on the type of cost and element

 TCostf(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_costf")
 {
  top();
  tag_select(tag("fpcode"));
  fCode		= (Field4 *) new Field4(*(Data4 *)this,"fpcode");
  fFulln        = (Field4 *) new Field4(*(Data4 *)this,"ffulln");
  fPeriod  	= (Field4 *) new Field4(*(Data4 *)this,"fperiod");
  fCosttype	= (Field4 *) new Field4(*(Data4 *)this,"fcosttype");
  fElement 	= (Field4 *) new Field4(*(Data4 *)this,"felement");
  fUnitPrice	= (Field4 *) new Field4(*(Data4 *)this,"funitprice");
  fQuantity 	= (Field4 *) new Field4(*(Data4 *)this,"fquantity");
 }
 double dQuantity() {return double(Str4ptr(fQuantity->str()));};
 double dUnitPrice() {return double(Str4ptr(fUnitPrice->str()));};
 ~TCostf(){close();};
};

/*
   Total cost for each manufactured product by item code and item type.
   When a new period is added the ACT cost are moved to HST costs.
   Current period only.
*/

_CLASSDEF(TSCCostfList)
class TSCCostfList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTCostf Costf;
 TSCCostfList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTCostf c):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  Costf = c;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};
