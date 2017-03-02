package sql.inspect.wips.air;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class SQLite3Connection {
	static {
		try {
			Class.forName("org.sqlite.JDBC");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}	
	}
	
	static public Connection getConnection(String dbfile) {
		Connection conn = null;		
		try {
			conn = DriverManager.getConnection("jdbc:sqlite:"+dbfile);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return conn;
	}
	
	static public void sqlClose(Connection conn) {
		sqlClose(conn, null, null);
	}
	
	static public void sqlClose(Connection conn, PreparedStatement pstmt) {
		sqlClose(conn, pstmt, null);
	}
	
	static public void sqlClose(Connection conn, PreparedStatement pstmt, ResultSet rs) {
		try {			
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (rs != null) try { rs.close(); } catch(Exception e) {}
			if (pstmt != null) try { pstmt.close(); } catch(Exception e) {}
			if (conn != null) try { conn.close(); } catch(Exception e) {}
		}
	}
}
