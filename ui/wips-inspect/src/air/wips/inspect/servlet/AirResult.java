package air.wips.inspect.servlet;

import javax.websocket.Session;

public abstract class AirResult {
	protected Session webSocketSession;
	protected boolean exit;	
	
	public AirResult() {
		this.webSocketSession = null;
		exit = false;
	}
	
	public Session getWebSocketSession() {
		return webSocketSession;
	}
	
	public void setWebSocketSession(Session session) {
		this.webSocketSession = session;
	}
	
	public boolean isExit() {
		return exit;
	}
	
	public void doExit() {
		this.exit = true;
	}
}
