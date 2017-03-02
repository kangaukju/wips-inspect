package air.wips.inspect.error;

import com.google.gson.Gson;

public class GoodBad {
	private boolean good;
	private String cause;

	public GoodBad good() {
		this.good = true;
		return this;
	}
	public GoodBad bad(String cause) {
		this.good = false;
		this.cause = cause;
		return this;
	}
	public GoodBad bad(Exception e) {
		this.good = false;
		this.cause = e.getMessage();
		return this;
	}
	public String getCause() {
		return cause;
	}
	
	public static void main(String [] args) {
		System.out.println(new Gson().toJson(new GoodBad().bad(new Exception("kaka"))));
		
		System.out.println(new Gson().toJson(new GoodBad().good()));
	}
}
