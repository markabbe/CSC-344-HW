import os
import subprocess
import re
import shutil
from collections import defaultdict


def clear_pycache(script_path):
    pycache_path = os.path.join(script_path, '__pycache__')
    if os.path.exists(pycache_path) and os.path.isdir(pycache_path):
        shutil.rmtree(pycache_path)
        print(f"Cleared pycache at {pycache_path}")


def extract_identifiers(file_path):
    identifiers = set()
    with open(file_path, 'r') as file:
        for line in file:
            line = re.sub(r'".*?"|\'.*?\'|#.*|//.*|/\*.*?\*/', '', line)
            possible_identifiers = re.findall(r'\b[_a-zA-Z][_a-zA-Z0-9]*\b', line)
            identifiers.update(possible_identifiers)
    print(f"Extracted identifiers from {file_path}")
    return list(identifiers)


def create_html_summary(directory, summary_name, file_name=None):
    summary = defaultdict(list)
    if file_name:
        file_list = [file_name]
    else:
        file_list = os.listdir(directory)

    for filename in file_list:
        file_path = os.path.join(directory, filename) if not file_name else filename
        if os.path.isfile(file_path) and file_path.endswith(('.c', '.clj', '.ml', '.lp', '.py')):
            result = subprocess.run(['wc', '-l', file_path], capture_output=True, text=True)
            line_count = result.stdout.split()[0]
            identifiers = extract_identifiers(file_path)
            summary[filename] = {'lines': line_count, 'identifiers': identifiers}
            print(f"Processed file {file_path}")

    html_content = "<html><body><h1>Summary of " + os.path.basename(directory) + "</h1>"
    for file, details in summary.items():
        html_content += f"<h2>{file} ({details['lines']} lines)</h2><ul>"
        for identifier in sorted(details['identifiers']):
            html_content += f"<li>{identifier}</li>"
        html_content += "</ul>"
    html_content += "</body></html>"

    summary_file_path = os.path.join(directory, summary_name)
    with open(summary_file_path, 'w') as file:
        file.write(html_content)
        print(f"Created summary at {summary_file_path}")


def process_directories(parent_directory):
    assignments = {
        'Lab1MarkAbbe': 'summary_a1.html',
        'Micro2MarkAbbe': 'summary_a2.html',
        'Lab3MarkAbbe': 'summary_a3.html',
        'Micro5MarkAbbe': 'summary_a5.html',
        'Lab4MarkAbbe.lp': 'summary_a4.html'
    }
    for assignment, summary_name in assignments.items():
        path = os.path.join(parent_directory, assignment)
        if os.path.isdir(path):
            create_html_summary(path, summary_name)
        elif os.path.isfile(path):
            # For Lab4MarkAbbe.lp, we want to create the summary in the parent directory
            create_html_summary(parent_directory, summary_name, file_name=path)
        print(f"Processed {assignment}")


# Clear the __pycache__ directory to avoid any caching issues
clear_pycache(os.getcwd())

# Assuming the script is located in Micro5MarkAbbe, so we traverse back up one level to reach the CSC-344-HW directory.
script_location = os.getcwd()
parent_directory = os.path.abspath(os.path.join(script_location, '..'))
print(f"Script started. Parent directory: {parent_directory}")
process_directories(parent_directory)
