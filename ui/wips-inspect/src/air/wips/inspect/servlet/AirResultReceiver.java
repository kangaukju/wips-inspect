package air.wips.inspect.servlet;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.google.gson.Gson;

import air.wips.inspect.analyzer.Inspector;
import air.wips.inspect.log.D;
import air.wips.inspect.result.AirCaptureResult;
import air.wips.inspect.result.AirCurrentAP;
import air.wips.inspect.result.AirCurrentST;
import air.wips.inspect.utils.StringUtil;


public class AirResultReceiver implements Runnable {
	public static final int WIPSINSPECT_PORT = 8554;
	public static final int AIRCURRENT_PORT = 8555;

	private long maxRunningTime;
	private int port;
	private AirResult airResult;
	private DocumentBuilder xmlBuilder;
	private static Charset charset = Charset.defaultCharset();

	public AirResultReceiver(AirResult airResult, int port, long maxRunningTime) throws Exception {		
		this.airResult = airResult;		
		this.maxRunningTime = maxRunningTime * 1000; // sec -> msec
		this.port = port;
		this.xmlBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
	}
	
	private void sendResultAirCurrent(String result) {
		D.log("call sendResultAirCurrent");
		ByteArrayInputStream input = null;
		try {
			input = new ByteArrayInputStream(result.getBytes());
			Document doc = xmlBuilder.parse(input);
			
			List<AirCurrentAP> aplist = AirResultParse.getAircurrentAP(doc);
			List<AirCurrentST> stlist = AirResultParse.getAircurrentST(doc);
			List<Object> all = new ArrayList<>();
			
			all.add(aplist);
			all.add(stlist);
			
			airResult.getWebSocketSession().getBasicRemote().sendText(new Gson().toJson(all));
			
		} catch (Exception e) {
			e.printStackTrace();
			System.err.println(result);
		} finally {
			if (input != null) try { input.close(); } catch (IOException e) {}
		}
	}
		
	private void sendResultAirCapture(String result) {
		ByteArrayInputStream input = null;
		try {
			input = new ByteArrayInputStream(result.getBytes());
			Document doc = xmlBuilder.parse(input);
			
			NodeList resultnode = doc.getElementsByTagName("result");
			if (resultnode.getLength() == 1) {
				Node node = resultnode.item(0);
				NamedNodeMap attrs = node.getAttributes();
				
				AirCaptureResult rs = new AirCaptureResult();
				rs.setProfile(AirResultParse.getNodeValue(attrs, "profile", ""));
				rs.setConf   (AirResultParse.getNodeValue(attrs, "conf", ""));
				rs.setKey    (AirResultParse.getNodeValue(attrs, "key", ""));
				rs.setXid    (AirResultParse.getNodeValue(attrs, "xid", ""));
				rs.setSec    (AirResultParse.getNodeValue(attrs, "sec", ""));
				rs.setUsec   (AirResultParse.getNodeValue(attrs, "usec", ""));
				rs.setElapsed(AirResultParse.getNodeValue(attrs, "elapsed", ""));
				rs.setPwr    (AirResultParse.getNodeValue(attrs, "pwr", ""));
				rs.setType   (AirResultParse.getNodeValue(attrs, "type", ""));
				rs.setSubtype(AirResultParse.getNodeValue(attrs, "subtype", ""));
				rs.setAddr1  (AirResultParse.getNodeValue(attrs, "addr1", ""));
				rs.setAddr2  (AirResultParse.getNodeValue(attrs, "addr2", ""));
				rs.setAddr3  (AirResultParse.getNodeValue(attrs, "addr3", ""));
				rs.setAddr4  (AirResultParse.getNodeValue(attrs, "addr4", ""));
				rs.setDs     (AirResultParse.getNodeValue(attrs, "ds", ""));
				rs.setSeq    (AirResultParse.getNodeValue(attrs, "seq", ""));
				
				if (StringUtil.isNull(rs.getProfile())) {
					throw new Exception("empty profile of aircapture result");
				}
				if (StringUtil.isNull(rs.getConf())) {
					throw new Exception("empty config of aircapture result");
				}				
				if (StringUtil.isNull(rs.getKey())) {
					throw new Exception("empty key of aircapture result");
				}
				if (StringUtil.isNull(rs.getXid())) {
					throw new Exception("empty xid of aircapture result");
				}
				
				Inspector.getInstance().appendAirCaptureResult(rs);
				
				airResult.getWebSocketSession().getBasicRemote().sendText(new Gson().toJson(rs));
			}			
		} catch (Exception e) {
			D.log(result);
			e.printStackTrace();
		} finally {
			if (input != null) try { input.close(); } catch (IOException e) {}
		}
	}
	
	static long loop = 0;
	private boolean readData(SocketChannel client) throws Exception {
		boolean clientContinue = true;
		int bytes = -1;
		String decodeData;
		ByteBuffer lenBuffer;
		ByteBuffer dataBuffer;
		
		switch (port) {
		case WIPSINSPECT_PORT: {
			/*
			 * recv length field - 4 bytes (big endian)
			 */
			bytes = 0;
			lenBuffer = ByteBuffer.allocate(4);
			try {
				while (true) {
					bytes += client.read(lenBuffer);
					if (bytes == 4) break;
					continue;
				}
			} catch (IOException e) {
				client.close();
				clientContinue = false;
				break;
			}
			lenBuffer.flip();
			int length = lenBuffer.order(ByteOrder.BIG_ENDIAN).getInt();
			
			/*
			 * recv real data - n bytes
			 */
			bytes = 0;
			dataBuffer = ByteBuffer.allocate(length);
			try {
				while (true) {
					bytes += client.read(dataBuffer);
					if (bytes == length) break;
					continue;
				}
			} catch (IOException e) {
				client.close();
				clientContinue = false;
				break;
			}
			dataBuffer.flip();
			decodeData = charset.decode(dataBuffer).toString();
			//D.log(decodeData);
			
			sendResultAirCapture(decodeData);
			break;
		}	
		case AIRCURRENT_PORT: {
			/*
			 * recv length field - 4 bytes (big endian)
			 */
			bytes = 0;
			lenBuffer = ByteBuffer.allocate(4);
			try {
				while (true) {
					bytes += client.read(lenBuffer);
					if (bytes == 4) break;
					continue;
				}
			} catch (IOException e) {
				client.close();
				clientContinue = false;
				break;
			}
			lenBuffer.flip();
			int length = lenBuffer.order(ByteOrder.BIG_ENDIAN).getInt();
			{
				byte [] b = lenBuffer.array();
				D.log(b.length+" / "+b[0]+"."+b[1]+"."+b[2]+"."+b[3]+".");
				D.log(length + "   " + lenBuffer + " => " + new String(lenBuffer.array()));
			}
			lenBuffer.clear();
			lenBuffer = null;
			/*
			 * recv real data - n bytes
			 */			
			bytes = 0;
			dataBuffer = ByteBuffer.allocate(length);
			try {
				while (true) {
					bytes += client.read(dataBuffer);
					if (bytes == length) break;
					continue;
				}
			} catch (IOException e) {
				client.close();
				clientContinue = false;
				break;
			}
			D.log("bytes  ===> "+bytes);
			dataBuffer.flip();
			decodeData = charset.decode(dataBuffer).toString();
			//D.log(decodeData);
			
			sendResultAirCurrent(decodeData);
			dataBuffer.clear();
			dataBuffer = null;
			break;
		}
		}
		
		return clientContinue;
	}
	
	@Override
	public void run() {
		final int timeout = 100;
		Selector selector = null;
		SocketChannel client = null;
		ServerSocketChannel serverChannel = null;
		long runningStart, now;
		
		try {
			// open selector
			selector = Selector.open();			
			// open socket channel
			serverChannel = ServerSocketChannel.open()
					.setOption(StandardSocketOptions.SO_REUSEADDR, true);
			// bind port
			serverChannel.socket().bind(new InetSocketAddress(port));
			// set nonblocking
			serverChannel.configureBlocking(false);
			// register accept select
			serverChannel.register(selector, SelectionKey.OP_ACCEPT);
			
			Inspector.getInstance().clear();
			runningStart = System.currentTimeMillis();
			D.log("Listening...");
			
			while (!airResult.isExit()) {
				
				if (maxRunningTime > 0) {
					now = System.currentTimeMillis();
					if ((now - runningStart) > maxRunningTime) {
						D.log("listening server time over (max: "+maxRunningTime/1000+") sec");
						return;
					}
				}
				
				selector.select(timeout);
				
				Iterator<SelectionKey> keys = selector.selectedKeys().iterator();				
				while (keys.hasNext()) {
					SelectionKey selkey = keys.next();
					keys.remove();
					
					if (!selkey.isValid()) {
						continue;
					}					
					/* accept */
					if (selkey.isAcceptable()) {
						if (client != null) {
							D.log("Only one tool runable");
							break;
						}
						client = serverChannel.accept();
						client.shutdownOutput(); // close write channel
						client.configureBlocking(false);
						client.register(selector, SelectionKey.OP_READ);
						
						D.log("Connect: "+client.socket().getRemoteSocketAddress());
					}
					/* read */
					else if (selkey.isReadable()) {
						client = (SocketChannel)selkey.channel();
						/* clientContinue - 'false' means client process killed */
						if (!readData(client)) {
							return;
						}
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (client != null && client.isOpen()) try { client.close(); } catch (IOException e) { }
			if (serverChannel != null) try { serverChannel.close(); } catch (IOException e) { }
			if (selector != null) try { selector.close(); } catch (IOException e) { }
			try { airResult.getWebSocketSession().close(); } catch (IOException e) { }
			D.log("AirResultReceiver terminated");
		}
	}
}
