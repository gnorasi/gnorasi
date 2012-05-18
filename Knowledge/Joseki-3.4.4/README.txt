Joseki: The Jena RDF Server
===========================

    http://www.joseki.org/

	Andy Seaborne
	andy.seaborne@epimorphics.com

Joseki is a server implementing the SPARQL protocol.

SPARQL (query language and protocol is the product of the 
W3C RDF Data Access Working Group (DAWG).

Joseki is part of the Jena RDF framework.  Joseki is open source under a
BSD-style license.


Working Group home page:
http://www.w3.org/2001/sw/DataAccess/

Documents:
<to be added>

Joseki3 provides SPARQL as the protocol and the main query
langauge. Joseki3 is not compatible with Joseki2. 

The differences in architecture aren't huge but there are many minor
differences but the objective of Joseki3 is to provide all the new features
of SPARQL.

Porting from Joseki2
====================

Client Libraries
----------------
The client library is part of ARQ, an implementation of the
SPARQL query language for Jena.

http://jena.hpl.hp.com/ARQ

ARQ is included in the Joseki download.

Inference Models
----------------

Joseki hosts models provided by Jena2, including inferencing models: data
can be combined with an OWL ontology or RDFS vocabulary description to
produce an RDF model that has both ground and inferred statements.  Query
is then used to access the model.

The Download
------------

The main directory is Joseki-<version>.  Servers run in this directory to
find auxiliary files.

The main sub-directories are:

    + lib/       - JARs needed
    + bin/       - scripts (check before use)
    + etc/       - configuration and various support files
    + doc/       - a copy of the online documentation
    + webapps/   - Joseki is a web application


Running Examples
----------------

A simple server can be run by:

1/ Add all the JARs in lib/ to the CLASSPATH
2/ Run
   java -cp <classpath> joseki.rdfserver joseki-config.ttl

For more details on configuring and running a server, see:

http://www.joseki.org/documentation.html

Documentation
-------------

The website http://www.joseki.org/ describes Joseki and provides
documentation on server configuration and on the client library.  It also
includes examples of access using common, unmodified applications like
wget.  Details about the use of HTTP GET, how to create query language
processors and how to add modules for data-specific fetch operations can
also be found on the web site.  A copy of the web site is in the doc/
directory of the download.


Support
-------

Please send questions to jena-dev@groups.yahoo.com


Development
-----------

The Joseki development project is hosted by SourceForge:

Project:   http://sourceforge.net/projects/joseki
CVS:       http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/joseki/

It relies on other open source software: 

    JUnit, Jetty, Jena (which uses Xerces,
     Log4J, ORO, util.concurrent)

