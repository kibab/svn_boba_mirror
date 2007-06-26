#include "..\inc\swilib.h"
#include "main.h"
#include "clist_util.h"
#include "jabber_util.h"
#include "item_info.h"

#define MAX_SYMB 1024
WSHDR* ws_info = NULL;

int info_onkey(GUI *gui, GUI_MSG *msg)
{
 if (msg->keys==0x18)
  {
    return(-1); //do redraw
  }
  return(0); //Do standart keys

}

void info_ghook(GUI *gui, int cmd)
{
  //static SOFTKEY_DESC sk={0x0018, 0x0000,(int)"������"};
//  EDITCONTROL ec;

  if (cmd==2)
  {
    //Called after onCreate
  }
  if (cmd==7)
  {
#ifdef NEWSGOLD
    //SetSoftKey(gui,&sk,0);
#else
    //SetSoftKey(gui,&sk,2);
#endif
    //ExtractEditControl(gui,1,&ec);
    //wstrcpy(ws_info, ec.pWS);
  }

  if(cmd==0xA)
  {
    DisableIDLETMR();   // ��������� ������ ������ �� ��������
  }

  
  if(cmd==0x03)     // onDestroy
  {
    FreeWS(ws_info);
    ws_info = NULL;
  }
}

extern void inp_locret(void);
extern const char percent_t[];
extern const char percent_s[];
extern const char* PRESENCES[PRES_COUNT];
extern const char* JABBER_AFFS[];
extern const char* JABBER_ROLS[];
extern void patch_header(HEADER_DESC* head);
extern void patch_input(INPUTDIA_DESC* inp);


SOFTKEY_DESC info_sk[]=
{
  {0x0018,0x0000,(int)"�����"},
  {0x0001,0x0000,(int)"�����"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB info_skt=
{
  info_sk,0
};

INPUTDIA_DESC info_desc=
{
  1,
  info_onkey,
  info_ghook,
  (void *)inp_locret,
  0,
  &info_skt,
  {0,22,131,153},
  4,
  100,
  101,
  0,
  0,
  0x40000000// �������� ����-������
};

HEADER_DESC info_hdr={0,0,0,0,NULL,(int)"Contact Info",LGP_NULL};


// ����� ���������� � ��������
void Disp_Info(TRESOURCE* ResEx)
{
  
  // ������� ������� ���� � ���, �� ����� ������� ��� �� ����������
  CLIST* ClEx = CList_FindContactByJID(ResEx->full_name);
  
  // ������ �������� ������������� ������ ������� �����
  ws_info = AllocWS(MAX_SYMB);
  EDITCONTROL ec;
  void *ma=malloc_adr();
  void *eq;
  PrepareEditControl(&ec);
  eq=AllocEQueue(ma,mfree_adr());
  
  // ������ ����
/*
  T_NORMAL,       // ������� (������ ������)
  T_VIRTUAL,      // �������� ��� �������� ��������, ��� ��������
  T_CONF_ROOT,    // ������ ����������� (����� �������� ��������� ���������)
  T_CONF_NODE,    // �������� ����������� (���������� ������ "������" ����)
  T_TRANSPORT,    // ������������ �����
  T_GROUP   
  */  
  if(ResEx->entry_type==T_NORMAL)wsprintf(ws_info,percent_t,"������� �������");
  if(ResEx->entry_type==T_VIRTUAL)wsprintf(ws_info,percent_t,"������� ��� ������� �� ����������");
  if(ResEx->entry_type==T_CONF_ROOT)wsprintf(ws_info,percent_t,"�����������");
  if(ResEx->entry_type==T_CONF_NODE)wsprintf(ws_info,percent_t,"�������� �����������");
  if(ResEx->entry_type==T_TRANSPORT)wsprintf(ws_info,percent_t,"������ ������� �������� ����������� Jabber");
  if(ResEx->entry_type==T_GROUP)wsprintf(ws_info,percent_t,"������ �������");
  
  ConstructEditControl(&ec,1,0x40,ws_info,256);  
  AddEditControlToEditQend(eq,&ec,ma);
  
  if(ResEx->entry_type==T_NORMAL)
  {
    // JID 
    wsprintf(ws_info,percent_t,"JID:");
    ConstructEditControl(&ec,1,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);
    
    wsprintf(ws_info,percent_s,ClEx->JID);
    ConstructEditControl(&ec,3,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);

    // ������
    wsprintf(ws_info,percent_t,"������:");
    ConstructEditControl(&ec,1,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);
    
    wsprintf(ws_info,percent_s,ResEx->name);
    ConstructEditControl(&ec,3,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);
     
  }
  
  if(ResEx->entry_type==T_NORMAL || ResEx->entry_type==T_CONF_NODE)
  {
    // ������ � ��������� �������
    wsprintf(ws_info,percent_t,"������:");
    ConstructEditControl(&ec,1,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);
    if(ResEx->status_msg)
    {
      if(strlen(ResEx->status_msg)<512)
      {
        WSHDR *s=AllocWS(512);
        utf8_2ws(s, ResEx->status_msg, 512);
        wsprintf(ws_info,"%s: %w",PRESENCES[ResEx->status], s);
        FreeWS(s);
      }
    }
    else wsprintf(ws_info,percent_s,PRESENCES[ResEx->status]);
    ConstructEditControl(&ec,3,0x40,ws_info,600);
    AddEditControlToEditQend(eq,&ec,ma);    
  }
  
  
  if(ResEx->entry_type==T_CONF_NODE)
  {
    // Afflliation, role
    wsprintf(ws_info,percent_t,"����������:");
    ConstructEditControl(&ec,1,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);
    
    wsprintf(ws_info, percent_s, JABBER_AFFS[ResEx->muc_privs.aff]);
    ConstructEditControl(&ec,3,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);

    wsprintf(ws_info,percent_t,"����:");
    ConstructEditControl(&ec,1,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);
    
    wsprintf(ws_info, percent_s, JABBER_ROLS[ResEx->muc_privs.role]);
    ConstructEditControl(&ec,3,0x40,ws_info,256);
    AddEditControlToEditQend(eq,&ec,ma);     
  }  
  
  patch_header(&info_hdr);
  patch_input(&info_desc);
  CreateInputTextDialog(&info_desc,&info_hdr,eq,1,0);
}

//EOL,EOF
