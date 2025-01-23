# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/Template+Template"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/1"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/Template+Template"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/Template+Template/tmp"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/Template+Template/src/Template+Template-stamp"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/Template+Template/src"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/Template+Template/src/Template+Template-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/Template+Template/src/Template+Template-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/CyberNaviRobot/MCU_MainController/MDK-ARM/tmp/Template+Template/src/Template+Template-stamp${cfgdir}") # cfgdir has leading slash
endif()
