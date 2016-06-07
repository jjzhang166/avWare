#AvWare

vs2013 及linux HISI 下都可以使用cmake 来创建工程文件。

cmake 版本最好是最新版本的。

vs2013要安装pthread相关库及头文件。

#2016年4月21日9:42:44
同步更新LINUX
LINUX X86上编译请先参照 Doc/linux_env.txt 文件，将系统所需库安装完整


cmake -DCMAKE_BUILD_TYPE=Debug(或Release)
cmake -DCMAKE_BUILD_TYPE=Release ../

默认rellease 版本


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




