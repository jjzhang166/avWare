
#ifndef _DDNS_ENABLE_H
#define _DDNS_ENABLE_H

typedef enum
{
	DDNS_3322,
	DDNS_DYNDNS,
	DDNS_DNSDYNMAIC,
	DDNS_BUTT,
}ENU_DDNS;

/* ===================================================================
** 函数名  ：DDNS_Enable
** 函数功能：外网ip变,ddns
** 参数    ：
**           nType-------类型     [in]
**           pDoamain----域名     [in]
**           pName-------用户名   [in]
**           Pwd---------密码     [in]
** 返回值  ：0----更新成功 
**           1----外网ip没变化无需更新
**          -1---参数错误或更新失败
** 说明    ：
**          while(1)
**          {
**              DDNS_Enable(nType, pDoamain, pName, Pwd);
**              sleep(120);
**          }
** ===================================================================*/
#ifdef __cplusplus
		extern "C" {
#endif
int DDNS_Enable(ENU_DDNS nType, char *pDoamain, char *pName, char *Pwd);
#ifdef __cplusplus
}
#endif 

#endif  //_DDNS_ENABLE_H

