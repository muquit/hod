#!/usr/bin/env ruby

# make the minimal pod doc and then generate man page from it
# muquit@muquit.com  Mar-27-2012 
class MakeMailsendPod

  PROG = "./hod"

  def initialize
    @version = ''
  end

  def log(msg)
    t = Time.new
    puts "#{t}: #{msg}"
  end

  def version
    ver = `./hod -V 2>&1`
    a = ver.split("\n")
    l = a[0].to_s.chomp
    if l =~ /.*v(.*)$/
      @version = $1
    end
  end

  def check_binary
    if !File.exists?("#{PROG}")
      log "#{PROG} does not exist. exiting"
      exit
    end
  end

  def gen_man
    cmd = "pod2man --release \"hod #{@version}\" -c \"User Commands\" docs/hod.pod > docs/hod.1"
    system(cmd)
    # compress
    cmd = "gzip --best -c docs/hod.1 > docs/hod.1.gz"
    system(cmd)
  end

  def doit
    version
    gen_man
  end
end

if __FILE__ == $0
  MakeMailsendPod.new.doit
end
