import os
import subprocess
import re
from collections import defaultdict

def extract_identifiers(file_path):
    identifiers = set()
    with open(file_path, 'r') as file:
        for line in file:
            line = re.sub(r'".*?"|\'.*?\'|#.*|//.*|/\*.*?\*/', '', line)
            possible_identifiers = re.findall(r'\b[_a-zA-Z][_a-zA-Z0-9]*\b', line)
            identifiers.update(possible_identifiers)
    return list(identifiers)

def create_html_summary(directory):
    summary = defaultdict(list)

    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)

        if os.path.isfile(file_path) and file_path.endswith(('.c', '.clj', '.ml', '.lp', '.py')):
            result = subprocess.run(['wc', '-l', file_path], capture_output=True, text=True)
            line_count = result.stdout.split()[0]

            identifiers = extract_identifiers(file_path)
            summary[filename] = {'lines': line_count, 'identifiers': identifiers}

    html_content = "<html><body><h1>Summary of " + os.path.basename(directory) + "</h1>"
    for file, details in summary.items():
        html_content += f"<h2>{file} ({details['lines']} lines)</h2><ul>"
        for identifier in sorted(details['identifiers']):
            html_content += f"<li>{identifier}</li>"
        html_content += "</ul>"
    html_content += "</body></html>"

    with open(os.path.join(directory, 'summary.html'), 'w') as file:
        file.write(html_content)

def process_directories(parent_directory):
    for directory in os.listdir(parent_directory):
        dir_path = os.path.join(parent_directory, directory)
        if os.path.isdir(dir_path):
            create_html_summary(dir_path)
            process_directories(dir_path)  # Recursively process each subdirectory

# Call this function with the parent directory path
process_directories(os.path.join(os.getcwd(), '../..'))
