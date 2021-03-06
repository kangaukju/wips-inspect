package air.wips.inspect.Login;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import air.wips.inspect.utils.CryptoUtil;
import air.wips.inspect.utils.DateUtil;
import sql.inspect.wips.air.DBFILE;
import sql.inspect.wips.air.SQLite3Connection;

public class Admin {
	private String username;
	private String password;
	private String lastLogin;
	private String expireDate;
	private long sessionTimeout = 60 * 60 * 1000; // second
	private long sessionTimestamp = 0;
	
	
	
	public long getSessionTimeout() {
		return sessionTimeout;
	}

	public void setSessionTimeout(long sessionTimeout) {
		this.sessionTimeout = sessionTimeout;
	}

	public long getSessionTimestamp() {
		return sessionTimestamp;
	}

	public void setSessionTimestamp(long sessionTimestamp) {
		this.sessionTimestamp = sessionTimestamp;
	}

	@Override
	public String toString() {
		return "Admin [username=" + username + ", password=" + password + ", lastLogin=" + lastLogin + "]";
	}
	
	public static void del(String username) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		String sql;
		try {
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			conn.setAutoCommit(false);
			sql = "delete from admin where username=?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, username);
			pstmt.executeUpdate();
			conn.commit();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
	
	public static void add(String username, String password) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			String sql = "insert into admin (username, password, last_login) values (?,?,?)";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, username);
			pstmt.setString(2, password);
			pstmt.setString(3, DateUtil.yyyyMMDDHHmmss());
			pstmt.executeUpdate();
			conn.commit();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt);
		}
	}
	
	public static Admin get(String username) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		Admin admin = null;
		try {
			String sql = "select * from admin where username=?";
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, username);
			rs = pstmt.executeQuery();
			if (rs == null || !rs.next()) {
				return null;
			}
			admin = new Admin();
			admin.username = rs.getString("username");
			admin.password = rs.getString("password");
			admin.lastLogin = rs.getString("last_login");
			admin.expireDate = rs.getString("expire_date");
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return admin;
	}
	
	public static Admin check(String username, String plainPassword) throws Exception {
		Admin admin = get(username);
		if (admin == null) {
			throw new Exception("Not found user");
		}		
		if (!CryptoUtil.sha512(plainPassword).equals(admin.password)) {
			throw new Exception("Incorrect password");
		}
		return admin;
	}	
}
