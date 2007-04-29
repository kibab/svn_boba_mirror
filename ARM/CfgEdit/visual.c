#include "..\inc\swilib.h"

char colors[4][4]={{0xFF,0,0,0x64},{0,0xFF,0,0x64},{0,0,0xFF,0x64},{0xC6,0xAA,0xAF,0x32}};
extern long  strtol (const char *nptr,char **endptr,int base);
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

//===============================================================================================
// ELKA Compatibility
#pragma inline
void patch_header(HEADER_DESC* head)
{
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}
#pragma inline
void patch_input(INPUTDIA_DESC* inp)
{
  inp->rc.x=0;
  inp->rc.y=HeaderH()+1+YDISP;
  inp->rc.x2=ScreenW()-1;
  inp->rc.y2=ScreenH()-SoftkeyH()-1;
}
//===============================================================================================

typedef struct
{
  GUI gui;
  WSHDR*ws1;
  int x_pos;
  int y_pos;
  unsigned int* xy_pos;
  int cstep;
}MAIN_GUI_1;

typedef struct
{
  GUI gui;
  WSHDR*ws1; 
  int r;
  int g;
  int b;
  int a;
  char* color;
  int current_column;
  char testcolor[4];
  int cstep;
}MAIN_GUI_2;

const char Pointer[5]={0x27,0x27,0xFF,0x27,0x27};
const IMGHDR imgPointer = {5,5,0x1,(char *)Pointer};

void DrwImg(IMGHDR *img, int x, int y, char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}


void DrawWhiteScreen(void)
{
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,
                   GetPaletteAdrByColorIndex(0),
                   GetPaletteAdrByColorIndex(0));
}  

void method0_1(MAIN_GUI_1 *data)
{
  DrawWhiteScreen();
  int scr_w=ScreenW();
  int scr_h=ScreenH();
  for (int y_0=0; y_0< scr_h;y_0+=10)
  {
    DrawLine(0,y_0,scr_w-1,y_0,1,colors[3]);
  }
  
  for (int x_0=0; x_0<scr_w;x_0+=10)
  {
    DrawLine(x_0,0,x_0, scr_h-1,1,colors[3]);
  }
  wsprintf(data->ws1,"%u,%u",data->x_pos,data->y_pos);
  DrwImg((IMGHDR *)&imgPointer,data->x_pos-2,data->y_pos-2,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(23));
  DrawString(data->ws1,3,scr_h-GetFontYSIZE(SMALL_FONT)-1,scr_w-4,scr_h-1,SMALL_FONT,1,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(23));
}

void method1_1(MAIN_GUI_1 *data, void *(*malloc_adr)(int))
{
  data->ws1=AllocWS(256);
  data->gui.state=1;
}

void method2_1(MAIN_GUI_1 *data, void (*mfree_adr)(void *))
{
  FreeWS(data->ws1);
  data->gui.state=0;
}

void method3_1(MAIN_GUI_1 *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
}

void method4_1(MAIN_GUI_1 *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}
/*
#define MIN_STEP 1
#define MAX_STEP 8
*/

int method5_1(MAIN_GUI_1 *data, GUI_MSG *msg)
{
  if ((msg->gbsmsg->msg==KEY_DOWN)||(msg->gbsmsg->msg==LONG_PRESS))
  {
    if (msg->gbsmsg->msg==KEY_DOWN)
    {
       switch(msg->gbsmsg->submess)
       {
       case RIGHT_SOFT:
       case RED_BUTTON:
         return (1);
         
       case ENTER_BUTTON:
         data->xy_pos[0]=data->x_pos;
         data->xy_pos[1]=data->y_pos;
         return (1);
       }
    }
    if (msg->gbsmsg->msg==LONG_PRESS)
      data->cstep=5;
    
    switch(msg->gbsmsg->submess)
    {
    case '1':
      if ((data->x_pos-=data->cstep)<0)
        data->x_pos=0;
      if ((data->y_pos-=data->cstep)<0)
        data->y_pos=0;
      break;
      
    case '2':
    case UP_BUTTON:
      if ((data->y_pos-=data->cstep)<0)
        data->y_pos=0;
      break;
      
    case '3':
      if ((data->x_pos+=data->cstep)>ScreenW()-1)
        data->x_pos=ScreenW()-1;
      if ((data->y_pos-=data->cstep)<0)
        data->y_pos=0;
      break;
     
    case '4':
    case LEFT_BUTTON:
      if ((data->x_pos-=data->cstep)<0)
        data->x_pos=0;
      break;
      
    case '6':
    case RIGHT_BUTTON:
      if ((data->x_pos+=data->cstep)>ScreenW()-1)
        data->x_pos=ScreenW()-1;
      break;
      
    case '7': 
      if ((data->x_pos-=data->cstep)<0)
        data->x_pos=0;      
      if ((data->y_pos+=data->cstep)>ScreenH()-1)
        data->y_pos=ScreenH()-1;
      break;
      
    case '8':
    case DOWN_BUTTON:
      if ((data->y_pos+=data->cstep)>ScreenH()-1)
        data->y_pos=ScreenH()-1;
      break;
      
    case '9': 
      if ((data->x_pos+=data->cstep)>ScreenW()-1)
        data->x_pos=ScreenW()-1;
      if ((data->y_pos+=data->cstep)>ScreenH()-1)
        data->y_pos=ScreenH()-1;
      break; 
    }
  }
  if (msg->gbsmsg->msg==KEY_UP)
    data->cstep=1;
  
  DirectRedrawGUI();
  return(0);
}

void method7_1(MAIN_GUI_1 *data, void (*mfree_adr)(void *))
{
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(data,mfree_adr);
}

int method8_1(void){return(0);}

int method9_1(void){return(0);}

const void * const gui_methods_1[11]={
  (void *)method0_1,	//Redraw
  (void *)method1_1,	//Create
  (void *)method2_1,	//Close
  (void *)method3_1,	//Focus
  (void *)method4_1,	//Unfocus
  (void *)method5_1,	//OnKey
  0,
  (void *)method7_1,	//Destroy
  (void *)method8_1,
  (void *)method9_1,
  0
};

const RECT Canvas_1={0,0,0,0};

void EditCoordinates(unsigned int*xy)
{
  MAIN_GUI_1 *main_gui=malloc(sizeof(MAIN_GUI_1));
  zeromem(main_gui,sizeof(MAIN_GUI_1));
  main_gui->xy_pos=xy;
  main_gui->x_pos=xy[0];
  main_gui->y_pos=xy[1];
  patch_rect((RECT*)&Canvas_1,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas_1);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods_1;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
}


void method0_2(MAIN_GUI_2 *data)
{
  DrawWhiteScreen();
  int scr_w=ScreenW();
  int scr_h=ScreenH();
  int column_height=scr_h-35;
  int column_width=scr_w/9;
  int start_column;
  int y_line;
  wsprintf(data->ws1,"%02X,%02X,%02X,%02X",data->r,data->g,data->b,data->a);
  DrawString(data->ws1,1,1,scr_w-20,12,SMALL_FONT,1,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(23));
  
  for (int i=0;i!=4;i++)
  {
    start_column=column_width+2*i*column_width;
    if (data->current_column==i)
      DrawRectangle(start_column-2,20-2,start_column+column_width+2,20+column_height+2,
                    0,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(0));

    DrawRectangle(start_column,20,start_column+column_width,20+column_height,
                  0,GetPaletteAdrByColorIndex(1),colors[i]);
    switch(i)
    {
    case 0:
      y_line=20+column_height-(data->r*column_height)/0xFF;
      break;
    case 1:
      y_line=20+column_height-(data->g*column_height)/0xFF;
      break;      
    case 2:
      y_line=20+column_height-(data->b*column_height)/0xFF;
      break;
    case 3:
      y_line=20+column_height-(data->a*column_height)/0x64;
      break;
    }
    DrawLine(start_column,y_line,start_column+column_width,y_line,0,GetPaletteAdrByColorIndex(1));
  }
  setColor(data->r,data->g,data->b,data->a,data->testcolor);
  DrawRoundedFrame(scr_w-17,1,scr_w-2,16,2,2,0,GetPaletteAdrByColorIndex(1),data->testcolor);

}

void method1_2(MAIN_GUI_2 *data, void *(*malloc_adr)(int))
{
  data->ws1=AllocWS(256);
  data->gui.state=1;
}

void method2_2(MAIN_GUI_2 *data, void (*mfree_adr)(void *))
{
  FreeWS(data->ws1);
  data->gui.state=0;
}

void method3_2(MAIN_GUI_2 *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
}

void method4_2(MAIN_GUI_2 *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int method5_2(MAIN_GUI_2 *data, GUI_MSG *msg)
{
  if ((msg->gbsmsg->msg==KEY_DOWN)||(msg->gbsmsg->msg==LONG_PRESS))
  {
    if (msg->gbsmsg->msg==KEY_DOWN)
    {
       switch(msg->gbsmsg->submess)
       {
       case RIGHT_SOFT:
       case RED_BUTTON:
         return (1);
         
       case ENTER_BUTTON:
         setColor(data->r,data->g,data->b,data->a,data->color);
         return (1);
       }
    }
    if (msg->gbsmsg->msg==LONG_PRESS)  
      data->cstep=8;

    switch(msg->gbsmsg->submess)
    {
    case UP_BUTTON:
    case '2':
      switch(data->current_column)
      {
      case 0:
        if ((data->r+=data->cstep)>0xFF)
          data->r=0;
        break;
      case 1:
        if ((data->g+=data->cstep)>0xFF)
          data->g=0;
        break;
      case 2:
        if ((data->b+=data->cstep)>0xFF)
          data->b=0;
        break;
      case 3:
        if ((data->a+=(data->cstep==8?data->cstep>>1:data->cstep))>0x64)
          data->a=0;
        break;
      }
      break;
      
    case LEFT_BUTTON:
    case '4':
      if (--data->current_column<0)
        data->current_column=3;
      break;  
    case RIGHT_BUTTON:
    case '6':
      if (++data->current_column>3)
        data->current_column=0;
      break;
      
    case DOWN_BUTTON:
    case '8':
      switch(data->current_column)
      {
      case 0:
        if ((data->r-=data->cstep)<0)
          data->r=0xFF;
        break;
      case 1:
        if ((data->g-=data->cstep)<0)
          data->g=0xFF;
        break;
      case 2:
        if ((data->b-=data->cstep)<0)
          data->b=0xFF;
        break;
      case 3:
        if ((data->a-=(data->cstep==8?data->cstep>>1:data->cstep))<0)
          data->a=0x64;
        break;
      }
      break;
    }
  }
  if (msg->gbsmsg->msg==KEY_UP)
    data->cstep=1;
  DirectRedrawGUI();
  return(0);
}


void method7_2(MAIN_GUI_2 *data, void (*mfree_adr)(void *))
{
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(data,mfree_adr);
}

int method8_2(void){return(0);}

int method9_2(void){return(0);}


const void * const gui_methods_2[11]={
  (void *)method0_2,	//Redraw
  (void *)method1_2,	//Create
  (void *)method2_2,	//Close
  (void *)method3_2,	//Focus
  (void *)method4_2,	//Unfocus
  (void *)method5_2,	//OnKey
  0,
  (void *)method7_2,	//Destroy
  (void *)method8_2,
  (void *)method9_2,
  0
};

const RECT Canvas_2={0,0,0,0};

void EditColors(char*color)
{
  MAIN_GUI_2 *main_gui=malloc(sizeof(MAIN_GUI_2));
  zeromem(main_gui,sizeof(MAIN_GUI_2));
  main_gui->r=color[0];
  main_gui->g=color[1];
  main_gui->b=color[2];
  main_gui->a=color[3];
  main_gui->color=color;
  patch_rect((RECT*)&Canvas_2,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas_2);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods_2;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
}

typedef struct
{
  void *next;
  char *fullname;
  char *name;
  int is_folder;
}FLIST;

typedef struct
{
  void *gui;
  int type;
}FVIEW;

volatile FLIST *fltop;


void Free_FLIST(void)
{
  LockSched();
  FLIST *fl=(FLIST *)fltop;
  fltop=0;
  UnlockSched();
  while(fl)
  {
    FLIST *fl_prev;
    fl_prev=fl;
    fl=fl->next;
    mfree(fl_prev->fullname);
    mfree(fl_prev->name);
    mfree(fl_prev);
  }
}

#define IS_FOLDER 1
#define IS_FILE 0

const char back[]="..";             
int GetFListN()
{
  int i=0;
  FLIST *fl=(FLIST*)&fltop;
  while((fl=fl->next)) i++;
  return (i);
}

int GetFoldersLevel(char *name)
{
  int i=0;
  char *s=name;
  while(*s++)
  {
    if (*s=='\\' &&*(s+1)!=0) i++;    
  }
  return (i);
}

int strcmp_nocase(const char *s, const char *d)
{
  int cs;
  int ds;
  do
  {
    cs=*s++;
    if (cs&0x40) cs&=0xDF;
    ds=*d++;
    if (ds&0x40) ds&=0xDF;
    cs-=ds;
    if (cs) break;
  }
  while(ds);
  return(cs);
}

FLIST *AddToFList(const char* full_name, const char *name, int is_folder)
{
  FLIST *fl;
  FLIST *pr;
  FLIST *fn=malloc(sizeof(FLIST));
  fn->fullname=malloc(strlen(full_name)+1);
  strcpy(fn->fullname,full_name);
  
  fn->name=malloc(strlen(name)+1);
  strcpy(fn->name,name);
  
  fn->is_folder=is_folder;
  fn->next=0;
  fl=(FLIST *)fltop;
  if (fl)
  {
    pr=(FLIST *)&fltop;
    while(strcmp_nocase(fl->name,fn->name)<0)
    {
      pr=fl;
      fl=fl->next;
      if (!fl) break;
    }
    fn->next=fl;
    pr->next=fn;
  }
  else
  {
    fltop=fn;
  }
  return (fn);  
}

void FindFiles(char *str, int type)  // type == 0 SelectFolder, type == 1 SelectFile
{
  DIR_ENTRY de;
  unsigned int err;
  int i, c;
  char name[256]; 
  char fname[128];

  Free_FLIST();
  strcpy(name,str);
  strcat(name,"*");
  
  i=GetFoldersLevel(str);
  if (i==0)
  {
    AddToFList("ROOT",back,IS_FOLDER);
  }
  else
  {
    char *s=str;
    char *d=fname;
    for (int k=0; k!=i && *s; )
    {
      c=*s++;
      *d++=c;
      if (c=='\\')  k++;
    }
    *d=0;
    AddToFList(fname,back,IS_FOLDER);
  }

  if (FindFirstFile(&de,name,&err))
  {
    do
    {
      strcpy(name,de.folder_name);
      strcat(name,"\\");
      strcat(name,de.file_name);
      if (de.file_attr&FA_DIRECTORY)
      {
        strcpy(fname,"\\");
        strcat(fname,de.file_name);
        strcat(name,"\\");
        AddToFList(name,fname,IS_FOLDER);
      }
      else
      {
        if (type!=0)
        {
          AddToFList(name,de.file_name,IS_FILE);
        }
      }
    }
    while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
}



FLIST *FindFLISTtByNS(int *i, int si)
{
  FLIST *fl;
  fl=(FLIST *)fltop;
  while(fl)
  {
    if (fl->is_folder==si)
    {
      if (!(*i)) return (fl);
      (*i)--;
    }    
    fl=fl->next;
  }
  return fl;
}
  
FLIST *FindFLISTtByN(int n)
{
  FLIST *fl;
  fl=FindFLISTtByNS(&n,IS_FOLDER); if ((!n)&&(fl)) return(fl);
  fl=FindFLISTtByNS(&n,IS_FILE); if ((!n)&&(fl)) return(fl);
  return fl;
}




void SavePath(void *ed_gui, FLIST *fl)
{
  EDITCONTROL ec;
  EDIT_ExtractFocusedControl(ed_gui,&ec);
  WSHDR *ws=AllocWS(ec.maxlen);
  str_2ws(ws,fl->fullname,ec.maxlen);
  EDIT_SetTextToFocused(ed_gui,ws);
  FreeWS(ws);
}
  

char header[];
int filelist_menu_onkey(void *data, GUI_MSG *msg)
{
  FVIEW *fview=MenuGetUserPointer(data);
  FLIST *fl;
  int i;
  i=GetCurMenuItem(data);
  fl=FindFLISTtByN(i);
  
  if (msg->keys==0x3D)
  {
    if (fl) 
    {
      if (fl->is_folder==IS_FOLDER)
      {
        int len;
        if (strcmp(fl->fullname,"ROOT"))
        {
          strncpy(header,fl->fullname,127);
          len=strlen(fl->fullname);
          header[len>127?127:len]=0;
          FindFiles(fl->fullname,fview->type);
        }
        else
        {
          void CreateRootMenu();
          CreateRootMenu();
        }         
        Menu_SetItemCountDyn(data,GetFListN());
        SetCursorToMenuItem(data, 0);
        RefreshGUI();
      }
      else
      {
        SavePath(fview->gui,fl);        
        return (1);
      }
    }
    //    GeneralFunc_F1(1);
    return(-1);
  }
  if (msg->keys==0x18)
  {
    if (fl)
    {
      if (strcmp(fl->name,back))
      {
        SavePath(fview->gui,fl);
        return (1);
      }      
    }   
    return (-1);
  }
  return (0);
}

void filelist_menu_ghook(void *data, int cmd)
{
  FVIEW *fview=MenuGetUserPointer(data);
  if (cmd==3)
  {
    Free_FLIST();
    mfree(fview);    
  }
}

void filelist_menu_iconhndl(void *data, int curitem, void *user_pointer)
{
  FLIST *fl;
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  fl=FindFLISTtByN(curitem);
  if (fl)
  {
    if (fl->is_folder)
    {
      ws=AllocMenuWS(data,strlen(fl->name)+2);
      str_2ws(ws,fl->name,128);
      wsInsertChar(ws,0x0002,1);
      wsInsertChar(ws,0xE008,1);
    }
    else
    {
      ws=AllocMenuWS(data,strlen(fl->name));
      str_2ws(ws,fl->name,128);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws, "������");
  }
  SetMenuItemText(data, item, ws, curitem);
}

int fmenusoftkeys[]={0,1,2};
SOFTKEY_DESC fmenu_sk[]=
{
  {0x0018,0x0000,(int)"Select"},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)"+"}
};

SOFTKEYSTAB fmenu_skt=
{
  fmenu_sk,0
};
char header[128];
HEADER_DESC filelist_HDR={0,0,0,0,NULL,(int)header,LGP_NULL};


MENU_DESC filelist_STRUCT=
{
  8,(void *)filelist_menu_onkey,(void*)filelist_menu_ghook,NULL,
  fmenusoftkeys,
  &fmenu_skt,
  0x10,
  (void *)filelist_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

void CreateRootMenu()
{
  Free_FLIST();
  AddToFList("0:\\","0:\\",IS_FOLDER);
  AddToFList("1:\\","1:\\",IS_FOLDER);
  AddToFList("2:\\","2:\\",IS_FOLDER);
  AddToFList("4:\\","4:\\",IS_FOLDER);
  strcpy(header,"Root");
}


void open_select_file_gui(void *ed_gui, int type)
{
  EDITCONTROL ec;
  FVIEW *fview;
  char *s;
  fview=malloc(sizeof(FVIEW));
  fview->gui=ed_gui;
  fview->type=type;
  EDIT_ExtractFocusedControl(ed_gui,&ec);
  if (ec.pWS->wsbody[0]==0)
  {
    CreateRootMenu();
  }
  else
  {
    ws_2str(ec.pWS,header,127);
    s=strrchr(header, '\\');
    if (s) *(s+1)=0;
    int len=strlen(header);
    header[len>127?127:len]=0;
    FindFiles(header,type);
  }    
  patch_header(&filelist_HDR);
  CreateMenu(0,0,&filelist_STRUCT,&filelist_HDR,0,GetFListN(),fview,0);
}
