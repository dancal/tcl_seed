#!/usr/bin/tcl
#load ./libstrutil.so
package require encode
load ./libseed.so

set x1	{���ݱ��� ���¸� \n<a href=''>����</a>�ϸ鼭 <b>MakefileM</b>�� ���� ���� �������� �����Ͽ���. \n���¿� ���� �������� ���ݸ� �ٲٸ� �ڽ��� Makefile�ν� ����� �� �ִ�. \n<b>���⿡���� ���� ���� <u>Makefile</u>���� �⺻Ʋ(template)���� �Ұ��ϰ��� �Ѵ�.&lt;/FONT&gt;&lt;/P&gt�츮�� ������}

#set x	{SDFOPASFKO AAS saisadfj iosadfj ioasjfsido jioasdf �� d}
set xx	{SDFOPASFKO1 sd}
#set x	{safd jio ���������� �� �̤Ť����� ��}

for { set i 0 } { $i < 100 } { incr i } {
	set sKey	{abcdefrghitofddf}
	puts key=$sKey\n
	set x		[ format "$i==$x1=$i=$xx" $i ]
	puts "str length : [ string length $x ]"
	set sEnc	[ seed::encrypt $sKey $x ]
	puts "enc length : [ string length $sEnc ]"
	puts "$i / enc=$sEnc"
	set sDec	[ seed::decrypt $sKey $sEnc ]
	puts "$i / dec=$sDec\n"
}
set x		"a"
set sEnc	[ seed::encrypt $sKey $x ]
set sDec	[ seed::decrypt $sKey $sEnc ]
puts $sEnc
puts $sDec
puts [ base64encode a ]
