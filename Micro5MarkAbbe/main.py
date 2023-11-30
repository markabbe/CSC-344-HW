import os
import subprocess
import sys

def count_lines_in_directory(directory):
    # Check if the directory exists
    if not os.path.isdir(directory):
        print(f"Directory '{directory}' not found.")
        return

    # Iterate over files in the directory
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)

        # Check if it's a file
        if os.path.isfile(file_path):
            # Use wc command to count lines
            result = subprocess.run(['wc', '-l', file_path], capture_output=True, text=True)

            # Extract the line count from the command output
            line_count = result.stdout.split()[0]
            print(f"{filename}: {line_count} lines")

# Check if a directory argument is provided
if len(sys.argv) != 2:
    print("Usage: python3 main.py <directory>")
else:
    count_lines_in_directory(sys.argv[1])
