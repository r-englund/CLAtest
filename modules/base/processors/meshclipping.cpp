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
	plane_(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
	clippingEnabled_("clippingEnabled", "Enable clipping", false),
	EPSILON(0.00001f) // For float comparison
{
	addPort(inport_);
	addPort(outport_);
	/*addProperty(floatPropertyRotX_);
	addProperty(floatPropertyRotY_);
	addProperty(floatPropertyRotZ_);
	addProperty(floatPropertyPlaneHeight);*/
	addProperty(clippingEnabled_);
}

MeshClipping::~MeshClipping() {}

void MeshClipping::initialize() {
	ProcessorGL::initialize();
}

void MeshClipping::deinitialize() {
	ProcessorGL::deinitialize();
}

// Convert degrees to radians
float MeshClipping::degreeToRad(float degree) {
	return degree * (glm::pi<float>() / 180.f);
}

std::vector<unsigned int> edgeListtoTriangleList(std::vector<Edge>& edges) {
	// Traverse edge list and construct correctly sorted triangle strip list.
	return std::vector<unsigned int>();
}

// Extract edges from triangle strip list
std::vector<Edge> triangleListtoEdgeList(std::vector<unsigned int> triList) {
	std::vector<Edge> result;
	//std::cout << "Size of tri list: " << triList.size()-1 << std::endl;
	for (size_t i=0; i<triList.size(); ++i) {
		Edge e1;
		if(i==0 || i%3 == 0)  {

			if(i+1<static_cast<int>(triList.size())) {
				e1.v1 = triList.at(i);
				e1.v2 = triList.at(i+1);
				if (result.empty() || std::find(result.begin(),result.end(),e1) == result.end()) {
					result.push_back(e1);
				}
			}
			if(i+2<static_cast<int>(triList.size())) {
				e1.v1 = triList.at(i);
				e1.v2 = triList.at(i+2);
				if(result.empty() || std::find(result.begin(),result.end(),e1) == result.end()) {
					result.push_back(e1);
				}
			}
		} else if((i-1)%3 == 0) {
			if(i+2<static_cast<int>(triList.size())) {
				e1.v1 = triList.at(i);
				e1.v2 = triList.at(i+2);
				if(result.empty() || std::find(result.begin(), result.end(),e1) == result.end()) {
					result.push_back(e1);
				}
			}
		} else if((i+1)%3 == 0) {
			if(i+1<static_cast<int>(triList.size())) {
				e1.v1 = triList.at(i);
				e1.v2 = triList.at(i+1);
				if(result.empty() || std::find(result.begin(), result.end(),e1) == result.end()) {
					result.push_back(e1);
				}
			}
		}
	}

	//std::cout << "Size of edge list: " << result.size() << std::endl;

	/*for(size_t i=0; i<result.size();++i) {
		std::cout << "Edge, " << i << " = " << result.at(i).v1 << "->" << result.at(i).v2 << std::endl;
	}*/
	return result;
}

GeometryRAM* MeshClipping::clipGeometry(GeometryRAM* in, BaseMeshRAM* clip) {
	// STUB
	// For clipping against qudrilateral (finite plane)
	if(!dynamic_cast<BaseMeshRAM*>(in)) {
		std::cerr << "Can only clip a BaseMeshRAM";
		return NULL;
	}
	return in;
}

GeometryRAM* MeshClipping::clipGeometryAgainstPlane(GeometryRAM* in, Plane &plane) {
	//std::cout << "Entered clipGeometryAgainstPlane(...).\n";
	if(!clippingEnabled_.get())
		return in;
	
	if(!dynamic_cast<BaseMeshRAM*>(in)) {
		std::cerr << "Can only clip a BaseMeshRAM*";
		return NULL;
	}

	//std::cout << "Casting inputMesh.\n";
	BaseMeshRAM *inputMesh = dynamic_cast<BaseMeshRAM*>(in);

	/* ---TODO / bugs
		-	Create correct outputEdgeList while running clipping algorithm, currently edges between clipped verts
			sometimes end up in incorrect order.
		-	Use correct outputEdgeList to create a correctly sorted triangle strip list
		-	If entire mesh is outside of clip plane, empty mesh is returned causing crash.
	*/

	//std::cout << "Fetching vertex- and triangle lists.\n";
	std::vector<glm::vec3> outputList = inputMesh->getVertexList()->getAttributeContainer(); // Vertex list
	std::vector<unsigned int> triangleList = inputMesh->getIndexList()->getAttributeContainer(); // Triangle list
	std::vector<Edge> edgeList = triangleListtoEdgeList(triangleList);
	std::vector<unsigned int> clippedVertInd;

	// For each clip plane, do:
	std::vector<glm::vec3> inputList = outputList;
	outputList.clear();

	std::vector<unsigned int> outputIndexList; // vertex index list
	std::vector<Edge> outputEdgeList; // output edge list

	// Iterate over edges extracted from triangle strip list, and perform clipping against plane
	for(size_t i=0; i<edgeList.size(); ++i) {

		//std::cout << "i = "<<i<<std::endl;

		unsigned int Sind = edgeList.at(i).v1;
		unsigned int Eind = edgeList.at(i).v2;

		glm::vec3 S = inputList.at( Sind );
		glm::vec3 E = inputList.at( Eind );

		Edge edge;
		int duplicate = -1;

		// For each clip plane
		if(plane.isInside(E)) {
			if(!plane.isInside(S)) { // Going in
				//std::cout<<"Going in!\n";
				//Måste stoppa in alla verts till edges i rätt ordning för att inte mucka upp vert-id:n
				glm::vec3 clippedVert = plane.getIntersection(S,E);
				outputList.push_back(clippedVert);
				outputIndexList.push_back(outputList.size()-1); // Ny vertex, uppdatera edge-listan
				clippedVertInd.push_back(outputList.size()-1);
				edge.v1=outputList.size()-1;



				for (size_t j=0; j<outputList.size(); ++j) {
					if (std::fabs(E.x-outputList.at(j).x)<EPSILON && std::fabs(E.y-outputList.at(j).y)<EPSILON && std::fabs(E.z-outputList.at(j).z)<EPSILON) {
						duplicate = j;
					}
				}

				if(duplicate != -1) { // Duplicate found
					edge.v2 = duplicate;
					duplicate = -1;
				} else { // No duplicate end vertex found
					outputList.push_back(E);
					outputIndexList.push_back(outputList.size()-1);
					edge.v2 = outputList.size()-1;
				}

				if( std::find(outputEdgeList.begin(), outputEdgeList.end(),edge) == outputEdgeList.end() ) {
					//std::cout << "Going in, Before: "<<Sind<<"->"<<Eind<<", after: "<<edge.v1<<"->"<<edge.v2<<std::endl;
					outputEdgeList.push_back(edge);
				}

			} else { // S and E both inside
				//std::cout<<"Both inside! S = "<<glm::to_string(S)<<std::endl;
				
				for (size_t j=0; j<outputList.size(); ++j) {
					if (std::fabs(S.x-outputList.at(j).x)<EPSILON && std::fabs(S.y-outputList.at(j).y)<EPSILON && std::fabs(S.z-outputList.at(j).z)<EPSILON) {
						duplicate = j;
					}
				}

				if(duplicate != -1) {
					//std::cout<<"Duplicate found at index "<<std::distance(outputList.begin(),it)<<", position "<<glm::to_string(*it)<<std::endl;
					edge.v1 = duplicate;
					duplicate = -1;
				} else { // No duplicate found
					outputList.push_back(S);
					outputIndexList.push_back(outputList.size()-1);
					edge.v1 = outputList.size()-1;
				}

				for (size_t j=0; j<outputList.size(); ++j) {
					if (std::fabs(E.x-outputList.at(j).x)<EPSILON && std::fabs(E.y-outputList.at(j).y)<EPSILON && std::fabs(E.z-outputList.at(j).z)<EPSILON) {
						duplicate = j;
					}
				}

				if(duplicate != -1) {
					//std::cout<<"Duplicate found at index "<<std::distance(outputList.begin(),it)<<std::endl;
					edge.v2 = duplicate;
					duplicate = -1;
				} else { // Duplicate found
					outputList.push_back(E);
					outputIndexList.push_back(outputList.size()-1);
					edge.v2 = outputList.size()-1;
				}

				if( std::find(outputEdgeList.begin(), outputEdgeList.end(),edge) == outputEdgeList.end() ) {
					//std::cout << "Both inside, Before: "<<Sind<<"->"<<Eind<<", after: "<<edge.v1<<"->"<<edge.v2<<std::endl;
					outputEdgeList.push_back(edge);
				}
			}			
			

		} else if(plane.isInside(S)) { // Going out (S inside, E outside) ( fungerar ej atm, skapar ingen S),
			//std::cout<<"Going out!\n";
			// Check if S aldready in outputList, otherwise add it. Add clippedVert between S->E
		
			for (size_t j=0; j<outputList.size(); ++j) {
				if (std::fabs(S.x-outputList.at(j).x)<EPSILON && std::fabs(S.y-outputList.at(j).y)<EPSILON && std::fabs(S.z-outputList.at(j).z)<EPSILON) {
					duplicate = j;
				}
			}

			if(duplicate != -1) {
				//std::cout<<"Duplicate found at index "<<std::distance(outputList.begin(),it)<<std::endl;
				edge.v1 = duplicate;
				duplicate = -1;
			} else { // No duplicate found
				outputList.push_back(S);
				outputIndexList.push_back(outputList.size()-1);
				edge.v1 = outputList.size()-1;
			}

			glm::vec3 clippedVert = plane.getIntersection(S,E);
			outputList.push_back(clippedVert);
			outputIndexList.push_back(outputList.size());
			clippedVertInd.push_back(outputList.size()-1);
			edge.v2 = outputList.size()-1;

			if( std::find(outputEdgeList.begin(), outputEdgeList.end(),edge) == outputEdgeList.end() ) {
				//std::cout << "Going out, Before: "<<Sind<<"->"<<Eind<<", after: "<<edge.v1<<"->"<<edge.v2<<std::endl;
				outputEdgeList.push_back(edge);
			}
		} else {
			// Nothing - Entire edge outside clip plane
		}
	}
	// End, for each clip plane

	/*std::cout<<"outputList.size() = "<<outputList.size()<<", std::distance = "<<std::distance(outputList.begin(), outputList.end())<<std::endl;
	for(size_t i=0; i<outputIndexList.size(); ++i) 
		std::cout << "Vertex indices: " << outputIndexList.at(i) << std::endl;*/

	/*for(size_t i=0; i<outputList.size(); ++i)
		std::cout << "Output verts, " << i << ": ("+glm::to_string(outputList.at(i)[0])+", "+glm::to_string(outputList.at(i)[1])+", "+glm::to_string(outputList.at(i)[2])+")\n";*/

	//std::cout << "Size of clipped verts vector:" << clippedVertInd.size()<<std::endl;

	// Create edges between new (clipped) vertices
	for (size_t i=0; i<clippedVertInd.size(); ++i) {
		Edge edge;
		unsigned int idx1,idx2,idx3;
		idx1 = clippedVertInd.at(i % clippedVertInd.size());
		idx2 = clippedVertInd.at((i+1) % clippedVertInd.size());
		idx3 = clippedVertInd.at((i+2) % clippedVertInd.size());

		edge.v1 = idx1;
		edge.v2 = idx2;

		if(std::find(outputEdgeList.begin(), outputEdgeList.end(), edge) == outputEdgeList.end() ) {
			outputEdgeList.push_back(edge);
		}
	}

	/*std::cout << "Size of clipped edge list: " << outputEdgeList.size() << std::endl;
	for(size_t i=0; i<outputEdgeList.size();++i) {
		std::cout << "Edge, " << i << " = " << outputEdgeList.at(i).v1 << "->" << outputEdgeList.at(i).v2 << std::endl;
	}*/

	// Bygg ny BaseMeshRAM här från outputList-vektor
	//std::cout << "Buildning new mesh from clipped vertices.\n";
	BaseMeshRAM* outputMesh = new BaseMeshRAM();
	outputMesh->initialize();

	for(size_t i=0; i<outputList.size(); ++i) {
		outputMesh->addVertex(outputList.at(i), glm::vec3(1.f), glm::vec4(1.,i/(float)outputList.size(),0.,1.0f));
	}

	//std::cout << "Number of verts in output mesh: " << 
	//	outputList.size() << std::endl;

  	outputMesh->setIndicesInfo(GeometryRepresentation::POINTS, GeometryRepresentation::NONE);
   	for(size_t i=0; i<outputList.size(); ++i) {
   		outputMesh->addIndex(i);
   		//std::cout << "Adding to index list, vertex no.: " << i << std::endl;
   	}


	//std::cout << "Returning new mesh.\n";

	return outputMesh;
}

void MeshClipping::process() {
	/* Processor overview
		- Take axis-aligned boudning box (AABB) mesh as input.
		- Call clipGeometryAgainstPlane(...) with input and plane_ as arguments
		- Extract and store an edge list from the input mesh's triangle list
		- Start with an empty outputList and empty outputEdgeList. Iterate over the edge list
		  extracted from the triangle list and check each edge against the clip plane. Store verts
		  and edges in outputList and outputEdgeList as you go.
		- Use outputEdgeList and, indirectly, outputList to rebuild a correctly sorted
		  triangle strip list.
		- Build new mesh from the triangle strip list and return it.
	*/
	
	// This cast works once, throws qt event exception otherwise.
	GeometryRAM *geom = const_cast<GeometryRAM*>(inport_.getData()->getRepresentation<GeometryRAM>());


	if(clippingEnabled_.get()) {
		//std::cout << "Calling clipping method.\n";
		GeometryRAM *clippedPlaneGeom = clipGeometryAgainstPlane(geom, plane_);

		//std::cout << "Setting new mesh as outport data.\n";
		outport_.setData(new Geometry(clippedPlaneGeom));
		//std::cout << "Done.\n";
	} else {
		outport_.setData(new Geometry(geom));
	}
}

} // namespace