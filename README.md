1. 安装Visual Studio 2022个人版
2. https://github.com/Kitware/CMake/releases/download/v3.25.0-rc4/cmake-3.25.0-rc4-windows-x86_64.msi下载cmake并安装，安装完后打开Cmake Gui
3. [Download | GLFW](https://www.glfw.org/download.html)中下载Source Package解压 在cmake中选择where is the source code为解压的文件夹，where to build the binaries为上面的目录加上/build，然后点击Configure并选择Visual Studio2022，最后Generate，接下来可以在build文件夹内找到GLFW.sln，用VS打开并直接点击Build Solution，在build/src/Debug文件夹内可以找到我们需要的glfw3.lib放到lib文件夹下
4. https://github.com/assimp/assimp.git下载源代码（直接clone）就行，类似上面的步骤build，找到assimp-vc143-mtd.lib放到lib文件夹，assimp-vc143-mtd.dll放到dlls文件夹



我们期望的目录格式是：

home

----Cosmos

-------Cosmos

-------README.md

----include

----dlls

----lib



使用visual studio打开Cusmos.sln

左边Solution Explorer，右键Cusmos打开Properties

Configuration Properties内VC++ Directories内修改Include和Library改为刚弄好的include和lib文件夹

Linker内Input内Additional Dependences中加入

glfw3.lib
opengl32.lib
assimp-vc143-mtd.lib（先不弄算了）

这个时候在Ball.cpp里面F7编译F5运行应该就可以了