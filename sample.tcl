#!/usr/bin/tcl
#load ./libstrutil.so
package require encode
load ./libseed.so

set x1	{지금까지 강좌를 \n<a href=''>진행</a>하면서 <b>MakefileM</b>의 여러 가지 예제들을 제시하였다. \n강좌에 나온 예제들을 조금만 바꾸면 자신의 Makefile로써 사용할 수 있다. \n<b>여기에서는 여러 가지 <u>Makefile</u>들의 기본틀(template)들을 소개하고자 한다.&lt;/FONT&gt;&lt;/P&gt우리가 누루루루}

#set x	{SDFOPASFKO AAS saisadfj iosadfj ioasjfsido jioasdf 우 d}
set xx	{SDFOPASFKO1 sd}
#set x	{safd jio ㄴㄻ원ㅇ물 ㅓ ㅜㅕㄴㅇㅁ ㄹ}

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
