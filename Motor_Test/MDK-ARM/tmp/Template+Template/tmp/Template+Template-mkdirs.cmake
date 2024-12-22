# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/Template+Template"
  "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/1"
  "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/Template+Template"
  "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/Template+Template/tmp"
  "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/Template+Template/src/Template+Template-stamp"
  "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/Template+Template/src"
  "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/Template+Template/src/Template+Template-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/Template+Template/src/Template+Template-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/tungchiahui/UserFloder/mysource/CyberNaviRobot/Motor_Test/MDK-ARM/tmp/Template+Template/src/Template+Template-stamp${cfgdir}") # cfgdir has leading slash
endif()
