#include "rtpSend.h"

RtpSession *session;
uint32_t user_ts;

int getCurrentMilliseconds()
{
	unsigned int time=0;
	SYSTEMTIME sys;
	GetLocalTime( &sys );
	time = sys.wHour*60*60*1000 + sys.wMinute*60*1000
		+ sys.wSecond*1000 + sys.wMilliseconds;
	return time;
}
int rtpInitialize(const char* ipAddr="127.0.0.1", int port=8000)
{
	//RTP³õÊ¼»¯		
	char *ssrc;
	int clockslide=0;
	int jitter=0;
	user_ts=getCurrentMilliseconds();

	ortp_init();
	ortp_scheduler_init();
	ortp_set_log_level_mask(ORTP_MESSAGE|ORTP_WARNING|ORTP_ERROR);
	session=rtp_session_new(RTP_SESSION_SENDONLY);	
	
	rtp_session_set_scheduling_mode(session,1);
	rtp_session_set_blocking_mode(session,1);
	rtp_session_set_connected_mode(session,TRUE);
	rtp_session_set_remote_addr(session,ipAddr,port);
	rtp_session_set_payload_type(session, 0);
	rtp_session_set_seq_number(session, 1);
	ssrc=getenv("SSRC");
	if (ssrc!=NULL) {
		printf("using SSRC=%i.\n",atoi(ssrc));
		rtp_session_set_ssrc(session,atoi(ssrc));
	}
	return 1;
}
int rtpSend(unsigned char* buffer, int size)
{//if size=0, send a empty packet
	static int count=0;
	static int send_ts=0;
	mblk_t *m;

	//m = rtp_session_create_packet(session,RTP_FIXED_HEADER_SIZE,(uint8_t*)buffer,size);
	//rtp_session_sendm_with_sendts(session,m,user_ts,send_ts);

	rtp_session_send_with_ts(session, buffer, size, send_ts);
	count++;
	//printf("packet number:%d\tpacket size:%d\n",count, size);
	user_ts += 20;
	send_ts += 160;
	return 1;
}
int send_packet_split_frames(unsigned char* out,int encoder_size, int frame_num)
{
	if(encoder_size>0)
	{
		for(int i=0;i<frame_num;i++)
		{
			rtpSend(out+i*encoder_size/frame_num, encoder_size/frame_num);
		}
	}
	return 0;
}
int rtpDestory()
{
	rtp_session_destroy(session);
	ortp_exit();
	ortp_global_stats_display();

	return 0;
}