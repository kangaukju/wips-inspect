package air.wips.inspect.profile;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import air.wips.inspect.conf.XmlAirConf;
import sql.inspect.wips.air.SQLite3Connection;

public class AirProfiles {
	private String name;
	private String airCaptureXMLFile;
	private String airShooterXMLFile;
	private String createdTime;
	private String updatedTime;	
	private List<XmlAirConf> airCaptureList;
	private List<XmlAirConf> airShooterList;
	
	public static final String SQLITE3_DB_HOME = "/home/kinow/git/wips-inspect/db";
	public static final String AIR_PROFILES_DB = SQLITE3_DB_HOME+"/profiles.db";
	
	
	@Override
	public String toString() {
		return "AirProfiles [name=" + name + ", airCaptureXMLFile=" + airCaptureXMLFile + ", airShooterXMLFile="
				+ airShooterXMLFile + ", createdTime=" + createdTime + ", updatedTime=" + updatedTime + "]";
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getAirCaptureXMLFile() {
		return airCaptureXMLFile;
	}
	public void setAirCaptureXMLFile(String airCaptureXMLFile) {
		this.airCaptureXMLFile = airCaptureXMLFile;
	}
	public String getAirShooterXMLFile() {
		return airShooterXMLFile;
	}
	public void setAirShooterXMLFile(String airShooterXMLFile) {
		this.airShooterXMLFile = airShooterXMLFile;
	}
	public String getCreatedTime() {
		return createdTime;
	}
	public void setCreatedTime(String createdTime) {
		this.createdTime = createdTime;
	}
	public String getUpdatedTime() {
		return updatedTime;
	}
	public void setUpdatedTime(String updatedTime) {
		this.updatedTime = updatedTime;
	}
	public List<XmlAirConf> getAirCaptureList() {
		return airCaptureList;
	}
	public void setAirCaptureList(List<XmlAirConf> airCaptureList) {
		this.airCaptureList = airCaptureList;
	}
	public List<XmlAirConf> getAirShooterList() {
		return airShooterList;
	}
	public void setAirShooterList(List<XmlAirConf> airShooterList) {
		this.airShooterList = airShooterList;
	}
	
	private static final SimpleDateFormat _format_date_hour = new SimpleDateFormat("yyy-MM-DD HH:mm:ss");
	public static final String getTime(long time) {
		java.util.Date timestamp = new java.util.Date(time*1000);
		return _format_date_hour.format(timestamp);
	}
	
	public static boolean exist(String name) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		boolean find = false;
		
		try {
			String sql = "select name from profiles where name=?";
			conn = SQLite3Connection.getConnection(AIR_PROFILES_DB);
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, name);
			rs = pstmt.executeQuery();
			if (rs == null || !rs.next()) {
				find = false;
			} else {
				if (rs.getString("name").equals(name)) {
					find = true;
				} else {
					find = false;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return find;
	}
	
	public static List<AirProfiles> getAll() throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<AirProfiles> list = new ArrayList<>();
		
		try {
			String sql = "select * from profiles";
			conn = SQLite3Connection.getConnection(AIR_PROFILES_DB);
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				AirProfiles p = new AirProfiles();
				p.setName(rs.getString("name"));
				p.setAirCaptureXMLFile(rs.getString("aircapture_xml_file"));
				p.setAirShooterXMLFile(rs.getString("airshooter_xml_file"));
				p.setCreatedTime(rs.getString("createdtime"));
				p.setUpdatedTime(rs.getString("updatedtime"));
				
				p.setAirCaptureList(XmlAirConf.loadAirConfList(p.getAirCaptureXMLFile()));
				p.setAirShooterList(XmlAirConf.loadAirConfList(p.getAirShooterXMLFile()));
				
				list.add(p);
			}			
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return list;
	}
	
	public static AirProfiles get(String name) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		AirProfiles p = new AirProfiles();
		
		try {
			String sql = "select * from profiles where name = ?";
			conn = SQLite3Connection.getConnection(AIR_PROFILES_DB);
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, name);
			rs = pstmt.executeQuery();
			if (!rs.next()) {
				return null;
			}
			
			p.setName(rs.getString("name"));
			p.setAirCaptureXMLFile(rs.getString("aircapture_xml_file"));
			p.setAirShooterXMLFile(rs.getString("airshooter_xml_file"));
			p.setCreatedTime(rs.getString("createdtime"));
			p.setUpdatedTime(rs.getString("updatedtime"));
			
			p.setAirCaptureList(XmlAirConf.loadAirConfList(p.getAirCaptureXMLFile()));
			p.setAirShooterList(XmlAirConf.loadAirConfList(p.getAirShooterXMLFile()));
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return p;
	}	
	
	public static void insert(String name, String airCaptureXMLFile, String airShooterXMLFile) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		long now = new Date().getTime() / 1000;
		try {
			if (exist(name)) {
				throw new Exception("already exist name - "+name);
			}
			
			String sql = "insert into profiles "
					+ "(name, aircapture_xml_file, airshooter_xml_file, createdtime, updatedtime) "
					+ "values (?,?,?,?,?)";
			conn = SQLite3Connection.getConnection(AIR_PROFILES_DB);
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, name);
			pstmt.setString(2, airCaptureXMLFile);
			pstmt.setString(3, airShooterXMLFile);
			pstmt.setString(4, getTime(now));
			pstmt.setString(5, getTime(now));
			pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
	
	public static void update(String name, String airCaptureXMLFile, String airShooterXMLFile) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		long now = new Date().getTime() / 1000;
		try {
			String sql = "update profiles set "
					+ "aircapture_xml_file=?, airshooter_xml_file=?, updatedtime=? "
					+ "where name=?";
			conn = SQLite3Connection.getConnection(AIR_PROFILES_DB);
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, airCaptureXMLFile);
			pstmt.setString(2, airShooterXMLFile);
			pstmt.setString(3, getTime(now));
			pstmt.setString(4, name);
			pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
}
