package gr.iti.gnorasi.rules.fuzzy;

import java.util.ArrayList;
import java.util.List;

import org.openrdf.model.URI;

public class FuzzyRule {
	
	List<FuzzyRestriction> restrictionList;
	URI head;
	String id;
	String op;
	
	public FuzzyRule() {
		restrictionList = new ArrayList<FuzzyRestriction>();
	}
	
	public String getId() {
		return id;
	}
	
	public void setId(String id) {
		this.id = id;
	}
	
	public List<FuzzyRestriction> getRestrictionList() {
		return restrictionList;
	}

	public void setRestrictionList(List<FuzzyRestriction> list) {
		this.restrictionList = list;
	}

	public URI getHead() {
		return head;
	}

	public void setHead(URI head) {
		this.head = head;
	}

	public String getOp() {
		return op;
	}

	public void setOp(String op) {
		this.op = op;
	}
	
}
