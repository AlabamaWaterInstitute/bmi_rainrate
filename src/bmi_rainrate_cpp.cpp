#ifndef BMI_RAINRATE_CPP_CPP
#define BMI_RAINRATE_CPP_CPP

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define SOURCE_LOC " (" __FILE__ ":" TOSTRING(__LINE__) ")"
// ^ Credit https://www.decompile.com/cpp/faq/file_and_line_error_string.htm



// #define DEGUG 1
// #ifndef DEBUG_UTILS
//   #define DEBUG_UTILS
//   #if DEGUG >= 1
//     #define _log(msg) fprintf(stderr, "[%s] %s\n", __func__, msg)
//     #define _en() fprintf(stderr, "[%s] %s\n", __func__, "ENTER");
//     #define _ex() fprintf(stderr, "[%s] %s\n", __func__, "EXIT");
//     #define _ln() fprintf(stderr, "%d\n", __LINE__);
//   #else
//     #define _log(msg)
//     #define _en()
//     #define _ex()
//     #define _ln()
//   #endif
// #endif

#include "bmi_rainrate_cpp.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <math.h>
#include <stdexcept>

std::string BmiRainRateCpp::GetComponentName() {/*_ln()*/
  // return "Testing BMI C++ Model";
  return "BMI Rain Rate C++";
}

double BmiRainRateCpp::GetCurrentTime() {/*_ln()*/
  return current_model_time;
}

double BmiRainRateCpp::GetEndTime() {/*_ln()*/
  double time = this->GetStartTime();
  time += this->num_time_steps * this->time_step_size;
  return time;
}

int BmiRainRateCpp::GetGridRank(const int grid) {/*_ln()*/
  if (grid == 0) {/*_ln()*/
      return 1;
  }
  else {
    throw std::runtime_error("Rank requested for non-existent grid." SOURCE_LOC);
  }
}

int BmiRainRateCpp::GetGridSize(const int grid) {/*_ln()*/
  if (grid == 0) {/*_ln()*/
      return 1;
  }
  else {
    throw std::runtime_error("Size requested for non-existent grid." SOURCE_LOC);
  }
}

std::string BmiRainRateCpp::GetGridType(const int grid) {/*_ln()*/
  if (grid == 0) {/*_ln()*/
      return "scalar";
  }
  else {
    throw std::runtime_error("Type requested for non-existent grid." SOURCE_LOC);
  }
}


std::vector<std::string> BmiRainRateCpp::GetInputVarNames() {/*_ln()*/
  return this->var_names[this->INPUT]; 
}
std::vector<std::string> BmiRainRateCpp::GetOutputVarNames() {/*_ln()*/
  return this->var_names[this->OUTPUT];
}
int BmiRainRateCpp::GetInputItemCount() {/*_ln()*/
  return this->input_vars.size();
}
int BmiRainRateCpp::GetOutputItemCount() {/*_ln()*/
  return this->output_vars.size();
}

double BmiRainRateCpp::GetStartTime() {/*_ln()*/
  return 0.0;
}

double BmiRainRateCpp::GetTimeStep() {/*_ln()*/
  return this->time_step_size;
}

std::string BmiRainRateCpp::GetTimeUnits() {/*_ln()*/
  return "s";
}

std::string BmiRainRateCpp::alert_unknown_variable(std::string name) {/*_ln()*/
  std::string msg = "\n\tAvailable variables are: \n";
  for (size_t i = 0; i < 3; i++) {/*_ln()*/
    std::vector<StoredVar*> vars = *this->all_vars[i];
    for (size_t j = 0; j < vars.size(); j++) {/*_ln()*/
      msg += "\t\t" + vars[j]->name + "\n";
    }
  }
  return msg;
}

void BmiRainRateCpp::GetValue(std::string name, void* dest) {/*_ln()*/
  //FIXME inds should be related to var size???
  StoredVar* var = this->get_var(name);
  // GetVarNbytes calls GetVarItemSize and get_var()->count
  // i.e. Nbytes = item_size * item_count
  // Then, the line following would immediately divide by item_size..
  // Far more direct to just get the count directly.
  // int num_elements = this->GetVarNbytes(name)/this->GetVarItemsize(name);
  int num_elements = var->item_count;
  std::vector<int> indicies(num_elements);
  std::iota (std::begin(indicies), std::end(indicies), 0);
  this->GetValueAtIndices(name, dest, indicies.data(), indicies.size());
}

void BmiRainRateCpp::GetValueAtIndices(std::string name, void* dest, int* inds, int count){/*en()*/
  if (count < 1) {/*_ln()*/
    throw std::runtime_error(std::string("Illegal count ") + std::to_string(count) + std::string(" provided to SetValueAtIndices(name, dest, inds, count)" SOURCE_LOC));
  }
  StoredVar* var = this->get_var(name);

  void *ptr = var->get_ptr();
  std::string type = var->type;

  // Thought about making a small template function to handle these, but you'd
  // have to do one of the casts first anyway...refactor?
  if(type == BMI_TYPE_NAME_DOUBLE) {/*_ln()*/
    double* out = static_cast<double*>(dest);
    for (size_t i = 0; i < count; ++i) {/*_ln()*/
      out[i] = static_cast<double*>(ptr)[inds[i]];
    }
    return;
  }

  if (type == BMI_TYPE_NAME_INT) {/*_ln()*/
    int* out = static_cast<int*>(dest);
    for (size_t i = 0; i < count; ++i) {/*_ln()*/
      out[i] = static_cast<int*>(ptr)[inds[i]];
    }
  }

  if (type == BMI_TYPE_NAME_FLOAT) {/*_ln()*/
    float* out = static_cast<float*>(dest);
    for (size_t i = 0; i < count; ++i) {/*_ln()*/
      out[i] = static_cast<float*>(ptr)[inds[i]];
    }
    return;
  }

  if (type == BMI_TYPE_NAME_LONG) {/*_ln()*/
    long* out = static_cast<long*>(dest);
    for (size_t i = 0; i < count; ++i) {/*_ln()*/
      out[i] = static_cast<long*>(ptr)[inds[i]];
    }
    return;
  }

}

void* BmiRainRateCpp::GetValuePtr(std::string name){/*en()*/
  StoredVar* var = this->get_var(name);
  if (var != nullptr) {/*_ln()*/
    return var->get_ptr();
  }
  std::string msg = "GetValuePtr called for non-existent variable: "+name+"";
  msg += this->alert_unknown_variable(name);
  throw std::runtime_error(msg);
}

int BmiRainRateCpp::GetVarItemsize(std::string name) {/*en()*/
  std::map<std::string,int>::const_iterator iter = this->type_sizes.find(this->GetVarType(name));
  if(iter != this->type_sizes.end()) {/*_ln()*/
    return iter->second;
  }
  throw std::runtime_error("Item \""+name+"\" has illegal type \""+(this->GetVarType(name))+"\"!" SOURCE_LOC);
}

std::string BmiRainRateCpp::GetVarLocation(std::string name) {/*en()*/
  StoredVar* var = this->get_var(name);
  if (var != nullptr) {/*_ln()*/
    return var->location;
  }
  std::string msg = "GetVarLocation called for non-existent variable: "+name+"";
  msg += this->alert_unknown_variable(name);
  throw std::runtime_error(msg);
}

int BmiRainRateCpp::GetVarNbytes(std::string name){/*en()*/
  int item_size = this->GetVarItemsize(name);

  // this will never actually get used, but mimicing the C version...
  // if a different item_count isn't found below, the call above will have already thrown.
  int item_count = -1; 

  StoredVar* var = this->get_var(name);
  if (var != nullptr) {/*_ln()*/
    item_count = var->item_count;
  }

  if(item_count == -1) {/*_ln()*/
    // This is probably impossible to reach--the same conditions above failing will cause a throw
    // in GetVarItemSize --> GetVarType (called earlier) instead.
    std::string msg = "GetVarNbytes called for non-existent variable: "+name+"" SOURCE_LOC;
    msg += this->alert_unknown_variable(name);
    throw std::runtime_error(msg);
  }
  return item_size * item_count;
}

std::string BmiRainRateCpp::GetVarType(std::string name){/*en()*/
  StoredVar* var = this->get_var(name);
  if (var != nullptr) {/*_ln()*/
    return var->type;
  }
  std::string msg = "GetVarType called for non-existent variable: "+name+"" SOURCE_LOC;
  msg += this->alert_unknown_variable(name);
  throw std::runtime_error(msg);
}

std::string BmiRainRateCpp::GetVarUnits(std::string name){/*en()*/
  StoredVar* var = this->get_var(name);
  if (var != nullptr) {/*_ln()*/
    return var->units;
  }
  std::string msg = "GetVarUnits called for non-existent variable: "+name+"";
  msg += this->alert_unknown_variable(name);
  throw std::runtime_error(msg);
}

StoredVar* BmiRainRateCpp::get_var(std::string name){/*en()*/
  for (size_t i = 0; i < 3; i++) {/*_ln()*/
    std::vector<StoredVar*> vars = *all_vars[i];
    for (size_t j = 0; j < vars.size(); j++) {/*_ln()*/
      if (vars[j]->name == name) {/*_ln()*/
        return vars[j];
      }
    }
  }
  return nullptr;
}

void BmiRainRateCpp::finalize_vars() {/*_ln()*/
  /* The following are heretofore uninitialized, so we'll need to set them up
  std::vector<std::vector<std::string>> var_names;
  std::vector<std::vector<std::string>> var_types;
  std::vector<std::vector<std::string>> var_units;
  std::vector<std::vector<std::string>> var_locations;
  std::vector<std::vector<int>> var_item_count;
  std::vector<std::vector<int>> var_grids;
  */
  for (size_t i = 0; i < 3; i++) {/*_ln()*/
    //Take the vector of StoredVar* and extract the values into the appropriate vectors
    std::vector<StoredVar*> vars = *this->all_vars[i]; // e.g. input_vars, output_vars, model_vars
    std::vector<std::string> names;
    std::vector<std::string> types;
    std::vector<std::string> units;
    std::vector<std::string> locations;
    std::vector<int> item_counts;
    std::vector<int> grids;
    for (size_t j = 0; j < vars.size(); j++) {/*_ln()*/
      names.push_back(vars[j]->name);
      types.push_back(vars[j]->type);
      units.push_back(vars[j]->units);
      locations.push_back(vars[j]->location);
      item_counts.push_back(vars[j]->item_count);
      grids.push_back(vars[j]->grid);
    }
    this->var_names.push_back(names);
    this->var_types.push_back(types);
    this->var_units.push_back(units);
    this->var_locations.push_back(locations);
    this->var_item_count.push_back(item_counts);
    this->var_grids.push_back(grids);
  }
}

void BmiRainRateCpp::Initialize(std::string file){/*en()*/
  //_log("Initializing...");
  if (file == "")
    throw std::runtime_error("No configuration file path provided.");

  this->read_init_config(file);/*_en()*/

  this->current_model_time = this->GetStartTime();/*_en()*/

  // If neither of these is read from config (remain 0 as initialized), fall back to default for ts count
  if (this->num_time_steps == 0 && this->model_end_time == 0) {/*_ln()*/
      this->num_time_steps = DEFAULT_TIME_STEP_COUNT;
  }
  // Now at least one must be set
  assert(this->model_end_time != 0 || this->num_time_steps != 0);
  // Whenever end time is not already set here, derive based on num_time_steps
  if (this->model_end_time == 0) {/*_ln()*/
      assert(this->num_time_steps != 0);
      this->model_end_time = this->current_model_time + (this->num_time_steps * this->time_step_size);
  }
  assert(this->model_end_time != 0);
  if (this->num_time_steps == 0) {/*_ln()*/
      this->num_time_steps = floor((this->model_end_time - this->current_model_time) / this->time_step_size);
  }
  /*_ln()*/
  #ifdef PARTIAL_OUTPUTS
  // Partial output variables for calculation diagnostics
  // "partial_calc__TMP_2maboveground_apply_rho",
  // "partial_calc__APCP_surface_shifted",
  // "partial_calc__shifted_div_rho"
  VarList partial_output_vars = {
    (new StoredVar(0.0, "partial_calc__TMP_2maboveground_apply_rho", "double", "kg m-3", "node", 1, 0))->setup(_mk_setup_info()),
    (new StoredVar(0.0, "partial_calc__APCP_surface_shifted", "double", "kg m-2", "node", 1, 0))->setup(_mk_setup_info()),
    (new StoredVar(0.0, "partial_calc__shifted_div_rho", "double", "m", "node", 1, 0))->setup(_mk_setup_info())
  };
  this->output_vars.extend(partial_output_vars);
  #endif
  /*_ln()*/
  #ifdef INVERSION
  // Inversion takes precip_rate and outputs APCP_surface
  VarList inversion_input_vars = {
    (new StoredVar(0.0, "precip_rate", "double", "m/s", "node", 1, 0))->setup(_mk_setup_info())
  };
  this->input_vars.extend(inversion_input_vars);
  // Where standard stores APCP_surface, inversion stores precip_rate
  VarList inversion_model_vars = {
    (new StoredVar(0.0, "precip_rate_store", "double", "kg m-2", "node", 1, 0))->setup(_mk_setup_info()),
    (new StoredVar(0.0, "TMP_2aboveground_store", "double", "K", "node", 1, 0))->setup(_mk_setup_info())
  };
  this->model_vars.extend(inversion_model_vars);
  /*_ln()*/
  // Where standard outputs precip_rate as atmosphere_water__precipitation_rate, inversion outputs APCP_surface_shifted
  VarList inversion_output_vars = {
    (new StoredVar(0.0, "APCP_surface_shifted", "double", "kg m-2", "node", 1, 0))->setup(_mk_setup_info())
  };
  this->output_vars.extend(inversion_output_vars);
  #endif
  /*_ln()*/
  this->finalize_vars();
}

double BmiRainRateCpp::get_input_var_value(int index) {/*_ln()*/
    if (index < 0 || index >= this->input_vars.size()) {/*_ln()*/
        throw std::runtime_error("Invalid index provided to get_input_var_value" SOURCE_LOC);
    }
    return this->input_vars[index]->get_value();
}

double BmiRainRateCpp::get_output_var_value(int index) {/*_ln()*/
    if (index < 0 || index >= this->output_vars.size()) {/*_ln()*/
        throw std::runtime_error("Invalid index provided to get_output_var_value" SOURCE_LOC);
    }
    return this->output_vars[index]->get_value();
}

double BmiRainRateCpp::get_model_var_value(int index) {/*_ln()*/
    if (index < 0 || index >= this->model_vars.size()) {/*_ln()*/
        throw std::runtime_error("Invalid index provided to get_model_var_value" SOURCE_LOC);
    }
    return this->model_vars[index]->get_value();
}

void BmiRainRateCpp::SetValueAtIndices(std::string name, int* inds, int len, void* src){/*en()*/
  if (len < 1)
    throw std::runtime_error(std::string("Illegal count ") + std::to_string(len) + std::string(" provided to SetValueAtIndices(name, dest, inds, count)" SOURCE_LOC));

  void* ptr;
  ptr = this->GetValuePtr(name);
  std::string type = this->GetVarType(name);

  if (type == BMI_TYPE_NAME_DOUBLE) {/*_ln()*/
    double* in = static_cast<double*>(ptr);
    for (size_t i = 0; i < len; ++i) {/*_ln()*/
      in[inds[i]] = static_cast<double*>(src)[i];
    }
  } else if (type == BMI_TYPE_NAME_INT) {/*_ln()*/
    int* in = static_cast<int*>(ptr);
    for (size_t i = 0; i < len; ++i) {/*_ln()*/
      in[inds[i]] = static_cast<int*>(src)[i];
    }
  } else if (type == BMI_TYPE_NAME_FLOAT) {/*_ln()*/
    float* in = static_cast<float*>(ptr);
    for (size_t i = 0; i < len; ++i) {/*_ln()*/
      in[inds[i]] = static_cast<float*>(src)[i];
    }
  } else if (type == BMI_TYPE_NAME_LONG) {/*_ln()*/
    long* in = static_cast<long*>(ptr);
    for (size_t i = 0; i < len; ++i) {/*_ln()*/
      in[inds[i]] = static_cast<long*>(src)[i];
    }
  }
}

void BmiRainRateCpp::SetValue(std::string name, void* src){/*en()*///_log(name.c_str());
  StoredVar* var = this->get_var(name);
  if (var == nullptr) {/*_ln()*/
    throw std::runtime_error("SetValue called for unknown variable: "+name);
  }
  memcpy(var->get_ptr(), src, this->GetVarNbytes(name));
}

void BmiRainRateCpp::Update(){/*en()*/
  this->UpdateUntil(this->current_model_time + this->time_step_size);
}

void BmiRainRateCpp::UpdateUntil(double future_time){/*en()*/
  this->run((long)(future_time - this->current_model_time));
  if (this->current_model_time != future_time)
    this->current_model_time = future_time;
}

void BmiRainRateCpp::Finalize(){/*en()*/
  return;
}

void BmiRainRateCpp::read_init_config(std::string config_file)
{/*en()*/
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

  for (size_t i = 0; i < config_line_count; i++) {/*_ln()*/
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

    if (strcmp(param_key, "epoch_start_time") == 0) {/*_ln()*/
      this->epoch_start_time = strtol(param_value, NULL, 10);
      is_epoch_start_time_set = TRUE;
      continue;
    }
    if (strcmp(param_key, "num_time_steps") == 0) {/*_ln()*/
      this->num_time_steps = (int)strtol(param_value, NULL, 10);
      continue;
    }
    if (strcmp(param_key, "time_step_size") == 0) {/*_ln()*/
      this->time_step_size = (int)strtol(param_value, NULL, 10);
      continue;
    }
    if (strcmp(param_key, "model_end_time") == 0) {/*_ln()*/
      this->time_step_size = (int)strtol(param_value, NULL, 10);
      continue;
    }
    if( strcmp(param_key, "use_input_array") == 0) {/*_ln()*/
      this->use_input_array = true;
    }
    if( strcmp(param_key, "use_output_array") == 0) {/*_ln()*/
      this->use_output_array = true;
    }
    if( strcmp(param_key, "use_model_params") == 0) {/*_ln()*/
      this->use_model_params = true;
    }
  }

  // if (is_epoch_start_time_set == FALSE) {/*_ln()*/
  //   throw std::runtime_error("Config param 'epoch_start_time' not found in config file" SOURCE_LOC);
  // }

#if DEGUG >= 1
  std::cout<<"All BmiRainRateCpp config params present; finished parsing config"<<std::endl;
#endif

  //dynamic creation/usage of optional var 3
  if(use_input_array || use_output_array || use_model_params) {/*_ln()*/
    set_usage(use_input_array, use_output_array, use_model_params);
  }
}

void BmiRainRateCpp::read_file_line_counts(std::string file_name, int* line_count, int* max_line_length)
{/*en()*/
  *line_count = 0;
  *max_line_length = 0;
  int current_line_length = 0;
  FILE* fp = fopen(file_name.c_str(), "r");
  // Ensure exists
  if (fp == NULL) {/*_ln()*/
    throw std::runtime_error("Configuration file does not exist." SOURCE_LOC);
  }
  int seen_non_whitespace = 0;
  int c; //EOF is a negative constant...and char may be either signed OR unsigned
  //depending on the compiler, system, achitectured, ect.  So there are cases
  //where this loop could go infinite comparing EOF to unsigned char
  //the return of fgetc is int, and should be stored as such!
  //https://stackoverflow.com/questions/35356322/difference-between-int-and-char-in-getchar-fgetc-and-putchar-fputc
  for (c = fgetc(fp); c != EOF; c = fgetc(fp)) {/*_ln()*/
    // keep track if this line has seen any char other than space or tab
    if (c != ' ' && c != '\t' && c != '\n')
      seen_non_whitespace++;
    // Update line count, reset non-whitespace count, adjust max_line_length (if needed), and reset current line count
    if (c == '\n') {/*_ln()*/
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
  if (seen_non_whitespace > 0) {/*_ln()*/
    *line_count += 1;
  }

  // Before returning, increment the max line length by 1, since the \n will be on the line also.
  *max_line_length += 1;
}

double BmiRainRateCpp::calculate_rho_from_temp(double temp)
{/*en()*/
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
{/*en()*/
    // Calculate change in surface water between time steps
    // python ver: return current_amt - last_amt
    // # oops! it is supposed to be last_amt directly, rather than doing a subtraction
    StoredVar* input_var_0 = this->input_vars[0]; /*_lnf()*/
    StoredVar* input_var_1 = this->input_vars[1]; /*_lnf()*/
    StoredVar* output_var_0 = this->output_vars[0]; /*_lnf()*/
    StoredVar* model_var_0 = this->model_vars[0]; /*_lnf()*/
    double res = model_var_0->get_value(); //_ex()
    return res;
}

double BmiRainRateCpp::calculate_rain_rate(double rho, double precip)
{/*en()*/
    // Calculate rain rate
    // python ver: ( dataFrame['APCP_surface'].shift(-1) / dataFrame['TMP_2maboveground'].apply(rho) ) / interval
    double interval = this->time_step_size;
    return (precip / rho) / interval;
}

#ifdef INVERSION
double BmiRainRateCpp::calculate_precipitation(double rho, double rain_rate)
{/*en()*/
    // Calculate precipitation rate
    // python ver: ( dataFrame['APCP_surface_shifted'] * dataFrame['TMP_2maboveground'].apply(rho) ) * interval
    double interval = this->time_step_size;
    return (rain_rate * rho) * interval;
}
#endif

std::string BmiRainRateCpp::format_rain_rate(double rain_rate)
{/*en()*/
    // Format the rain rate value for readability
    // python ver: return "{:.2f}".format(rain_rate)
    char* buffer = new char[20];
    int size = snprintf(buffer, 20, "%.2f", rain_rate);
    if (size >= 20) {/*_ln()*/
      buffer = new char[size + 1];
      snprintf(buffer, size + 1, "%.2f", rain_rate);
    }
    std::string result(buffer);
    delete[] buffer;
    return result;
}


int lastprint = 0;

void BmiRainRateCpp::run(long dt)
{ //_en()
    StoredVar* input_var_0 = this->input_vars[0]; // Surface water amount
    StoredVar* input_var_1 = this->input_vars[1]; // Temperature
    StoredVar* output_var_0 = this->output_vars[0]; // Rain rate
    StoredVar* model_var_0 = this->model_vars[0]; // Surface water amount
    int output_offset = 1;
    int input_offset = 2;
    int model_offset = 1;
    #ifdef PARTIAL_OUTPUTS
    int partial_offset = output_offset + 1;
    StoredVar* partial_var_0 = this->output_vars[partial_offset]; // partial_calc__TMP_2maboveground_apply_rho
    StoredVar* partial_var_1 = this->output_vars[partial_offset + 1]; // partial_calc__APCP_surface_shifted
    StoredVar* partial_var_2 = this->output_vars[partial_offset + 2]; // partial_calc__shifted_div_rho
    output_offset += 3;
    #endif
    #ifdef INVERSION
    StoredVar* inversion_input_var_0 = this->input_vars[input_offset]; // precip_rate
    StoredVar* inversion_model_var_0 = this->model_vars[model_offset]; // precip_rate_store
    StoredVar* inversion_model_var_1 = this->model_vars[model_offset + 1]; // TMP_2aboveground_store
    StoredVar* inversion_output_var_0 = this->output_vars[output_offset]; // APCP_surface_shifted
    input_offset += 1;
    model_offset += 2;
    output_offset += 1;
    #endif

    double kelvin_offset = 273.15;


    // Get input values
    double precip = this->calculate_surface_water_change(); /*_lnf()*/
    // double temp = *this->input_var_1.get();
    double temp = input_var_1->get_value(); /*_lnf()*/

    // Calculate density
    double rho = calculate_rho_from_temp(temp-273.15); /*_lnf()*/
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

    if (lastprint + 10 < time(0)) {/*_ln()*/
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

// extern "C"
// {
    /**
    * Construct this BMI instance as a normal C++ object, to be returned to the framework.
    *
    * @return A pointer to the newly allocated instance.
    */
	extern "C" BmiRainRateCpp *bmi_model_create() {/*_ln()*/
		return new BmiRainRateCpp();
	}

    /**
     * @brief Destroy/free an instance created with @see bmi_model_create
     * 
     * @param ptr 
     */
	extern "C" void bmi_model_destroy(BmiRainRateCpp *ptr) {/*_ln()*/
		delete ptr;
	}
// }

#endif