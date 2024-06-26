# bmi_rainrate

A BMI model intended to compute `precip_rate` from `APCP_surface` and `TMP_2maboveground`.

## Overview

Despite the intent being to match the method used to generate the forcing files initially, this ends up being impossible to do exactly, as the original method uses data from the *next* timestep to calculate the current `precip_rate`. This model instead uses the current `APCP_surface` and `TMP_2maboveground` to calculate the current `precip_rate`.

It does this concurrently with the reverse computation, computing `APCP_surface` from `precip_rate` and `TMP_2maboveground`.

This calculation manages to be faithful to the original process by default, as it is the reverse of the other. Since the first one is calculated using data from timestep `t+1`, this one is calculated using the data from timestep `t-1`, which is actually possible to do by storing past values and accessing them when needed.

Further, the reverse computation is designed with configurability in mind, allowing the user to set `TMP_time_offset` and `precip_rate_time_offset` in the `model_params` to any value they wish, allowing them to calculate `APCP_surface` from `precip_rate` and `TMP_2maboveground` at any (non-positive) offset from the current timestep. An offset of 0 is equivalent to directly using current data, rather than storing it.

## Output Variables

These are variables that can be requested from the model:

- Input Variables
  - `precip_rate` - The rate of precipitation in mm/s
  - `APCP_surface` - The total precipitation in kg/m^2
  - `TMP_2maboveground` - The temperature in K

- Output Variables
  - `atmosphere_water__precipitation_rate` - Calculated equivalent to `precip_rate`
  - `APCP_surface_shifted` - Calculated equivalent to `APCP_surface`

## Installation

1. Clone the repository
2. Install or symlink bmi-cxx in an adjacent directory
3. Run `make`
4. Use whatever methods you have available to link the produced shared library to ngen

## Usage

Once you have built the model, you can use the included realization.json file with a dataset to run the model.

Here is the dataset provided for use with [ngen](www.github.com/NOAA-OWP/ngen):

```bash
wget --no-parent https://ciroh-ua-ngen-data.s3.us-east-2.amazonaws.com/AWI-006/AWI_16_2853886_006.tar.gz
```

The model when run will output csv files with any output variables requested via the realization.json file.
