#include "scs.h"
#include <alloc.h>
/* Database Files */

#include "db_topic.h"
#include "db_mrslt.h"
#include "db_jiss.h"
#include "db_resrc.h"
#include "db_trstr.h"
#include "db_ctemp.h"
#include "sc_topic.h"
#include "sc_trans.h"

BOOL TTStruct::PeriodScope(int dPeriod)
{
  char *szStart = new char[40];
  char *szEnd   = new char[40]; 

  profile = new Tprofile(&cb);
  profile->top();

  Date4 StartDate;
  Date4 EndDate;
  switch(dPeriod)
    {
    case 1: StartDate.assign(profile->period1->str());EndDate.assign(profile->period1e->str()); break;
    case 2: StartDate.assign(profile->period2->str());EndDate.assign(profile->period2e->str()); break;
    case 3: StartDate.assign(profile->period3->str());EndDate.assign(profile->period3e->str()); break;
    case 4: StartDate.assign(profile->period4->str());EndDate.assign(profile->period4e->str()); break;
    case 5: StartDate.assign(profile->period5->str());EndDate.assign(profile->period5e->str()); break;
    case 6: StartDate.assign(profile->period6->str());EndDate.assign(profile->period6e->str()); break;
    case 7: StartDate.assign(profile->period7->str());EndDate.assign(profile->period7e->str()); break;
    case 8: StartDate.assign(profile->period8->str());EndDate.assign(profile->period8e->str()); break;
    case 9: StartDate.assign(profile->period9->str());EndDate.assign(profile->period9e->str()); break;
    case 10:StartDate.assign(profile->period10->str());EndDate.assign(profile->period10e->str()); break;
    case 11:StartDate.assign(profile->period11->str());EndDate.assign(profile->period11e->str()); break;
    case 12:StartDate.assign(profile->period12->str());EndDate.assign(profile->period12e->str()); break;
    default:StartDate.assign(profile->period1->str());EndDate.assign(profile->period1e->str()); break;
   }
 StartDate.format("MM/DD/YY");
 EndDate.format("MM/DD/YY");
 //
 
 strcpy(szStart,"Starting Date :");
 strcat(szStart,StartDate.str());
 strcpy(szEnd,"Ending Date  :");
 strcat(szEnd,EndDate.str());
 SetField(106,szStart);
 SetField(103,szEnd);
}
