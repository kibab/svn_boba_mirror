#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\cfg_items.h"
#include "conf_loader.h"


const wchar_t* successed_config_path = L"";
const wchar_t* successed_config_name = L"";

#pragma segment = "CONFIG_C"
int LoadConfigData( const wchar_t* path, const wchar_t* fname )
{
  int f;
  int result = - 1;
  void* cfg;
  FSTAT _fstat;
  unsigned int rlen;
  
  cfg = ( char* )__segment_begin( "CONFIG_C" );
  
  unsigned int len = ( char* )__segment_end( "CONFIG_C" ) - ( char* )__segment_begin( "CONFIG_C" );
  
  char* buf = new char[len];
  if ( buf )
  {
    if ( fstat( path, fname, &_fstat ) != - 1 )
    {
      if ( ( f = _fopen( path, fname, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
      {
        rlen = fread( f, buf, len );
        fclose( f );
        
        if ( rlen == _fstat.fsize && rlen == len )
        {
          memcpy( cfg, buf, len );
          result = 0;
        }
      }
    }
    if( result != 0 )
    {
      if ( ( f = _fopen( path, fname, FSX_O_WRONLY, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
      {
        if ( fwrite( f, cfg, len ) == len )
          result = 0;
        
        fclose( f );
      }
    }
    
    delete buf;
  }
  if ( result >= 0 )
  {
    successed_config_path = path;
    successed_config_name = fname;
  }
  return result;
}


void InitConfig( void )
{
  if ( LoadConfigData( GetDir( DIR_ELFS_CONFIG|MEM_EXTERNAL ), L"bookman.bcfg" ) < 0 )
  {
    LoadConfigData( GetDir( DIR_ELFS_CONFIG|MEM_INTERNAL ), L"bookman.bcfg" );
  }
}
