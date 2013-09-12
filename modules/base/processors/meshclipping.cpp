#include "meshclipping.h"
#include <inviwo/core/datastructures/geometry/edge.h>
#include <inviwo/core/datastructures/geometry/basemeshcreator.h>

namespace inviwo {

ProcessorClassName(MeshClipping, "MeshClipping"); 
ProcessorCategory(MeshClipping, "Geometry Creation");
ProcessorCodeState(MeshClipping, CODE_STATE_EXPERIMENTAL);

MeshClipping::MeshClipping()
	: ProcessorGL(),
	inport_("geometry.input"),
	outport_("geomtry.output"),
	floatPropertyRotX_("Rotation X", "Rotation X", 0.0f, 0.0f, 360.0f, 0.1f),
	floatPropertyRotY_("Rotation Y", "Rotation Y", 0.0f, 0.0f, 360.0f, 0.1f),
	floatPropertyRotZ_("Rotation Z", "Rotation Z", 0.0f, 0.0f, 360.0f, 0.1f),
	floatPropertyPlaneHeight("Cutting plane y coord.", "Cutting plane Y", 0.0f,-1.2f,1.2f,0.1f),
	plane_(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, -0.5f)),
	clippingEnabled_("clippingEnabled", "Enable clipping", false)
{
	addPort(inport_);
	addPort(outport_);
	/*addProperty(floatPropertyRotX_);
	addProperty(floatPropertyRotY_);
	addProperty(floatPropertyRotZ_);*/
	//addProperty(floatPropertyPlaneHeight);
	addProperty(clippingEnabled_);
}

MeshClipping::~MeshClipping() {}

void MeshClipping::initialize() {
	ProcessorGL::initialize();
}

void MeshClipping::deinitialize() {
	ProcessorGL::deinitialize();
}

float MeshClipping::degreeToRad(float degree) {
	return degree * (glm::pi<float>() / 180.f);
}

std::vector<Edge> triangleListtoEdgeList(std::vector<unsigned int> triList) {
	std::vector<Edge> result;
	std::cout << "Sixe of tri list: " << triList.size()-1 << std::endl;
	for (int i=0; i<triList.size(); ++i) {
		std::cout << "i =" << i << std::endl;
		Edge e1;
		if(i==0 || i%3 == 0)  {

			if(i+1<triList.size()) {
				e1.v1 = triList.at(i);
				e1.v2 = triList.at(i+1);
				if (!result.empty() && std::find(result.begin(),result.end(),e1) != result.end()) {
					std::cout << "Found duplicate!, " << e1.v1 << "->" << e1.v2 << std::endl;
				} else {
					result.push_back(e1);
				}
			}
			if(i+2<triList.size()) {
				e1.v1 = triList.at(i);
				e1.v2 = triList.at(i+2);
				if (!result.empty() && std::find(result.begin(),result.end(),e1) != result.end()) {
					std::cout << "Found duplicate, " << e1.v1 << "->" << e1.v2 << std::endl;
				} else {
					result.push_back(e1);
				}
			}
		} else if((i-1)%3 == 0) { // +2
			if(i+2<triList.size()) {
				e1.v1 = triList.at(i);
				e1.v2 = triList.at(i+2);
				if (!result.empty() && std::find(result.begin(),result.end(),e1) != result.end()) {
					std::cout << "Found duplicate, " << e1.v1 << "->" << e1.v2 << std::endl;
				} else {
					result.push_back(e1);
				}
				
			}
		} else if((i+1)%3 == 0) { //+1
			if(i+1<triList.size()) {
				e1.v1 = triList.at(i);
				e1.v2 = triList.at(i+1);
				if (!result.empty() && std::find(result.begin(),result.end(),e1) != result.end()) {
					std::cout << "Found duplicate, " << e1.v1 << "->" << e1.v2 << std::endl;
				} else {
					result.push_back(e1);
				}
			}
		}
	}

	std::cout << "Size of edge list: " << result.size() << std::endl;
	for(int i=0; i<result.size();++i) {
		std::cout << "Edge, " << i << " = " << result.at(i).v1 << "->" << result.at(i).v2 << std::endl;
	}
	return result;
}

GeometryRAM* MeshClipping::clipGeometry(GeometryRAM* in, BaseMeshRAM* clip) {
	// Funktioner för att klippa mot geometri
	if(!dynamic_cast<BaseMeshRAM*>(in)) {
		std::cerr << "Can only clip a BaseMeshRAM";
		return NULL;
	}

	// 2. Tillämpa klippning mot plan
	// 3. Skapa ny geometri (mesh) från vertexlistan skapad av klippningen

	return in; // Return new clipped mesh instead of clip plane later
}

GeometryRAM* MeshClipping::clipGeometryAgainstPlane(GeometryRAM* in, Plane &plane) {
	std::cout << "Entered clipGeometryAgainstPlane(...).\n";
	if(!clippingEnabled_.get())
		return in;
	
	if(!dynamic_cast<BaseMeshRAM*>(in)) {
		std::cerr << "Can only clip a BaseMeshRAM*";
		return NULL;
	}

	std::cout << "Casting inputMesh.\n";
	BaseMeshRAM *inputMesh = dynamic_cast<BaseMeshRAM*>(in);

	// --- Sutherland-Hogdman algorithm---

	/* ---Logg/buggar
		-	Har en bugg nu där om meshen är helt "utanför" clip-planet så finns det
			ingen mesh att returnera, och programmet crashar.
		-	Måste nu lyckas bygga en korrekt sorterad triangellista från de nya vertsen
	*/

	std::cout << "Fetching vertex- and triangle lists.\n";
	std::vector<glm::vec3> outputList = inputMesh->getVertexList()->getAttributeContainer();
	std::vector<unsigned int> triangleList = inputMesh->getIndexList()->getAttributeContainer();
	std::vector<Edge> edgeList = triangleListtoEdgeList(triangleList);
	/*for(int i=0; i<outputList.size(); ++i)
		std::cout << "Original verts: ("+std::to_string(outputList.at(i)[0])+", "+std::to_string(outputList.at(i)[1])+", "+std::to_string(outputList.at(i)[2])+")\n";
	for(int i=0; i<triangleList.size(); ++i)
		std::cout << "indexList: " << triangleList.at(i) << std::endl;
	std::cout << "Number of verts in input mesh: " << inputList.size() << std::endl;*/

	// For each clip plane, do:
	std::cout << "Running clipping algorithm.\n";
	std::vector<glm::vec3> inputList = outputList;
	outputList.clear();
	std::vector<unsigned int> outputIndexList;
	for(int i=0; i<edgeList.size(); ++i) {
		unsigned int Sind = edgeList.at(i).v1;
		unsigned int Eind = edgeList.at(i).v2;
		glm::vec3 S = inputList.at( Sind );
		glm::vec3 E = inputList.at( Eind );
		if(plane.isInside(E)) {
			if(!plane.isInside(S)) { // Going in
				outputIndexList.push_back(999);
				outputList.push_back(plane.getIntersection(S,E));
			} else {
				//outputIndexList.push_back(edgeList.at(i).v1);
				//outputList.push_back(S); // Osäker på denna
			}
			if(outputIndexList.empty() || 
				std::find(outputIndexList.begin(),outputIndexList.end(),Eind) == outputIndexList.end()) {
				outputIndexList.push_back(Eind);
 				outputList.push_back(E);
			}
		} else if(plane.isInside(S)) { // Going out
			outputIndexList.push_back(999);
			outputList.push_back(plane.getIntersection(S,E));
		}
	}
	// End, for each clip plane


	// Old algorithm - DO NOT USE
	// For each clip plane, do:
	/*std::vector<glm::vec3> inputList = outputList;
	outputList.clear();
	glm::vec3 S = inputList.at(customIndexList.back());
	for(int i=0; i<customIndexList.size(); ++i) {
		glm::vec3 E = inputList.at(customIndexList.at(i));
		if(plane.isInside(E)) {
			if(!plane.isInside(S)) { // Going in
				outputList.push_back(plane.getIntersection(S,E));
			}
			outputList.push_back(E);
		} else if(plane.isInside(S)) { // Going out
			outputList.push_back(plane.getIntersection(S,E));
		}
		S = E; // Traverse all vertices in subject polygon (for 3d impl. need to traverse every edge)
	}
	// End, for each clip plane
	*/

	for(int i=0; i<outputIndexList.size(); ++i) 
		std::cout << "Vertex indices: " << outputIndexList.at(i) << std::endl;

	for(int i=0; i<outputList.size(); ++i)
		std::cout << "Clipped verts, " << i << ": ("+std::to_string(outputList.at(i)[0])+", "+std::to_string(outputList.at(i)[1])+", "+std::to_string(outputList.at(i)[2])+")\n";


	// Bygg ny BaseMeshRAM här från outputList-vektor
	std::cout << "Buildning new mesh from clipped vertices.\n";
	BaseMeshRAM* outputMesh = new BaseMeshRAM();
	outputMesh->initialize();

	for(int i=0; i<outputList.size(); ++i) {
		outputMesh->addVertex(outputList.at(i), glm::vec3(1.f), glm::vec4(1.,i/(float)outputList.size(),0.,1.0f));
	}

	std::cout << "Number of verts in output mesh: " << 
		outputList.size() << std::endl;

// 	outputMesh->setIndicesInfo(GeometryRepresentation::POINTS, GeometryRepresentation::NONE);
//  	for(int i=0; i<outputList.size(); ++i) {
//  		outputMesh->addIndex(i);
//  		//std::cout << "Adding to index list, vertex no.: " << i << std::endl;
//  	}
 	outputMesh->setIndicesInfo(GeometryRepresentation::TRIANGLES, GeometryRepresentation::STRIP);
 	outputMesh->addIndex(0);
 	outputMesh->addIndex(2);
 	outputMesh->addIndex(1);
 	outputMesh->addIndex(3);
 	outputMesh->addIndex(4);
 	outputMesh->addIndex(2);
 	outputMesh->addIndex(5);
 	outputMesh->addIndex(6);
 	outputMesh->addIndex(4);
 	outputMesh->addIndex(7);
 	outputMesh->addIndex(1);
 	outputMesh->addIndex(6);
 	outputMesh->addIndex(0);
 	outputMesh->addIndex(2);


	std::cout << "Returning new mesh.\n";

	return outputMesh;
}

void MeshClipping::process() {
	// 1. Läs in geometrin från inport
	GeometryRAM *geom = const_cast<GeometryRAM*>(inport_.getData()->getRepresentation<GeometryRAM>());

	/*
	// 2. Skappa klipp-plans-meshen (rektangel)
	glm::vec3 posLl(-1.,-1.,0.);
	glm::vec3 posUr(1.,1.,0.);
	
	BaseMeshRAM* rec = BaseMeshCreator::rectangle(posLl, posUr);
	
	// 3. Applicera klipplan på geometrin från inport
	GeometryRAM *clippedGeom = clipGeometry(geom, rec);*/
	if(clippingEnabled_.get()) {
		std::cout << "Setting plane point from property.\n";
		//plane_.setPoint(glm::vec3(0.0f,floatPropertyPlaneHeight.get(),0.0f));
		std::cout << "Calling clipping method.\n";
		GeometryRAM *clippedPlaneGeom = clipGeometryAgainstPlane(geom, plane_);

		std::cout << "Setting new mesh as outport data.\n";
		// 4. Sätt utporten till nya geometrin
		outport_.setData(new Geometry(clippedPlaneGeom));
		std::cout << "Done.\n";
	} else {
		outport_.setData(new Geometry(geom));
	}
}

} // namespace