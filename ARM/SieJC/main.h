#ifndef _MAIN_H_
  #define _MAIN_H_

#include "jabber.h"

////////////////////////////////////////////////////////////////////////////////
//////////////////// ��������� �������������� ��������� ������� ////////////////
//#define LOG_ALL       // �������� ���������������� �������� / ��������� �������
                        // ����� ��������� ���, ����� �� ��������

//#define LOG_XML_TREE    // �������� ������ ������������ XML-������
#ifdef NEWSGOLD
//#define LOG_TO_COM_PORT // ��� ������ � ���-���� (�� DCA-510) - ������ NEWSGOLD
#endif
//#define LOG_IN_DATA     // �������� ������ �������������� ������ ������

#define VIBRA_POWER 10 // ���� ����� ��� ������� ������� ����

#define USE_PNG_EXT   // ������������ �� ���������� PNG ��� �������� (ELFLoader >2.0)
////////////////////////////////////////////////////////////////////////////////

#define TMR_SECOND 216

// ����� ������� ������ MAIN.C

#define color(x) (char *)(&(x))

#define SCR_START YDISP


typedef struct
{
  int buf_size;
  char* xml_buffer;
} IPC_BUFFER;


typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

typedef enum
{
  JS_NOT_CONNECTED,           // ��� ����������, ��� ������
  JS_CONNECTED_STATE,         // ������� stream:stream
  JS_ZLIB_INIT_ACK,           // �������� ������������� ������ ZLib
  JS_ZLIB_STREAM_INIT_ACK,    // �������� ��������� ZLib-������
  JS_SASL_NEGOTIATION,        // ����� ������������� SASL
  JS_SASL_NEG_ANS_WAIT,       // �������� ������ �� ������� �� ��������� SASL
  JS_SASL_AUTH_ACK,           // �������� <success/> ����� ������ �� rspauth
  JS_SASL_NEW_STREAM_ACK,     // �������� ������������� ������ ������
  JS_SASL_RESBIND_ACK,        // �������� �������� �������
  JS_SASL_SESS_INIT_ACK,      // �������� ������������� ������
  JS_AUTH_OK,                 // ��-SASL: �������� �����������
  JS_AUTH_ERROR,              // ������ ������������
  JS_ONLINE,                  // ������� ��������� ��� ������������� �����������
  JS_ERROR                    // ����� �������
} JABBER_STATE;

////////////////////////////////
// ���-����� ���� ��������� �� http://www.xmpp.org/rfcs/rfc3921.html � + ����
typedef enum
{
  MSG_NORMAL, // ���� "������", �� �������, See RFC
  MSG_ME,     // �� ���� ��������
  MSG_CHAT,   // �� �������� �� ���
  MSG_GCHAT,   // �����������
  MSG_SYSTEM,  // ������� ���� ��������� ���������
  MSG_STATUS,   // ��������� ���������
  MSG_SUBJECT   // ���� �����������
} MESS_TYPE;


// ���� ����� � ������ ���������
typedef enum
{
  T_NORMAL,       // ������� (������ ������)
  T_VIRTUAL,      // �������� ��� �������� ��������, ��� ��������
  T_CONF_ROOT,    // ������ ����������� (����� �������� ��������� ���������)
  T_CONF_NODE,    // �������� ����������� (���������� ������ "������" ����)
  T_TRANSPORT,    // ������������ �����
  T_GROUP         // ������
}RES_TYPE;

typedef struct
{
  MESS_TYPE mtype;
  char *muc_author;// ����� ����� ������������ ������ � ������������
  char *mess;
  void *next;
}LOG_MESSAGE;

typedef struct
{
  JABBER_GC_AFFILIATION aff;
  JABBER_GC_ROLE role;
  char *real_jid;
}CONF_DATA;

typedef struct
{
  char* name;                   // ������������ ��� �������
  char* full_name;              // ������ JID (jid/resource)
  RES_TYPE entry_type;          // ��� ������ (��. RES_TYPE)
  char status;                  // ������
  char* status_msg;             // ��������� ���������
  unsigned int has_unread_msg;  // ���� �� ������������� � �������
  unsigned int total_msg_count; // ����� ���������� ���������
  LOG_MESSAGE* log;             // ������ ���� ���������
  CONF_DATA muc_privs;          // ��� ��� ��������������� ��������� - ��� ���� � ����-���
  void* next;                   // ��������� ���������
} TRESOURCE;

typedef struct
{
  char* JID;
  char IsVisible;             // ���������� �� � ������ (������� � ��������)
  int ResourceCount;
  TRESOURCE* res_list;
  char* name;
  JABBER_SUBSCRIPTION subscription;
  char wants_subscription;
  char group;     // >128 -> �����������
  void *next;
} CLIST;


// ��� ������ �����������
typedef struct
{
  char* conf_jid; //UTF-8
  void* next;
}MUC_ITEM;

// ��� ������� ����� ���������� ���������� � ����
typedef struct
{
  short priority;
  char status;
  char* message;
}PRESENCE_INFO;

typedef struct
{
  char r;
  char g;
  char b;
  char a;
}RGBA;

#ifdef NEWSGOLD
#define CBOX_CHECKED 0xE116
#define CBOX_UNCHECKED 0xE117
#else
#define CBOX_CHECKED 0xE10B
#define CBOX_UNCHECKED 0xE10C
#endif

void SendAnswer(char *str);

void Vibrate(int Req_Vibra_Count);

void Play(const char *fname);

void QuitCallbackProc(int decision);

void patch_rect(RECT*rc,int x,int y, int x2, int y2);
void patch_header(HEADER_DESC* head);
void patch_input(INPUTDIA_DESC* inp);
void SMART_REDRAW(void);
#endif
