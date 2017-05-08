#AvWare

vs2013 及linux HISI 下都可以使用cmake 来创建工程文件。

cmake 版本最好是最新版本的。

vs2013要安装pthread相关库及头文件。

#2016年4月21日9:42:44
同步更新LINUX
LINUX X86上编译请先参照 Doc/linux_env.txt 文件，将系统所需库安装完整


cmake -DCMAKE_BUILD_TYPE=Debug ../

cmake -DCMAKE_BUILD_TYPE=Release ../

make VERBOSE=1


build:

在top 目录下 

mkdir ProjectHisi;

cd ProjectHisi

cmake ../

make -j4

#2016年3月7日18:25:23
windwos 及 linux都可以读取 cam 及 screen 的数据。。然后进行H264编码。
windows下编码支持 intel编码。及ffmpeg 编码。。
linux只支持ffmpeg 软编码。

![输入图片说明](http://files.git.oschina.net/group1/M00/00/71/ZxV3cFd2BqKAbdlHAABd5hqmaGU392.jpg?token=dfcf13c39b7bcc9d19e2645e2e56ae4b&ts=1467352773&attname=system.jpg "在这里输入图片标题")



