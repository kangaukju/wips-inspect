package sql.inspect.wips.air;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;

import air.wips.inspect.conf.XmlAirConf;
import air.wips.inspect.utils.DateUtil;

public class Config {
	private final static String CONFIG_ID_PREFIX = "C_";
	private String id;
	private String name;
	private String capturexml;
	private String shooterxml;
	private String created;
	private String updated;
	private XmlAirConf captureAirConf;
	private XmlAirConf shooterAirConf;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getCapturexml() {
		return capturexml;
	}
	public void setCapturexml(String capturexml) {
		this.capturexml = capturexml;
	}
	public String getShooterxml() {
		return shooterxml;
	}
	public void setShooterxml(String shooterxml) {
		this.shooterxml = shooterxml;
	}
	public String getCreated() {
		return created;
	}
	public void setCreated(String created) {
		this.created = created;
	}
	public String getUpdated() {
		return updated;
	}
	public void setUpdated(String updated) {
		this.updated = updated;
	}
	@Override
	public String toString() {
		return "Config [id=" + id + ", name=" + name + ", capturexml=" + capturexml + ", shooterxml=" + shooterxml
				+ ", created=" + created + ", updated=" + updated + "]";
	}
	
	public static void del(String configId) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		String sql;
		try {
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			conn.setAutoCommit(false);
			sql = "delete from config where id=?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, configId);
			pstmt.executeUpdate();
			pstmt.close();
			
			sql = "delete from profile_config_ref where config_id=?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, configId);
			pstmt.executeUpdate();
			
			XmlAirConf.deleteAirConf(configId);
			
			conn.commit();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}	
	
	public void add() {
		Config.add(this);
	}
	
	public static void add(Config config) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		String now = DateUtil.yyyyMMDDHHmmss();
		try {
			String sql = "insert into config (id,name,capturexml,shooterxml,created,updated) values (?,?,?,?,?,?)";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, config.id);
			pstmt.setString(2, config.name);
			pstmt.setString(3, config.capturexml);
			pstmt.setString(4, config.shooterxml);
			pstmt.setString(5, now); config.created = now;
			pstmt.setString(6, now); config.updated = now;
			pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
	
	public void update() {
		Config.update(this);
	}
	
	public static void update(Config config) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		String now = DateUtil.yyyyMMDDHHmmss();
		int idx = 1;
		try {
			String sql;
			String namesql = "";
			String capturesql = "";
			String shootersql = "";
			if (config.name != null)
				namesql = ",name=?";
			if (config.capturexml != null)
				capturesql = ",capturexml=?";
			if (config.shooterxml != null)
				shootersql = ",shooterxml=?";
			
			sql = "update config set updated=?"+namesql+capturesql+shootersql+" where id=?";
			
			
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(idx++, now); config.updated = now;
			if (config.name != null)
				pstmt.setString(idx++, config.name);
			if (config.capturexml != null)
				pstmt.setString(idx++, config.capturexml);
			if (config.shooterxml != null)
				pstmt.setString(idx++, config.shooterxml);			
			pstmt.setString(idx++, config.id);
			pstmt.executeUpdate();
			System.out.println(config);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
	
	public static void update(long id) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			String sql = "delete from config where id=?";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setLong(1, id);
			pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}		
	}
	

	public static String newConfigId() {
		return CONFIG_ID_PREFIX + System.currentTimeMillis();
	}
	
	public static List<Config> getByName(String name) {
		return getByName(name, false);
	}
	
	public static List<Config> getByName(String name, boolean loadAirConf) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<Config> list = new ArrayList<>();
		
		try {
			String sql = "select * from config where name = ? order by id asc";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, name);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				Config c = new Config();
				c.id = rs.getString("id");
				c.name = rs.getString("name");
				c.capturexml = rs.getString("capturexml");
				c.shooterxml = rs.getString("shooterxml");
				c.updated = rs.getString("updated");
				c.created = rs.getString("created");				
				if (loadAirConf) {
					try {
						String captureFilename = XmlAirConf.captureFilename(c.id);
						c.captureAirConf = XmlAirConf.loadAirConf(captureFilename);
					} catch (Exception e) {
						c.captureAirConf = null;
						e.printStackTrace();
					}
					try {
						String shooterFilename = XmlAirConf.shooterFilename(c.id);
						c.shooterAirConf = XmlAirConf.loadAirConf(shooterFilename);
					} catch (Exception e) {
						c.shooterAirConf = null;
						e.printStackTrace();
					}
				}
				
				list.add(c);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return list;
	}
	
	public static Config getById(String id) {
		return getById(id, false);
	}
	
	public static Config getById(String id, boolean loadAirConf) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		Config c = null;
		
		try {
			String sql = "select * from config where id = ?";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, id);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				c = new Config();
				c.id = rs.getString("id");
				c.name = rs.getString("name");
				c.capturexml = rs.getString("capturexml");
				c.shooterxml = rs.getString("shooterxml");
				c.updated = rs.getString("updated");
				c.created = rs.getString("created");
				if (loadAirConf) {
					try {
						String captureFilename = XmlAirConf.captureFilename(c.id);
						c.captureAirConf = XmlAirConf.loadAirConf(captureFilename);
					} catch (Exception e) {
						c.captureAirConf = null;
						e.printStackTrace();
					}
					try {
						String shooterFilename = XmlAirConf.shooterFilename(c.id);
						c.shooterAirConf = XmlAirConf.loadAirConf(shooterFilename);
					} catch (Exception e) {
						c.shooterAirConf = null;
						e.printStackTrace();
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return c;
	}
	
	public static List<Config> getAll() {
		return getAll(false);
	}
	
	public static List<Config> getAll(boolean loadAirConf) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<Config> list = new ArrayList<>();
		
		try {
			String sql = "select * from config order by id asc";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				Config c = new Config();
				c.id = rs.getString("id");
				c.name = rs.getString("name");
				c.capturexml = rs.getString("capturexml");
				c.shooterxml = rs.getString("shooterxml");
				c.updated = rs.getString("updated");
				c.created = rs.getString("created");
				if (loadAirConf) {
					try {
						String captureFilename = XmlAirConf.captureFilename(c.id);
						c.captureAirConf = XmlAirConf.loadAirConf(captureFilename);
					} catch (Exception e) {
						c.captureAirConf = null;
						e.printStackTrace();
					}
					try {
						String shooterFilename = XmlAirConf.shooterFilename(c.id);
						c.shooterAirConf = XmlAirConf.loadAirConf(shooterFilename);
					} catch (Exception e) {
						c.shooterAirConf = null;
						e.printStackTrace();
					}
				}
				list.add(c);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return list;
	}
}