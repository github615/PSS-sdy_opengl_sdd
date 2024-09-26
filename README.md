# PSS-sdy_opengl_sdd

PSS-sdy_opengl_sdd是一个基于商用工具SCADE Display（OpenGL）的二次开发项目。

该项目构建的工具可用于生成SCADE Display（OpenGL）软件模型的软件设计文档。其设计思想详见：
  https://mp.csdn.net/mp_blog/creation/editor/130075655
  
该工具的编译、运行依赖于SCADE Display R16版，因此其开发者和使用者的计算机环境必需安装有SCADE Display R16，并具有相应的许可证。

该工具包含以下两个可执行文件：
. sdy_r16_opengl_sdd_xml.exe
. sdyddword.exe

sdy_r16_opengl_sdd_xml.exe解析SCADE Display软件模型文件，生成SDD的xml文件。

sdyddword.exe把SDD的xml文件转换为word文件。当xml文件比较大时，sdyddword.exe的生成速度很慢。

我的同事用python开发了把SDD的xml文件转换为word文件的工具，转换速度很快。

-----------------------

PSS-sdy_opengl_sdd is a secondary development project based on the commercial tool SCADE Display (OpenGL).

The tools built in this project can be used to generate software design documents for SCADE Display (OpenGL) software models. The design idea is detailed in:
  https://mp.csdn.net/mp_blog/creation/editor/130075655
  
The compilation and operation of the tool depends on SCADE Display R16, so its developers and users must have SCADE Display R16 installed and have the appropriate license in their computer environment.

The tool contains the following two executables:
. sdy_r16_opengl_sdd_xml.exe
. sdyddword.exe

sdy_r16_opengl_sdd_xml.exe parse the SCADE Display software model file and generate an xml file for the SDD.

sdyddword.exe convert the xml file of the SDD to a word file. When the XML file is large, the sdyddword.exe generation is slow.

My colleague developed a tool in python to convert an SDD xml file to a word file, and the conversion speed is very fast.
