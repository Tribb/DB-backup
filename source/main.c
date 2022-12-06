//#define DEBUG_SOCKET
#define DEBUG_IP "192.168.2.2"
#define DEBUG_PORT 9023

#include "ps4.h"

int internal_backup() {
  unlink("/system_data/priv/mms/app.bak");
  unlink("/system_data/priv/mms/addcont.bak");
  unlink("/system_data/priv/mms/av_content_bg.bak");
  unlink("/user/system/webkit/secure/appcache/ApplicationCache.bak");
  unlink("/user/system/webkit/webbrowser/appcache/ApplicationCache.bak");
  copy_file("/system_data/priv/mms/app.db", "/system_data/priv/mms/app.bak");
  copy_file("/system_data/priv/mms/addcont.db", "/system_data/priv/mms/addcont.bak");
  copy_file("/system_data/priv/mms/av_content_bg.db", "/system_data/priv/mms/av_content_bg.bak");
  copy_file("/user/system/webkit/secure/appcache/ApplicationCache.db", "/user/system/webkit/secure/appcache/ApplicationCache.bak");
  copy_file("/user/system/webkit/webbrowser/appcache/ApplicationCache.db", "/user/system/webkit/webbrowser/appcache/ApplicationCache.bak");
  return 0;
}

int _main(struct thread *td) {
  UNUSED(td);

  initKernel();
  initLibc();

#ifdef DEBUG_SOCKET
  initNetwork();
  DEBUG_SOCK = SckConnect(DEBUG_IP, DEBUG_PORT);
#endif

  jailbreak();

  initSysUtil();

  touch_file("/mnt/usb0/.probe");
  if (!file_exists("/mnt/usb0/.probe")) {
    touch_file("/mnt/usb1/.probe");
    if (!file_exists("/mnt/usb1/.probe")) {
      internal_backup();
      printf_notification("Internal backup complete.\nThis was only a database backup, use a USB device for a full backup.");
    } else {
      printf_notification("Backing up to USB1");
      unlink("/mnt/usb1/.probe");
      internal_backup();
      mkdir("/mnt/usb1/PS4/", 0777);
      rmtree("/mnt/usb1/PS4/Backup/");
      mkdir("/mnt/usb1/PS4/Backup/", 0777);
      copy_file("/system_data/priv/mms/app.db", "/mnt/usb1/PS4/Backup/app.db");
      copy_file("/system_data/priv/mms/addcont.db", "/mnt/usb1/PS4/Backup/addcont.db");
      copy_file("/system_data/priv/mms/av_content_bg.db", "/mnt/usb1/PS4/Backup/av_content_bg.db");
      copy_file("/user/system/webkit/secure/appcache/ApplicationCache.db", "/mnt/usb1/PS4/Backup/ApplicationCache.dbsec");
      copy_file("/user/system/webkit/webbrowser/appcache/ApplicationCache.db", "/mnt/usb1/PS4/Backup/ApplicationCache.db");
      printf_notification("USB backup complete!");
    }
  } else {
    printf_notification("Backing up to USB0");
    unlink("/mnt/usb0/.probe");
    internal_backup();
    mkdir("/mnt/usb0/PS4/", 0777);
    rmtree("/mnt/usb0/PS4/Backup/");
    mkdir("/mnt/usb0/PS4/Backup/", 0777);
    copy_file("/system_data/priv/mms/app.db", "/mnt/usb0/PS4/Backup/app.db");
    copy_file("/system_data/priv/mms/addcont.db", "/mnt/usb0/PS4/Backup/addcont.db");
    copy_file("/system_data/priv/mms/av_content_bg.db", "/mnt/usb0/PS4/Backup/av_content_bg.db");
    copy_file("/user/system/webkit/secure/appcache/ApplicationCache.db", "/mnt/usb0/PS4/Backup/ApplicationCache.dbsec");
    copy_file("/user/system/webkit/webbrowser/appcache/ApplicationCache.db", "/mnt/usb0/PS4/Backup/ApplicationCache.db");
    printf_notification("USB backup complete!");
  }

#ifdef DEBUG_SOCKET
  printf_debug("Closing socket...\n");
  SckClose(DEBUG_SOCK);
#endif

  return 0;
}
