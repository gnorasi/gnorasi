package gr.iti.gnorasi.rules.geo;

import java.util.ArrayList;
import java.util.List;

import org.openrdf.model.URI;

public class GeoRule {
	List<GeoRulePredicate> predicates;
	GeoRuleHead head;
	String id;
	URI restriction;
	
	
	public GeoRule() { 
		predicates = new ArrayList<GeoRulePredicate>();
		head = new GeoRuleHead();
		id = "";
	}
	
	public void setId(String id) {
		this.id = id;
	}
	
	public String getId() {
		return id;
	}
	
	public void addPredicate(GeoRulePredicate grp) {
		predicates.add(grp);
	}
	public List<GeoRulePredicate> getPredicates() {
		return predicates;
	}
	public void setPredicates(List<GeoRulePredicate> l) {
		predicates = l;
	}
	public void clearPredicates() {
		predicates.clear();
	}
	
	public void setHead(GeoRuleHead grh) {
		head = grh;
	}
	public GeoRuleHead getHead() {
		return head;
	}
	
	public void setRestriction(URI res) {
		restriction = res;
	}
	public URI getRestriction() {
		return restriction;
	}
	
	@Override
	public String toString() {
		String r = "\nRule '"+id+"' = {";
		r += head.toString();
		for (GeoRulePredicate predicate : predicates) {
			r += predicate;
		}
		r += "}";
		
		return r;
	}
}
