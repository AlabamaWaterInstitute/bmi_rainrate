#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define SOURCE_LOC " (" __FILE__ ":" TOSTRING(__LINE__) ")"
// ^ Credit https://www.decompile.com/cpp/faq/file_and_line_error_string.htm



#define DEGUG 0
#if DEGUG >= 1
  #define _log(msg) fprintf(stderr, "[%s] %s\n", __func__, msg)
  #define _en() fprintf(stderr, "[%s] %s\n", __func__, "ENTER");
  #define _ex() fprintf(stderr, "[%s] %s\n", __func__, "EXIT");
#else
  #define _log(msg)
  #define _en()
  #define _ex()
#endif

#include "bmi_rainrate_cpp.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <math.h>
#include <stdexcept>

std::string BmiRainRateCpp::GetComponentName(){_en()
  // return "Testing BMI C++ Model";
  return "BMI Rain Rate C++";
}

double BmiRainRateCpp::GetCurrentTime(){_en()
  return current_model_time;
}

double BmiRainRateCpp::GetEndTime(){_en()
  double time = this->GetStartTime();
  time += this->num_time_steps * this->time_step_size;
  return time;
}

int BmiRainRateCpp::GetGridRank(const int grid){_en()
  if (grid == 0) {
      return 1;
  }
  else {
    throw std::runtime_error("Rank requested for non-existent grid." SOURCE_LOC);
  }
}

int BmiRainRateCpp::GetGridSize(const int grid){_en()
  if (grid == 0) {
      return 1;
  }
  else {
    throw std::runtime_error("Size requested for non-existent grid." SOURCE_LOC);
  }
}

std::string BmiRainRateCpp::GetGridType(const int grid){_en()
  if (grid == 0) {
      return "scalar";
  }
  else {
    throw std::runtime_error("Type requested for non-existent grid." SOURCE_LOC);
  }
}


std::vector<std::string> BmiRainRateCpp::GetInputVarNames(){_en()
  return input_var_names;  
}
std::vector<std::string> BmiRainRateCpp::GetOutputVarNames(){_en()
  return output_var_names;
}
int BmiRainRateCpp::GetInputItemCount(){_en()
  return input_var_names.size();
}
int BmiRainRateCpp::GetOutputItemCount(){_en()
  return output_var_names.size();
}

double BmiRainRateCpp::GetStartTime(){_en()
  return 0.0;
}

double BmiRainRateCpp::GetTimeStep(){_en()
  return this->time_step_size;
}

std::string BmiRainRateCpp::GetTimeUnits(){_en()
  return "s";
}

void BmiRainRateCpp::GetValue(std::string name, void* dest){_en()
  //FIXME inds should be related to var size???
  int num_elements = this->GetVarNbytes(name)/this->GetVarItemsize(name);
  std::vector<int> indicies(num_elements);
  std::iota (std::begin(indicies), std::end(indicies), 0);
  this->GetValueAtIndices(name, dest, indicies.data(), indicies.size());
}

void BmiRainRateCpp::GetValueAtIndices(std::string name, void* dest, int* inds, int count){_en()
  if (count < 1)
    throw std::runtime_error(std::string("Illegal count ") + std::to_string(count) + std::string(" provided to SetValueAtIndices(name, dest, inds, count)" SOURCE_LOC));

  void *ptr;
  std::string type;
  ptr = this->GetValuePtr(name);
  type = this->GetVarType(name);

  // Thought about making a small template function to handle these, but you'd
  // have to do one of the casts first anyway...refactor?
  if(type == BMI_TYPE_NAME_DOUBLE){
    double* out = static_cast<double*>(dest);
    for (size_t i = 0; i < count; ++i) {
      out[i] = static_cast<double*>(ptr)[inds[i]];
    }
    return;
  }

  if (type == BMI_TYPE_NAME_INT) {
    int* out = static_cast<int*>(dest);
    for (size_t i = 0; i < count; ++i) {
      out[i] = static_cast<int*>(ptr)[inds[i]];
    }
  }

  if (type == BMI_TYPE_NAME_FLOAT) {
    float* out = static_cast<float*>(dest);
    for (size_t i = 0; i < count; ++i) {
      out[i] = static_cast<float*>(ptr)[inds[i]];
    }
    return;
  }

  if (type == BMI_TYPE_NAME_LONG) {
    long* out = static_cast<long*>(dest);
    for (size_t i = 0; i < count; ++i) {
      out[i] = static_cast<long*>(ptr)[inds[i]];
    }
    return;
  }

}

void* BmiRainRateCpp::GetValuePtr(std::string name){_en()
  auto iter = std::find(this->output_var_names.begin(), this->output_var_names.end(), name);
  // fprintf(stderr, "output var names has %d elements\n", (int)this->output_var_names.size());
  // fprintf(stderr, "output vars has %d elements\n", (int)this->output_vars.size());
  if(iter != this->output_var_names.end()){
    // fprintf(stderr, "Checking element %d\n", (int)(iter - this->output_var_names.begin()));
    return this->output_vars[iter - this->output_var_names.begin()]->get_ptr();
  }
  iter = std::find(this->input_var_names.begin(), this->input_var_names.end(), name);
  // fprintf(stderr, "input var names has %d elements\n", (int)this->input_var_names.size());
  // fprintf(stderr, "input vars has %d elements\n", (int)this->input_vars.size());
  if(iter != this->input_var_names.end()){
    // fprintf(stderr, "Checking element %d\n", (int)(iter - this->input_var_names.begin()));
    return this->input_vars.at(iter - this->input_var_names.begin())->get_ptr();
  }
  iter = std::find(this->model_var_names.begin(), this->model_var_names.end(), name);
  // fprintf(stderr, "model var names has %d elements\n", (int)this->model_var_names.size());
  // fprintf(stderr, "model vars has %d elements\n", (int)this->model_vars.size());
  if(iter != this->model_var_names.end()){
    // fprintf(stderr, "Checking element %d\n", (int)(iter - this->model_var_names.begin()));
    return this->model_vars[iter - this->model_var_names.begin()]->get_ptr();
  }
  throw std::runtime_error("GetValuePtr called for unknown variable: "+name);
}

int BmiRainRateCpp::GetVarItemsize(std::string name) {_en()
  std::map<std::string,int>::const_iterator iter = this->type_sizes.find(this->GetVarType(name));
  if(iter != this->type_sizes.end()){
    return iter->second;
  }
  throw std::runtime_error("Item \""+name+"\" has illegal type \""+(this->GetVarType(name))+"\"!" SOURCE_LOC);
}

std::string BmiRainRateCpp::GetVarLocation(std::string name) {_en()
  auto iter = std::find(this->output_var_names.begin(), this->output_var_names.end(), name);
  if(iter != this->output_var_names.end()){
    //WHY??? just return *iter;
    return this->output_var_locations[iter - this->output_var_names.begin()];
  }
  iter = std::find(this->input_var_names.begin(), this->input_var_names.end(), name);
  if(iter != this->input_var_names.end()){
    return this->input_var_locations[iter - this->input_var_names.begin()];
  }
  iter = std::find(this->model_var_names.begin(), this->model_var_names.end(), name);
  if(iter != this->model_var_names.end()){
    return this->model_var_locations[iter - this->model_var_names.begin()];
  }
  throw std::runtime_error("GetVarLocation called for non-existent variable: "+name+"" SOURCE_LOC);
}

int BmiRainRateCpp::GetVarNbytes(std::string name){_en()
  int item_size = this->GetVarItemsize(name);

  // this will never actually get used, but mimicing the C version...
  // if a different item_count isn't found below, the call above will have already thrown.
  int item_count = -1; 

  auto iter = std::find(this->output_var_names.begin(), this->output_var_names.end(), name);
  if(iter != this->output_var_names.end()){
    item_count = this->output_var_item_count[iter - this->output_var_names.begin()];
  }
  iter = std::find(this->input_var_names.begin(), this->input_var_names.end(), name);
  if(iter != this->input_var_names.end()){
    item_count = this->input_var_item_count[iter - this->input_var_names.begin()];
  }
  iter = std::find(this->model_var_names.begin(), this->model_var_names.end(), name);
  if(iter != this->model_var_names.end()){
    item_count = this->model_var_item_count[iter - this->model_var_names.begin()];
  }
  if(item_count == -1){
    // This is probably impossible to reach--the same conditions above failing will cause a throw
    // in GetVarItemSize --> GetVarType (called earlier) instead.
    throw std::runtime_error("GetVarNbytes called for non-existent variable: "+name+"" SOURCE_LOC );
  }
  return item_size * item_count;
}

std::string BmiRainRateCpp::GetVarType(std::string name){_en()
  auto iter = std::find(this->output_var_names.begin(), this->output_var_names.end(), name);
  if(iter != this->output_var_names.end()){
    return this->output_var_types[iter - this->output_var_names.begin()];
  }
  iter = std::find(this->input_var_names.begin(), this->input_var_names.end(), name);
  if(iter != this->input_var_names.end()){
    return this->input_var_types[iter - this->input_var_names.begin()];
  }
  iter = std::find(this->model_var_names.begin(), this->model_var_names.end(), name);
  if(iter != this->model_var_names.end()){
    return this->model_var_types[iter - this->model_var_names.begin()];
  }
  throw std::runtime_error("GetVarType called for non-existent variable: "+name+"" SOURCE_LOC );
}

std::string BmiRainRateCpp::GetVarUnits(std::string name){_en()
  auto iter = std::find(this->output_var_names.begin(), this->output_var_names.end(), name);
  if(iter != this->output_var_names.end()){
    return this->output_var_units[iter - this->output_var_names.begin()];
  }
  iter = std::find(this->input_var_names.begin(), this->input_var_names.end(), name);
  if(iter != this->input_var_names.end()){
    return this->input_var_units[iter - this->input_var_names.begin()];
  }
  iter = std::find(this->model_var_names.begin(), this->model_var_names.end(), name);
  if(iter != this->model_var_names.end()){
    return this->model_var_types[iter - this->model_var_names.begin()];
  }
  throw std::runtime_error("GetVarUnits called for non-existent variable: "+name+"" SOURCE_LOC);
}

void BmiRainRateCpp::Initialize(std::string file){_en()
  _log("Initializing...");
  if (file == "")
    throw std::runtime_error("No configuration file path provided.");

  this->read_init_config(file);

  this->current_model_time = this->GetStartTime();

  // If neither of these is read from config (remain 0 as initialized), fall back to default for ts count
  if (this->num_time_steps == 0 && this->model_end_time == 0) {
      this->num_time_steps = DEFAULT_TIME_STEP_COUNT;
  }
  // Now at least one must be set
  assert(this->model_end_time != 0 || this->num_time_steps != 0);
  // Whenever end time is not already set here, derive based on num_time_steps
  if (this->model_end_time == 0) {
      assert(this->num_time_steps != 0);
      this->model_end_time = this->current_model_time + (this->num_time_steps * this->time_step_size);
  }
  assert(this->model_end_time != 0);
  if (this->num_time_steps == 0) {
      this->num_time_steps = floor((this->model_end_time - this->current_model_time) / this->time_step_size);
  }

  #ifdef PARTIAL_OUTPUTS
  // Partial output variables for calculation diagnostics
  // "partial_calc__TMP_2maboveground_apply_rho",
  // "partial_calc__APCP_surface_shifted",
  // "partial_calc__shifted_div_rho"
  std::vector<std::string> partial_output_var_names = {
    "partial_calc__TMP_2maboveground_apply_rho", // K -> kg m-3
    "partial_calc__APCP_surface_shifted", // kg m-2
    "partial_calc__shifted_div_rho" // m (before division by time step size)
  };
  std::vector<std::string> partial_output_var_types = {
    "double",
    "double",
    "double"
  };
  std::vector<std::string> partial_output_var_units = {
    "kg m-3",
    "kg m-2",
    "m"
  };
  std::vector<std::string> partial_output_var_locations = {
    "node",
    "node",
    "node"
  };
  std::vector<int> partial_output_var_item_count = {
    1,
    1,
    1
  };
  std::vector<int> partial_output_var_grids = {
    0,
    0,
    0
  };
  this->output_var_names.insert(this->output_var_names.end(), partial_output_var_names.begin(), partial_output_var_names.end());
  this->output_var_types.insert(this->output_var_types.end(), partial_output_var_types.begin(), partial_output_var_types.end());
  this->output_var_units.insert(this->output_var_units.end(), partial_output_var_units.begin(), partial_output_var_units.end());
  this->output_var_locations.insert(this->output_var_locations.end(), partial_output_var_locations.begin(), partial_output_var_locations.end());
  this->output_var_item_count.insert(this->output_var_item_count.end(), partial_output_var_item_count.begin(), partial_output_var_item_count.end());
  this->output_var_grids.insert(this->output_var_grids.end(), partial_output_var_grids.begin(), partial_output_var_grids.end());
  #endif

  #ifdef INVERSION
  // Inversion takes precip_rate and outputs APCP_surface
  std::vector<std::string> inversion_input_var_names = {
    "precip_rate"
  };
  std::vector<std::string> inversion_input_var_types = {
    "double"
  };
  std::vector<std::string> inversion_input_var_units = {
    "m/s"
  };
  std::vector<std::string> inversion_input_var_locations = {
    "node"
  };
  std::vector<int> inversion_input_var_item_count = {
    1
  };
  std::vector<int> inversion_input_var_grids = {
    0
  };
  this->input_var_names.insert(this->input_var_names.end(), inversion_input_var_names.begin(), inversion_input_var_names.end());
  this->input_var_types.insert(this->input_var_types.end(), inversion_input_var_types.begin(), inversion_input_var_types.end());
  this->input_var_units.insert(this->input_var_units.end(), inversion_input_var_units.begin(), inversion_input_var_units.end());
  this->input_var_locations.insert(this->input_var_locations.end(), inversion_input_var_locations.begin(), inversion_input_var_locations.end());
  this->input_var_item_count.insert(this->input_var_item_count.end(), inversion_input_var_item_count.begin(), inversion_input_var_item_count.end());
  this->input_var_grids.insert(this->input_var_grids.end(), inversion_input_var_grids.begin(), inversion_input_var_grids.end());

  // Where standard stores APCP_surface, inversion stores precip_rate
  std::vector<std::string> inversion_model_var_names = {"precip_rate_store", "TMP_2aboveground_store"};
  std::vector<std::string> inversion_model_var_types = {"double", "double"};
  std::vector<std::string> inversion_model_var_units = {"kg m-2", "K"};
  std::vector<std::string> inversion_model_var_locations = {"node", "node"};
  std::vector<int> inversion_model_var_item_count = {1, 1};
  std::vector<int> inversion_model_var_grids = {0, 0};
  this->model_var_names.insert(this->model_var_names.end(), inversion_model_var_names.begin(), inversion_model_var_names.end());
  this->model_var_types.insert(this->model_var_types.end(), inversion_model_var_types.begin(), inversion_model_var_types.end());
  this->model_var_units.insert(this->model_var_units.end(), inversion_model_var_units.begin(), inversion_model_var_units.end());
  this->model_var_locations.insert(this->model_var_locations.end(), inversion_model_var_locations.begin(), inversion_model_var_locations.end());
  this->model_var_item_count.insert(this->model_var_item_count.end(), inversion_model_var_item_count.begin(), inversion_model_var_item_count.end());
  this->model_var_grids.insert(this->model_var_grids.end(), inversion_model_var_grids.begin(), inversion_model_var_grids.end());

  // Where standard outputs precip_rate as atmosphere_water__precipitation_rate, inversion outputs APCP_surface_shifted
  std::vector<std::string> inversion_output_var_names = {"APCP_surface_shifted"};
  std::vector<std::string> inversion_output_var_types = {"double"};
  std::vector<std::string> inversion_output_var_units = {"kg m-2"};
  std::vector<std::string> inversion_output_var_locations = {"node"};
  std::vector<int> inversion_output_var_item_count = {1};
  std::vector<int> inversion_output_var_grids = {0};
  this->output_var_names.insert(this->output_var_names.end(), inversion_output_var_names.begin(), inversion_output_var_names.end());
  this->output_var_types.insert(this->output_var_types.end(), inversion_output_var_types.begin(), inversion_output_var_types.end());
  this->output_var_units.insert(this->output_var_units.end(), inversion_output_var_units.begin(), inversion_output_var_units.end());
  this->output_var_locations.insert(this->output_var_locations.end(), inversion_output_var_locations.begin(), inversion_output_var_locations.end());
  this->output_var_item_count.insert(this->output_var_item_count.end(), inversion_output_var_item_count.begin(), inversion_output_var_item_count.end());
  this->output_var_grids.insert(this->output_var_grids.end(), inversion_output_var_grids.begin(), inversion_output_var_grids.end());
  #endif



  // Initialize the output variables
  for (size_t i = 0; i < this->output_var_names.size(); i++) {
    std::string name = this->output_var_names[i];
    std::string type = this->output_var_types[i];
    std::string units = this->output_var_units[i];
    std::string location = this->output_var_locations[i];
    int item_count = this->output_var_item_count[i];
    int grid = this->output_var_grids[i];
    StoredVar<double> *var = new StoredVar<double>(0.0, name, type, units, location, item_count, grid);
    this->output_vars.push_back(std::move(var));
  }

  // Initialize the input variables
  for (size_t i = 0; i < this->input_var_names.size(); i++) {
    std::string name = this->input_var_names[i];
    std::string type = this->input_var_types[i];
    std::string units = this->input_var_units[i];
    std::string location = this->input_var_locations[i];
    int item_count = this->input_var_item_count[i];
    int grid = this->input_var_grids[i];
    StoredVar<double> *var = new StoredVar<double>(0.0, name, type, units, location, item_count, grid);
    this->input_vars.push_back(std::move(var));
  }

  // Initialize the model variables
  for (size_t i = 0; i < this->model_var_names.size(); i++) {
    std::string name = this->model_var_names[i];
    std::string type = this->model_var_types[i];
    std::string units = this->model_var_units[i];
    std::string location = this->model_var_locations[i];
    int item_count = this->model_var_item_count[i];
    int grid = this->model_var_grids[i];
    StoredVar<double> *var = new StoredVar<double>(0.0, name, type, units, location, item_count, grid);
    this->model_vars.push_back(std::move(var));
  }

}

double BmiRainRateCpp::get_input_var_value(int index) {
    if (index < 0 || index >= this->input_vars.size()) {
        throw std::runtime_error("Invalid index provided to get_input_var_value" SOURCE_LOC);
    }
    return this->input_vars[index]->get_value();
}

double BmiRainRateCpp::get_output_var_value(int index) {
    if (index < 0 || index >= this->output_vars.size()) {
        throw std::runtime_error("Invalid index provided to get_output_var_value" SOURCE_LOC);
    }
    return this->output_vars[index]->get_value();
}

double BmiRainRateCpp::get_model_var_value(int index) {
    if (index < 0 || index >= this->model_vars.size()) {
        throw std::runtime_error("Invalid index provided to get_model_var_value" SOURCE_LOC);
    }
    return this->model_vars[index]->get_value();
}

void BmiRainRateCpp::SetValueAtIndices(std::string name, int* inds, int len, void* src){_en()
  if (len < 1)
    throw std::runtime_error(std::string("Illegal count ") + std::to_string(len) + std::string(" provided to SetValueAtIndices(name, dest, inds, count)" SOURCE_LOC));

  void* ptr;
  ptr = this->GetValuePtr(name);
  std::string type = this->GetVarType(name);

  if (type == BMI_TYPE_NAME_DOUBLE) {
    double* in = static_cast<double*>(ptr);
    for (size_t i = 0; i < len; ++i) {
      in[inds[i]] = static_cast<double*>(src)[i];
    }
  } else if (type == BMI_TYPE_NAME_INT) {
    int* in = static_cast<int*>(ptr);
    for (size_t i = 0; i < len; ++i) {
      in[inds[i]] = static_cast<int*>(src)[i];
    }
  } else if (type == BMI_TYPE_NAME_FLOAT) {
    float* in = static_cast<float*>(ptr);
    for (size_t i = 0; i < len; ++i) {
      in[inds[i]] = static_cast<float*>(src)[i];
    }
  } else if (type == BMI_TYPE_NAME_LONG) {
    long* in = static_cast<long*>(ptr);
    for (size_t i = 0; i < len; ++i) {
      in[inds[i]] = static_cast<long*>(src)[i];
    }
  }
}

void BmiRainRateCpp::SetValue(std::string name, void* src){_en()_log(name.c_str());
  void *dest = this->GetValuePtr(name);
  int nbytes = this->GetVarNbytes(name);
  std::memcpy (dest, src, nbytes);
  // fprintf(stderr, "Set value: %s to %f\n", name.c_str(), *(double*)dest);
}

void BmiRainRateCpp::Update(){_en()
  this->UpdateUntil(this->current_model_time + this->time_step_size);
}

void BmiRainRateCpp::UpdateUntil(double future_time){_en()
  this->run((long)(future_time - this->current_model_time));
  if (this->current_model_time != future_time)
    this->current_model_time = future_time;
}

void BmiRainRateCpp::Finalize(){_en()
  return;
}

int BmiRainRateCpp::GetGridEdgeCount(const int grid){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
int BmiRainRateCpp::GetGridFaceCount(const int grid){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridEdgeNodes(const int grid, int* edge_nodes){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridFaceEdges(const int grid, int* face_edges){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridFaceNodes(const int grid, int* face_nodes){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
int BmiRainRateCpp::GetGridNodeCount(const int grid){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridNodesPerFace(const int grid, int* nodes_per_face){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridOrigin(const int grid, double* origin){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridShape(const int grid, int* shape){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridSpacing(const int grid, double* spacing){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridX(const int grid, double* x){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridY(const int grid, double* y){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
void BmiRainRateCpp::GetGridZ(const int grid, double* z){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}
int BmiRainRateCpp::GetVarGrid(std::string name){_en()
  throw std::logic_error("Not implemented." SOURCE_LOC);
}

void BmiRainRateCpp::read_init_config(std::string config_file)
{_en()
  int config_line_count, max_config_line_length;
  // Note that this determines max line length including the ending return character, if present
  read_file_line_counts(config_file, &config_line_count, &max_config_line_length);

  FILE* fp = fopen(config_file.c_str(), "r");
  if (fp == NULL)
    throw std::runtime_error("Invalid config file \""+config_file+"\"" SOURCE_LOC);

  std::vector<char> config_line(max_config_line_length + 1);

  // TODO: may need to add other variables to track that everything that was required was properly set

  // Keep track of whether required values were set in config
  int is_epoch_start_time_set = FALSE;

  for (size_t i = 0; i < config_line_count; i++) {
    char *param_key, *param_value;
    char* ret = fgets(config_line.data(), max_config_line_length + 1, fp);
    if (ret == nullptr)
        throw std::runtime_error("Error or EOF when reading '" + config_file + "'");

    char* config_line_ptr = config_line.data();
    config_line_ptr = strsep(&config_line_ptr, "\n");
    param_key = strsep(&config_line_ptr, "=");
    param_value = strsep(&config_line_ptr, "=");

#if DEGUG >= 1
    std::cout<<"Config Value - Param: '"<<param_key<<"' | Value: '"<<param_value<<"'"<<std::endl;
#endif

    if (strcmp(param_key, "epoch_start_time") == 0) {
      this->epoch_start_time = strtol(param_value, NULL, 10);
      is_epoch_start_time_set = TRUE;
      continue;
    }
    if (strcmp(param_key, "num_time_steps") == 0) {
      this->num_time_steps = (int)strtol(param_value, NULL, 10);
      continue;
    }
    if (strcmp(param_key, "time_step_size") == 0) {
      this->time_step_size = (int)strtol(param_value, NULL, 10);
      continue;
    }
    if (strcmp(param_key, "model_end_time") == 0) {
      this->time_step_size = (int)strtol(param_value, NULL, 10);
      continue;
    }
    if( strcmp(param_key, "use_input_array") == 0) {
      this->use_input_array = true;
    }
    if( strcmp(param_key, "use_output_array") == 0) {
      this->use_output_array = true;
    }
    if( strcmp(param_key, "use_model_params") == 0) {
      this->use_model_params = true;
    }
  }

  // if (is_epoch_start_time_set == FALSE) {
  //   throw std::runtime_error("Config param 'epoch_start_time' not found in config file" SOURCE_LOC);
  // }

#if DEGUG >= 1
  std::cout<<"All BmiRainRateCpp config params present; finished parsing config"<<std::endl;
#endif

  //dynamic creation/usage of optional var 3
  if(use_input_array || use_output_array || use_model_params){
    set_usage(use_input_array, use_output_array, use_model_params);
  }
}

void BmiRainRateCpp::read_file_line_counts(std::string file_name, int* line_count, int* max_line_length)
{_en()
  *line_count = 0;
  *max_line_length = 0;
  int current_line_length = 0;
  FILE* fp = fopen(file_name.c_str(), "r");
  // Ensure exists
  if (fp == NULL) {
    throw std::runtime_error("Configuration file does not exist." SOURCE_LOC);
  }
  int seen_non_whitespace = 0;
  int c; //EOF is a negative constant...and char may be either signed OR unsigned
  //depending on the compiler, system, achitectured, ect.  So there are cases
  //where this loop could go infinite comparing EOF to unsigned char
  //the return of fgetc is int, and should be stored as such!
  //https://stackoverflow.com/questions/35356322/difference-between-int-and-char-in-getchar-fgetc-and-putchar-fputc
  for (c = fgetc(fp); c != EOF; c = fgetc(fp)) {
    // keep track if this line has seen any char other than space or tab
    if (c != ' ' && c != '\t' && c != '\n')
      seen_non_whitespace++;
    // Update line count, reset non-whitespace count, adjust max_line_length (if needed), and reset current line count
    if (c == '\n') {
      *line_count += 1;
      seen_non_whitespace = 0;
      if (current_line_length > *max_line_length)
        *max_line_length = current_line_length;
      current_line_length = 0;
    }
    else {
      current_line_length += 1;
    }
  }
  fclose(fp);

  // If we saw some non-whitespace char on last line, assume last line didn't have its own \n, so count needs to be
  // incremented by 1.
  if (seen_non_whitespace > 0) {
    *line_count += 1;
  }

  // Before returning, increment the max line length by 1, since the \n will be on the line also.
  *max_line_length += 1;
}

double BmiRainRateCpp::calculate_rho_from_temp(double temp)
{_en()
    // return 0.998;
    // Calculate density
    // python ver: return 999.99399 + 0.04216485*temp - 0.007097451*(temp**2) + 0.00003509571*(temp**3) - 9.9037785E-8*(temp**4) 
    double rho = 999.99399 + 0.04216485 * temp;
    double temp2 = temp * temp;
    rho -= 0.007097451 * temp2;
    double temp3 = temp2 * temp;
    rho += 0.00003509571 * temp3;
    double temp4 = temp3 * temp;
    rho -= 9.9037785E-8 * temp4;
    return rho;
}



double BmiRainRateCpp::calculate_surface_water_change()
{_en()
    // Calculate change in surface water between time steps
    // python ver: return current_amt - last_amt
    // # oops! it is supposed to be last_amt directly, rather than doing a subtraction
    StoredVar<double>* input_var_0 = this->input_vars[0];
    StoredVar<double>* input_var_1 = this->input_vars[1];
    StoredVar<double>* output_var_0 = this->output_vars[0];
    StoredVar<double>* model_var_0 = this->model_vars[0];
    return model_var_0->get_value();
}

double BmiRainRateCpp::calculate_rain_rate(double rho, double precip)
{_en()
    // Calculate rain rate
    // python ver: ( dataFrame['APCP_surface'].shift(-1) / dataFrame['TMP_2maboveground'].apply(rho) ) / interval
    double interval = this->time_step_size;
    return (precip / rho) / interval;
}

#ifdef INVERSION
double BmiRainRateCpp::calculate_precipitation(double rho, double rain_rate)
{_en()
    // Calculate precipitation rate
    // python ver: ( dataFrame['APCP_surface_shifted'] * dataFrame['TMP_2maboveground'].apply(rho) ) * interval
    double interval = this->time_step_size;
    return (rain_rate * rho) * interval;
}
#endif

std::string BmiRainRateCpp::format_rain_rate(double rain_rate)
{_en()
    // Format the rain rate value for readability
    // python ver: return "{:.2f}".format(rain_rate)
    char* buffer = new char[20];
    int size = snprintf(buffer, 20, "%.2f", rain_rate);
    if (size >= 20) {
      buffer = new char[size + 1];
      snprintf(buffer, size + 1, "%.2f", rain_rate);
    }
    std::string result(buffer);
    delete[] buffer;
    return result;
}


int lastprint = 0;

void BmiRainRateCpp::run(long dt)
{  _en()
    StoredVar<double>* input_var_0 = this->input_vars[0]; // Surface water amount
    StoredVar<double>* input_var_1 = this->input_vars[1]; // Temperature
    StoredVar<double>* output_var_0 = this->output_vars[0]; // Rain rate
    StoredVar<double>* model_var_0 = this->model_vars[0]; // Surface water amount
    int output_offset = 1;
    int input_offset = 2;
    int model_offset = 1;
    #ifdef PARTIAL_OUTPUTS
    int partial_offset = output_offset + 1;
    StoredVar<double>* partial_var_0 = this->output_vars[partial_offset]; // partial_calc__TMP_2maboveground_apply_rho
    StoredVar<double>* partial_var_1 = this->output_vars[partial_offset + 1]; // partial_calc__APCP_surface_shifted
    StoredVar<double>* partial_var_2 = this->output_vars[partial_offset + 2]; // partial_calc__shifted_div_rho
    output_offset += 3;
    #endif
    #ifdef INVERSION
    StoredVar<double>* inversion_input_var_0 = this->input_vars[input_offset]; // precip_rate
    StoredVar<double>* inversion_model_var_0 = this->model_vars[model_offset]; // precip_rate_store
    StoredVar<double>* inversion_model_var_1 = this->model_vars[model_offset + 1]; // TMP_2aboveground_store
    StoredVar<double>* inversion_output_var_0 = this->output_vars[output_offset]; // APCP_surface_shifted
    input_offset += 1;
    model_offset += 2;
    output_offset += 1;
    #endif

    double kelvin_offset = 273.15;


    // Get input values
    double precip = this->calculate_surface_water_change();
    // double temp = *this->input_var_1.get();
    double temp = input_var_1->get_value();

    // Calculate density
    double rho = calculate_rho_from_temp(temp-273.15);
    #ifdef PARTIAL_OUTPUTS
    // Partial output variables for calculation diagnostics
    // "partial_calc__TMP_2maboveground_apply_rho",
    // "partial_calc__APCP_surface_shifted",
    // "partial_calc__shifted_div_rho"
    double partial_calc__TMP_2maboveground_apply_rho = rho;
    double partial_calc__APCP_surface_shifted = precip;
    double partial_calc__shifted_div_rho = precip / rho;
    partial_var_0->set_value(partial_calc__TMP_2maboveground_apply_rho);
    partial_var_1->set_value(partial_calc__APCP_surface_shifted);
    partial_var_2->set_value(partial_calc__shifted_div_rho);
    #endif

    // Calculate rain rate
    double rain_rate = calculate_rain_rate(rho, precip);

    #ifdef INVERSION
    // Inversion takes precip_rate and outputs APCP_surface
    // Store precip_rate now to be used in next time step
    // Use precip_rate_store to calculate precipitation
    //Get precip_rate_store
    double precip_rate_store = inversion_model_var_0->get_value();
    double TMP_2aboveground_store = inversion_model_var_1->get_value();

    // Set precip_rate_store to precip_rate
    inversion_model_var_0->set_value(inversion_input_var_0->get_value());
    inversion_model_var_1->set_value(input_var_1->get_value());

    // Calculate precipitation using precip_rate_store
    double prev_rho = calculate_rho_from_temp(TMP_2aboveground_store-273.15);
    double precipitation = calculate_precipitation(prev_rho, precip_rate_store);

    // Set output value
    inversion_output_var_0->set_value(precipitation);
    #endif


    // Set output value
    // this->output_var_0.get()[0] = rain_rate;
    output_var_0->set_value(rain_rate);

    // Set model variable
    // this->model_var_0.get()[0] = *this->input_var_0.get();
    model_var_0->set_value(input_var_0->get_value());

    if (lastprint + 10 < time(0)) {
        lastprint = time(0);
        std::string msg_start = "\033[1;40m";
        std::string msg_end = "\033[0m";
        std::string model_id = "bmi_rainrate_cpp";
        std::string msg = msg_start + "[" + model_id + "] " + "Rain rate: " + format_rain_rate(rain_rate) + " m/s" + msg_end;
        fprintf(stderr, "%s\n", msg.c_str());
    }
    
    // Update time
    this->current_model_time += dt;
}