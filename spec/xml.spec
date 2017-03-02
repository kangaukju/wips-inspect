xml 설정 정의


<list>                               <- root 노드
	<airconf>                        <- 설정 섹션
		<key>12345</key>             <- 설정을 구분하는 key, 필수:O
		<desc>test</desc>            <- 설명, 필수:X
		<version>0</version>         <- 802.11 version, 필수:X
		<protect>0</protect>         <- 802.11 mac frame protected flag, 필수:X
		<duration>0</duration>       <- 802.11 mac frame duration, 필수:X
		<type>0</type>               <- 802.11 mac frame type, 필수:O
		<subtype>0</subtype>         <- 802.11 mac frame subtype, 필수:O
		<ds tods="0" fromds="0">     <- 패킷 DS 방향, 필수:O
                                     <- 주소 정의 <ap>, <sta>, <addr> 중 하나만 선택가능, 필수:X
		<ap>00:00:00:00:00:00</ap>   <- AP 주소
		<sta>00:00:00:00:00:00</sta> <- 단말 주소
		<addr count="3"/>            <- 'count'는 주소 갯수 정의, 필수:<addr/>태크를 사용하는 경우 'count'애트리뷰트는 필수

		<addr count="1">                     <- address-1 예시
			<da>00:00:00:00:00:00</da>       <- dst address, da정의 필수
		</addr>
		<addr count="2">                     <- address-2 예시
			<da>00:00:00:00:00:00</da>       <- dst address, da정의 필수
			<sa>00:00:00:00:00:00</sa>       <- src  address, sa정의 필수
		</addr>
		<addr count="3">                     <- address-3 예시
			<da>00:00:00:00:00:00</da>       <- dst address, da정의 필수
			<sa>00:00:00:00:00:00</sa>       <- src  address, sa정의 필수
			<bssid>00:00:00:00:00:00</bssid> <- bssid  address, bssid정의 필수
		</addr>
		<addr count="4">                     <- address-4 예시 (WDS 주소체계)
			<ra>00:00:00:00:00:00</ra>       <- Rx AP  (수신AP),   ra정의 필수
			<da>00:00:00:00:00:00</da>       <- Rx STA (수신단말), da정의 필수
			<sa>00:00:00:00:00:00</sa>       <- Tx STA (송신단말), sa정의 필수
			<ta>00:00:00:00:00:00</ta>       <- Tx AP  (송신AP),   ta정의 필수
		</addr>
		<deauth_reason>0</deauth_reason>     <- De-authentication packet인 경우 reason code, 필수:X
		<taglist>    <- tag parameter 정의
                     <- 일반태그> id: 태크id
                                 len: data 길이, 
                                data: 문자열
                     <- HEX 태그> id: 태크id, 
                                 len: hex data byte 길이 (data의 /2가 될것임)
                                data: hex 문자열
                          type="hex": data hex 문자열임

			<tag id="0" len="7" data="UNETV20" />                     <- ESSID 태그 정의
			<tag id="1" len="8" type="hex" data="82848b0c12961824" /> <- Support Rates 태그 정의
		</taglist>
	</airconf>
</list>
