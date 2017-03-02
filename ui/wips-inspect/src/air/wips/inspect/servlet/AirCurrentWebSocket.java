package air.wips.inspect.servlet;

import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

import com.google.gson.Gson;

import air.wips.inspect.error.GoodBad;
import air.wips.inspect.log.D;
import air.wips.inspect.osdep.Tools;

@ServerEndpoint("/aircurrent")
public class AirCurrentWebSocket extends AirResult {
	private Thread thread;
	
	private class RequestParam {
		public String state;
		public String ifname;
		public String channel;
		public String refreshTime;
		@Override
		public String toString() {
			return "RequestParam [state=" + state + ", ifname=" + ifname + ", channel=" + channel + ", refreshTime="
					+ refreshTime + "]";
		}
	}
	
	public AirCurrentWebSocket() {
		this.thread = null;
	}
	
	@OnOpen
	public void onOpen(Session session) {
		this.webSocketSession = session;
	}
	
	@OnMessage
	public String onMessage(String msg) {
		RequestParam p = new Gson().fromJson(msg, RequestParam.class);
		
		D.log(p);
		
		if ("start".equals(p.state)) {
			try {
				thread = new Thread(new AirResultReceiver(this, AirResultReceiver.AIRCURRENT_PORT));
				thread.start();
				
				Tools.startAircurrent(p.ifname, p.channel, p.refreshTime);
				
			} catch (Exception e) {
				e.printStackTrace();
				if (thread != null) {
					thread.interrupt();
					thread = null;
				}
				return new Gson().toJson(new GoodBad().bad(e));
			}
		}
		else if ("stop".equals(p.state)) {
			onClose();
		}
		else {
			return new Gson().toJson(new GoodBad().bad("unknown state - "+p.state));
		}
		
		return new Gson().toJson(new GoodBad().good());
	}
	
	@OnClose
	public void onClose() {
		exit = true;
		try {
			Tools.stopAirCurrent();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	@OnError
	public String onError(Throwable e) {
		e.printStackTrace();
		onClose();
		return e.getMessage();
	}
}
