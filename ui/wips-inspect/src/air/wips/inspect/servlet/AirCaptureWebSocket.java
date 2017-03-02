package air.wips.inspect.servlet;

import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

import com.google.gson.Gson;

import air.wips.inspect.log.D;
import air.wips.inspect.osdep.Tools;

@ServerEndpoint("/aircapture")
public class AirCaptureWebSocket extends AirResult {
	private Thread thread;
	
	class AirCaptureParams {
		private String state;
		private String ifname;
		private String filename;
		private String timer;
		
		public String getState() {
			return state;
		}
		public void setState(String state) {
			this.state = state;
		}
		public String getIfname() {
			return ifname;
		}
		public void setIfname(String ifname) {
			this.ifname = ifname;
		}
		public String getTimer() {
			return timer;
		}
		public void setTimer(String timer) {
			this.timer = timer;
		}
		public String getFilename() {
			return filename;
		}
		public void setFilename(String filename) {
			this.filename = filename;
		}
		@Override
		public String toString() {
			return "AirCaptureParams [state=" + state + ", ifname=" + ifname + ", filename=" + filename + ", timer="
					+ timer + "]";
		}
	}
	
	public AirCaptureWebSocket() {
		this.thread = null;
	}
	
	@OnOpen
	public void onOpen(Session session) {
		D.log(this.getClass(), "onOpen()");
		this.webSocketSession = session;
	}
	
	@OnMessage
	public String onMessage(String msg) {
		AirCaptureParams params = new Gson().fromJson(msg, AirCaptureParams.class);
		
		D.log(params);
		
		if ("start".equals(params.getState())) {
			try {
				thread = new Thread(new AirResultReceiver(this, AirResultReceiver.WIPSINSPECT_PORT));
				thread.start();
	
				Tools.startAirCapture(
						params.getIfname(),
						Integer.valueOf(params.getTimer() == null ? "0" : params.getTimer()),
						params.getFilename());
				
			} catch (Exception e) {
				e.printStackTrace();
				return "";//FAILURE;
			}
			return "";//SUCCESS;
		}
		else if ("stop".equals(params.getState())) {
			onClose();
			return "";//SUCCESS;
		}
		
		return "";//FAILURE;
	}
	
	@OnClose
	public void onClose() {
		D.log(this.getClass(), "onClose()");
		exit = true;
		try {
			Tools.stopAirCurrent(null);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	@OnError
	public String onError(Throwable t) {
		t.printStackTrace();
		D.log(this.getClass(), "onError()");
		onClose();
		return t.getMessage();
	}
	
}
