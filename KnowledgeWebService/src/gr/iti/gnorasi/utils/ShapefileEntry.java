package gr.iti.gnorasi.utils;

public class ShapefileEntry {
	private String[] entry;
	
	public ShapefileEntry(){};
	
	public ShapefileEntry(int size) {
		entry = new String[size];
	}
	
	public ShapefileEntry(String[] data) {
		entry = data;
	}
	
	public int getSize() {
		return entry.length;
	}
	
	public void setEntry(String[] data) {
		entry = data;
	}
	
	public void setEntryIndex(String data, int index) {
		entry[index] = data;
	}
	
	public String[] getEntry() {
		return entry;
	}
	
	public String getEntryIndex(int i) {
		if (i>-1 && i<entry.length )
			return entry[i];
		
		return null;
	}
}
