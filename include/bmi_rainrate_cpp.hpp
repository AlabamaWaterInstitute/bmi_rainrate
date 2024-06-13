#pragma once
#ifndef BMI_RAINRATE_CPP_H
#define BMI_RAINRATE_CPP_H

#include "basic_includes.hpp"
#include "stored_var.hpp"

#define TRUE 1
#define FALSE 0
#define DEBUG 1

#define DEFAULT_TIME_STEP_SIZE 3600
#define DEFAULT_TIME_STEP_COUNT 24

#define BMI_TYPE_NAME_DOUBLE "double"
#define BMI_TYPE_NAME_FLOAT "float"
#define BMI_TYPE_NAME_INT "int"
#define BMI_TYPE_NAME_SHORT "short"
#define BMI_TYPE_NAME_LONG "long"

#define PARTIAL_OUTPUTS 1
#define INVERSION 1

class BmiRainRateCpp : public bmi::Bmi {
    public:
        /**
        * Create a new model data struct instance, allocating memory for the struct itself but not any pointers within it.
        *
        * The ``time_step_size`` member is set to a defined default.  All other members are set to ``0`` or ``NULL`` (for
        * pointers).
        *
        * @return Pointer to the newly created @ref test_bmi_c_model struct instance in memory.
        */
        BmiRainRateCpp(bool input_array = false, bool output_array = false, bool model_params = false):
            use_input_array(input_array), use_output_array(output_array), use_model_params(model_params)
        {
            set_usage(input_array, output_array, model_params);
        };

        virtual void Initialize(std::string config_file);
        virtual void Update();
        virtual void UpdateUntil(double time);
        virtual void Finalize();

        // Model information functions.
        virtual std::string GetComponentName();
        virtual int GetInputItemCount();
        virtual int GetOutputItemCount();
        virtual std::vector<std::string> GetInputVarNames();
        virtual std::vector<std::string> GetOutputVarNames();

        // Variable information functions
        virtual int GetVarGrid(std::string name);
        virtual std::string GetVarType(std::string name);
        virtual std::string GetVarUnits(std::string name);
        virtual int GetVarItemsize(std::string name);
        virtual int GetVarNbytes(std::string name);
        virtual std::string GetVarLocation(std::string name);

        virtual double GetCurrentTime();
        virtual double GetStartTime();
        virtual double GetEndTime();
        virtual std::string GetTimeUnits();
        virtual double GetTimeStep();

        // Variable getters
        virtual void GetValue(std::string name, void *dest);
        virtual void *GetValuePtr(std::string name);
        virtual void GetValueAtIndices(std::string name, void *dest, int *inds, int count);

        // Variable setters
        virtual void SetValue(std::string name, void *src);
        virtual void SetValueAtIndices(std::string name, int *inds, int count, void *src);

        // Grid information functions
        virtual int GetGridRank(const int grid);
        virtual int GetGridSize(const int grid);
        virtual std::string GetGridType(const int grid);

        virtual void GetGridShape(const int grid, int *shape);
        virtual void GetGridSpacing(const int grid, double *spacing);
        virtual void GetGridOrigin(const int grid, double *origin);

        virtual void GetGridX(int grid, double *x);
        virtual void GetGridY(const int grid, double *y);
        virtual void GetGridZ(const int grid, double *z);

        virtual int GetGridNodeCount(const int grid);
        virtual int GetGridEdgeCount(const int grid);
        virtual int GetGridFaceCount(const int grid);

        virtual void GetGridEdgeNodes(const int grid, int *edge_nodes);
        virtual void GetGridFaceEdges(const int grid, int *face_edges);
        virtual void GetGridFaceNodes(const int grid, int *face_nodes);
        virtual void GetGridNodesPerFace(const int grid, int *nodes_per_face);



    private:

        // typedef std::vector<StoredVar*> VarList;

        inline void set_usage(bool input_array = false, bool output_array = false, bool model_params = false){
            use_input_array = input_array;
            use_output_array = output_array;
            use_model_params = model_params;
        }
        //flags for conditional use
        bool use_input_array, use_output_array, use_model_params;
        
        //variable name equivalents
        // APCP_surface:
        // atmosphere_water__liquid_equivalent_precipitation_rate
        // TMP_2maboveground:
        // land_surface_air__temperature

        // Access vectors
        VarList input_vars = {
            new StoredVar(0.0, "APCP_surface", "double", "kg m-2", "node", 1, 1),
            new StoredVar(0.0, "TMP_2maboveground", "double", "K", "node", 1, 1)
        };
        VarList output_vars = {
            new StoredVar(0.0, "atmosphere_water__precipitation_rate", "double", "m s-1", "node", 1, 1)
        };
        VarList model_vars = {
            new StoredVar(0.0, "surface_water__last_value", "double", "kg m-2", "node", 1, 1)
        };

        std::vector<VarList> all_vars = {input_vars, output_vars, model_vars};

        StoredVar* get_var(std::string name);

        // Converting the above mess into a more compact and efficient form
        enum VarType { INPUT, OUTPUT, MODEL }; // Access the correct vector indices by name
        std::vector<std::vector<std::string>> var_names;
        std::vector<std::vector<std::string>> var_types;
        std::vector<std::vector<std::string>> var_units;
        std::vector<std::vector<std::string>> var_locations;
        std::vector<std::vector<int>> var_item_count;
        std::vector<std::vector<int>> var_grids;

        void finalize_vars();
        
        
        std::map<std::string,int> type_sizes = {
            {BMI_TYPE_NAME_DOUBLE, sizeof(double)},
            {BMI_TYPE_NAME_FLOAT, sizeof(float)},
            {BMI_TYPE_NAME_INT, sizeof(int)},
            {BMI_TYPE_NAME_SHORT, sizeof(short)},
            {BMI_TYPE_NAME_LONG, sizeof(long)}
        };

        // ***********************************************************
        // ***************** Non-dynamic allocations *****************
        // ***********************************************************

        // Epoch-based start time (BMI start time is considered 0.0)
        long epoch_start_time = 0.0;
        int num_time_steps = 0;
        double current_model_time = 0.0;
        double model_end_time = 0.0;
        int time_step_size = DEFAULT_TIME_STEP_SIZE;

        // ***********************************************************
        // ******************* Dynamic allocations *******************
        // ***********************************************************
        // std::unique_ptr<double> input_var_0 = nullptr; //APCP_surface //atmosphere_water__liquid_equivalent_precipitation_rate
        // std::unique_ptr<double> input_var_1 = nullptr; //TMP_2maboveground //land_surface_air__temperature
        // std::unique_ptr<double> output_var_0 = nullptr; //atmosphere_water__precipitation_rate
        // std::unique_ptr<double> model_var_0 = nullptr; //surface_water__last_value
        
        

        double get_input_var_value(int index);
        double get_output_var_value(int index);
        double get_model_var_value(int index);

        // ***********************************************************
        // ********************* Model functionality *****************
        // ***********************************************************

        //calculate water density from temperature
        double calculate_rho_from_temp(double temp);

        //calculate change in surface water between time steps
        double calculate_surface_water_change();

        //calculate rain rate from density and precipitation
        double calculate_rain_rate(double rho, double precip);

        #ifdef INVERSION
        //calculate precipitation from rain rate and density
        double calculate_precipitation(double rho, double rain_rate);
        #endif

        //format the rain rate value for readability
        std::string format_rain_rate(double rain_rate);
        

        /**
        * Read the BMI initialization config file and use its contents to set the state of the model.
        *
        * @param config_file The path to the config file.
        * @param model Pointer to the model struct instance.
        * @return The BMI return code indicating success or failure as appopriate.
        */
        void read_init_config(std::string config_file);

        /**
         * Read number of lines in file and max line length, returning -1 if it does not exist or could not be read.
         *
         * @param file_name The name of the file to open and read.
         * @param line_count A pointer to a location in which to write the value for the number of lines in the file.
         * @param max_line_length A pointer to a location in which to write the value of the max line length for the file.
         * @return 0 if successful or -1 otherwise.
         */
        void read_file_line_counts(std::string file_name, int* line_count, int* max_line_length);

        /**
         * Run this model into the future.
         *
         * @param model The model struct instance.
         * @param dt The number of seconds into the future to advance the model.
         * @return 0 if successful or 1 otherwise.
         */
        void run(long dt);

};

extern "C"
{
    /**
    * Construct this BMI instance as a normal C++ object, to be returned to the framework.
    *
    * @return A pointer to the newly allocated instance.
    */
	inline BmiRainRateCpp *bmi_model_create()
	{
		return new BmiRainRateCpp();
	}

    /**
     * @brief Destroy/free an instance created with @see bmi_model_create
     * 
     * @param ptr 
     */
	inline void bmi_model_destroy(BmiRainRateCpp *ptr)
	{
		delete ptr;
	}
}

#endif //BMI_RAINRATE_CPP_H