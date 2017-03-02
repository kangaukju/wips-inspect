package sql.inspect.wips.air;

public class DBFILE {
	public static final String getDBFILE(String dbname) {
		return System.getProperty("catalina.home") + "/db/" + dbname + ".db";
		//return "/home/kinow/git/wips-inspect/tomcat/db/profiles.db";
	}
}
