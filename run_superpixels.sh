#!/bin/bash

# Define the list of CLI script names (without path)
CLI_SCRIPTS=("etps_cli" "slic_cli" "seeds_cli" "ers_cli")  # Add your script names here

# Input parameters
INPUT_PATH="/Users/aleksandrekandelaki/git/private/superpixel-benchmark/folder_test"

# Superpixels array
SUPERPIXELS_ARRAY=(50 100 200 500)  # Example array of superpixel numbers

# Base directories
SCRIPT_DIR="./bin"

# Loop over each superpixel number in the array
for SUPERPIXELS in "${SUPERPIXELS_ARRAY[@]}"; do
    # Loop over each script name
    for SCRIPT_NAME in "${CLI_SCRIPTS[@]}"; do
        SCRIPT_PATH="$SCRIPT_DIR/$SCRIPT_NAME"

        # Extract base name without _cli
        METHOD_NAME="${SCRIPT_NAME%_cli}"

        # Define output directories with superpixel folder included
        OUTPUT_BASE="./outputs/$SUPERPIXELS"
        CSV_DIR="$OUTPUT_BASE/$METHOD_NAME/csv"
        VIS_DIR="$OUTPUT_BASE/$METHOD_NAME/vis"

        # Create output directories if they don't exist
        mkdir -p "$CSV_DIR" "$VIS_DIR"

        echo "Running $SCRIPT_PATH with superpixels=$SUPERPIXELS..."
        "$SCRIPT_PATH" \
            --input "$INPUT_PATH" \
            --csv "$CSV_DIR" \
            --vis "$VIS_DIR" \
            --superpixels "$SUPERPIXELS"
    done
done
