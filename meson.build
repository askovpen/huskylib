project('huskylib','c', version:'1.9.0')

cc = meson.get_compiler('c')

link_args = []
compile_args = []

if cc.get_id() == 'msvc'
  add_project_arguments(
    '-D_CRT_SECURE_NO_DEPRECATE',
    '-D_CRT_NONSTDC_NO_DEPRECATE', 
    language : 'c')
else
  add_project_arguments(
    '-DUNIX',
  language : 'c')
endif

src=files([
  'src/adcase.c',
  'src/calendar.c',
  'src/cmpfname.c',
  'src/crc.c',
  'src/cvtdate.c',
  'src/dirlayer.c',
  'src/fexist.c',
  'src/ffind.c',
  'src/flush.c',
  'src/ftnaddr.c',
  'src/genmsgid.c',
  'src/genverst.c',
  'src/getfree.c',
  'src/ioutil.c',
  'src/locking.c',
  'src/log.c',
  'src/mapfile.c',
  'src/memory.c',
  'src/parsenn.c',
  'src/patmat.c',
  'src/qksort.c',
  'src/recode.c',
  'src/setfsize.c',
  'src/strext.c',
  'src/strftime.c',
  'src/tdelay.c',
  'src/temp.c',
  'src/tree.c',
  'src/version.c',
  'src/xstr.c'
])

headers = files([
  'huskylib/BCD.h',
  'huskylib/BCO.h',
  'huskylib/BCW.h',
  'huskylib/BEOS5.h',
  'huskylib/BORLANDC.h',
  'huskylib/calendar.h',
  'huskylib/compiler.h',
  'huskylib/crc.h',
  'huskylib/cvtdate.h',
  'huskylib/dirlayer.h',
  'huskylib/DJGPP.h',
  'huskylib/EMX.h',
  'huskylib/fexist.h',
  'huskylib/ffind.h',
  'huskylib/ftnaddr.h',
  'huskylib/HIGHC.h',
  'huskylib/huskyext.h',
  'huskylib/huskylib.h',
  'huskylib/IBMC_OS2.h',
  'huskylib/locking.h',
  'huskylib/log.h',
  'huskylib/memory.h',
  'huskylib/MINGW32.h',
  'huskylib/MSC.h',
  'huskylib/MSVC.h',
  'huskylib/parsenn.h',
  'huskylib/pstdint.h',
  'huskylib/recode.h',
  'huskylib/SASC.h',
  'huskylib/strext.h',
  'huskylib/syslogp.h',
  'huskylib/temp.h',
  'huskylib/tree.h',
  'huskylib/typesize.h',
  'huskylib/UNIX.h',
  'huskylib/unused.h',
  'huskylib/vixie.h',
  'huskylib/WATCOMC.h',
  'huskylib/WCD.h',
  'huskylib/WCO.h',
  'huskylib/WCW.h',
  'huskylib/WCX.h',
  'huskylib/xstr.h'
])

incdir = include_directories('huskylib')

libhusky = library('husky', src,
  include_directories : incdir,
  version:'1.9.0',
  install : true)

husky_dep = declare_dependency(
  link_with : libhusky,
  include_directories : incdir)

install_headers(headers, subdir: 'huskylib')