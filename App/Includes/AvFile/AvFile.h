/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/

#ifndef __AVFILE_H__
#define __AVFILE_H__

#include <string>
#ifdef WIN32
#include <io.h>
#endif

#ifdef __TCS__
#include <time.h>
#endif

#if 1
#ifndef __XC_TYPES_H__
#define __XC_TYPES_H__

typedef signed char                schar;
typedef unsigned char              uchar;    
typedef unsigned int               uint;
typedef unsigned short             ushort;
typedef unsigned long              ulong;
#ifdef WIN32
typedef __int64                    int64;
typedef unsigned __int64           uint64;
#elif defined(__GNUC__)
typedef unsigned long long         int64;
typedef long long                  uint64;
#elif defined(__TCS__)
typedef signed   long long int     int64;
typedef unsigned long long int     uint64;
#endif

#endif// 
#endif

#ifndef FILE_INFO_DEFINED
typedef struct _FileInfo
{
    char    name[108];
    unsigned int    attrib;
    uint64    time;
    uint64    size;
}FileInfo;
#define FILE_INFO_DEFINED
#endif

struct FSOperations;

class CFile
{
public:

    /// 文件打开标志
    enum OpenFlags {
        modeRead =         (int) 0x00000,
        modeWrite =        (int) 0x00001,
        modeReadWrite =    (int) 0x00002,
        shareCompat =      (int) 0x00000,
        shareExclusive =   (int) 0x00010,
        shareDenyWrite =   (int) 0x00020,
        shareDenyRead =    (int) 0x00030,
        shareDenyNone =    (int) 0x00040,
        modeNoInherit =    (int) 0x00080,
        modeCreate =       (int) 0x01000,
        modeNoTruncate =   (int) 0x02000,
        typeText =         (int) 0x04000,
        typeBinary =       (int) 0x08000,
        osNoBuffer =       (int) 0x10000,
        osWriteThrough =   (int) 0x20000,
        osRandomAccess =   (int) 0x40000,
        osSequentialScan = (int) 0x80000,
    };

    /// 文件属性
    enum Attribute {
        normal =    0x00,
        readOnly =  0x01,
        hidden =    0x02,
        system =    0x04,
        volume =    0x08,
        directory = 0x10,
        archive =   0x20
    };

    /// 文件定位参考位置
    enum SeekPosition
    {
        begin = 0x0,
        current = 0x1,
        end = 0x2 
    };

    CFile();
    virtual ~CFile();


    virtual bool Open(const char* pFileName, unsigned int dwFlags = modeReadWrite);
    virtual void Close();
    unsigned char * Load(const char* pFileName);
    void UnLoad();
    virtual unsigned int Read(void *pBuffer, unsigned int dwCount);
    virtual unsigned int Write(void *pBuffer, unsigned int dwCount);
    virtual void Flush();
    virtual unsigned int Seek(long lOff, unsigned int nFrom);
    virtual unsigned int GetPosition();
    virtual unsigned int GetLength();
    virtual char * Gets(char *s, int size);
    virtual int Puts(char *s);
    virtual bool IsOpened();
    static bool Rename(const char* oldName, const char* newName);
    static bool Remove(const char* fileName);
    static bool MakeDirectory(const char* dirName);
	static bool MakeDeepDirectory(const char *DeepDirName);
    static bool RemoveDirectory(const char* dirName);
    static bool StatFS(const char* path,
        uint64* userFreeBytes,
        uint64* totalBytes,
        uint64* totalFreeBytes);
    static bool Access(const char* path, int mode);
    static bool Stat(const char* path, FileInfo* info);

protected:
    FILE *m_pFile;                
    unsigned char *m_pBuffer;    
    unsigned int m_dwLength;   
    FSOperations* m_opt;       
};



class CFileFind
{
public:
    CFileFind();
    virtual ~CFileFind();
    virtual bool findFile(const char* fileName);
    virtual bool findNextFile();
    virtual void close();
    unsigned int getLength();
    std::string getFileName();
    std::string getFilePath();
    bool isReadOnly();
    bool isDirectory();
    bool isHidden();
    bool isNormal();

protected:
    long m_handle;
    FileInfo m_fileInfo;
    FSOperations* m_opt;       
    std::string m_path;         
};


struct FSOperations
{
    FILE* (*fopen)(const char *, const char *);
    int (*fclose)(FILE *);
    size_t (*fread)(void *, size_t, size_t, FILE *);
    size_t (*fwrite)(const void *, size_t, size_t, FILE *);
    int (*fflush)(FILE *);
    int (*fseek)(FILE *, long, int);
    long (*ftell)(FILE *);
    char * (*fgets)(char *, int, FILE *);
    int (*fputs)(const char *, FILE *);
    int (*rename)(const char *oldname, const char *newname);
    int (*remove)(const char *path);
    long (*findfirst)(const char *, FileInfo *);
    int (*findnext)(long, FileInfo * );
    int (*findclose)(long);
    int (*mkdir)( const char *dirname);
    int (*rmdir)( const char *dirname);
    int (*statfs)( const char *path, uint64* userFreeBytes, uint64* totalBytes, uint64* totalFreeBytes);
    int (*access)( const char *path, int mode);
    int (*stat)( const char *path, FileInfo *);
};

void hookFS(const char* path, const FSOperations* opts);

#endif

