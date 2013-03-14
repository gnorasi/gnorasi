package gr.iti.gnorasi.owlim;

import org.openrdf.repository.Repository;
import org.openrdf.sail.Sail;

public class OWLIMRepositoryFactory extends OWLIMRepository {
	
	public static Repository getNativeRepository(String configFilename, String mainNS, String repName) {
		OWLIMRepository repClass = new OWLIMRepository();
		repClass.setNativeRepository(configFilename, mainNS, repName);
		
		return repClass.getRepository();
	}
	
	public static Sail getLiteSail() {
		OWLIMRepository repClass = new OWLIMRepository();
		return repClass.getOWLIMLiteSail();
	}
	
	public static Sail getSESail() {
		OWLIMRepository repClass = new OWLIMRepository();
		return repClass.getOWLIMSESail();
	}
}
