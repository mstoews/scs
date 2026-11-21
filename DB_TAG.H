TAG4INFO glsysTag[] =
{
 {"fdbf","fdbf",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO allocTag[] =
{
 {"fcode","fcode",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO procTag[] =
{
 {"fcode","fcode",0,0,0},
 {"fkoutei","fkoutei",0,0,0},
 {"fcode1","fcode+fkoutei",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO cntreTag[] =
{
 {"fcode","fcode",0,0,0},
 {"fcode1","fcode",0,TRUE,0},
 {0,0,0,0,0},
};

static TAG4INFO cststTag[] =
{
   { "FOPCODE", "ftype+fopcode+fseqno", 0, 0, 0 },
   { "FPCODE",  "fopcode", 0, 0, 0 },
   { "FOPCODE2", "ftype+fopcode+fkpcode+fseqno", 0, 0, 0 },
   { "PROD",     "fopcode+ftype",0,TRUE,0},
   { 0,0,0,0,0 }
} ;

TAG4INFO elmntTag[] =
{
 {"fcode","fcode",0,0,0},
 {0,0,0,0,0},
};

static TAG4INFO glspcTag[] =
{
   { "FPCODE", "fpcode+account+dept", 0, 0, 0 },
   { "ACCOUNTS", "account+dept", 0, 0, 0 },
   { "UNIQUE", "fpcode", 0,TRUE, 0 },
   { 0,0,0,0,0 }
} ;


TAG4INFO itemmTag[] =
{
 {"elements","fpcode","fptype <> 'A' .OR. fptype <> 'B'",0,0},
 {"finished","fpcode+fskbn","fptype = 'A' .OR. fptype = 'B'",0,0},
 {"fpcode"  ,"fpcode",0,0,0},
 {"raw"     ,"fpcode","fptype = 'R' .OR. fptype = 'RI' .OR. fptype = 'W' .OR. fptype = 'WI'",0,0},
 {"units"   ,"funit",0,TRUE,0},
 {"man"     ,"fpcode","fptype = 'A' .OR. fptype = 'B' .OR. fptype = 'M' ",0,0},
 {"fshop1"   ,"fshop1",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO jissTag[] =
{
 {"fshop","fshop",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO typemTag[] =
{
 {"fcode","fcode",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO resrcTag[] =
{
 {"fcode","fcode+fcosttype",0,0,0},
 {"fname","fname",0,0,0},
 {"felmncd","felmncd",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO DistTag[]=
{
 {"account","account+dept",0,0,0},
 {"batchno","batchno",0,0,0},
 {"descript","descript",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO ReptTag[]=
{
 {"descript","descript",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO SectTag[]=
{
 {"process","process",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO SrccTag[]=
{
 {"Sourcecode","Sourcecode",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO glacctTag[]=
{
 {"account","account+dept",0,0,0},
 {"accounts","account+dept",0,0,0},
 {"dept","dept",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO gldeptTag[]=
{
 {"depart","depart",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO  thrmTag[]=
{
 {"fshop","fshop",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO  titemmTag[]=
{
 {"fpcode","fpcode",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO  tjissTag[]=
{
 {"fshop","fshop",0,0,0},
 {"fjcode","fjcode",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO  tkouteiTag[]=
{
 {"fkoutei","fkoutei",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO  tmmastTag[]=
{
 {"fpcode","fpcode",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO  tmrsltTag[]=
{
 {"fopcode1","fopcode+folotno",0,0,0},
 {"fopcode2","fopcode+fpcode+flotno",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO  tpstrTag[]=
{
 { "FOPCODE", "fopcode+fseqno", 0, 0, 0 },
 { "FPCODE",  "fopcode", 0, 0, 0 },
 { "FOPCODE2","fopcode+fkpcode+fseqno", 0, 0, 0 },
 { 0,0,0,0,0 }
};

TAG4INFO  accessTag[]=
{
 {"username","username",0,0,0},
 {0,0,0,0,0},
};

TAG4INFO  gl_tagTag[] =
{
 {"fdbf","fdbf",0,0,0},
 {0,0,0,0,0},
};


TAG4INFO  glcstTag[]=
{
 {"frescode","frescode",0,0,0},
 {0,0,0,0,0},
};
