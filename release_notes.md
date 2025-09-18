## Release v1.8

# Install windows Intel/Amd64 bit version

```bash
> unzip -l bin/hod-v1.8-win_amd64.zip
Archive:  bin/hod-v1.8-win_amd64.zip
  Length      Date    Time    Name
---------  ---------- -----   ----
        0  09-17-2025 23:25   hod-v1.8-win_amd64/
      452  09-17-2025 23:25   hod-v1.8-win_amd64/ChangeLog.md
   265906  09-17-2025 23:25   hod-v1.8-win_amd64/hod.exe
    11451  09-17-2025 23:25   hod-v1.8-win_amd64/README.md
---------                     -------
   277809                     4 files
```

* Unzip `hod-v1.8-winamd64.zip` and copy `hod.exe` somewhere in your PATH

# Install Ubuntu package

* List the contents of the package

```bash
$ dpkg -c hod_1.8-ubuntu_amd64.deb
drwxrwxr-x root/root         0 2025-09-17 20:24 ./
drwxrwxr-x root/root         0 2025-09-17 20:24 ./usr/
drwxrwxr-x root/root         0 2025-09-17 20:45 ./usr/bin/
-rwxrwxr-x root/root     18568 2025-09-17 20:45 ./usr/bin/hod
drwxrwxr-x root/root         0 2025-09-17 20:24 ./usr/share/
drwxrwxr-x root/root         0 2025-09-17 20:24 ./usr/share/doc/
drwxrwxr-x root/root         0 2025-09-17 20:45 ./usr/share/doc/hod/
-rw-rw-r-- root/root       171 2025-09-17 20:45 ./usr/share/doc/hod/changelog.Debian.gz
-rw-rw-r-- root/root       164 2025-09-17 20:45 ./usr/share/doc/hod/changelog.gz
-rw-rw-r-- root/root        71 2025-09-17 20:45 ./usr/share/doc/hod/copyright
drwxrwxr-x root/root         0 2025-09-17 20:24 ./usr/share/man/
drwxrwxr-x root/root         0 2025-09-17 20:25 ./usr/share/man/man1/
-rw-rw-r-- root/root      3586 2025-09-17 20:45 ./usr/share/man/man1/hod.1.gz
```

* Show Package Info
```bash
$ dpkg -I hod_1.8-ubuntu_amd64.deb
 new Debian package, version 2.0.
 size 10642 bytes: control archive=419 bytes.
     460 bytes,    14 lines      control
 Package: hod
 Section: utils
 Version: 1.8-ubuntu
 Maintainer: Muhammad Muquit <muquit@muquit.com>
 Installed-Size: 104
 Homepage: http://muquit.com/muquit/software/hod/hod.html
 Priority: optional
 Architecture: amd64
 Depends: libc6 (>= 2.4)
 Description: Hexadecimal and Octal data dumper.
  Hod is a simple hexadecimal and octal data dumper. Originally I wrote
  it because I did not like the ugly output of od. You might find it useful as
  well.  License is MIT.
```


* Install the package

```bash
$ sudo dpkg -i hod_1.8-ubuntu_amd64.deb
[sudo] password for muquit:
(Reading database ... 244798 files and directories currently installed.)
Preparing to unpack hod_1.8-ubuntu_amd64.deb ...
Unpacking hod (1.8-ubuntu) over (1.8-ubuntu) ...
Setting up hod (1.8-ubuntu) ...
Processing triggers for man-db (2.12.0-4build2) ...
```
```bash
man hod
```

* List
```bash
$ dpkg -l hod
Desired=Unknown/Install/Remove/Purge/Hold
| Status=Not/Inst/Conf-files/Unpacked/halF-conf/Half-inst/trig-aWait/Trig-pend
|/ Err?=(none)/Reinst-required (Status,Err: uppercase=bad)
||/ Name           Version      Architecture Description
+++-==============-============-============-==================================
ii  hod            1.8-ubuntu   amd64        Hexadecimal and Octal data dumper.
```

* Remove the package
```bash
$ sudo dpkg -r hod
(Reading database ... 244798 files and directories currently installed.)
Removing hod (1.8-ubuntu) ...
Processing triggers for man-db (2.12.0-4build2) ...
```

Please look at [ChangeLog](ChangeLog.md) for what has changed in the current version.

Please do not forget to verify Checksums before installing.

Thanks!

---
Released with [go-xbuild-go](https://github.com/muquit/go-xbuild-go)
