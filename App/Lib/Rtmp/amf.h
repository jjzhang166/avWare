#ifndef __AMF_H__
#define __AMF_H__
/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *      Copyright (C) 2008-2009 Andrej Stepanchuk
 *      Copyright (C) 2009-2010 Howard Chu
 *
 *  This file is part of librtmp.
 *
 *  librtmp is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1,
 *  or (at your option) any later version.
 *
 *  librtmp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with librtmp see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *  http://www.gnu.org/copyleft/lgpl.html
 */

#include <stdint.h>

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  typedef enum
  { AMF_NUMBER = 0, AMF_BOOLEAN, AMF_STRING, AMF_OBJECT,
    AMF_MOVIECLIP,		/* reserved, not used */
    AMF_NULL, AMF_UNDEFINED, AMF_REFERENCE, AMF_ECMA_ARRAY, AMF_OBJECT_END,
    AMF_STRICT_ARRAY, AMF_DATE, AMF_LONG_STRING, AMF_UNSUPPORTED,
    AMF_RECORDSET,		/* reserved, not used */
    AMF_XML_DOC, AMF_TYPED_OBJECT,
    AMF_AVMPLUS,		/* switch to AMF3 */
    AMF_INVALID = 0xff
  } AMFDataType;

  typedef enum
  { AMF3_UNDEFINED = 0, AMF3_NULL, AMF3_FALSE, AMF3_TRUE,
    AMF3_INTEGER, AMF3_DOUBLE, AMF3_STRING, AMF3_XML_DOC, AMF3_DATE,
    AMF3_ARRAY, AMF3_OBJECT, AMF3_XML, AMF3_BYTE_ARRAY
  } AMF3DataType;

  typedef struct AVal
  {
    char *av_val;
    int av_len;
  } AVal;
#define AVC(str)	{str,sizeof(str)-1}
#define AVMATCH(a1,a2)	((a1)->av_len == (a2)->av_len && !memcmp((a1)->av_val,(a2)->av_val,(a1)->av_len))

  struct AMFObjectProperty;

  typedef struct AMFObject
  {
    int o_num;
    struct AMFObjectProperty *o_props;
  } AMFObject;

  typedef struct AMFObjectProperty
  {
    AVal p_name;
    AMFDataType p_type;
    union
    {
      double p_number;
      AVal p_aval;
      AMFObject p_object;
    } p_vu;
    int16_t p_UTCoffset;
  } AMFObjectProperty;
#ifdef WIN32
#ifndef _EXPORT_API
#define _EXPORT_API __declspec(dllexport) 
#endif
#else
#ifndef _EXPORT_API
#define _EXPORT_API
#endif
#endif
 _EXPORT_API char *AMF_EncodeString(char *output, char *outend, const AVal * str);
 _EXPORT_API char *AMF_EncodeNumber(char *output, char *outend, double dVal);
 _EXPORT_API char *AMF_EncodeInt16(char *output, char *outend, short nVal);
 _EXPORT_API char *AMF_EncodeInt24(char *output, char *outend, int nVal);
 _EXPORT_API char *AMF_EncodeInt32(char *output, char *outend, int nVal);
 _EXPORT_API char *AMF_EncodeBoolean(char *output, char *outend, int bVal);

  /* Shortcuts for AMFProp_Encode */
 _EXPORT_API char *AMF_EncodeNamedString(char *output, char *outend, const AVal * name, const AVal * value);
 _EXPORT_API char *AMF_EncodeNamedNumber(char *output, char *outend, const AVal * name, double dVal);
 _EXPORT_API char *AMF_EncodeNamedBoolean(char *output, char *outend, const AVal * name, int bVal);

 _EXPORT_API unsigned short AMF_DecodeInt16(const char *data);
 _EXPORT_API unsigned int AMF_DecodeInt24(const char *data);
 _EXPORT_API unsigned int AMF_DecodeInt32(const char *data);
 _EXPORT_API void AMF_DecodeString(const char *data, AVal * str);
 _EXPORT_API void AMF_DecodeLongString(const char *data, AVal * str);
 _EXPORT_API int AMF_DecodeBoolean(const char *data);
 _EXPORT_API double AMF_DecodeNumber(const char *data);

 _EXPORT_API char *AMF_Encode(AMFObject * obj, char *pBuffer, char *pBufEnd);
 _EXPORT_API int AMF_Decode(AMFObject * obj, const char *pBuffer, int nSize,
		 int bDecodeName);
 _EXPORT_API int AMF_DecodeArray(AMFObject * obj, const char *pBuffer, int nSize,
		      int nArrayLen, int bDecodeName);
 _EXPORT_API int AMF3_Decode(AMFObject * obj, const char *pBuffer, int nSize,
		  int bDecodeName);
 _EXPORT_API void AMF_Dump(AMFObject * obj);
 _EXPORT_API void AMF_Reset(AMFObject * obj);

 _EXPORT_API void AMF_AddProp(AMFObject * obj, const AMFObjectProperty * prop);
 _EXPORT_API int AMF_CountProp(AMFObject * obj);
 _EXPORT_API AMFObjectProperty *AMF_GetProp(AMFObject * obj, const AVal * name,
				 int nIndex);

 _EXPORT_API AMFDataType AMFProp_GetType(AMFObjectProperty * prop);
 _EXPORT_API void AMFProp_SetNumber(AMFObjectProperty * prop, double dval);
 _EXPORT_API void AMFProp_SetBoolean(AMFObjectProperty * prop, int bflag);
 _EXPORT_API void AMFProp_SetString(AMFObjectProperty * prop, AVal * str);
 _EXPORT_API void AMFProp_SetObject(AMFObjectProperty * prop, AMFObject * obj);

 _EXPORT_API void AMFProp_GetName(AMFObjectProperty * prop, AVal * name);
 _EXPORT_API void AMFProp_SetName(AMFObjectProperty * prop, AVal * name);
 _EXPORT_API double AMFProp_GetNumber(AMFObjectProperty * prop);
 _EXPORT_API int AMFProp_GetBoolean(AMFObjectProperty * prop);
 _EXPORT_API void AMFProp_GetString(AMFObjectProperty * prop, AVal * str);
 _EXPORT_API void AMFProp_GetObject(AMFObjectProperty * prop, AMFObject * obj);

 _EXPORT_API int AMFProp_IsValid(AMFObjectProperty * prop);

 _EXPORT_API char *AMFProp_Encode(AMFObjectProperty * prop, char *pBuffer, char *pBufEnd);
 _EXPORT_API int AMF3Prop_Decode(AMFObjectProperty * prop, const char *pBuffer,
		      int nSize, int bDecodeName);
 _EXPORT_API int AMFProp_Decode(AMFObjectProperty * prop, const char *pBuffer,
		     int nSize, int bDecodeName);

 _EXPORT_API void AMFProp_Dump(AMFObjectProperty * prop);
 _EXPORT_API void AMFProp_Reset(AMFObjectProperty * prop);

  typedef struct AMF3ClassDef
  {
    AVal cd_name;
    char cd_externalizable;
    char cd_dynamic;
    int cd_num;
    AVal *cd_props;
  } AMF3ClassDef;

 _EXPORT_API void AMF3CD_AddProp(AMF3ClassDef * cd, AVal * prop);
 _EXPORT_API AVal *AMF3CD_GetProp(AMF3ClassDef * cd, int idx);

#ifdef __cplusplus
}
#endif

#endif				/* __AMF_H__ */
