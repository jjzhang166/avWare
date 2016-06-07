#ifndef _CSMTPEXPORT_H_
#define _CSMTPEXPORT_H_

#ifdef WIN32
#if defined IPCSDK_EXPORTS 
	#define RLIPC_API  extern __declspec(dllexport) 
#else 
	#define RLIPC_API  extern __declspec(dllimport) 
#endif
#else
	#define RLIPC_API
#endif

typedef enum
{
	ENU_MAIL_EnMode_TLS,
	ENU_MAIL_EnMode_SSL,
	ENU_MAIL_EnMode_NULL
}ENU_MAIL_EnMode;
#ifdef __cplusplus
extern "C" {
#endif 
#if 0
RLIPC_API int SendMailSSL( const char * sMainServer,unsigned short port, ENU_MAIL_EnMode EncryptionMode, const char *  sMailFrom, const char *sMailTo,
	                const char *  sUserName, const char *sPasswd,
	                const char *  sTitle,  const char *sContent,
	                char *  sFileName,char * sFileData,long FileLength);

#else
RLIPC_API int SmtpSetServer(const char *sMainServer, unsigned short port, ENU_MAIL_EnMode EncryptionMode);
RLIPC_API int SmtpSetAuthorization(const char *  sUserName, const char *sPasswd);
RLIPC_API int SmtpSetSender(const char *sMailFrom);
RLIPC_API int SmtpSetAddRecver(const char *sMailTo);
RLIPC_API int SmtpSetTitle(const char *sTitle);
RLIPC_API int SmtpSetContent(const char *sContent);
RLIPC_API int SmtpSetAddAttachment(const char *localfile);
RLIPC_API int SmtpSetAddAttachinfo(const char *RawFileName, char * sFileData, long FileLength);
RLIPC_API int SmtpSendmail();
RLIPC_API int SmtpClear();
#endif
#ifdef __cplusplus
}
#endif 




#endif