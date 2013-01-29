#@include <RDFS>

@prefix rdf:        <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix rdfs:	    <http://www.w3.org/2000/01/rdf-schema#> .
@prefix xsd:        <http://www.w3.org/2001/XMLSchema#> .
@prefix gno:        <http://www.gnorasi.gr/ontology#> .


#-> tableAll().

[rdfs6:  (?a ?p ?b), (?p rdfs:subPropertyOf ?q) -> (?a ?q ?b)]
[rdfs9:  (?x rdfs:subClassOf ?y), (?a rdf:type ?x) -> (?a rdf:type ?y)]
[rdfs11: (?x rdfs:subClassOf ?y), (?y rdfs:subClassOf ?z) -> (?x rdfs:subClassOf ?z)]

[rule1: 
(?reg gno:hasFeature ?feat)
(?feat gno:featureRelatedTo gno:Band1)
(?feat gno:featureHasMeasure gno:Kurtosis)
(?feat gno:featureHasValue ?val)
greaterThan(?val, 0.0)
noValue(?reg, gno:depictsObject)
->
(?reg gno:depictsObject gno:ins_shrub)
]

[rule2: 
(?reg gno:hasFeature ?feat)
(?feat gno:featureRelatedTo gno:Band1)
(?feat gno:featureHasMeasure gno:Kurtosis)
(?feat gno:featureHasValue ?val)
lessThan(?val, -1.0)
noValue(?reg, gno:depictsObject)
->
(?reg gno:depictsObject gno:ins_house)
]

[rule3: 
(?reg gno:hasFeature ?feat)
(?feat gno:featureRelatedTo gno:Band1)
(?feat gno:featureHasMeasure gno:Mean)
(?feat gno:featureHasValue ?val)
greaterThan(?val, 400.0)
noValue(?reg, gno:depictsObject)
->
(?reg gno:depictsObject gno:ins_sea)
]


#[rule11: (?reg gno:depictsObject gno:ins_building)
#<-
#(?reg gno:hasFeature ?feat)
#(?feat gno:hasFeatureDescriptor ?featDesc)
#(?featDesc gno:hasFeatureDescriptorValue ?featDescVal)
#(?featDescVal gno:hasValueFloat ?featDescValFloat)
#lessThan(?featDescValFloat, 5.0)
#(?feat rdf:type gno:Spectral) 
#(?featDesc rdf:type gno:MSAVI)
#(?reg rdf:type gno:ImageRegion)
#]

	
	
	
	
	
	
	