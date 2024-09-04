#ifndef BMI_RAINRATE_UNIMPLEMENTED_CPP
#define BMI_RAINRATE_UNIMPLEMENTED_CPP

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define SOURCE_LOC " (" __FILE__ ":" TOSTRING(__LINE__) ")"
// ^ Credit https://www.decompile.com/cpp/faq/file_and_line_error_string.htm

#include "bmi_rainrate_cpp.hpp"



int BmiRainRateCpp::GetGridEdgeCount(const int grid){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
int BmiRainRateCpp::GetGridFaceCount(const int grid){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridEdgeNodes(const int grid, int* edge_nodes){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridFaceEdges(const int grid, int* face_edges){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridFaceNodes(const int grid, int* face_nodes){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
int BmiRainRateCpp::GetGridNodeCount(const int grid){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridNodesPerFace(const int grid, int* nodes_per_face){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridOrigin(const int grid, double* origin){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridShape(const int grid, int* shape){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridSpacing(const int grid, double* spacing){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridX(const int grid, double* x){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridY(const int grid, double* y){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridZ(const int grid, double* z){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
int BmiRainRateCpp::GetVarGrid(std::string name){
  throw std::logic_error("Not implemented." SOURCE_LOC);
}

#endif