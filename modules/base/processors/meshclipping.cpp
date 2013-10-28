#include "meshclipping.h"
#include <inviwo/core/datastructures/geometry/edge.h>
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

ProcessorClassName(MeshClipping, "MeshClipping"); 
ProcessorCategory(MeshClipping, "Geometry Creation");
ProcessorCodeState(MeshClipping, CODE_STATE_EXPERIMENTAL);

const float MeshClipping::EPSILON = 0.00001f;

MeshClipping::MeshClipping()
	: ProcessorGL(),
	inport_("geometry.input"),
	outport_("geometry.output"),
	clippingEnabled_("clippingEnabled", "Enable clipping", false),
    planePoint_("planePoint", "Plane Point", vec3(0.0f), vec3(-10.0f), vec3(10.0f), vec3(0.1f)),
    planeNormal_("planeNormal", "Plane Normal", vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f), vec3(1.0f), vec3(0.1f)),
    renderAsPoints_("renderAsPoints", "Render As Points by Default", false)
{
	addPort(inport_);
	addPort(outport_);

    addProperty(clippingEnabled_);
	addProperty(planePoint_);
	addProperty(planeNormal_);
    addProperty(renderAsPoints_);
}

MeshClipping::~MeshClipping() {}

void MeshClipping::initialize() {
	ProcessorGL::initialize();
}

void MeshClipping::deinitialize() {
	ProcessorGL::deinitialize();
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
	
	if(clippingEnabled_.get()) {
        const Geometry *geom = inport_.getData();

		//LogInfo("Calling clipping method.");
		Geometry *clippedPlaneGeom = clipGeometryAgainstPlaneRevised(geom, Plane(planePoint_.get(), planeNormal_.get()));

		//LogInfo("Setting new mesh as outport data.");
		outport_.setData(clippedPlaneGeom);
		//LogInfo("Done.");
	} else {
		outport_.setData(const_cast<Geometry*>(inport_.getData()), false);
	}
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
std::vector<Edge> triangleListtoEdgeList(const std::vector<unsigned int> triList) {
	std::vector<Edge> result;
	LogInfoCustom("MeshClipping", "Size of tri list: " << triList.size()-1);
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

	LogInfoCustom("MeshClipping", "Size of edge list: " << result.size());

	for(size_t i=0; i<result.size();++i) {
		LogInfoCustom("MeshClipping", "Edge, " << i << " = " << result.at(i).v1 << "->" << result.at(i).v2);
	}
	return result;
}

Geometry* MeshClipping::clipGeometryAgainstPlaneRevised(const Geometry* in, Plane plane) {
    const SimpleMeshRAM *inputMesh = dynamic_cast<const SimpleMeshRAM*>(in);
    if(!inputMesh) {
        LogError("Can only clip a SimpleMeshRAM*");
        return NULL;
    }

    std::vector<vec3> inputList = inputMesh->getVertexList()->getRepresentation<Position3dBufferRAM>()->getDataContainer(); 
    std::vector<unsigned int> triangleList = inputMesh->getIndexList()->getRepresentation<IndexBufferRAM>()->getDataContainer();

    SimpleMeshRAM* outputMesh = new SimpleMeshRAM(Mesh::TRIANGLES);
    outputMesh->initialize();

    //Check if we are using indicies
    if(triangleList.size() > 0){
        //Check if it is a Triangle Strip
        if(inputMesh->getIndexAttributesInfo(0).ct == Mesh::STRIP){
            // Iterate over edges by edge
            Triangle3D triangle;
            for(unsigned int i=0; i<triangleList.size()-2; ++i) {
                triangle.vertices()[0] = inputList.at(triangleList[i]);
                triangle.vertices()[1] = inputList.at(triangleList[i+1]);
                triangle.vertices()[2] = inputList.at(triangleList[i+2]);
                clipAndAddTriangle(triangle, plane, outputMesh);
            }
        }
    }

    return outputMesh;
}

void MeshClipping::clipAndAddTriangle(Triangle3D& poly, Plane& plane, SimpleMeshRAM* output){
    /* Sutherland-Hodgman Clipping
       1) Traverse each edge by edge, such as successive vertex pairs make up edges
       2) For each edge with vertices [v1, v2]
          Case 1: If v1 and v2 is inside, add v2
          Case 2: If v1 inside and v2 outside, add intersection
          Case 3: If v1 outside and v2 inside, add intersection and then add v2
          Case 4: If v1 and v2 is outside, add nothing
       Observation: A clipped triangle can either contain 
          3 points (if only case 1 and 4 occurred) or                                  
          4 points (if case 2 and 3 occurred) or
          0 points (if only case 4 occurred, thus no points)
       3) If 4 points, make two triangles, 0 1 2 and 0 3 2, total 6 points.
    */
    std::vector<vec3> newVertices;
    for(size_t i=0; i<3; ++i){ 
        size_t j = (i==2 ? 0 : i+1);
        if(plane.isInside(poly.at(i))) {
            if(plane.isInside(poly.at(j))) { // Case 1
                newVertices.push_back(poly.at(j));
            }
            else{ //Case 2
                newVertices.push_back(plane.getIntersection(poly.at(i),poly.at(j)));
            }
        }
        else{
            if(plane.isInside(poly.at(j))) { // Case 3
                newVertices.push_back(plane.getIntersection(poly.at(i),poly.at(j))); 
                newVertices.push_back(poly.at(j));
            }
            // Case 4
        }
    }

    //Handle more then 3 vertices
    if(newVertices.size() > 3){
        ivwAssert(newVertices.size() < 5, "Can't handle " << newVertices.size() << " vertices after clipping");
        vec3 last = newVertices.at(3);
        newVertices.pop_back();
        newVertices.push_back(newVertices.at(0));
        newVertices.push_back(last);
        newVertices.push_back(newVertices.at(2));
    }

    //Add vertices to mesh
    for(size_t i=0; i<newVertices.size(); ++i){
        output->addVertex(newVertices.at(i), newVertices.at(i), glm::vec4(newVertices.at(i), 1.0f));
    }
}

Geometry* MeshClipping::clipGeometryAgainstPlane(const Geometry* in, Plane plane) {
	//LogInfo("Entered clipGeometryAgainstPlane(...).");

    const SimpleMeshRAM *inputMesh = dynamic_cast<const SimpleMeshRAM*>(in);
	if(!inputMesh) {
		LogError("Can only clip a SimpleMeshRAM*");
		return NULL;
	}

	/* ---TODO / bugs
		-	Create correct outputEdgeList while running clipping algorithm, currently edges between clipped verts
			sometimes end up in incorrect order.
		-	Use correct outputEdgeList to create a correctly sorted triangle strip list
	*/

	//LogInfo("Fetching vertex- and triangle lists.");
    std::vector<vec3> inputList = inputMesh->getVertexList()->getRepresentation<Position3dBufferRAM>()->getDataContainer(); 
    std::vector<unsigned int> triangleList = inputMesh->getIndexList()->getRepresentation<IndexBufferRAM>()->getDataContainer();
	std::vector<Edge> edgeList = triangleListtoEdgeList(triangleList);
	std::vector<unsigned int> clippedVertInd;

	// For each clip plane, do:
	std::vector<glm::vec3> outputList;
	std::vector<unsigned int> outputIndexList; // vertex index list
	std::vector<Edge> outputEdgeList; // output edge list

	// Iterate over edges extracted from triangle strip list, and perform clipping against plane
	for(unsigned int i=0; i<edgeList.size(); ++i) {

		//LogInfo("i = "<<i);

		unsigned int Sind = edgeList.at(i).v1;
		unsigned int Eind = edgeList.at(i).v2;

		glm::vec3 S = inputList.at( Sind );
		glm::vec3 E = inputList.at( Eind );

		Edge edge;
		int duplicate = -1;

		// For each clip plane
		if(plane.isInside(E)) {
			if(!plane.isInside(S)) { // Going in
				//LogInfo("Going in!");
				//Måste stoppa in alla verts till edges i rätt ordning för att inte mucka upp vert-id:n
				glm::vec3 clippedVert = plane.getIntersection(S,E);
				outputList.push_back(clippedVert);
				outputIndexList.push_back(static_cast<unsigned int>(outputList.size()-1)); // Ny vertex, uppdatera edge-listan
				clippedVertInd.push_back(static_cast<unsigned int>(outputList.size()-1));
				edge.v1=static_cast<unsigned int>(outputList.size()-1);



				for (unsigned int j=0; j<outputList.size(); ++j) {
					if (std::fabs(E.x-outputList.at(j).x)<EPSILON && std::fabs(E.y-outputList.at(j).y)<EPSILON && std::fabs(E.z-outputList.at(j).z)<EPSILON) {
						duplicate = j;
					}
				}

				if(duplicate != -1) { // Duplicate found
					edge.v2 = duplicate;
					duplicate = -1;
				} else { // No duplicate end vertex found
					outputList.push_back(E);
					outputIndexList.push_back(static_cast<unsigned int>(outputList.size()-1));
					edge.v2 = static_cast<unsigned int>(outputList.size()-1);
				}

				if( std::find(outputEdgeList.begin(), outputEdgeList.end(),edge) == outputEdgeList.end() ) {
					//LogInfo("Going in, Before: "<<Sind<<"->"<<Eind<<", after: "<<edge.v1<<"->"<<edge.v2);
					outputEdgeList.push_back(edge);
				}

			} else { // S and E both inside
				//LogInfo("Both inside! S = "<<glm::to_string(S));
				
				for (unsigned int j=0; j<outputList.size(); ++j) {
					if (std::fabs(S.x-outputList.at(j).x)<EPSILON && std::fabs(S.y-outputList.at(j).y)<EPSILON && std::fabs(S.z-outputList.at(j).z)<EPSILON) {
						duplicate = j;
					}
				}

				if(duplicate != -1) {
					//LogInfo("Duplicate found at index "<<std::distance(outputList.begin(),it)<<", position "<<glm::to_string(*it));
					edge.v1 = duplicate;
					duplicate = -1;
				} else { // No duplicate found
					outputList.push_back(S);
					outputIndexList.push_back(static_cast<unsigned int>(outputList.size()-1));
					edge.v1 = static_cast<unsigned int>(outputList.size()-1);
				}

				for (int j=0; j<static_cast<int>(outputList.size()); ++j) {
					if (std::fabs(E.x-outputList.at(j).x)<EPSILON && std::fabs(E.y-outputList.at(j).y)<EPSILON && std::fabs(E.z-outputList.at(j).z)<EPSILON) {
						duplicate = j;
					}
				}

				if(duplicate != -1) {
					//LogInfo("Duplicate found at index "<<std::distance(outputList.begin(),it));
					edge.v2 = duplicate;
					duplicate = -1;
				} else { // Duplicate found
					outputList.push_back(E);
					outputIndexList.push_back(static_cast<unsigned int>(outputList.size()-1));
					edge.v2 = static_cast<unsigned int>(outputList.size()-1);
				}

				if( std::find(outputEdgeList.begin(), outputEdgeList.end(),edge) == outputEdgeList.end() ) {
					//LogInfo("Both inside, Before: "<<Sind<<"->"<<Eind<<", after: "<<edge.v1<<"->"<<edge.v2);
					outputEdgeList.push_back(edge);
				}
			}			
			

		} else if(plane.isInside(S)) { // Going out (S inside, E outside) ( fungerar ej atm, skapar ingen S),
			//LogInfo("Going out!");
			// Check if S aldready in outputList, otherwise add it. Add clippedVert between S->E
		
			for (unsigned int j=0; j<outputList.size(); ++j) {
				if (std::fabs(S.x-outputList.at(j).x)<EPSILON && std::fabs(S.y-outputList.at(j).y)<EPSILON && std::fabs(S.z-outputList.at(j).z)<EPSILON) {
					duplicate = j;
				}
			}

			if(duplicate != -1) {
				//LogInfo("Duplicate found at index "<<std::distance(outputList.begin(),it));
				edge.v1 = duplicate;
				duplicate = -1;
			} else { // No duplicate found
				outputList.push_back(S);
				outputIndexList.push_back(static_cast<unsigned int>(outputList.size()-1));
				edge.v1 = static_cast<unsigned int>(outputList.size()-1);
			}

			glm::vec3 clippedVert = plane.getIntersection(S,E);
			outputList.push_back(clippedVert);
			outputIndexList.push_back(static_cast<unsigned int>(outputList.size()));
			clippedVertInd.push_back(static_cast<unsigned int>(outputList.size()-1));
			edge.v2 = static_cast<unsigned int>(outputList.size()-1);

			if( std::find(outputEdgeList.begin(), outputEdgeList.end(),edge) == outputEdgeList.end() ) {
				//LogInfo("Going out, Before: "<<Sind<<"->"<<Eind<<", after: "<<edge.v1<<"->"<<edge.v2);
				outputEdgeList.push_back(edge);
			}
		} else {
			// Nothing - Entire edge outside clip plane
		}
	}
	// End, for each clip plane

	LogInfo("outputList.size() = "<<outputList.size()<<", std::distance = "<<std::distance(outputList.begin(), outputList.end()));
	for(size_t i=0; i<outputIndexList.size(); ++i) 
		LogInfo("Vertex indices: " << outputIndexList.at(i));

	for(size_t i=0; i<outputList.size(); ++i)
		LogInfo("Output verts, " << i << ": ("+glm::to_string(outputList.at(i)[0])+", "+glm::to_string(outputList.at(i)[1])+", "+glm::to_string(outputList.at(i)[2])+")");

	LogInfo("Size of clipped verts vector:" << clippedVertInd.size());

	// Create edges between new (clipped) vertices
	for (unsigned int i=0; i<clippedVertInd.size(); ++i) {
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

	/*LogInfo("Size of clipped edge list: " << outputEdgeList.size());
	for(size_t i=0; i<outputEdgeList.size();++i) {
		LogInfo("Edge, " << i << " = " << outputEdgeList.at(i).v1 << "->" << outputEdgeList.at(i).v2);
	}*/

	// Bygg ny SimpleMeshRAM här från outputList-vektor
	//LogInfo("Buildning new mesh from clipped vertices.");
	SimpleMeshRAM* outputMesh = new SimpleMeshRAM();
	outputMesh->initialize();

	for(unsigned int i=0; i<outputList.size(); ++i) {
		outputMesh->addVertex(outputList.at(i), glm::vec3(1.f), glm::vec4(1.,i/(float)outputList.size(),0.,1.0f));
	}

	//LogInfo("Number of verts in output mesh: " << 
	//	outputList.size());
    if(renderAsPoints_.get())
  	    outputMesh->setIndicesInfo(Mesh::POINTS, Mesh::NONE); 
    else
        outputMesh->setIndicesInfo(Mesh::TRIANGLES, Mesh::STRIP);

   	for(unsigned int i=0; i<outputList.size(); ++i) {
   		outputMesh->addIndex(i);
   		//LogInfo("Adding to index list, vertex no.: " << i);
   	}


	//LogInfo("Returning new mesh.");

	return outputMesh;
}

} // namespace