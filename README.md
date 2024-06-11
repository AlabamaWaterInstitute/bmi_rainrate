# bmi_rainrate

Compute `APCP_Surface` from `precip_rate`

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

The model when run will output csv files with the input precip_rate, TMP_2maboveground, and APCP_surface, and the output APCP_surface.