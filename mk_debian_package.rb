#!/usr/bin/env ruby

##################################################
# make a debian binary package for ubuntu
# muquit@muquit.com Mar-28-2012 
##################################################
#
require 'fileutils'

class MakeDebianPackage

  def initialize
    @dirs = []

    @dirs << "debian/DEBIAN"
    @dirs << "debian/usr/bin"
    @dirs << "debian/usr/share/doc/hod"
    @dirs << "debian/usr/share/man/man1"

    @depends = "libc6 (>= 2.4)" # XXX CHANGE if needed
    @prog_ver = ''
    @package_name = ''
    @control_file = "debian/DEBIAN/control"
  end

  def log(msg)
    t = Time.new
    puts "#{t}: #{msg}"
  end

  def check
    file = "./hod"
    if !File.exist?(file)
      log "#{file} does not exist, compile first"
      exit(1)
    end

    file = "./changelog.debian"
    if !File.exist?(file)
      log "#{file} does not exist, compile first"
      exit(1)
    end

  end

  def gen_control_file
    prog_version
    begin
    f = File.open(@control_file, "w")
    f.puts <<EOD
Package: hod
Section: utils
Version: #{@prog_ver}-ubuntu
Maintainer: Muhammad Muquit <muquit@muquit.com>
Installed-Size: 104
Homepage: http://muquit.com/muquit/software/hod/hod.html
Priority: optional
Architecture: i386
Depends: #{@depends}
Description: Hexadecimal and Octal data dumper.
 Hod is a simple hexadecimal and octal data dumper. Originally I wrote 
 it because I did not like the ugly output of od. You might find it useful as 
 well.  License is MIT.

EOD
    f.close
    rescue => ex
      log "Could not create '#{@control_file}' (#{ex})"
    end
  end

  def gen_copyright
    f = File.open("./debian/usr/share/doc/hod/copyright", "w")
    year = Time.new.year
    f.puts <<EOD
hod

Copyright #{year} Muhammad Muquit <muquit@muquit.com>

License: MIT

EOD
    f.close
  end

  def compress(file)
    cmd = "gzip --force --best #{file}"
    system(cmd)
  end

  def strip_binary(file)
    cmd = "strip #{file}"
    system(cmd)
  end
  

  def copy_files
    FileUtils.copy("./hod", "./debian/usr/bin")
    strip_binary("./debian/usr/bin/hod")
    FileUtils.copy("./doc/hod.1.gz", "./debian/usr/share/man/man1")

    file = "./debian/usr/share/doc/hod/changelog"
    FileUtils.copy("./changelog.debian", file)
    compress(file)

    file = "./debian/usr/share/doc/hod/changelog.Debian"
    FileUtils.copy("./changelog.debian", file)
    compress(file)
  end

  def make_dirs
    @dirs.each do |dir|
      log "Make directory: #{dir}"
      FileUtils.mkdir_p(dir)
    end
  end

  def lintian
    package_name = get_package_name
    log "Running lintian on #{package_name}"
    cmd = "lintian #{package_name}"
    system(cmd)
  end

  def prog_version
    ver = `./hod -V 2>&1`
    log "ver: #{ver}"
    a = ver.split("\n")
    l = a[0].to_s.chomp
    if l =~ /.*v(.*)$/
      @prog_ver = $1
      log "hod version: '#{@prog_ver}'"
    else
      log "Could not determine hod version"
      exit(1)
    end
  end

  def get_package_name
    pname = "hod_#{@prog_ver}-ubuntu_i386.deb"
    @package_name = pname
    return pname
  end

  def make_package
    package_name = get_package_name
    FileUtils.remove("./debian.deb") if File.exist?("./debian.deb")
    cmd = "fakeroot dpkg-deb --build debian"
    log "Running: #{cmd}"
    system(cmd)
    FileUtils.mv("debian.deb", package_name)
    log "Package: #{package_name}"
  end

  def print_install_inst
  puts <<EOD
##########################################
  
To install the package, type:
  sudo dpkg -i #{@package_name}

To remove the package, type:
  sudo dpkg -r hod

##########################################

EOD
  end

  def doit
    check
    make_dirs
    gen_control_file
    gen_copyright
    copy_files
    make_package
    lintian
    print_install_inst
  end

end

if __FILE__ == $0
  deb = MakeDebianPackage.new
  deb.doit
end

