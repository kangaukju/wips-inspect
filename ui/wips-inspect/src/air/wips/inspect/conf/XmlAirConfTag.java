package air.wips.inspect.conf;

public class XmlAirConfTag {
	public static final String TYPE_STRING = "string";
	public static final String TYPE_HEX = "hex";
	
	private String id;
	private String len;
	private String data;
	private String type = TYPE_STRING;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getLen() {
		return len;
	}
	public void setLen(String len) {
		this.len = len;
	}
	public String getData() {
		return data;
	}
	public void setData(String data) {
		this.data = data;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	@Override
	public String toString() {
		return "AirConfTag [id=" + id + ", len=" + len + ", data=" + data + ", type=" + type + "]";
	}
}