package sql.inspect.wips.air;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class GetSeq {
	public static String get(final String idname, final String tablename, final String dbname) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		String seq = "0";
		
		try {
			String sql = String.format("select %s from %s", idname, tablename);
			conn = SQLite3Connection.getConnection(DBFILE.getDBFILE(dbname));
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				seq = rs.getString(1);
			}
		} catch (Exception e) {
			e.printStackTrace();
			seq = null;
		} finally {
			SQLite3Connection.sqlClose(conn, pstmt, rs);
		}
		return seq;
	}
}
