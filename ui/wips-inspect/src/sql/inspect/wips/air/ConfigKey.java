package sql.inspect.wips.air;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class ConfigKey {

	public static long newKey() {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		String sql;
		long key = 0;
		try {
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE("profiles"));
			conn.setAutoCommit(false);
			
			sql = "select max(key) from configkey";
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			if (rs == null || rs.next() == false) {
				key = 0;
			} else {
				key = rs.getLong(1);
			}
			rs.close();
			pstmt.close();
						
			key++;
			if (key == 1) {
				sql = "insert into configkey (key) values (1)";
				pstmt = conn.prepareStatement(sql);
			} else {
				sql = "update configkey set key = ?";
				pstmt = conn.prepareStatement(sql);
				pstmt.setLong(1, key);
			}
			pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try { conn.commit(); } catch (Exception e) { e.printStackTrace(); }
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return key;
	}
}
