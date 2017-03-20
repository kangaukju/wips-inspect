package sql.inspect.wips.air;

import java.io.File;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;

import org.json.JSONException;
import org.json.XML;

import air.wips.inspect.history.InspectHistory;
import air.wips.inspect.utils.FileUtil;

public class History {
	private String profileId;
	private String runTimer;
	private String timestamp;
	private String savepath;
	private Profile profile;
	private List<InspectXmlLog> inspectXmlLogList;
	private List<InspectPcapLog> inspectPcapLogList;
	
	static class InspectXmlLog {
		private Config config;
		private String xmlLog;
		public InspectXmlLog(Config config, String xmlLog) {
			this.config = config;
			this.xmlLog = xmlLog;
		}
	}
	
	static class InspectPcapLog {
		private Config config;
		private String PcapFilepath;
		public InspectPcapLog(Config config, String pcapFilepath) {
			this.config = config;
			PcapFilepath = pcapFilepath;
		}
	}
	
	@Override
	public String toString() {
		return "History [profileId=" + profileId + ", runTimer=" + runTimer + ", timestamp=" + timestamp + ", savepath="
				+ savepath + ", profile=" + profile + ", inspectXmlLogList=" + inspectXmlLogList
				+ ", inspectPcapLogList=" + inspectPcapLogList + "]";
	}

	public static void del(String profileId) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		String sql;
		try {
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			conn.setAutoCommit(false);
			sql = "delete from inspect_history where profile_id=?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, profileId);
			pstmt.executeUpdate();
			
			File historyDir = new File(InspectHistory.path(profileId));
			if (historyDir.exists()) {
				historyDir.delete();
			}
			conn.commit();
		} catch (Exception e) {
			e.printStackTrace();
			try { conn.rollback(); } catch (Exception e1) {}
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
	
	public static void del(String profileId, String timestamp) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		String sql;
		try {
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			conn.setAutoCommit(false);
			sql = "delete from inspect_history where profile_id=? and timestamp=?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, profileId);
			pstmt.setString(2, timestamp);
			pstmt.executeUpdate();
			
			File historyDir = new File(InspectHistory.path(profileId+"/"+timestamp));
			if (historyDir.exists()) {
				historyDir.delete();
			}
			conn.commit();
		} catch (Exception e) {
			e.printStackTrace();
			try { conn.rollback(); } catch (Exception e1) {}
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
	
	public static void add(String profileId, String timestamp, String runTimer) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		
		try {
			String sql = "insert into inspect_history (profile_id, timestamp, savepath, run_timer) values (?,?,?,?)";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			conn.setAutoCommit(false);
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, profileId);
			pstmt.setString(2, timestamp);
			pstmt.setString(3, InspectHistory.path(profileId+"/"+timestamp));
			pstmt.setString(4, runTimer);
			pstmt.executeUpdate();
			
			// create history directory
			File historyDir = new File(InspectHistory.path(profileId+"/"+timestamp));
			if (!historyDir.exists()) {
				historyDir.mkdirs();
			}
			conn.commit();
		} catch (Exception e) {
			e.printStackTrace();
			try { conn.rollback(); } catch (Exception e1) {}
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
	
	public static History get(String profileId, String timestamp, boolean loadLog) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		History history = null;
		
		try {
			String sql = "select * from inspect_history where profile_id=? and timestamp=?";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, profileId);
			pstmt.setString(2, timestamp);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				history = new History();
				history.profileId = profileId;
				history.timestamp = timestamp;
				history.savepath = rs.getString("savepath");
				history.runTimer = rs.getString("run_timer");
				history.profile = Profile.getById(profileId, true);
				
				if (loadLog) {
					history.inspectXmlLogList = getInspectXmlLogList(history.savepath);
					history.inspectPcapLogList = getInspectPcapLogList(history.savepath);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
		return history;
	}
	
	public static List<History> get(String profileId, boolean loadLog) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<History> list = new ArrayList<>();
		
		try {
			String sql = "select * from inspect_history where profile_id=?";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, profileId);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				History history = new History();
				history.profileId = profileId;
				history.timestamp = rs.getString("timestamp");
				history.savepath = rs.getString("savepath");
				history.runTimer = rs.getString("run_timer");
				history.profile = Profile.getById(profileId, true);
				
				if (loadLog) {
					history.inspectXmlLogList = getInspectXmlLogList(history.savepath);
					history.inspectPcapLogList = getInspectPcapLogList(history.savepath);
				}
				
				list.add(history);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
		return list;
	}
	
	public static List<History> getAll(boolean loadLog) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<History> list = new ArrayList<>();
		
		try {
			String sql = "select * from inspect_history";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				History history = new History();
				history.profileId = rs.getString("profile_id");
				history.timestamp = rs.getString("timestamp");
				history.savepath = rs.getString("savepath");
				history.runTimer = rs.getString("run_timer");
				history.profile = Profile.getById(history.profileId, true);
				
				if (loadLog) {
					history.inspectXmlLogList = getInspectXmlLogList(history.savepath);
					history.inspectPcapLogList = getInspectPcapLogList(history.savepath);
				}
				
				list.add(history);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
		return list;
	}
	
	public static List<InspectXmlLog> getInspectXmlLogList(String path) {
		List<InspectXmlLog> list = new ArrayList<>();
		String [] xmls = FileUtil.scanFile(path, "^C_.*.xml$");
		if (xmls != null) {
			for (String xml : xmls) {
				String configId = xml.substring(0, xml.length()-4);
				Config config = Config.getById(configId, true);
				if (config != null) {
					try {
						list.add(new InspectXmlLog(
								config, 
								XML.toJSONObject(FileUtil.getContent(path+"/"+xml)).toString()));
					} catch (JSONException e) {
						e.printStackTrace();
					}
				}
			}
		}
		return list;
	}
	
	public static List<InspectPcapLog> getInspectPcapLogList(String path) {
		List<InspectPcapLog> list = new ArrayList<>();
		String [] pcaps = FileUtil.scanFile(path, "^C_.*.pcap$");
		if (pcaps != null) {
			for (String pcap : pcaps) {
				String configId = pcap.substring(0, pcap.length()-5);
				Config config = Config.getById(configId, true);
				if (config != null) {
					list.add(new InspectPcapLog(config, path+"/"+pcap));
				}
			}
		}
		return list;
	}
	
	public static void main(String [] args) {
		System.setProperty("catalina.home", "/home/kinow/git/wips-inspect/tomcat");
		List<History> list = History.getAll(false);
		for (History h : list) {
			System.out.println(h);
		}
	}
}
