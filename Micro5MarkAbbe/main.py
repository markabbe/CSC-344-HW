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

def extract_identifiers(file_content):
    identifiers = set()
    lines = file_content.split('\n')
    for line in lines:
        line = re.sub(r'".*?"|\'.*?\'|#.*|//.*|/\*.*?\*/', '', line)
        possible_identifiers = re.findall(r'\b[_a-zA-Z][_a-zA-Z0-9]*\b', line)
        identifiers.update(possible_identifiers)
    return sorted(identifiers)

def create_html_summary(directory, summary_name, file_name=None):
    all_identifiers = set()
    html_content = f"<html><head><title>Summary of {os.path.basename(directory)}</title></head><body><h1>Summary of {os.path.basename(directory)}</h1>"

    file_list = [file_name] if file_name else [f for f in os.listdir(directory) if os.path.isfile(os.path.join(directory, f))]

    for filename in file_list:
        file_path = os.path.join(directory, filename) if not file_name else filename
        if file_path.endswith(('.c', '.clj', '.ml', '.lp', '.py')):
            try:
                with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
                    content = file.read()
                line_count = content.count('\n') + 1
                identifiers = extract_identifiers(content)
                all_identifiers.update(identifiers)
                html_content += f"<h2><a href=\"{filename}\">{filename}</a> ({line_count} lines)</h2>\n<ul>"
                for identifier in identifiers:
                    html_content += f"<li>{identifier}</li>"
                html_content += "</ul>"
            except OSError as e:
                print(f"Error processing file {file_path}: {e}")

    html_content += "</body></html>"
    summary_file_path = os.path.join(directory, summary_name)
    with open(summary_file_path, 'w', encoding='utf-8') as file:
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
            create_html_summary(parent_directory, summary_name, file_name=path)
        print(f"Processed {assignment}")

clear_pycache(os.getcwd())

script_location = os.getcwd()
parent_directory = os.path.abspath(os.path.join(script_location, '..'))
print(f"Script started. Parent directory: {parent_directory}")
process_directories(parent_directory)
