# Pig Lameness Detection

This project is designed to analyze pig gait and detect lameness by processing video frame data and extracting step and stride information from CSV files. The main code is written in C and processes the outputs of a computer vision system (such as YOLO/darknet) that detects pig leg positions in video frames.

## Overview

The workflow of the project is as follows:

1. **Input Data**: The program reads several CSV files containing:
    - Detected leg positions per frame (`steps.csv`)
    - Number of frames in the video (`frames.csv`)
    - Foot size information (`totalFootSize.csv`)
2. **Processing**: The code:
    - Tracks the positions of each leg (Front Right, Front Left, Rear Right, Rear Left) across frames.
    - Handles missing detections and attempts to match legs between frames.
    - Determines the direction of movement.
    - Calculates stride lengths and step counts for each leg.
3. **Output Data**: The program writes processed data to several CSV files:
    - `output1.csv`, `output2.csv`, `finaloutput.csv`: Intermediate and final leg position data.
    - `stepPositionFR.csv`, `stepPositionFL.csv`, `stepPositionRR.csv`, `stepPositionRL.csv`: Step positions for each leg.
    - `stepCounting.csv`: Number of steps detected per leg.
    - `Result.csv`: Average stride lengths for each leg.

## File Structure

- **main.c**: Main C source code for processing the CSV files and calculating gait metrics.
- **csv/**: Folder containing all input and output CSV files.
    - `steps.csv`: Detected leg positions per frame.
    - `frames.csv`: Number of frames in the video.
    - `totalFootSize.csv`: Foot size data.
    - `output1.csv`, `output2.csv`, `finaloutput.csv`: Intermediate/final leg position data.
    - `stepPositionFR.csv`, `stepPositionFL.csv`, `stepPositionRR.csv`, `stepPositionRL.csv`: Step positions for each leg.
    - `stepCounting.csv`: Step counts.
    - `Result.csv`: Stride length results.

## How It Works

1. **Initialization**: The program opens all required CSV files for reading or writing.
2. **Frame Processing**: For each frame, the code:
    - Reads detected leg positions.
    - Matches legs between frames, handling missing detections.
    - Determines the movement direction (left-to-right or right-to-left).
    - Stores leg positions for further analysis.
3. **Reverse Pass**: The code processes frames in reverse to improve leg matching and fill in missing data.
4. **Stride and Step Calculation**: After processing all frames, the code:
    - Calculates stride lengths for each leg.
    - Counts the number of steps for each leg.
    - Outputs results to CSV files.

## Usage

1. **Prepare Input Files**: Place the required CSV files (`steps.csv`, `frames.csv`, `totalFootSize.csv`) in the specified directory.
2. **Compile the Code**: Use a C compiler to compile `main.c`. For example, using `gcc`:
    ```bash
    gcc -o pig_lameness_detect main.c -lm
    ```
3. **Run the Program**: Execute the compiled program, providing the necessary arguments (if any). For example:
    ```bash
    ./pig_lameness_detect
    ```
4. **Output Files**: After execution, the output CSV files will be available in the specified output directory.

## Requirements

- C compiler (e.g., `gcc`)
- Math library (for mathematical calculations in C)
- Access to CSV files with the correct format

## Notes

- Ensure that the input CSV files are correctly formatted and contain the necessary data.
- The output files will be overwritten if they already exist in the output directory.
- For best results, use high-quality video data with clear pig leg detections.
